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

MCompositorNotificationSink::MCompositorNotificationSink() :
        sinkDisabled(false),
        window(new MWindow())
{
    connect(window, SIGNAL(displayEntered()), this, SLOT(addInfoBannerToWindow()));
    window->setTranslucentBackground(true);
    window->setAttribute(Qt::WA_X11NetWmWindowTypeNotification);
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
    if (!canAddNotification(notification)) return;
    if(sinkDisabled) {
        emit notificationAdded(notification);
        return;
    }

    if (idToBanner.contains(notification.notificationId())) {
        // The notification already exists so update it
        updateNotification(notification);
    } else {
        window->show();
        currentNotification = notification;
        // Create info banner widget
        MInfoBanner *infoBanner = createInfoBanner(currentNotification);

        setupWindowTimer(infoBanner);

        // Keep track of the mapping between IDs and private notification information classes
        idToBanner.insert(currentNotification.notificationId(), infoBanner);
        emit notificationAdded(currentNotification);
        //TODO: Remove sending fake displaychange events when setTranslucentBackground bug is solved
        MOnDisplayChangeEvent* event = new MOnDisplayChangeEvent(true, QRectF(0,0,1,1));
        QApplication::sendEvent(window,event);
    }
}

void MCompositorNotificationSink::setupWindowTimer(MInfoBanner *infoBanner)
{
    // Create a timer for the info banner; make it a child of the infobanner so it is destroyed automatically
    QTimer *timer = new QTimer(infoBanner);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->setProperty("notificationId", currentNotification.notificationId());
    timer->start(currentNotification.timeout());
}

void MCompositorNotificationSink::updateNotification(const Notification &notification)
{
    MInfoBanner *infoBanner = idToBanner.value(notification.notificationId());

    if (infoBanner) {
        // Update the info banner widget
        infoBanner->setImageID(notification.parameters().value(NotificationWidgetParameterFactory::imageIdKey()).toString());
        infoBanner->setBodyText(infoBannerBodyText(notification.parameters()));
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
    MInfoBanner *infoBanner = idToBanner.take(notificationId);
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

void MCompositorNotificationSink::addInfoBannerToWindow()
{
    MInfoBanner *infoBanner = idToBanner.value(currentNotification.notificationId());
    if (infoBanner != NULL) {
        window->sceneManager()->appearSceneWindow(infoBanner, MSceneWindow::DestroyWhenDone);
    }
}
