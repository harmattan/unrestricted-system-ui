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
#include <QDebug>

#include "sysuidrequest.h"
#include "lockscreenbusinesslogic.h"
#include "lockscreenbusinesslogicadaptor.h"

static const QString DBUS_SERVICE = "com.nokia.system_ui";
static const QString DBUS_PATH = "/com/nokia/system_ui/request";

SysUidRequest::SysUidRequest() :
    lockScreenBusinessLogic_(NULL)
{
    /*
     * Registering on the system bus, because MCE needs us to provide interface
     * on the system bus, not the session bus. This is not going to work under
     * the scratchbox if the libosso-dbus-conf package is not installed.
     */
    QDBusConnection bus = QDBusConnection::systemBus();

    if (!bus.registerService(DBUS_SERVICE)) {
        dbusError(bus, false);
    }

    if (!bus.registerObject(DBUS_PATH, this)) {
        dbusError(bus, false);
    }

    lockScreenBusinessLogic_ = new LockScreenBusinessLogic(this);
    new LockScreenBusinessLogicAdaptor(this, lockScreenBusinessLogic_);
}

void SysUidRequest::dbusError(QDBusConnection &connection, bool abortProgram)
{
    qWarning() << "DBus error: " << connection.lastError().message();

    if (abortProgram) {
        abort();
    }
}

LockScreenBusinessLogic *SysUidRequest::lockScreenBusinessLogic()
{
    return lockScreenBusinessLogic_;
}
