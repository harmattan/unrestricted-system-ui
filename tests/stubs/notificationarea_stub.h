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
#include <MBanner>

class NotificationAreaStub : public StubBase
{
public:
    virtual void notificationAreaConstructor(NotificationArea *notificationArea, QGraphicsItem *parent, bool notificationsClickable);
    virtual void notificationAreaDestructor();
    virtual void addNotification(MBanner &notification);
    virtual void moveNotificationToTop(MBanner &notification);
    virtual void removeNotification(MBanner &notification);
    virtual void removeAllRemovableBanners();
    virtual void setHonorPrivacySetting(bool honor);
    virtual void setNotificationManagerInterface(NotificationManagerInterface &notificationManagerInterface);
};

void NotificationAreaStub::notificationAreaConstructor(NotificationArea *notificationArea, QGraphicsItem *parent, bool notificationsClickable)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<NotificationArea *>(notificationArea));
    params.append(new Parameter<QGraphicsItem *>(parent));
    params.append((new Parameter<bool>(notificationsClickable)));
    stubMethodEntered("notificationAreaConstructor", params);
}

void NotificationAreaStub::notificationAreaDestructor()
{
    stubMethodEntered("notificationAreaDestructor");
}

void NotificationAreaStub::addNotification(MBanner &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MBanner &>(notification));
    stubMethodEntered("addNotification", params);
}

void NotificationAreaStub::moveNotificationToTop(MBanner &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MBanner &>(notification));
    stubMethodEntered("moveNotificationToTop",params);
}

void NotificationAreaStub::removeNotification(MBanner &notification)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MBanner &>(notification));
    stubMethodEntered("removeNotification", params);
}

void NotificationAreaStub::removeAllRemovableBanners()
{
    stubMethodEntered("removeAllRemovableBanners");
}

void NotificationAreaStub::setHonorPrivacySetting(bool honor)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(honor));
    stubMethodEntered("setHonorPrivacySetting", params);
}

void NotificationAreaStub::setNotificationManagerInterface(NotificationManagerInterface &notificationManagerInterface)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<NotificationManagerInterface *>(&notificationManagerInterface));
    stubMethodEntered("setNotificationManagerInterface", params);
}

NotificationAreaStub gDefaultNotificationAreaStub;
NotificationAreaStub *gNotificationAreaStub = &gDefaultNotificationAreaStub;

NotificationArea::NotificationArea(QGraphicsItem *parent, bool notificationsClickable)
{
    gNotificationAreaStub->notificationAreaConstructor(this, parent, notificationsClickable);
}

NotificationArea::~NotificationArea()
{
    gNotificationAreaStub->notificationAreaDestructor();
}

void NotificationArea::addNotification(MBanner &notification)
{
    gNotificationAreaStub->addNotification(notification);
}

void NotificationArea::moveNotificationToTop(MBanner &notification)
{
    gNotificationAreaStub->moveNotificationToTop(notification);
}

void NotificationArea::removeNotification(MBanner &notification)
{
    gNotificationAreaStub->removeNotification(notification);
}

void NotificationArea::removeAllRemovableBanners()
{
    gNotificationAreaStub->removeAllRemovableBanners();
}

void NotificationArea::setHonorPrivacySetting(bool honor)
{
    gNotificationAreaStub->setHonorPrivacySetting(honor);
}

void NotificationArea::setNotificationManagerInterface(NotificationManagerInterface &notificationManagerInterface)
{
    gNotificationAreaStub->setNotificationManagerInterface(notificationManagerInterface);
}

#endif
