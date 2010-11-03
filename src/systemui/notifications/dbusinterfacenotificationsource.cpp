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

#include "dbusinterfacenotificationsource.h"
#include "dbusinterfacenotificationsourceadaptor.h"
#include "mnotificationproxy.h"

Q_DECLARE_METATYPE(MNotificationProxy)
Q_DECLARE_METATYPE(MNotificationWithIdentifierProxy)
Q_DECLARE_METATYPE(MNotificationGroupProxy)
Q_DECLARE_METATYPE(MNotificationGroupWithIdentifierProxy)
Q_DECLARE_METATYPE(QList<MNotificationProxy>)
Q_DECLARE_METATYPE(QList<MNotificationWithIdentifierProxy>)
Q_DECLARE_METATYPE(QList<MNotificationGroupProxy>)
Q_DECLARE_METATYPE(QList<MNotificationGroupWithIdentifierProxy>)

DBusInterfaceNotificationSource::DBusInterfaceNotificationSource(NotificationManagerInterface &interface)
    : NotificationSource(interface)
{
    qDBusRegisterMetaType<Notification>();
    qDBusRegisterMetaType<QList<Notification> >();
    qDBusRegisterMetaType<NotificationGroup>();
    qDBusRegisterMetaType<QList<NotificationGroup> >();
    qDBusRegisterMetaType<NotificationParameters>();
    qDBusRegisterMetaType<MNotificationProxy>();
    qDBusRegisterMetaType<QList<MNotificationProxy> >();
    qDBusRegisterMetaType<MNotificationWithIdentifierProxy>();
    qDBusRegisterMetaType<QList<MNotificationWithIdentifierProxy> >();
    qDBusRegisterMetaType<MNotificationGroupProxy>();
    qDBusRegisterMetaType<QList<MNotificationGroupProxy> >();
    qDBusRegisterMetaType<MNotificationGroupWithIdentifierProxy>();
    qDBusRegisterMetaType<QList<MNotificationGroupWithIdentifierProxy> >();

    new DBusInterfaceNotificationSourceAdaptor(this);
}

uint DBusInterfaceNotificationSource::notificationUserId()
{
    return manager.notificationUserId();
}

uint DBusInterfaceNotificationSource::addNotification(uint notificationUserId, uint groupId, const QString &eventType)
{
    return manager.addNotification(notificationUserId, notificationParameters(eventType), groupId);
}

uint DBusInterfaceNotificationSource::addNotification(uint notificationUserId, uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return manager.addNotification(notificationUserId, notificationParameters(eventType, summary, body, action, imageURI, count), groupId);
}

uint DBusInterfaceNotificationSource::addNotification(uint notificationUserId, uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, const QString &identifier)
{
    return manager.addNotification(notificationUserId, notificationParameters(eventType, summary, body, action, imageURI, count, identifier), groupId);
}

bool DBusInterfaceNotificationSource::updateNotification(uint notificationUserId, uint notificationId, const QString &eventType)
{
    return manager.updateNotification(notificationUserId, notificationId, notificationParameters(eventType));
}

bool DBusInterfaceNotificationSource::updateNotification(uint notificationUserId, uint notificationId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return manager.updateNotification(notificationUserId, notificationId, notificationParameters(eventType, summary, body, action, imageURI, count));
}

bool DBusInterfaceNotificationSource::updateNotification(uint notificationUserId, uint notificationId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, const QString &identifier)
{
    return manager.updateNotification(notificationUserId, notificationId, notificationParameters(eventType, summary, body, action, imageURI, count, identifier));
}

bool DBusInterfaceNotificationSource::removeNotification(uint notificationUserId, uint notificationId)
{
    return manager.removeNotification(notificationUserId, notificationId);
}

uint DBusInterfaceNotificationSource::addGroup(uint notificationUserId, const QString &eventType)
{
    return manager.addGroup(notificationUserId, notificationParameters(eventType));
}

uint DBusInterfaceNotificationSource::addGroup(uint notificationUserId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return manager.addGroup(notificationUserId, notificationParameters(eventType, summary, body, action, imageURI, count));
}

uint DBusInterfaceNotificationSource::addGroup(uint notificationUserId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, const QString &identifier)
{
    return manager.addGroup(notificationUserId, notificationParameters(eventType, summary, body, action, imageURI, count, identifier));
}

bool DBusInterfaceNotificationSource::updateGroup(uint notificationUserId, uint groupId, const QString &eventType)
{
    return manager.updateGroup(notificationUserId, groupId, notificationParameters(eventType));
}

bool DBusInterfaceNotificationSource::updateGroup(uint notificationUserId, uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return manager.updateGroup(notificationUserId, groupId, notificationParameters(eventType, summary, body, action, imageURI, count));
}

bool DBusInterfaceNotificationSource::updateGroup(uint notificationUserId, uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, const QString &identifier)
{
    return manager.updateGroup(notificationUserId, groupId, notificationParameters(eventType, summary, body, action, imageURI, count, identifier));
}

bool DBusInterfaceNotificationSource::removeGroup(uint notificationUserId, uint groupId)
{
    return manager.removeGroup(notificationUserId, groupId);
}

QList<uint> DBusInterfaceNotificationSource::notificationIdList(uint notificationUserId)
{
    return manager.notificationIdList(notificationUserId);
}

QList<MNotificationProxy> DBusInterfaceNotificationSource::notificationList(uint notificationUserId)
{
    return manager.notificationList(notificationUserId);
}

QList<MNotificationWithIdentifierProxy> DBusInterfaceNotificationSource::notificationListWithIdentifiers(uint notificationUserId)
{
    return manager.notificationListWithIdentifiers(notificationUserId);
}

QList<MNotificationGroupProxy> DBusInterfaceNotificationSource::notificationGroupList(uint notificationUserId)
{
    return manager.notificationGroupList(notificationUserId);
}

QList<MNotificationGroupWithIdentifierProxy> DBusInterfaceNotificationSource::notificationGroupListWithIdentifiers(uint notificationUserId)
{
    return manager.notificationGroupListWithIdentifiers(notificationUserId);
}
