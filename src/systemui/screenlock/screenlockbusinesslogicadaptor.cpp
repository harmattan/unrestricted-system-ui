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
#include "screenlockbusinesslogicadaptor.h"

ScreenLockBusinessLogicAdaptor::ScreenLockBusinessLogicAdaptor (
        QObject                 *obj,
        ScreenLockBusinessLogic *logic) :
    QDBusAbstractAdaptor (obj),
    m_LockScreenBusinessLogic (logic),
    m_CallbackDbusIf (0)
{
    Q_ASSERT (logic != 0);

    connect (logic, SIGNAL(unlockConfirmed ()),
            this, SLOT(unlockConfirmed ()));
}

ScreenLockBusinessLogicAdaptor::~ScreenLockBusinessLogicAdaptor ()
{
    if (m_CallbackDbusIf)
        delete m_CallbackDbusIf;
}

/*!
 * \param service DBus service to call when unlock is performed.
 * \param path DBus path to call when unlock is performed.
 * \param interface DBus interface to call when unlock is performed.
 * \param method DBus method to call when unlock is performed.
 * \param mode The ScreenLockBusinessLogicAdaptor::TkLockMode opcode.
 * \param silent Whether to show a notification or not (deprecated)
 * \param flicker Deprecated/not used
 *
 * DBus method to show the lock screen ui or the event eater window.
 */
int
ScreenLockBusinessLogicAdaptor::tklock_open (
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
            break;

        case TkLockModeEnable:

            /*
             * Show the visual earliest...
             * in this way we can avoid the full-screen window
             * creation time when EnableVisual requested later...
             */
            QTimer::singleShot (0, this, SLOT (enableVisual ()));
            break;

        case TkLockModeHelp:
            /*
             * Not used/deprecated.
             */
            break;

        case TkLockModeSelect:
            /*
             * Not used/deprecated.
             */
            break;

        case TkLockModeOneInput:
            /*
             * The event eater supposed to consume exactly one event, hence the
             * name.
             */
            QTimer::singleShot (0, this, SLOT (enableEater ()));
            break;

        case TkLockEnableVisual:
            /*
             * This mode is where we actually should show the screen to unlock
             * the screen.
             *
             * Calling again the enableVisual to raise the unlock-ui window.
             */
            QTimer::singleShot (0, this, SLOT (enableVisual ()));
            break;

        default:
            break;
    }

    return (int) TkLockReplyOk;
}

void
ScreenLockBusinessLogicAdaptor::enableVisual ()
{
    m_LockScreenBusinessLogic->toggleScreenLockUI (true);
    m_LockScreenBusinessLogic->toggleEventEater (false);
}

void
ScreenLockBusinessLogicAdaptor::enableEater ()
{
    m_LockScreenBusinessLogic->toggleEventEater (true);
}

void
ScreenLockBusinessLogicAdaptor::hideVisualAndEater ()
{
    /*
     * This functions will hides both page,
     * the event eater and the unlock-ui
     */
    m_LockScreenBusinessLogic->toggleEventEater (false);
    m_LockScreenBusinessLogic->toggleScreenLockUI (false);
}

/*!
 * \param silent Whether to show notificatio or not (deprecated).
 *
 * DBus method that is called to hide the lock screen UI.
 */
int
ScreenLockBusinessLogicAdaptor::tklock_close (
        bool   silent)
{
    Q_UNUSED (silent);

    QTimer::singleShot (0, this, SLOT (hideVisualAndEater ()));

    return (int) TkLockReplyOk;
}

void
ScreenLockBusinessLogicAdaptor::unlockConfirmed ()
{
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

    QDBusMessage message;
    message = m_CallbackDbusIf->call (QDBus::NoBlock,
            m_MCECallbackMethod, //QString ("tklock_callback"),
            (int) TkLockReplyOk);
}
