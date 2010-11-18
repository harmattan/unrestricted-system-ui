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
#include <MScene>
#include <MOnDisplayChangeEvent>
#include <QApplication>
#include <MGConfItem>
#include <QTimer>

static const QString NOTIFICATION_PREVIEW_ENABLED = "/desktop/meego/notifications/previews_enabled";

MCompositorNotificationSink::MCompositorNotificationSink() :
        sinkDisabled(false),
        allPreviewsDisabled(false),
        window(new MWindow)
{
    window->setTranslucentBackground(true);
    window->setAttribute(Qt::WA_X11DoNotAcceptFocus);
    window->setAttribute(Qt::WA_X11NetWmWindowTypeNotification);
    window->setObjectName("MCompositorNotificationSinkWindow");
    window->setWindowTitle("Notification");
    window->setProperty("followsCurrentApplicationWindowOrientation", true);

    notificationPreviewMode = new MGConfItem(NOTIFICATION_PREVIEW_ENABLED, this);
    changeNotificationPreviewMode();
    connect(notificationPreviewMode, SIGNAL(valueChanged()), this, SLOT(changeNotificationPreviewMode()));

    // Clear the mask for the duration of orientation change, because the mask is not rotated along with the notification
    connect(window->sceneManager(), SIGNAL(orientationAboutToChange(M::Orientation)), this, SLOT(clearWindowMask()));
    connect(window->sceneManager(), SIGNAL(orientationChangeFinished(M::Orientation)), this, SLOT(updateWindowMask()));
    connect(window, SIGNAL(displayEntered()), this, SLOT(addOldestBannerToWindow()));

    // Setup the timer which makes the banner disappear
    connect(&bannerTimer, SIGNAL(timeout()), this, SLOT(disappearCurrentBanner()));
    bannerTimer.setSingleShot(true);
}

MCompositorNotificationSink::~MCompositorNotificationSink()
{
    // Destroy the remaining notifications
    foreach(uint id, idToBanner.keys()) {
        removeNotification(id);
    }
    delete window;
}

MBanner *MCompositorNotificationSink::currentBanner()
{
    foreach (QGraphicsItem *item, window->sceneManager()->scene()->items()) {
        // The scene will have only one banner visible at any given time
        MBanner *banner = dynamic_cast<MBanner*>(item);
        if(banner) {
            return banner;
        }
    }

    return NULL;
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
        banner->setStyleName(banner->objectName());
        banner->setIconID(determinePreviewIconId(notification.parameters()));
        banner->setProperty("notificationId", notification.notificationId());
        banner->setProperty("timeout", notification.timeout());

        // Connect slots to cleanup disappearing banner and handle next banner when disappeared
        // The banner sends disappear either by timeout or by user clicking on it
        connect(banner, SIGNAL(disappearing()), this, SLOT(currentBannerDone()));
        connect(banner, SIGNAL(disappeared()), this, SLOT(addOldestBannerToWindow()));

        // Keep track of the mapping between IDs and private notification information classes
        idToBanner.insert(notification.notificationId(), banner);
        currentBanners.prepend(banner);
        emit notificationAdded(notification);

        if (!window->isVisible()) {
            window->show();
            // Calling hide() causes the ondisplay property of the window to change with delay,
            // so if show() is called fast enough after hiding, ondisplay is never changed to false and we
            // never get a new displayEntered signal, so go to the slot immediately in that case.
            if(window->isOnDisplay()) {
                addOldestBannerToWindow();
            }
        }
    }
}

void MCompositorNotificationSink::updateNotification(const Notification &notification)
{
    MBanner *banner = idToBanner.value(notification.notificationId());

    if (banner) {
        // Update the info banner widget
        banner->setTitle(infoBannerTitleText(notification.parameters()));
        banner->setSubtitle(infoBannerSubtitleText(notification.parameters()));
        banner->setIconID(determinePreviewIconId(notification.parameters()));
        banner->setProperty("notificationId", notification.notificationId());
        banner->setProperty("timeout", notification.timeout());

        // Update the info banner's actions
        updateActions(banner, notification.parameters());
    }
}

void MCompositorNotificationSink::removeNotification(uint notificationId)
{
    MBanner *banner = idToBanner.take(notificationId);

    if (banner) {
        if(currentBanner() == banner) {
            bannerTimer.stop();
            window->sceneManager()->disappearSceneWindow(banner);
        } else {
            currentBanners.removeAll(banner);
            delete banner;
        }
    }
}

void MCompositorNotificationSink::disappearCurrentBanner()
{
    MBanner *banner = currentBanner();
    if(banner) {
        window->sceneManager()->disappearSceneWindow(banner);
    }
}

void MCompositorNotificationSink::currentBannerDone()
{
    MBanner *banner = currentBanner();
    if(banner) {
        int id = banner->property("notificationId").toInt();
        idToBanner.remove(id);
        currentBanners.removeAll(banner);
    }
}

void MCompositorNotificationSink::addOldestBannerToWindow()
{
    if (!currentBanners.isEmpty()) {
        // The latest banner should be shown
        MBanner *banner = currentBanners.takeLast();
        window->sceneManager()->appearSceneWindow(banner, MSceneWindow::DestroyWhenDone);
        bannerTimer.start(banner->property("timeout").toInt());
        updateWindowMask(banner);
    } else {
        // No more banners exist to be shown -> hide the window
        window->hide();
    }
}

void MCompositorNotificationSink::setDisabled(bool disabled)
{
    sinkDisabled = disabled;
}

void MCompositorNotificationSink::updateWindowMask()
{
    MBanner *banner = currentBanner();
    if(banner) {
        updateWindowMask(banner);
    }
}

void MCompositorNotificationSink::updateWindowMask(MBanner* banner)
{
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

QString MCompositorNotificationSink::determinePreviewIconId(const NotificationParameters &parameters)
{
    QString previewIconID = parameters.value(NotificationWidgetParameterFactory::previewIconIdKey()).toString();
    if (previewIconID.isEmpty()) {
        previewIconID = determineIconId(parameters);
    }
    return previewIconID;
}
