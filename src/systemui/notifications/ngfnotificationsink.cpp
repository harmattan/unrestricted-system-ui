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

#include "ngfnotificationsink.h"
#include <MApplication>
#include "feedbackparameterfactory.h"
#include "eventtypestore.h"
#include <QSettings>
#include "genericnotificationparameterfactory.h"
#include "sysuid.h"
#include "ngfadapter.h"
#include "notificationmanager.h"

NGFNotificationSink::NGFNotificationSink()
{
    adapter = new NGFAdapter;
}

NGFNotificationSink::~NGFNotificationSink()
{
    delete adapter;
}

QString NGFNotificationSink::determineFeedbackId(const NotificationParameters &parameters)
{
    QString feedbackId = parameters.value(FeedbackParameterFactory::feedbackIdKey()).toString();
    if (feedbackId.isEmpty()) {
        const EventTypeStore &store = notificationManager().eventTypeStore();
        const QSettings *settings = store.settingsForEventType(parameters.value(GenericNotificationParameterFactory::eventTypeKey()).toString());
        if (settings) {
            feedbackId = settings->value(FeedbackParameterFactory::feedbackIdKey()).toString();
        }
    }
    return feedbackId;
}

void NGFNotificationSink::addNotification(const Notification &notification)
{
    if (canAddNotification(notification)) {
        QString feedbackId = determineFeedbackId(notification.parameters());
        if (!feedbackId.isEmpty()) {
            uint eventId = adapter->play(feedbackId);
            if (eventId > 0) {
                idToEventId.insert(notification.notificationId(), eventId);
            }
        }
    }
}

void NGFNotificationSink::removeNotification(uint notificationId)
{
    if (idToEventId.contains(notificationId)) {
        uint eventId = idToEventId.take(notificationId);
        if (eventId > 0) {
            adapter->stop(eventId);
        }
    }
}

NotificationManager &NGFNotificationSink::notificationManager()
{
    return Sysuid::sysuid()->notificationManager();
}
