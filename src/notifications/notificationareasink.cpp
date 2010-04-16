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

#include "notificationareasink.h"
#include "notificationwidgetparameterfactory.h"

#include <MInfoBanner>
#include <MRemoteAction>

NotificationAreaSink::NotificationAreaSink()
{
}

NotificationAreaSink::~NotificationAreaSink()
{
    // Destroy the remaining notifications
    foreach(MInfoBanner * n, notificationIdToMInfoBanner) {
        delete n;
    }

    // Destroy the remaining groups
    foreach(MInfoBanner * n, groupIdToMInfoBanner) {
        delete n;
    }
}

void NotificationAreaSink::setupInfoBanner(MInfoBanner *infoBanner)
{
    // Don't allow the scene manager to destroy the banner
    infoBanner->setManagedManually(true);

    // Catch clicks and send signal that a banner was clicked.
    connect(infoBanner, SIGNAL(clicked()), this, SIGNAL(bannerClicked()), Qt::QueuedConnection);
}

MInfoBanner *NotificationAreaSink::updateNotification(MInfoBanner *infoBanner, const NotificationParameters &parameters)
{
    // Update the info banner widget
    infoBanner->setImageID(parameters.value(NotificationWidgetParameterFactory::imageIdKey()).toString());
    infoBanner->setBodyText(parameters.value(NotificationWidgetParameterFactory::bodyKey()).toString());
    infoBanner->setIconID(determineIconId(parameters));
    infoBanner->setProperty(USER_REMOVABLE_PROPERTY, determineUserRemovability(parameters));

    // Update the info banner's actions
    updateActions(infoBanner, parameters);
    return infoBanner;
}

void NotificationAreaSink::addGroup(uint groupId, const NotificationParameters &parameters)
{
    MInfoBanner *infoBanner = groupIdToMInfoBanner.value(groupId);
    if (infoBanner != NULL) {
        // If the info banner is already in the map, only update it
        updateNotification(infoBanner, parameters);
    } else {
        // Keep track of the mapping between IDs and info banners
        MInfoBanner *infoBanner = createInfoBanner(MInfoBanner::Event, groupId, parameters);
        setupInfoBanner(infoBanner);
        groupIdToMInfoBanner.insert(groupId, infoBanner);
    }
}

void NotificationAreaSink::removeGroup(uint groupId)
{
    if (groupIdToMInfoBanner.contains(groupId)) {
        MInfoBanner *infoBanner = groupIdToMInfoBanner.take(groupId);

        // If the group is already visible, send signal to remove it
        if (infoBanner && infoBanner->parentItem()) {
            // Remove from the notification area
            emit removeNotification(*infoBanner);
        }

        // Destroy
        delete infoBanner;
        deleteGroupFromNotificationCountOfGroup(groupId);
    }
}

void NotificationAreaSink::removeGroupBanner(uint groupId)
{
    if (groupIdToMInfoBanner.contains(groupId)) {
        MInfoBanner *infoBanner = groupIdToMInfoBanner.value(groupId);

        // If the group is already visible, send signal to remove it
        if (infoBanner && infoBanner->parentItem()) {
            // Remove from the notification area
            emit removeNotification(*infoBanner);
            groupIdToMInfoBanner.insert(groupId,NULL);
            // Destroy
            delete infoBanner;
            deleteGroupFromNotificationCountOfGroup(groupId);
        }
    }
}

void NotificationAreaSink::deleteGroupFromNotificationCountOfGroup(const uint groupId)
{
    notificationCountOfGroup.remove(groupId);
    QList<uint> notificationIdList = notificationIdToGroupId.keys(groupId);
    foreach(uint notificationId, notificationIdList ) {
        notificationIdToGroupId.remove(notificationId);
    }
}

void NotificationAreaSink::increaseNotificationCountOfGroup(const Notification &notification)
{
    // Update the groupid to count of notification ids hash
    uint notificationIdCount = notificationCountOfGroup.value(notification.groupId());
    notificationCountOfGroup.insert(notification.groupId(), ++notificationIdCount);
    // Update the notification id to group id hash
    notificationIdToGroupId.insert(notification.notificationId(), notification.groupId());
}

MInfoBanner* NotificationAreaSink::reviveGroupBanner(const Notification &notification)
{
    MInfoBanner *infoBanner = createInfoBanner(MInfoBanner::Event, notification.groupId(), notification.parameters());
    setupInfoBanner(infoBanner);
    infoBanner->setParentItem(NULL);
    groupIdToMInfoBanner.insert(notification.groupId(), infoBanner);
    return infoBanner;
}

void NotificationAreaSink::addNotificationToGroup(const Notification &notification)
{
    // Does the group id exist ?
    if(groupIdToMInfoBanner.contains(notification.groupId())) {
        increaseNotificationCountOfGroup(notification);
        // Yes it does, so get the banner associated with this.
        MInfoBanner *infoBanner = groupIdToMInfoBanner.value(notification.groupId());

        if(infoBanner == NULL) {
            // Seems like the infoBanner is NULL. So it means that the group banner was removed, but group is alive. Revive the banner.
            infoBanner = reviveGroupBanner(notification);
        }

        if(infoBanner != NULL && infoBanner->parentItem() == NULL) {
            // Add the group to the notification area if this is the first notification to the group
            emit addNotification(*infoBanner);
        } else {
            emit notificationAddedToGroup(*infoBanner);
        }
    }
}

void NotificationAreaSink::addStandAloneNotification(const Notification &notification)
{
    // The notification is not in a group, add it as such to notification area
    MInfoBanner *infoBanner = notificationIdToMInfoBanner.value(notification.notificationId());
    if (infoBanner != NULL) {
        // If the notification is already in the map, only update it
        updateNotification(infoBanner, notification.parameters());
    } else {
        infoBanner = createInfoBanner(notification);
        setupInfoBanner(infoBanner);
        notificationIdToMInfoBanner.insert(notification.notificationId(), infoBanner);
        // Add to the notification area
        emit addNotification(*infoBanner);
    }
}

void NotificationAreaSink::addNotification(const Notification &notification)
{
    // Only application events are shown in the notification area
    if (notification.type() == Notification::ApplicationEvent) {
        if (notification.groupId() > 0) {
            addNotificationToGroup(notification);
        } else {
            addStandAloneNotification(notification);
        }
    }
}

void NotificationAreaSink::removeNotification(uint notificationId)
{
    if (notificationIdToMInfoBanner.contains(notificationId)) {
        MInfoBanner *infoBanner = notificationIdToMInfoBanner.take(notificationId);

        if (infoBanner != NULL) {
            if (!notificationIdToMInfoBanner.keys(infoBanner).isEmpty()) {
                // The info banner represents a single notification: Remove the notification ID mapping
                foreach(uint key, notificationIdToMInfoBanner.keys(infoBanner)) {
                    notificationIdToMInfoBanner.remove(key);
                }
            }
            // Remove from the notification area
            emit removeNotification(*infoBanner);

            // Destroy
            delete infoBanner;
        }
    }
    // If notifications in the banner are gone then delete the banner. Dont remove the group id.
    if(notificationIdToGroupId.contains(notificationId)) {
        uint groupid = notificationIdToGroupId.value(notificationId);
        if(decreaseNotificationCountOfGroup(groupid) == 0) {
            removeGroupBanner(groupid);
        }
    }
}

uint NotificationAreaSink::decreaseNotificationCountOfGroup(uint groupId)
{
    uint notificationIdsCount = notificationCountOfGroup.value(groupId);
    notificationCountOfGroup.insert(groupId, --notificationIdsCount);
    return  notificationIdsCount;
}
