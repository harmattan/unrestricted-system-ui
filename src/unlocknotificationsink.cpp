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

// Used event-types:
#define EVENT_EMAIL "email.arrived"
#define EVENT_MSG   "x-nokia.message.arrived"
#define EVENT_CALL  "x-nokia.call"
#define EVENT_IM    "im.received"

#undef DEBUG
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

    // Set the internal state
    m_enabled = islocked;

    UnlockMissedEvents::getInstance ().clearAll ();
}

bool
UnlockNotificationSink::canAddNotification (const Notification &notification)
{
    // Not locked state... skip
    if (m_enabled == false)
        return false;

// FIXME: Need to be determined what type of notification should
//        be visible in unlockscreen... For now only email, sms, 
//        call and instant message will be shown...
    bool retval = false;

    QString event_type =
        notification.parameters ().value (
            GenericNotificationParameterFactory::eventTypeKey ()).toString ();

    if ((event_type == EVENT_EMAIL) ||
        (event_type == EVENT_MSG)   ||
        (event_type == EVENT_CALL)  ||
        (event_type == EVENT_IM))
        retval = true;

    return retval;
}

void
UnlockNotificationSink::addNotification (const Notification &notification)
{
    QString lastSummary;
    UnlockMissedEvents::Types type = UnlockMissedEvents::NotifyOther;

    QString event_type =
        notification.parameters ().value (
            GenericNotificationParameterFactory::eventTypeKey ()).toString ();

    if (event_type == EVENT_EMAIL)
        type = UnlockMissedEvents::NotifyEmail;
    else if (event_type == EVENT_MSG)
        type = UnlockMissedEvents::NotifySms;
    else if (event_type == EVENT_CALL)
        type = UnlockMissedEvents::NotifyCall;
    else if (event_type == EVENT_IM)
        type = UnlockMissedEvents::NotifyMessage;

    if (! notification.parameters ().value
           (NotificationWidgetParameterFactory::bodyKey ()).isNull ())
    {
        // Extracting summary part from the body text...
        QString body = notification.parameters ().value (
            NotificationWidgetParameterFactory::bodyKey ()).toString ();

        // NotificationParameters makes the body-text from the summary and body
        // part of the notification, so here i have to extract only the summary:
        if (body.startsWith ("<p><b>") && (body.indexOf ("</b></p>") != -1))
        {
            lastSummary = body.mid (6, body.indexOf ("</b></p>") - 6);
            SYS_DEBUG ("summary = \"%s\"", SYS_STR (lastSummary));
        }
    }

    // FIXME: if lastSummary is empty, at least some text eg.: E-mail arrived
    // should be shown in unlockscreen window

    UnlockMissedEvents::getInstance ().addNotification (type, lastSummary);
}

void
UnlockNotificationSink::removeNotification (uint notificationId)
{
    Q_UNUSED (notificationId);

    // No operation here...
}

