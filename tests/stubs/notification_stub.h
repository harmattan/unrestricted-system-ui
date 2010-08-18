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

#ifndef NOTIFICATION_STUB
#define NOTIFICATION_STUB

#include "notification.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotificationStub : public StubBase {
public:
    virtual void NotificationConstructor();
    virtual void NotificationConstructor(uint notificationId, uint groupId, uint userId, const NotificationParameters &parameters, Notification::NotificationType type, int timeout);
    virtual void NotificationDestructor();
    virtual uint notificationId() const;
    virtual uint userId() const;
    virtual uint groupId() const;
    virtual const NotificationParameters & parameters() const;
    virtual void setParameters(const NotificationParameters &parameters);
    virtual Notification::NotificationType type() const;
    virtual int timeout() const;
};


// 2. IMPLEMENT STUB
void NotificationStub::NotificationConstructor() {

}
void NotificationStub::NotificationConstructor(uint notificationId, uint groupId, uint userId, const NotificationParameters &parameters, Notification::NotificationType type, int timeout) {
    Q_UNUSED(notificationId);
    Q_UNUSED(groupId);
    Q_UNUSED(userId);
    Q_UNUSED(parameters);
    Q_UNUSED(type);
    Q_UNUSED(timeout);
}
void NotificationStub::NotificationDestructor() {

}
uint NotificationStub::notificationId() const {
    stubMethodEntered("notificationId");
    return stubReturnValue<uint>("notificationId");
}

uint NotificationStub::userId() const {
    stubMethodEntered("userId");
    return stubReturnValue<uint>("userId");
}

uint NotificationStub::groupId() const {
    stubMethodEntered("groupId");
    return stubReturnValue<uint>("groupId");
}

const NotificationParameters & NotificationStub::parameters() const {
    stubMethodEntered("parameters");
    return stubReturnValueNoDefault<const NotificationParameters &>("parameters");
}

void NotificationStub::setParameters(const NotificationParameters &parameters) {
    QList<ParameterBase*> params;
    params.append( new Parameter<const NotificationParameters & >(parameters));
    stubMethodEntered("setParameters",params);
}

Notification::NotificationType NotificationStub::type() const {
    stubMethodEntered("type");
    return stubReturnValue<Notification::NotificationType>("type");
}

int NotificationStub::timeout() const {
    stubMethodEntered("timeout");
    return stubReturnValue<int>("timeout");
}

// 3. CREATE A STUB INSTANCE
NotificationStub gDefaultNotificationStub;
NotificationStub* gNotificationStub = &gDefaultNotificationStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
Notification::Notification() {
    gNotificationStub->NotificationConstructor();
}

Notification::Notification(uint notificationId, uint groupId, uint userId, const NotificationParameters &parameters,
                           Notification::NotificationType type, int timeout)
{
    gNotificationStub->NotificationConstructor(notificationId, groupId, userId, parameters, type, timeout);
}


Notification::~Notification() {
    gNotificationStub->NotificationDestructor();
}

uint Notification::notificationId() const {
    return gNotificationStub->notificationId();
}

uint Notification::userId() const {
    return gNotificationStub->userId();
}

uint Notification::groupId() const {
    return gNotificationStub->groupId();
}

Notification::NotificationType Notification::type() const {
    return gNotificationStub->type();
}

const NotificationParameters & Notification::parameters() const {
    return gNotificationStub->parameters();
}

void Notification::setParameters(const NotificationParameters &parameters) {
    gNotificationStub->setParameters(parameters);
}

int Notification::timeout() const {
    return gNotificationStub->timeout();
}

QDBusArgument &operator<<(QDBusArgument &argument, const Notification &)
{
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, Notification &)
{
    return argument;
}

#endif
