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
#ifndef NOTIFICATIONAREA_STUB_H
#define NOTIFICATIONAREA_STUB_H

#include <stubbase.h>
#include <notificationarea.h>
#include <MInfoBanner>

class NotificationAreaStub : public StubBase
{
public:
    virtual void notificationAreaConstructor(NotificationArea *notificationArea, MWidget *parent);
    virtual void notificationAreaDestructor();
    virtual void addNotification(MInfoBanner &notification);
    virtual void moveNotificationToTop(MInfoBanner &notification);
    virtual void removeNotification(MInfoBanner &notification);
};

void NotificationAreaStub::notificationAreaConstructor(NotificationArea *notificationArea, MWidget *parent)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<NotificationArea *>(notificationArea));
    params.append(new Parameter<MWidget *>(parent));
    stubMethodEntered("notificationAreaConstructor", params);
}

void NotificationAreaStub::notificationAreaDestructor()
{
    stubMethodEntered("notificationAreaDestructor");
}

void NotificationAreaStub::addNotification(MInfoBanner &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MInfoBanner &>(notification));
    stubMethodEntered("addNotification", params);
}

void NotificationAreaStub::moveNotificationToTop(MInfoBanner &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MInfoBanner &>(notification));
    stubMethodEntered("moveNotificationToTop",params);
}

void NotificationAreaStub::removeNotification(MInfoBanner &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MInfoBanner &>(notification));
    stubMethodEntered("removeNotification", params);
}

NotificationAreaStub gDefaultNotificationAreaStub;
NotificationAreaStub *gNotificationAreaStub = &gDefaultNotificationAreaStub;

NotificationArea::NotificationArea(MWidget *parent)
{
    gNotificationAreaStub->notificationAreaConstructor(this, parent);
}

NotificationArea::~NotificationArea()
{
    gNotificationAreaStub->notificationAreaDestructor();
}

void NotificationArea::addNotification(MInfoBanner &notification)
{
    gNotificationAreaStub->addNotification(notification);
}

void NotificationArea::moveNotificationToTop(MInfoBanner &notification)
{
    gNotificationAreaStub->moveNotificationToTop(notification);
}

void NotificationArea::removeNotification(MInfoBanner &notification)
{
    gNotificationAreaStub->removeNotification(notification);
}

#endif
