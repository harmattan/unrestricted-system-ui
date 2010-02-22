/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include <QDBusConnection>
#include <QDBusError>

#include "sysuidrequest.h"
#include "lockscreenbusinesslogic.h"
#include "lockscreenbusinesslogicadaptor.h"

//#define DEBUG
#include "../debug.h"

SysUidRequest::SysUidRequest ()
{
    m_LockScreenLogic = new LockScreenBusinessLogic (this);
    new LockScreenBusinessLogicAdaptor (this, m_LockScreenLogic);

    /*
     * Registering on the system bus, because MCE needs us to provide interface
     * on the system bus, not the session bus. This is not going to work under 
     * the scratchbox if the libosso-dbus-conf package is not installed.
     */
    QDBusConnection bus = QDBusConnection::systemBus ();
   
    SYS_DEBUG ("Registering service/object on system bus.");
    if (!bus.registerService (dbusService ())) {
        SYS_WARNING ("failed to register dbus service");
        dbusError (bus);
    }

    if (!bus.registerObject (dbusPath (), this)) {
        SYS_WARNING ("failed to register dbus object");
        dbusError (bus);
    }
}

/*!
 * An error printing method. It is implemented so we know what external cause is
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
        qCritical() << "DBus error: " << SYS_STR(error.message());
    } else {
        SYS_WARNING ("Unknown DBus error");
        qCritical() << "Unknown DBus error";
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

