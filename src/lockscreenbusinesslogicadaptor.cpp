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

#include <QDBusInterface>
#include <QTimer>
#include "lockscreenbusinesslogicadaptor.h"

#undef DEBUG
#include "debug.h"

LockScreenBusinessLogicAdaptor::LockScreenBusinessLogicAdaptor (
        QObject                 *obj,
        LockScreenBusinessLogic *logic) :
    QDBusAbstractAdaptor (obj),
    m_LockScreenBusinessLogic (logic),
    m_CallbackDbusIf (0)
{
    Q_ASSERT (logic != 0);

    connect (logic, SIGNAL(unlockConfirmed ()),
            this, SLOT(unlockConfirmed ()));
}

LockScreenBusinessLogicAdaptor::~LockScreenBusinessLogicAdaptor ()
{
    if (m_CallbackDbusIf)
        delete m_CallbackDbusIf;
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
"---------------Start------------------\n"
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

    /*
     * Functions are called by singleShot because send a TkLockReplyOk
     * to MCE as soon as possible is mandatory...
     */
    switch (mode) {
        case TkLockModeNone:
            SYS_DEBUG ("### TkLockModeNone");
            break;

        case TkLockModeEnable:
            SYS_DEBUG ("### TkLockModeEnable");

            /*
             * Show the visual earliest...
             * in this way we can avoid the full-screen window
             * creation time when EnableVisual requested later...
             *
             * XXX: Previously window-creation when screen off
             *      caused X11 crashes... now it seems to work.
             */
            QTimer::singleShot (100, this, SLOT (enableVisual ()));
            break;

        case TkLockModeHelp:
            /*
             * Not used/deprecated.
             */
            SYS_DEBUG ("### TkLockModeHelp");
            break;

        case TkLockModeSelect:
            /*
             * Not used/deprecated.
             */
            SYS_DEBUG ("### TkLockModeSelect");
            break;

        case TkLockModeOneInput:
            SYS_DEBUG ("### TkLockModeOneInput");
            /*
             * The event eater supposed to consume exactly one event, hence the
             * name.
             */
            QTimer::singleShot (0, this, SLOT (enableEater ()));
            break;

        case TkLockEnableVisual:
            SYS_DEBUG ("### TkLockEnableVisual");
            /*
             * This mode is where we actually should show the screen to unlock
             * the screen.
             *
             * Calling again the enableVisual to raise the unlock-ui window.
             */
            QTimer::singleShot (0, this, SLOT (enableVisual ()));
            break;

        default:
            SYS_WARNING ("### Unknown mode");
            break;
    }

    SYS_DEBUG ("-------------End----------------------\n");
    return (int) TkLockReplyOk;
}

void
LockScreenBusinessLogicAdaptor::enableVisual ()
{
    SYS_DEBUG ("");
    m_LockScreenBusinessLogic->toggleScreenLockUI (true);
}

void
LockScreenBusinessLogicAdaptor::enableEater ()
{
    SYS_DEBUG ("");
    m_LockScreenBusinessLogic->toggleEventEater (true);
}

void
LockScreenBusinessLogicAdaptor::hideVisualAndEater ()
{
    SYS_DEBUG ("");
    /*
     * This functions will hides both page,
     * the event eater and the unlock-ui
     */
    m_LockScreenBusinessLogic->toggleEventEater (false);
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
    SYS_DEBUG ("");
    Q_UNUSED (silent);

    QTimer::singleShot (0, this, SLOT (hideVisualAndEater ()));

    return (int) TkLockReplyOk;
}

void
LockScreenBusinessLogicAdaptor::unlockConfirmed ()
{
    SYS_DEBUG ("");

    if (m_CallbackDbusIf)
        delete m_CallbackDbusIf;

    m_CallbackDbusIf = new QDBusInterface (
            m_MCECallbackService, //"com.nokia.mce",
            m_MCECallbackPath, //"/com/nokia/mce/request",
            m_MCECallbackInterface, //"com.nokia.mce.request",
            QDBusConnection::systemBus ());

    /*
     * We have some information from the MCE team:
        typedef enum {
            TKLOCK_UNLOCK = 1,
            TKLOCK_RETRY,
            TKLOCK_TIMEOUT,
            TKLOCK_CLOSED
        } tklock_status;
     * Where TKLOCK_UNLOCK == TkLockReplyOk so I just leave it now.
     */
    SYS_DEBUG ("Sending answer:");
    SYS_DEBUG ("*** service    = %s", SYS_STR(m_MCECallbackService));
    SYS_DEBUG ("*** path       = %s", SYS_STR(m_MCECallbackPath));
    SYS_DEBUG ("*** interface  = %s", SYS_STR(m_MCECallbackInterface));
    SYS_DEBUG ("*** callback   = %s", SYS_STR(m_MCECallbackMethod));
    SYS_DEBUG ("*** param(int) = %d", (int) TkLockReplyOk);
   	QDBusMessage message;

    message = m_CallbackDbusIf->call (QDBus::NoBlock,
            m_MCECallbackMethod, //QString ("tklock_callback"),
            (int) TkLockReplyOk);
    SYS_DEBUG ("Answer sent...");

    QString errorString = message.errorMessage();
    QString errorName = message.errorName ();
    SYS_DEBUG ("*** errorMessage = %s", SYS_STR(errorString));
    SYS_DEBUG ("*** errorName    = %s", SYS_STR(errorName));
}

