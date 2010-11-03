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

#include <QDBusArgument>

#include "notification.h"
#include "notificationparameters.h"
#include "notificationgroup.h"
#include "genericnotificationparameterfactory.h"
#include "notificationwidgetparameterfactory.h"

#include "mnotificationproxy.h"

MNotificationProxy::MNotificationProxy()
{
}

MNotificationProxy::MNotificationProxy(const Notification &notification)
{
    notificationId = notification.notificationId();
    groupId = notification.groupId();

    NotificationParameters p = notification.parameters();

    eventType =
        p.value(GenericNotificationParameterFactory::eventTypeKey())
         .toString();

    summary =
        p.value(NotificationWidgetParameterFactory::summaryKey())
         .toString();

    body =
        p.value(NotificationWidgetParameterFactory::bodyKey())
         .toString();

    imageId =
        p.value(NotificationWidgetParameterFactory::imageIdKey())
         .toString();

    action =
        p.value(NotificationWidgetParameterFactory::actionKey())
         .toString();

    count =
        p.value(GenericNotificationParameterFactory::countKey())
         .toUInt();
}

QDBusArgument &operator<<(QDBusArgument &argument, const MNotificationProxy &notification)
{
    argument.beginStructure();
    argument << notification.notificationId;
    argument << notification.groupId;
    argument << notification.eventType;
    argument << notification.summary;
    argument << notification.body;
    argument << notification.imageId;
    argument << notification.action;
    argument << notification.count;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, MNotificationProxy &notification)
{
    argument.beginStructure();
    argument >> notification.notificationId;
    argument >> notification.groupId;
    argument >> notification.eventType;
    argument >> notification.summary;
    argument >> notification.body;
    argument >> notification.imageId;
    argument >> notification.action;
    argument >> notification.count;
    argument.endStructure();

    return argument;
}



MNotificationWithIdentifierProxy::MNotificationWithIdentifierProxy()
{
}

MNotificationWithIdentifierProxy::MNotificationWithIdentifierProxy(const Notification &notification) : MNotificationProxy(notification)
{
    const NotificationParameters &p = notification.parameters();

    identifier = p.value(GenericNotificationParameterFactory::identifierKey()).toString();
}

QDBusArgument &operator<<(QDBusArgument &argument, const MNotificationWithIdentifierProxy &notification)
{
    argument.beginStructure();
    argument << notification.notificationId;
    argument << notification.groupId;
    argument << notification.eventType;
    argument << notification.summary;
    argument << notification.body;
    argument << notification.imageId;
    argument << notification.action;
    argument << notification.count;
    argument << notification.identifier;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, MNotificationWithIdentifierProxy &notification)
{
    argument.beginStructure();
    argument >> notification.notificationId;
    argument >> notification.groupId;
    argument >> notification.eventType;
    argument >> notification.summary;
    argument >> notification.body;
    argument >> notification.imageId;
    argument >> notification.action;
    argument >> notification.count;
    argument >> notification.identifier;
    argument.endStructure();

    return argument;
}


MNotificationGroupProxy::MNotificationGroupProxy()
{
}

MNotificationGroupProxy::MNotificationGroupProxy(const NotificationGroup &group)
{
    groupId = group.groupId();

    NotificationParameters p = group.parameters();

    eventType =
        p.value(GenericNotificationParameterFactory::eventTypeKey())
         .toString();

    summary =
        p.value(NotificationWidgetParameterFactory::summaryKey())
         .toString();

    body =
        p.value(NotificationWidgetParameterFactory::bodyKey())
         .toString();

    imageId =
        p.value(NotificationWidgetParameterFactory::imageIdKey())
         .toString();

    action =
        p.value(NotificationWidgetParameterFactory::actionKey())
         .toString();

    count =
        p.value(GenericNotificationParameterFactory::countKey())
         .toUInt();
}

QDBusArgument &operator<<(QDBusArgument &argument, const MNotificationGroupProxy &group)
{
    argument.beginStructure();
    argument << group.groupId;
    argument << group.eventType;
    argument << group.summary;
    argument << group.body;
    argument << group.imageId;
    argument << group.action;
    argument << group.count;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, MNotificationGroupProxy &group)
{
    argument.beginStructure();
    argument >> group.groupId;
    argument >> group.eventType;
    argument >> group.summary;
    argument >> group.body;
    argument >> group.imageId;
    argument >> group.action;
    argument >> group.count;
    argument.endStructure();

    return argument;
}

MNotificationGroupWithIdentifierProxy::MNotificationGroupWithIdentifierProxy()
{
}

MNotificationGroupWithIdentifierProxy::MNotificationGroupWithIdentifierProxy(const NotificationGroup &group) : MNotificationGroupProxy(group)
{
    const NotificationParameters p = group.parameters();

    groupIdentifier = p.value(GenericNotificationParameterFactory::identifierKey()).toString();
}

QDBusArgument &operator<<(QDBusArgument &argument, const MNotificationGroupWithIdentifierProxy &group)
{
    argument.beginStructure();
    argument << group.groupId;
    argument << group.eventType;
    argument << group.summary;
    argument << group.body;
    argument << group.imageId;
    argument << group.action;
    argument << group.count;
    argument << group.groupIdentifier;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, MNotificationGroupWithIdentifierProxy &group)
{
    argument.beginStructure();
    argument >> group.groupId;
    argument >> group.eventType;
    argument >> group.summary;
    argument >> group.body;
    argument >> group.imageId;
    argument >> group.action;
    argument >> group.count;
    argument >> group.groupIdentifier;
    argument.endStructure();

    return argument;
}

