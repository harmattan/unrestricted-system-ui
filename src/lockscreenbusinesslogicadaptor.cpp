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

#include "QDBusInterface"
#include "lockscreenbusinesslogicadaptor.h"

#define DEBUG
#include "debug.h"

LockScreenBusinessLogicAdaptor::LockScreenBusinessLogicAdaptor (
        QObject                 *obj, 
        LockScreenBusinessLogic *logic) :
    QDBusAbstractAdaptor (obj),
    m_LockScreenBusinessLogic (logic)
{
    Q_ASSERT (logic != 0);
    connect (this, SIGNAL(delegateSetMissedEvents(int, int, int, int)),
            logic, SLOT(updateMissedEventAmounts(int, int, int, int)));

    connect (logic, SIGNAL(unlockConfirmed()),
            this, SLOT(unlockConfirmed ()));
}


void 
LockScreenBusinessLogicAdaptor::SetMissedEvents (
        int emails, 
        int messages, 
        int calls, 
        int im)
{
    SYS_DEBUG (
"\n*** emails    = %d"
"\n*** messages  = %d"
"\n*** calls     = %d"
"\n*** im        = %d",
    emails, messages, calls, im);

    emit delegateSetMissedEvents (emails, messages, calls, im);
}


/*!
 * \param service DBus service to call when unlock is performed.
 * \param path DBus path to call when unlock is performed.
 * \param interface DBus interface to call when unlock is performed.
 * \param method DBus method to call when unlock is performed.
 * \param mode The LockScreenBusinessLogicAdaptor::TkLockMode opcode.
 * \param silent Whether to show a notification or not (deprecated)
 * \param flicker Deprecated/not used
 * 
 * DBus method to show the lock screen ui or the event eater window.
 */
int 
LockScreenBusinessLogicAdaptor::tklock_open (
        const QString  &service,
        const QString  &path,
        const QString  &interface,
        const QString  &method,
        uint            mode,
        bool            silent,
        bool            flicker)
{
    Q_UNUSED(flicker);
    Q_UNUSED(silent);

#if 1
    SYS_DEBUG (
"--------------------------------------\n"
"\n*** service   = '%s'"
"\n*** path      = '%s'"
"\n*** interface = '%s'"
"\n*** method    = '%s'"
"\n*** u         = %u"
"\n*** silent    = %s"
"\n*** flicker   = %s",
            SYS_STR (service),
            SYS_STR (path),
            SYS_STR (interface),
            SYS_STR (method),
            mode,
            silent ? "true" : "false",
            flicker ? "true" : "false");
#endif
    m_MCECallbackService = service;
    m_MCECallbackPath = path;
    m_MCECallbackInterface = interface;
    m_MCECallbackMethod = method;

    switch (mode) {
        case TkLockModeNone:
            SYS_DEBUG ("### TkLockModeNone");
            break;
        
        case TkLockModeEnable:
            SYS_DEBUG ("### TkLockModeEnable");
	    m_LockScreenBusinessLogic->toggleScreenLockUI (false);
            break;

        case TkLockModeHelp:
            SYS_DEBUG ("### TkLockModeHelp");
            break;

        case TkLockModeSelect:
            SYS_DEBUG ("### TkLockModeSelect");
            break;

        case TkLockModeOneInput:
            SYS_DEBUG ("### TkLockModeOneInput");
	    m_LockScreenBusinessLogic->toggleEventEater (true);
            break;

        case TkLockEnableVisual:
            SYS_DEBUG ("### TkLockEnableVisual");
	    m_LockScreenBusinessLogic->toggleScreenLockUI (true);
            break;

        default:
            SYS_WARNING ("### Unknown mode");
            break;
    }

    SYS_DEBUG ("--------------------------------------\n");
    return (int) TkLockReplyOk;
}

/*!
 * \param silent Whether to show notificatio or not (deprecated).
 * 
 * DBus method that is called to hide the lock screen UI.
 */
int
LockScreenBusinessLogicAdaptor::tklock_close (
        bool   silent)
{
    Q_UNUSED (silent);

    m_LockScreenBusinessLogic->toggleScreenLockUI (false);

    return (int) TkLockReplyOk;
}

void 
LockScreenBusinessLogicAdaptor::unlockConfirmed ()
{
    QDBusInterface *dbusIf;
    SYS_DEBUG ("");

    dbusIf = new QDBusInterface (
            m_MCECallbackService, //"com.nokia.mce", 
            m_MCECallbackPath, //"/com/nokia/mce/request",
            m_MCECallbackInterface, //"com.nokia.mce.request",
            QDBusConnection::systemBus ());

    dbusIf->call (QDBus::NoBlock, 
            m_MCECallbackMethod, //QString ("tklock_callback"), 
            (int) TkLockReplyOk);

    delete dbusIf;
}

