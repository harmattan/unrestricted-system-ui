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

#include "notificationarea.h"
#include "notificationareasink.h"
#include "notificationmanagerinterface.h"
#include <MBanner>

NotificationArea::NotificationArea(QGraphicsItem *parent, bool notificationsClickable) :
    MWidgetController(new NotificationAreaModel, parent),
    notificationAreaSink(new NotificationAreaSink)
{
    // Connect notification signals
    notificationAreaSink->setNotificationsClickable(notificationsClickable);
    connect(notificationAreaSink, SIGNAL(addNotification(MBanner &)), this, SLOT(addNotification(MBanner &)));
    connect(notificationAreaSink, SIGNAL(removeNotification(MBanner &)), this, SLOT(removeNotification(MBanner &)));
    connect(notificationAreaSink, SIGNAL(notificationAddedToGroup(MBanner &)), this, SLOT(moveNotificationToTop(MBanner &)));
    connect(notificationAreaSink, SIGNAL(bannerClicked()), this, SIGNAL(bannerClicked()));
    connect(this, SIGNAL(notificationRemovalRequested(uint)), notificationAreaSink, SIGNAL(notificationRemovalRequested(uint)));
    connect(this, SIGNAL(notificationGroupClearingRequested(uint)), notificationAreaSink, SIGNAL(notificationGroupClearingRequested(uint)));
}

NotificationArea::~NotificationArea()
{
    delete notificationAreaSink;
}

void NotificationArea::setNotificationManagerInterface(NotificationManagerInterface &notificationManagerInterface)
{
    QObject *notificationManager = notificationManagerInterface.qObject();
    connect(notificationManager, SIGNAL(groupUpdated(uint, const NotificationParameters &)), notificationAreaSink, SLOT(addGroup(uint, const NotificationParameters &)));
    connect(notificationManager, SIGNAL(groupRemoved(uint)), notificationAreaSink, SLOT(removeGroup(uint)));
    connect(notificationManager, SIGNAL(notificationRemoved(uint)), notificationAreaSink, SLOT(removeNotification(uint)));
    connect(notificationManager, SIGNAL(notificationRestored(const Notification &)), notificationAreaSink, SLOT(addNotification(const Notification &)));
    connect(notificationManager, SIGNAL(notificationUpdated(const Notification &)), notificationAreaSink, SLOT(addNotification(const Notification &)));
    connect(notificationAreaSink, SIGNAL(notificationRemovalRequested(uint)), notificationManager, SLOT(removeNotification(uint)));
    connect(notificationAreaSink, SIGNAL(notificationGroupClearingRequested(uint)), notificationManager, SLOT(removeNotificationsInGroup(uint)));
    notificationAreaSink->updateCurrentNotifications(notificationManagerInterface);
}

void NotificationArea::moveNotificationToTop(MBanner &notification)
{
    QList<MBanner *> banners(model()->banners());
    if(banners.count() != 0) {
        banners.move(banners.indexOf(&notification), 0);
        model()->setBanners(banners);
    }
}

void NotificationArea::addNotification(MBanner &notification)
{
    // Put the notification into the model of the notification area
    QList<MBanner *> banners(model()->banners());
    banners.push_front(&notification);
    model()->setBanners(banners);
}

void NotificationArea::removeNotification(MBanner &notification)
{
    // Remove the notification from the model of the notification area
    QList<MBanner *> banners(model()->banners());
    banners.removeOne(&notification);
    model()->setBanners(banners);
    notification.setParentItem(NULL);
}

void NotificationArea::removeAllRemovableBanners()
{
    foreach(MBanner *banner, model()->banners()) {
        // Remove all user removable banners
        if (banner->property(WidgetNotificationSink::USER_REMOVABLE_PROPERTY).toBool()) {
            // Get the notification ID from the info banner
            bool ok = false;
            uint notificationId = banner->property(WidgetNotificationSink::NOTIFICATION_ID_PROPERTY).toUInt(&ok);
            if (ok) {
                // Request notification removal
                emit notificationRemovalRequested(notificationId);
            } else {
                uint groupId = banner->property(WidgetNotificationSink::GROUP_ID_PROPERTY).toUInt(&ok);
                if (ok) {
                    // Request notification group clearing
                    emit notificationGroupClearingRequested(groupId);
                }
            }
        }
    }
}

void NotificationArea::setHonorPrivacySetting(bool honor)
{
    notificationAreaSink->setHonorPrivacySetting(honor);
}
