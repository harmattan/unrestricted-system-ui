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

#include "notifiernotificationsink.h"
#include "genericnotificationparameterfactory.h"
#include "ngfadapter.h"

const QString NotifierNotificationSink::NOTIFIER_NGF_ID = "notifier";

NotifierNotificationSink::NotifierNotificationSink(QObject *parent) :
    NotificationSink(parent),
    ngfAdapter(new NGFAdapter),
    additionsDisabled(false),
    ngfEventId(0),
    notifierEnabled(false)
{
}

NotifierNotificationSink::~NotifierNotificationSink()
{
    delete ngfAdapter;
}

void NotifierNotificationSink::addNotification(const Notification &notification)
{
    if (!additionsDisabled && notification.type() == Notification::ApplicationEvent && isUnseen(notification)) {
        applicationEventIds.insert(notification.notificationId());
        if(applicationEventIds.count() == 1) {
            setNotifierEnabled(true);
        }
    }
}

void NotifierNotificationSink::removeNotification(uint notificationId)
{
    applicationEventIds.remove(notificationId);
    if(applicationEventIds.isEmpty()) {
        setNotifierEnabled(false);
    }
}

void NotifierNotificationSink::clearSink()
{
    applicationEventIds.clear();
    setNotifierEnabled(false);
}

void NotifierNotificationSink::disableNotificationAdditions(bool disable)
{
    additionsDisabled = disable;
}

bool NotifierNotificationSink::isUnseen(const Notification &notification)
{
    NotificationParameters parameters = notification.parameters();
    QVariant unseenVariant = parameters.value(GenericNotificationParameterFactory::unseenKey());
    return unseenVariant.toBool();
}

void NotifierNotificationSink::setNotifierEnabled(bool enabled)
{
    emit notifierSinkActive(enabled);
    notifierEnabled = enabled;
}
