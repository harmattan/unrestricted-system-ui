/***************************************************************************
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

#include "notificationstatusindicatorsink.h"
#include <notificationwidgetparameterfactory.h>
#include <notificationmanagerinterface.h>

NotificationStatusIndicatorSink::NotificationStatusIndicatorSink(QObject *parent) :
    NotificationSink(parent)
{
}

NotificationStatusIndicatorSink::~NotificationStatusIndicatorSink()
{
    foreach (NotificationData *data, dataForNotification.values()) {
        delete data;
    }

    foreach (NotificationData *data, dataForGroup.values()) {
        delete data;
    }
}

void NotificationStatusIndicatorSink::addNotification(const Notification &notification)
{
    // Only application events are shown in the status indicator
    if (notification.type() == Notification::ApplicationEvent) {
        int notificationId = notification.notificationId();
        int groupId = notification.groupId();
        QString iconId = notification.parameters().value("statusAreaIconId").toString();
        int priority = notification.parameters().value("priority").toInt();

        if (iconId.isEmpty()) {
            iconId = "icon-s-status-notifier";
        }

        updateNotificationData(notificationId, iconId, priority);

        if (!groupForNotification.contains(notificationId)) {
            // Add the notification to the known notifications
            groupForNotification.insert(notificationId, groupId);

            if (groupId > 0) {
                addNotificationToGroup(notificationId, groupId);
            } else {
                addStandaloneNotification(notificationId, iconId, priority);
            }
        }
    }
}

void NotificationStatusIndicatorSink::removeNotification(uint notificationId)
{
    int groupId = groupForNotification.value(notificationId, -1);
    if (groupId != -1 && notificationsForGroup.contains(groupId)) {
        removeNotificationFromGroup(notificationId, groupId);
    } else {
        removeStandaloneNotification(notificationId);
    }

    // Remove the notification from the known notifications
    groupForNotification.remove(notificationId);
}

void NotificationStatusIndicatorSink::addGroup(uint groupId, const NotificationParameters &parameters)
{
    QString iconId = parameters.value("statusAreaIconId").toString();
    int priority = parameters.value("priority").toInt();

    if (iconId.isEmpty()) {
        iconId = "icon-s-status-notifier";
    }

    updateGroupData(groupId, iconId, priority);

    // Add the group to the known groups if it's not known yet
    if (!notificationsForGroup.contains(groupId)) {
        notificationsForGroup.insert(groupId, QList<int>());

        // Create data for the group
        NotificationData *data = new NotificationData;
        data->first = iconId;
        data->second = priority;
        dataForGroup.insert(groupId, data);
    }
}

void NotificationStatusIndicatorSink::removeGroup(uint groupId)
{
    NotificationData *data = dataForGroup.value(groupId, NULL);
    if (data != NULL) {
        if (!notificationsForGroup.value(groupId).isEmpty()) {
            // The notification group had notifications in it: check whether this changes the most important notification
            datas.removeAll(data);
            checkMostImportantNotification();
        }
        dataForGroup.remove(groupId);
        delete data;
    }

    // Remove the group from the known groups
    notificationsForGroup.remove(groupId);
}

void NotificationStatusIndicatorSink::addStandaloneNotification(int notificationId, const QString &iconId, int priority)
{
    // Create data for the notification
    NotificationData *data = new NotificationData;
    data->first = iconId;
    data->second = priority;
    dataForNotification.insert(notificationId, data);
    datas.prepend(data);
    checkMostImportantNotification();
}

void NotificationStatusIndicatorSink::removeStandaloneNotification(int notificationId)
{
    // Standalone notification: remove the notification data
    NotificationData *data = dataForNotification.value(notificationId, NULL);
    if (data != NULL) {
        datas.removeAll(data);
        checkMostImportantNotification();
        dataForNotification.remove(notificationId);
        delete data;
    }
}

void NotificationStatusIndicatorSink::addNotificationToGroup(int notificationId, int groupId)
{
    // Add the notification to the list of notifications for the group
    notificationsForGroup[groupId].append(notificationId);

    NotificationData *data = dataForGroup.value(groupId, NULL);
    if (data != NULL) {
        if (notificationsForGroup[groupId].count() == 1) {
            // First notification in the group: check whether this changes the most important notification
            datas.prepend(data);
            checkMostImportantNotification();
        }
    }
}

void NotificationStatusIndicatorSink::removeNotificationFromGroup(int notificationId, int groupId)
{
    // Notification belongs to a group: remove the notification from the group
    notificationsForGroup[groupId].removeAll(notificationId);

    if (notificationsForGroup[groupId].isEmpty()) {
        // No more notifications in the group: check whether this changes the most important notification
        NotificationData *data = dataForGroup.value(groupId, NULL);
        if (data != NULL) {
            datas.removeAll(data);
            checkMostImportantNotification();
        }
    }
}

void NotificationStatusIndicatorSink::updateNotificationData(int notificationId, const QString &iconId, int priority)
{
    NotificationData *data = dataForNotification.value(notificationId, NULL);
    if (data != NULL) {
        // Icon ID set: update data
        data->first = iconId;
        data->second = priority;
        checkMostImportantNotification();
    }
}

void NotificationStatusIndicatorSink::updateGroupData(int groupId, const QString &iconId, int priority)
{
    NotificationData *data = dataForGroup.value(groupId, NULL);
    if (data != NULL) {
        // Icon ID set: update data
        data->first = iconId;
        data->second = priority;
        checkMostImportantNotification();
    }
}

void NotificationStatusIndicatorSink::checkMostImportantNotification()
{
    QString iconId;
    int highestPriority = -1;

    foreach (NotificationData *data, datas) {
        if (data->second > highestPriority) {
            iconId = data->first;
            highestPriority = data->second;
        }
    }

    emit iconIdChanged(iconId);
}
