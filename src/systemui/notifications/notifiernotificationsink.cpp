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

NotifierNotificationSink::NotifierNotificationSink() :
    ngfAdapter(new NGFAdapter),
    notificationCount(0),
    additionsDisabled(false),
    ngfEventId(0),
    notifierEnabled(false)
{
#ifdef HAVE_QMSYSTEM
    connect(&displayState, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)),
        this, SLOT(updateStatusOfLedFeedback()));
#endif
}

NotifierNotificationSink::~NotifierNotificationSink()
{
    delete ngfAdapter;
}

void NotifierNotificationSink::addNotification(const Notification &notification)
{
    if(additionsDisabled) {
        return;
    }
    if (notification.type() == Notification::SystemEvent) {
        // System notifications are not shown by the notifier sink
        systemNotificationIds.insert(notification.notificationId());
        return;
    }
    if(isUnseen(notification)) {
        notificationCount++;
        if(notificationCount == 1) {
            enableNotifierSink();
        }
    }
}

void NotifierNotificationSink::removeNotification(uint notificationId)
{
    if(systemNotificationIds.contains(notificationId)) {
        systemNotificationIds.remove(notificationId);
        return;
    }
    if(notificationCount > 0) {
        notificationCount--;
        if(notificationCount == 0) {
            disableNotifierSink();
        }
    }
}

void NotifierNotificationSink::clearSink()
{
    notificationCount = 0;
    disableNotifierSink();
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

void NotifierNotificationSink::enableNotifierSink()
{
    setNotifierSinkEnabled(true);
}

void NotifierNotificationSink::disableNotifierSink()
{
    setNotifierSinkEnabled(false);
}

void NotifierNotificationSink::setNotifierSinkEnabled(bool enabled)
{
    emit notifierSinkActive(enabled);
    notifierEnabled = enabled;
    updateStatusOfLedFeedback();
}

void NotifierNotificationSink::updateStatusOfLedFeedback()
{
#ifdef HAVE_QMSYSTEM
    if (displayState.get() == MeeGo::QmDisplayState::Off && notifierEnabled) {
        ngfEventId = ngfAdapter->play(NOTIFIER_NGF_ID);
    } else if (ngfEventId != 0) {
        ngfAdapter->stop(ngfEventId);
        ngfEventId = 0;
    }
#endif
}
