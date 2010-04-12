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

#include "mfeedbacknotificationsink.h"
#include <MApplication>
#include <MFeedbackPlayer>
#include "feedbackparameterfactory.h"
#include "eventtypestore.h"
#include <QSettings>
#include "genericnotificationparameterfactory.h"
#include "sysuid.h"

MFeedbackNotificationSink::MFeedbackNotificationSink()
{
}

MFeedbackNotificationSink::~MFeedbackNotificationSink()
{
}

QString MFeedbackNotificationSink::determineFeedbackId(const NotificationParameters &parameters)
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

void MFeedbackNotificationSink::addNotification(const Notification &notification)
{
    if (!canAddNotification(notification)) return;

    QString feedbackId = determineFeedbackId(notification.parameters());
    if (!feedbackId.isEmpty() && MApplication::feedbackPlayer() != NULL) {
        MApplication::feedbackPlayer()->play(feedbackId);
    }
}

void MFeedbackNotificationSink::removeNotification(uint notificationId)
{
    Q_UNUSED(notificationId)
}

NotificationManager &MFeedbackNotificationSink::notificationManager()
{
    return Sysuid::sysuid()->notificationManager();
}
