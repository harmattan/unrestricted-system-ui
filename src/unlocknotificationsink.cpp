/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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
#include "unlocknotificationsink.h"
#include "unlockmissedevents.h"
#include "notifications/genericnotificationparameterfactory.h"
#include "notifications/notificationwidgetparameterfactory.h"

/*
 * Used event-types:
 * Please check the /usr/share/meegotouch/notifications/eventtypes directory for
 * filenames, they can be changed. Oh, they can be!...
 */
#define EVENT_EMAIL "email.arrived"

// This one might be obsolete
#define EVENT_MSG "x-nokia.message.arrived"
#define EVENT_SMS "x-nokia.messaging.sms"
#define EVENT_MMS "x-nokia.messaging.mms"

// This one might be obsolete
#define EVENT_CALL  "x-nokia.call"
#define CALL_MISSED "x-nokia.call.missed"

// This one might be obsolete...
#define IM_RECEIVED    "im.received"
#define MESSAGING_IM   "x-nokia.messaging.im"

// These notifications will be filtered out.
#define EVENT_BATTERY  "x-nokia.battery"

#define DEBUG
#include "debug.h"

UnlockNotificationSink::UnlockNotificationSink () :
    m_enabled (false)
{
    SYS_DEBUG ("");
}

void
UnlockNotificationSink::setLockedState (bool islocked)
{
    if (islocked == m_enabled)
        return;

    SYS_DEBUG ("islocked = %s", SYS_BOOL (islocked));

    // Set the internal state
    m_enabled = islocked;

    UnlockMissedEvents::getInstance ().clearAll ();
}

/*!
 * \returns true for the notification that are going to be shown in the
 *   lockscreen.
 */
bool
UnlockNotificationSink::canAddNotification (
        const Notification &notification)
{
    Q_UNUSED (notification);

    // Not locked state... skip
    if (m_enabled == false)
        return false;

    /*
     * Filtering by notification event-type. Add all the event types that will
     * not be shown in the lockscreen here.
     */
    bool retval = true;
    QString event_type =
        notification.parameters ().value (
            GenericNotificationParameterFactory::eventTypeKey ()).toString ();

    if (event_type == EVENT_BATTERY)
        retval = false;

    return retval;
}

void
UnlockNotificationSink::addNotification (const Notification &notification)
{
    // not locked state... skip
    // XXX:
    // Why i called with addNotification when canAddNotification returns false?
    if (m_enabled == false)
        return;

    QString lastSummary;
    UnlockMissedEvents::Types type = UnlockMissedEvents::NotifyOther;

    QString event_type =
        notification.parameters ().value (
            GenericNotificationParameterFactory::eventTypeKey ()).toString ();

    if (event_type == EVENT_EMAIL)
        type = UnlockMissedEvents::NotifyEmail;
    else if (event_type == EVENT_MSG ||
            event_type == EVENT_SMS || 
            event_type == EVENT_MMS)
        type = UnlockMissedEvents::NotifySms;
    else if (event_type == EVENT_CALL ||
            event_type == CALL_MISSED)
        type = UnlockMissedEvents::NotifyCall;
    else if (event_type == IM_RECEIVED ||
            event_type == MESSAGING_IM)
        type = UnlockMissedEvents::NotifyMessage;

    if (! notification.parameters ().value
           (NotificationWidgetParameterFactory::summaryKey ()).isNull ())
    {
        lastSummary = notification.parameters ().value (
                NotificationWidgetParameterFactory::summaryKey ()).toString ();

        SYS_DEBUG ("summary = \"%s\"", SYS_STR (lastSummary));
    }

    UnlockMissedEvents::getInstance ().addNotification (type, lastSummary);
}

void
UnlockNotificationSink::removeNotification (uint notificationId)
{
    Q_UNUSED (notificationId);

    // No operation here...
}

