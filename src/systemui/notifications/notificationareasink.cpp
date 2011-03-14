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
#include "notificationmanagerinterface.h"
#include <MBanner>
#include <MRemoteAction>

NotificationAreaSink::NotificationAreaSink() : WidgetNotificationSink()
{
    connect(this, SIGNAL(privacySettingChanged(bool)), this, SLOT(applyPrivacySetting(bool)));
}

NotificationAreaSink::~NotificationAreaSink()
{
    // Destroy the remaining notifications
    foreach(MBanner *banner, notificationIdToMBanner) {
        delete banner;
    }

    // Destroy the remaining groups
    foreach(MBanner *banner, groupIdToMBanner) {
        delete banner;
    }
}

void NotificationAreaSink::updateCurrentNotifications(NotificationManagerInterface &notificationManagerInterface)
{
    QList <NotificationGroup> groups = notificationManagerInterface.groups();
    foreach(NotificationGroup group, groups) {
        addGroup(group.groupId(), group.parameters());
    }

    QList <Notification> notifications = notificationManagerInterface.notifications();
    foreach(Notification notification, notifications) {
        addNotification(notification);
    }
}

void NotificationAreaSink::setupInfoBanner(MBanner *infoBanner, const NotificationParameters &parameters)
{
    // Don't allow the scene manager to destroy the banner
    infoBanner->setManagedManually(true);

    // Since the notification area only shows event banners, set the style name
    infoBanner->setStyleName("FullEventBanner");

    // In the full event banners the order of the title and the subtitle needs to be reversed
    infoBanner->setProperty(TITLE_TEXT_PROPERTY, infoBannerSubtitleText(parameters));
    infoBanner->setProperty(SUBTITLE_TEXT_PROPERTY, infoBannerTitleText(parameters));
    updateTitles(infoBanner);

    // Catch clicks and send signal that a banner was clicked.
    connect(infoBanner, SIGNAL(clicked()), this, SIGNAL(bannerClicked()), Qt::QueuedConnection);
}

void NotificationAreaSink::updateNotification(MBanner *infoBanner, const NotificationParameters &parameters)
{
    // Update the info banner widget. In the full event banners the order of the title and the subtitle needs to be reversed.
    infoBanner->setProperty(TITLE_TEXT_PROPERTY, infoBannerSubtitleText(parameters));
    infoBanner->setProperty(SUBTITLE_TEXT_PROPERTY, infoBannerTitleText(parameters));
    infoBanner->setProperty(GENERIC_TEXT_PROPERTY, infoBannerGenericText(parameters));
    infoBanner->setProperty(USER_REMOVABLE_PROPERTY, determineUserRemovability(parameters));
    infoBanner->setBannerTimeStamp(QDateTime::fromTime_t(parameters.value("timestamp").toUInt()));

    updatePrefixForNotificationGroupBannerTimestamp(infoBanner, parameters.value("count").toUInt());

    // Update the info banner's image, titles and actions
    updateImage(infoBanner, parameters);
    updateTitles(infoBanner);
    updateActions(infoBanner, parameters);
}

void NotificationAreaSink::addGroup(uint groupId, const NotificationParameters &parameters)
{
    MBanner *infoBanner = groupIdToMBanner.value(groupId);
    if (infoBanner != NULL) {
        // If the info banner is already in the map, only update it
        updateNotification(infoBanner, parameters);
    } else {
        // Keep track of the mapping between IDs and info banners
        MBanner *infoBanner = createInfoBanner(Notification::ApplicationEvent, groupId, parameters);
        setupInfoBanner(infoBanner, parameters);
        groupIdToMBanner.insert(groupId, infoBanner);
    }

    notificationGroupParameters[groupId] = parameters;
}

void NotificationAreaSink::removeGroup(uint groupId)
{
    if (groupIdToMBanner.contains(groupId)) {
        MBanner *infoBanner = groupIdToMBanner.take(groupId);

        // If the group is already visible, send signal to remove it
        if (infoBanner && infoBanner->parentItem()) {
            // Remove from the notification area
            emit removeNotification(*infoBanner);
        }

        // Destroy
        delete infoBanner;
        deleteGroupFromNotificationCountOfGroup(groupId);
    }

    notificationGroupParameters.remove(groupId);
}

void NotificationAreaSink::removeGroupBanner(uint groupId)
{
    if (groupIdToMBanner.contains(groupId)) {
        MBanner *infoBanner = groupIdToMBanner.value(groupId);

        // If the group is already visible, send signal to remove it
        if (infoBanner && infoBanner->parentItem()) {
            // Remove from the notification area
            emit removeNotification(*infoBanner);
            groupIdToMBanner.insert(groupId,NULL);
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
    updatePrefixForNotificationGroupBannerTimestamp(groupIdToMBanner.value(notification.groupId()), notificationIdCount);
    // Update the notification id to group id hash
    notificationIdToGroupId.insert(notification.notificationId(), notification.groupId());
}

MBanner* NotificationAreaSink::createGroupBanner(uint groupId, const NotificationParameters &parameters)
{
    MBanner *infoBanner = createInfoBanner(Notification::ApplicationEvent, groupId, parameters);
    setupInfoBanner(infoBanner, parameters);
    infoBanner->setParentItem(NULL);
    groupIdToMBanner.insert(groupId, infoBanner);
    return infoBanner;
}

void NotificationAreaSink::addNotificationToGroup(const Notification &notification)
{
    uint groupId = notification.groupId();

    // Does the group id exist ?
    if(groupIdToMBanner.contains(groupId)) {
        // Yes it does, so get the banner associated with this.
        MBanner *infoBanner = groupIdToMBanner.value(groupId);

        if (infoBanner == NULL) {
            // Seems like the infoBanner is NULL. So it means that the group banner was removed, but group is alive. Recreate the banner.
            infoBanner = createGroupBanner(groupId, notificationGroupParameters.value(groupId));
        }
        infoBanner->setBannerTimeStamp(QDateTime::fromTime_t(notification.parameters().value("timestamp").toUInt()));

        if (infoBanner != NULL && infoBanner->parentItem() == NULL) {
            // Add the group to the notification area if this is the first notification to the group
            emit addNotification(*infoBanner);
        } else {
            emit notificationAddedToGroup(*infoBanner);
        }
        increaseNotificationCountOfGroup(notification);
    }
}

void NotificationAreaSink::addStandAloneNotification(const Notification &notification)
{
    // The notification is not in a group, add it as such to notification area
    MBanner *infoBanner = notificationIdToMBanner.value(notification.notificationId());
    if (infoBanner != NULL) {
        // If the notification is already in the map, only update it
        updateNotification(infoBanner, notification.parameters());
    } else {
        infoBanner = createInfoBanner(notification);
        setupInfoBanner(infoBanner, notification.parameters());
        notificationIdToMBanner.insert(notification.notificationId(), infoBanner);
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
    if (notificationIdToMBanner.contains(notificationId)) {
        MBanner *infoBanner = notificationIdToMBanner.take(notificationId);

        if (infoBanner != NULL) {
            if (!notificationIdToMBanner.keys(infoBanner).isEmpty()) {
                // The info banner represents a single notification: Remove the notification ID mapping
                foreach(uint key, notificationIdToMBanner.keys(infoBanner)) {
                    notificationIdToMBanner.remove(key);
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
    updatePrefixForNotificationGroupBannerTimestamp(groupIdToMBanner.value(groupId),
        notificationCountOfGroup.value(groupId));
    return  notificationIdsCount;
}

void NotificationAreaSink::applyPrivacySetting(bool)
{
    foreach (MBanner *banner, notificationIdToMBanner) {
        updateTitles(banner);
    }
    foreach (MBanner *banner, groupIdToMBanner) {
        if (banner != NULL) {
            updateTitles(banner);
        }
    }
}

void NotificationAreaSink::updatePrefixForNotificationGroupBannerTimestamp(MBanner *infoBanner, uint count)
{
    if (count > 1) {
        infoBanner->setPrefixTimeStamp(qtTrId("qtn_noti_timestamp_latest"));
    } else {
        infoBanner->setPrefixTimeStamp("");
    }
}
