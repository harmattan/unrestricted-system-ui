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

static const QString NOTIFICATION_PREVIEW_ENABLED = "/desktop/meego/notifications/previews_enabled";

MCompositorNotificationSink::MCompositorNotificationSink() :
        sinkDisabled(false), allPreviewsDisabled(false),
        window(new MWindow())
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
        window->show();
        currentNotification = notification;

        // Create and set up info banner widget
        MBanner *infoBanner = createInfoBanner(currentNotification);
        setupWindowTimer(infoBanner);

        // Keep track of the mapping between IDs and private notification information classes
        idToBanner.insert(currentNotification.notificationId(), infoBanner);
        emit notificationAdded(currentNotification);

        //TODO: Remove sending fake displaychange events when setTranslucentBackground bug is solved
        connect(window, SIGNAL(displayEntered()), this, SLOT(addInfoBannerToWindow()));
        MOnDisplayChangeEvent* event = new MOnDisplayChangeEvent(true, QRectF(0,0,1,1));
        QApplication::sendEvent(window,event);
    }
}

void MCompositorNotificationSink::setupWindowTimer(MBanner *infoBanner)
{
    // Create a timer for the info banner; make it a child of the infobanner so it is destroyed automatically
    QTimer *timer = new QTimer(infoBanner);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->setSingleShot(true);
    timer->setProperty("notificationId", currentNotification.notificationId());
    timer->start(currentNotification.timeout());
}

void MCompositorNotificationSink::updateNotification(const Notification &notification)
{
    MBanner *infoBanner = idToBanner.value(notification.notificationId());

    if (infoBanner) {
        // Update the info banner widget
        infoBanner->setTitle(infoBannerTitleText(notification.parameters()));
        infoBanner->setSubtitle(infoBannerSubtitleText(notification.parameters()));
        infoBanner->setIconID(determineIconId(notification.parameters()));

        // Update the info banner's actions
        updateActions(infoBanner, notification.parameters());
    }
}

void MCompositorNotificationSink::removeNotification(uint notificationId)
{
    notificationDone(notificationId, false);
}

void MCompositorNotificationSink::notificationDone(uint notificationId, bool notificationIdInUse)
{
    MBanner *infoBanner = idToBanner.take(notificationId);
    if (infoBanner != NULL) {
        window->sceneManager()->disappearSceneWindow(infoBanner);
        connect(infoBanner, SIGNAL(disappeared()), this, SLOT(hideWindow()));
    }
    if (notificationIdInUse) {
        idToBanner.insert(notificationId, NULL);
    }
}

void MCompositorNotificationSink::hideWindow()
{
    window->hide();
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
    MBanner *infoBanner = idToBanner.value(currentNotification.notificationId());
    if(infoBanner != NULL) {
        QSize  size = infoBanner->preferredSize().toSize();
        QPoint origin;

        switch(window->sceneManager()->orientationAngle()) {
        case M::Angle90:
            size.transpose();
            origin.setX(window->width() - size.width() - infoBanner->pos().y());
            origin.setY(infoBanner->pos().x());
            break;
        case M::Angle270:
            size.transpose();
            origin.setX(infoBanner->pos().y());
            origin.setY(-infoBanner->pos().x());
            break;
        case M::Angle180:
            origin.setY(window->height() - size.height() - infoBanner->pos().y());
            origin.setX(-infoBanner->pos().x());
            break;
        default:
            origin = infoBanner->pos().toPoint();
            break;
        }
        window->setMask(QRegion(QRect(origin, size), QRegion::Rectangle));
    }
}

void MCompositorNotificationSink::clearWindowMask()
{
    window->clearMask();
}

void MCompositorNotificationSink::addInfoBannerToWindow()
{
    MBanner *infoBanner = idToBanner.value(currentNotification.notificationId());
    if (infoBanner != NULL) {
        window->sceneManager()->appearSceneWindow(infoBanner, MSceneWindow::DestroyWhenDone);
        updateWindowMask();

        disconnect(window, SIGNAL(displayEntered()), this, SLOT(addInfoBannerToWindow()));
    } else {
        // If the window timer has timed out before displayEntered() was sent there is no banner anymore and the window should just be hidden
        window->hide();
    }
}

void MCompositorNotificationSink::changeNotificationPreviewMode()
{
    QVariant gconfValue = notificationPreviewMode->value();
    if (gconfValue.isValid() && (gconfValue.type() == QVariant::Bool)) {
        allPreviewsDisabled = !gconfValue.toBool();
    }
}
