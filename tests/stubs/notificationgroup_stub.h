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

#ifndef NOTIFICATIONGROUP_STUB
#define NOTIFICATIONGROUP_STUB

#include "notificationgroup.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotificationGroupStub : public StubBase {
public:
    virtual void NotificationGroupConstructor();
    virtual void NotificationGroupConstructor(uint groupId, uint userId, const NotificationParameters &parameters);
    virtual void NotificationGroupDestructor();
    virtual uint userId() const;
    virtual uint groupId() const;
    virtual const NotificationParameters & parameters() const;
    virtual void setParameters(const NotificationParameters &parameters);
};


// 2. IMPLEMENT STUB
void NotificationGroupStub::NotificationGroupConstructor() {
    stubMethodEntered("NotificationGroupConstructor");
}

void NotificationGroupStub::NotificationGroupConstructor(uint groupId, uint userId, const NotificationParameters &parameters) {
    QList<ParameterBase*> params;
    params.append(new Parameter<uint>(groupId));
    params.append(new Parameter<uint>(userId));
    params.append(new Parameter<const NotificationParameters>(parameters));
    stubMethodEntered("NotificationGroupConstructor", params);
}

void NotificationGroupStub::NotificationGroupDestructor() {
    stubMethodEntered("NotificationGroupDestructor");
}

uint NotificationGroupStub::userId() const {
    stubMethodEntered("userId");
    return stubReturnValue<uint>("userId");
}

uint NotificationGroupStub::groupId() const {
    stubMethodEntered("groupId");
    return stubReturnValue<uint>("groupId");
}

const NotificationParameters & NotificationGroupStub::parameters() const {
    stubMethodEntered("parameters");
    return stubReturnValueNoDefault<const NotificationParameters &>("parameters");
}

void NotificationGroupStub::setParameters(const NotificationParameters &parameters) {
    QList<ParameterBase*> params;
    params.append( new Parameter<const NotificationParameters & >(parameters));
    stubMethodEntered("setParameters",params);
}


// 3. CREATE A STUB INSTANCE
NotificationGroupStub gDefaultNotificationGroupStub;
NotificationGroupStub* gNotificationGroupStub = &gDefaultNotificationGroupStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
NotificationGroup::NotificationGroup() {
    gNotificationGroupStub->NotificationGroupConstructor();
}

NotificationGroup::NotificationGroup(uint groupId, uint userId, const NotificationParameters &parameters)
{
    gNotificationGroupStub->NotificationGroupConstructor(groupId, userId, parameters);
}


NotificationGroup::~NotificationGroup() {
    gNotificationGroupStub->NotificationGroupDestructor();
}

uint NotificationGroup::userId() const {
    return gNotificationGroupStub->userId();
}

uint NotificationGroup::groupId() const {
    return gNotificationGroupStub->groupId();
}

const NotificationParameters & NotificationGroup::parameters() const {
    return gNotificationGroupStub->parameters();
}

void NotificationGroup::setParameters(const NotificationParameters &parameters) {
    gNotificationGroupStub->setParameters(parameters);
}

QDBusArgument &operator<<(QDBusArgument &argument, const NotificationGroup &)
{
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, NotificationGroup &)
{
    return argument;
}

#endif
