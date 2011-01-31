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

ScreenLockBusinessLogicAdaptor::ScreenLockBusinessLogicAdaptor(QObject *obj, ScreenLockBusinessLogic &screenLockBusinessLogic) :
    QDBusAbstractAdaptor(obj),
    screenLockBusinessLogic(screenLockBusinessLogic),
    callbackInterface(0)
{
    connect(&screenLockBusinessLogic, SIGNAL(unlockConfirmed()), this, SLOT(callUnlockCallback()));
}

ScreenLockBusinessLogicAdaptor::~ScreenLockBusinessLogicAdaptor()
{
}

int ScreenLockBusinessLogicAdaptor::tklock_open(const QString &service, const QString &path, const QString &interface, const QString &method, uint mode, bool, bool)
{
    // Create a D-Bus interface if one doesn't exist or the D-Bus callback details have changed
    if (callbackInterface == NULL || callbackInterface->service() != service || callbackInterface->path() != path || callbackInterface->interface() != interface) {
        delete callbackInterface;
        callbackInterface = new QDBusInterface(service, path, interface, QDBusConnection::systemBus(), this);
    }

    // Store the callback method name
    callbackMethod = method;

    // MCE needs a response ASAP, so the actions are executed with single shot timers
    switch (mode) {
    case TkLockModeEnable:
        // Create the lock screen already so that it's readily available
        QTimer::singleShot(0, this, SLOT(enableVisual()));
        break;

    case TkLockModeOneInput:
        QTimer::singleShot(0, this, SLOT(enableEater()));
        break;

    case TkLockEnableVisual:
        // Display has been turned on, so raise the lock screen window on top if it isn't already
        QTimer::singleShot(0, this, SLOT(enableVisual()));
        break;

    default:
        break;
    }

    return TkLockReplyOk;
}

void ScreenLockBusinessLogicAdaptor::enableVisual()
{
    screenLockBusinessLogic.toggleScreenLockUI(true);
    screenLockBusinessLogic.toggleEventEater(false);
}

void ScreenLockBusinessLogicAdaptor::enableEater()
{
    screenLockBusinessLogic.toggleEventEater(true);
}

void ScreenLockBusinessLogicAdaptor::hideVisualAndEater()
{
    screenLockBusinessLogic.toggleEventEater(false);
    screenLockBusinessLogic.toggleScreenLockUI(false);
}

int ScreenLockBusinessLogicAdaptor::tklock_close(bool)
{
    QTimer::singleShot(0, this, SLOT(hideVisualAndEater()));

    return TkLockReplyOk;
}

void ScreenLockBusinessLogicAdaptor::callUnlockCallback()
{
    callbackInterface->call(QDBus::NoBlock, callbackMethod, TkLockUnlock);
}
