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

#include <QDBusConnection>
#include <QDBusError>

#include "sysuidrequest.h"
#include "lockscreenbusinesslogic.h"
#include "lockscreenbusinesslogicadaptor.h"

#define DEBUG
#define WARNING
#include "debug.h"

SysUidRequest::SysUidRequest ()
{
    /*
     * Registering on the system bus, because MCE needs us to provide interface
     * on the system bus, not the session bus. This is not going to work under
     * the scratchbox if the libosso-dbus-conf package is not installed.
     */
    QDBusConnection bus = QDBusConnection::systemBus ();

    SYS_DEBUG ("Registering service/object on system bus.");
    if (!bus.registerService (dbusService ())) {
        SYS_WARNING ("failed to register dbus service");
        dbusError (bus, false);
    }

    if (!bus.registerObject (dbusPath (), this)) {
        SYS_WARNING ("failed to register dbus object");
        dbusError (bus, false);
    }

    m_LockScreenLogic = new LockScreenBusinessLogic (this);
    new LockScreenBusinessLogicAdaptor (this, m_LockScreenLogic);
}

/*!
 * An error printing method. It is implemented so we know what external cause
 * aborted the sysuid.
 */
void 
SysUidRequest::dbusError (
        QDBusConnection &connection,
        bool abortProgram)
{
    QDBusError error = connection.lastError ();

    if (error.isValid()) {
        SYS_WARNING ("DBus error: %s", SYS_STR(error.message()));
    } else {
        SYS_WARNING ("Unknown DBus error");
    }

    if (abortProgram)
        abort();
}

QString 
SysUidRequest::dbusService ()
{
    return QString ("com.nokia.system_ui");
}

QString
SysUidRequest::dbusPath ()
{
    return QString ("/com/nokia/system_ui/request");
}

LockScreenBusinessLogic *
SysUidRequest::getLockScreenLogic ()
{
    return m_LockScreenLogic;
}

