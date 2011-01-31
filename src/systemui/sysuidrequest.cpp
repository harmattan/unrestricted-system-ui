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
#include "screenlockbusinesslogic.h"
#include "screenlockbusinesslogicadaptor.h"

static const QString DBUS_SERVICE = "com.nokia.system_ui";
static const QString DBUS_PATH = "/com/nokia/system_ui/request";

SysUidRequest::SysUidRequest() : screenLockBusinessLogic_(NULL)
{
    screenLockBusinessLogic_ = new ScreenLockBusinessLogic(this);
    new ScreenLockBusinessLogicAdaptor(screenLockBusinessLogic_);

    // MCE expects the service to be registered on the system bus
    QDBusConnection bus = QDBusConnection::systemBus();
    if (!bus.registerService(DBUS_SERVICE)) {
        qWarning() << "Unable to register D-Bus service:" << bus.lastError().message();
    }
    if (!bus.registerObject(DBUS_PATH, screenLockBusinessLogic_)) {
        qWarning() << "Unable to register D-Bus object:" << bus.lastError().message();
    }
}

ScreenLockBusinessLogic *SysUidRequest::screenLockBusinessLogic()
{
    return screenLockBusinessLogic_;
}
