/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include <QDBusConnection>

#include "sysuidrequest.h"
#include "lockscreenbusinesslogic.h"
#include "lockscreenbusinesslogicadaptor.h"

#define DEBUG
#include "../debug.h"

SysUidRequest::SysUidRequest ()
{
    m_LockScreenLogic = new LockScreenBusinessLogic (this);
    new LockScreenBusinessLogicAdaptor (this, m_LockScreenLogic);

    QDBusConnection bus = QDBusConnection::systemBus ();
    
    if (!bus.registerService (dbusService ())) {
        SYS_WARNING ("failed to register dbus service");
        abort();
    }

    if (!bus.registerObject (dbusPath (), this)) {
        SYS_WARNING ("failed to register dbus object");
        abort();
    }
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
