/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "mcompositornotificationsink.h"
#include "notificationwidgetparameterfactory.h"
#include <MSceneManager>
#include <MOnDisplayChangeEvent>
#include <QApplication>
#include <MGConfItem>
#include <QTimer>

static const QString NOTIFICATION_PREVIEW_ENABLED = "/desktop/meego/notifications/previews_enabled";

MCompositorNotificationSink::MCompositorNotificationSink() :
        sinkDisabled(false), allPreviewsDisabled(false),
        window(new MWindow)
{
    window->setTranslucentBackground(true);
    window->setAttribute(Qt::WA_X11DoNotAcceptFocus);
    window->setAttribute(Qt::WA_X11NetWmWindowTypeNotification);
    window->setObjectName("MCompositorNotificationSinkWindow");

    notificationPreviewMode = new MGConfItem(NOTIFICATION_PREVIEW_ENABLED, this);
    changeNotificationPreviewMode();
    connect(notificationPreviewMode, SIGNAL(valueChanged()), this, SLOT(changeNotificationPreviewMode()));

    // Clear the mask for the duration of orientation change, because the mask is not rotated along with the notification
    connect(window->sceneManager(), SIGNAL(orientationAboutToChange(M::Orientation)), this, SLOT(clearWindowMask()));
    connect(window->sceneManager(), SIGNAL(orientationChangeFinished(M::Orientation)), this, SLOT(updateWindowMask()));
}

MCompositorNotificationSink::~MCompositorNotificationSink()
{
    // Destroy the remaining notifications
    foreach(uint id, idToBanner.keys()) {
        notificationDone(id, false);
    }
    delete window;
}

void MCompositorNotificationSink::addNotification(const Notification &notification)
{
    if (!canAddNotification(notification)) {
        return;
    }

    if ((sinkDisabled && notification.type() != Notification::SystemEvent) || allPreviewsDisabled ) {
        emit notificationAdded(notification);
        return;
    }

    if (idToBanner.contains(notification.notificationId())) {
        // The notification already exists so update it
        updateNotification(notification);
    } else {
         // Create and set up info banner widget
        MBanner *banner = createInfoBanner(notification);
        setupWindowTimer(banner, notification);

        // Keep track of the mapping between IDs and private notification information classes
        idToBanner.insert(notification.notificationId(), banner);
        currentBanners.append(banner);
        emit notificationAdded(notification);

        // Check whether the window was open
        bool windowWasOpen = window->isVisible();

        // Make sure the window is open now
        showOrHideWindow();

        if (windowWasOpen) {
            // If the window was already open the latest banner can directly be added to the window
            addLatestBannerToWindow();
        } else {
            // If the window was not open yet a fake display change event is needed so that the banner will animate (since otherwise it thinks it's not on display)
            connect(window, SIGNAL(displayEntered()), this, SLOT(addLatestBannerToWindow()));
            MOnDisplayChangeEvent* event = new MOnDisplayChangeEvent(true, QRectF(0, 0, 1, 1));
            QApplication::sendEvent(window, event);
        }
    }
}

void MCompositorNotificationSink::setupWindowTimer(MBanner *banner, const Notification &notification)
{
    // Create a timer for the banner; make it a child of the banner so it is destroyed automatically
    QTimer *timer = new QTimer(banner);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->setSingleShot(true);
    timer->setProperty("notificationId", notification.notificationId());
    timer->start(notification.timeout());
}

void MCompositorNotificationSink::updateNotification(const Notification &notification)
{
    MBanner *banner = idToBanner.value(notification.notificationId());

    if (banner) {
        // Update the info banner widget
        banner->setTitle(infoBannerTitleText(notification.parameters()));
        banner->setSubtitle(infoBannerSubtitleText(notification.parameters()));
        banner->setIconID(determineIconId(notification.parameters()));

        // Update the info banner's actions
        updateActions(banner, notification.parameters());
    }
}

void MCompositorNotificationSink::removeNotification(uint notificationId)
{
    notificationDone(notificationId, false);
}

void MCompositorNotificationSink::notificationDone(uint notificationId, bool notificationIdInUse)
{
    MBanner *banner = idToBanner.take(notificationId);
    if (banner != NULL) {
        window->sceneManager()->disappearSceneWindow(banner);
        connect(banner, SIGNAL(disappeared()), this, SLOT(removeBannerFromCurrentBanners()));
    }
    if (notificationIdInUse) {
        idToBanner.insert(notificationId, NULL);
    }
}

void MCompositorNotificationSink::showOrHideWindow()
{
    if (window->isVisible() == currentBanners.isEmpty()) {
        // If the window is visible when there are no banners or is not visible when there are banners something needs to be done
        if (currentBanners.isEmpty()) {
            // If there are no banners to be shown the window should not be visible
            window->hide();
        } else {
            // If there is at least one banner to show the window should be visible
            window->show();
        }
    }
}

void MCompositorNotificationSink::addLatestBannerToWindow()
{
    if (!currentBanners.isEmpty()) {
        // The latest banner should be shown
        MBanner *banner = currentBanners.last();
        window->sceneManager()->appearSceneWindow(banner, MSceneWindow::DestroyWhenDone);
        updateWindowMask();
        disconnect(window, SIGNAL(displayEntered()), this, SLOT(addLatestBannerToWindow()));
    } else {
        // If the window timer has timed out before displayEntered() was sent there is no banner anymore and the window should just be hidden
        showOrHideWindow();
    }
}

void MCompositorNotificationSink::removeBannerFromCurrentBanners()
{
    MBanner *banner = qobject_cast<MBanner *>(sender());

    if (banner != NULL) {
        currentBanners.removeAll(banner);
        showOrHideWindow();
    }
}

void MCompositorNotificationSink::timeout()
{
    QTimer *timer = qobject_cast<QTimer *>(sender());

    if (timer != NULL) {
        // Get the notification ID from the timer
        bool ok = false;
        uint notificationId = timer->property("notificationId").toUInt(&ok);

        if (ok) {
            // Remove the notification
            notificationDone(notificationId, true);
        }
    }
}

void MCompositorNotificationSink::setDisabled(bool disabled)
{
    sinkDisabled = disabled;
}

void MCompositorNotificationSink::updateWindowMask()
{
    // Set up window mask so that mouse events are passed on to lower widgets.
    if (!currentBanners.isEmpty()) {
        MBanner *banner = currentBanners.last();
        QSize  size = banner->preferredSize().toSize();
        QPoint origin;

        switch(window->sceneManager()->orientationAngle()) {
        case M::Angle90:
            size.transpose();
            origin.setX(window->width() - size.width() - banner->pos().y());
            origin.setY(banner->pos().x());
            break;
        case M::Angle270:
            size.transpose();
            origin.setX(banner->pos().y());
            origin.setY(-banner->pos().x());
            break;
        case M::Angle180:
            origin.setY(window->height() - size.height() - banner->pos().y());
            origin.setX(-banner->pos().x());
            break;
        default:
            origin = banner->pos().toPoint();
            break;
        }
        window->setMask(QRegion(QRect(origin, size), QRegion::Rectangle));
    }
}

void MCompositorNotificationSink::clearWindowMask()
{
    window->clearMask();
}

void MCompositorNotificationSink::changeNotificationPreviewMode()
{
    QVariant gconfValue = notificationPreviewMode->value();
    if (gconfValue.isValid() && (gconfValue.type() == QVariant::Bool)) {
        allPreviewsDisabled = !gconfValue.toBool();
    }
}

