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
#ifndef NOTIFICATIONAREASINK_STUB
#define NOTIFICATIONAREASINK_STUB

#include "notificationareasink.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotificationAreaSinkStub : public StubBase
{
public:
    virtual void NotificationAreaSinkConstructor();
    virtual void NotificationAreaSinkDestructor();
    virtual void addGroup(uint groupId, const NotificationParameters &parameters);
    virtual void removeGroup(uint groupId);
    virtual void addNotification(const Notification &notification);
    virtual void removeNotification(uint notificationId);
    virtual void addNotification(MBanner &notification);
    virtual void removeNotification(MBanner &notification);
    virtual void setupInfoBanner(MBanner *infoBanner, const NotificationParameters &parameters);
    virtual void updateNotification(MBanner *dn, const NotificationParameters &parameters);
    virtual void removeGroupBanner(uint groupId);
    virtual void applyPrivacySetting(bool privacyEnabled);
    virtual void updateCurrentNotifications(NotificationManagerInterface &notificationManagerInterface);
};

// 2. IMPLEMENT STUB
void NotificationAreaSinkStub::NotificationAreaSinkConstructor()
{

}
void NotificationAreaSinkStub::NotificationAreaSinkDestructor()
{

}
void NotificationAreaSinkStub::addGroup(uint groupId, const NotificationParameters &parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    params.append(new Parameter<const NotificationParameters & >(parameters));
    stubMethodEntered("addGroup", params);
}

void NotificationAreaSinkStub::removeGroup(uint groupId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    stubMethodEntered("removeGroup", params);
}

void NotificationAreaSinkStub::addNotification(const Notification &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const Notification & >(notification));
    stubMethodEntered("addNotification", params);
}

void NotificationAreaSinkStub::removeNotification(uint notificationId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationId));
    stubMethodEntered("removeNotification", params);
}

void NotificationAreaSinkStub::addNotification(MBanner &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MBanner & >(notification));
    stubMethodEntered("addNotification", params);
}

void NotificationAreaSinkStub::removeNotification(MBanner &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MBanner & >(notification));
    stubMethodEntered("removeNotification", params);
}

void NotificationAreaSinkStub::setupInfoBanner(MBanner *infoBanner, const NotificationParameters &parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MBanner * >(infoBanner));
    params.append(new Parameter<const NotificationParameters & >(parameters));
    stubMethodEntered("setupInfoBanner", params);
}

void NotificationAreaSinkStub::updateNotification(MBanner *dn, const NotificationParameters &parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MBanner * >(dn));
    params.append(new Parameter<const NotificationParameters & >(parameters));
    stubMethodEntered("updateNotification", params);
}

void NotificationAreaSinkStub::removeGroupBanner(uint groupId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    stubMethodEntered("removeGroup", params);
}

void NotificationAreaSinkStub::applyPrivacySetting(bool privacyEnabled)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(privacyEnabled));
    stubMethodEntered("applyPrivacySetting", params);
}

void NotificationAreaSinkStub::updateCurrentNotifications(NotificationManagerInterface &notificationManagerInterface)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<NotificationManagerInterface * >(&notificationManagerInterface));
    stubMethodEntered("updateCurrentNotifications", params);
}


// 3. CREATE A STUB INSTANCE
NotificationAreaSinkStub gDefaultNotificationAreaSinkStub;
NotificationAreaSinkStub *gNotificationAreaSinkStub = &gDefaultNotificationAreaSinkStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
NotificationAreaSink::NotificationAreaSink()
{
    gNotificationAreaSinkStub->NotificationAreaSinkConstructor();
}

NotificationAreaSink::~NotificationAreaSink()
{
    gNotificationAreaSinkStub->NotificationAreaSinkDestructor();
}

void NotificationAreaSink::addGroup(uint groupId, const NotificationParameters &parameters)
{
    gNotificationAreaSinkStub->addGroup(groupId, parameters);
}

void NotificationAreaSink::removeGroup(uint groupId)
{
    gNotificationAreaSinkStub->removeGroup(groupId);
}

void NotificationAreaSink::addNotification(const Notification &notification)
{
    gNotificationAreaSinkStub->addNotification(notification);
}

void NotificationAreaSink::removeNotification(uint notificationId)
{
    gNotificationAreaSinkStub->removeNotification(notificationId);
}

void NotificationAreaSink::setupInfoBanner(MBanner *infoBanner, const NotificationParameters &parameters)
{
    gNotificationAreaSinkStub->setupInfoBanner(infoBanner, parameters);
}

void NotificationAreaSink::updateNotification(MBanner *dn, const NotificationParameters &parameters)
{
    gNotificationAreaSinkStub->updateNotification(dn, parameters);
}

void NotificationAreaSink::removeGroupBanner(uint groupId)
{
    return gNotificationAreaSinkStub->removeGroupBanner(groupId);
}

void NotificationAreaSink::applyPrivacySetting(bool privacyEnabled)
{
    return gNotificationAreaSinkStub->applyPrivacySetting(privacyEnabled);
}

void NotificationAreaSink::updateCurrentNotifications(NotificationManagerInterface &notificationManagerInterface)
{
    gNotificationAreaSinkStub->updateCurrentNotifications(notificationManagerInterface);
}
#endif
