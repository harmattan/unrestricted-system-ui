/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "leddbusinterface.h"
#include <QtDBus>
#include <QDBusError>
#include <QVariant>

//#define DEBUG
#include "../debug.h"

LedDBusInterface::LedDBusInterface ()
{
    SYS_DEBUG ("");
    m_DbusIf = new QDBusInterface (
            "com.nokia.systemui", "/", "com.nokia.systemui.leds",
            QDBusConnection::sessionBus ());

    connect (m_DbusIf, SIGNAL (ledsStateChanged(bool)),
            this, SIGNAL (ledStateReceived(bool)));
}

LedDBusInterface::~LedDBusInterface ()
{
    SYS_DEBUG ("");

    delete m_DbusIf;
    m_DbusIf = NULL;
}

void
LedDBusInterface::DBusMessagingFailure (
		QDBusError error)
{
    SYS_WARNING ("%s: %s", SYS_STR (error.name()), SYS_STR (error.message()));
}

void
LedDBusInterface::ledStateRequired ()
{
    SYS_DEBUG ("");

    m_DbusIf->callWithCallback (
            QString ("LedsEnabled"), QList<QVariant> (),
            this,
            SIGNAL (ledStateReceived (bool)),
            SLOT (DBusMessagingFailure (QDBusError)));
}

void
LedDBusInterface::setLedState (
        bool    enabled)
{
    SYS_DEBUG ("*** enabled = %s", enabled ? "yes" : "no");
    m_DbusIf->call (
            QDBus::NoBlock,
            QString ("setLedsEnabled"),
            enabled);
}

void
LedDBusInterface::illuminationLedStateRequired ()
{
    SYS_DEBUG ("");

    m_DbusIf->callWithCallback (
            QString ("IlluminationLedEnabled"), QList<QVariant> (),
            this,
            SIGNAL (illuminationLedStateReceived (bool)),
            SLOT (DBusMessagingFailure (QDBusError)));
}

void
LedDBusInterface::setIlluminationLedState (
        bool    enabled)
{
    SYS_DEBUG ("*** enabled = %s", enabled ? "yes" : "no");
    m_DbusIf->call (
            QDBus::NoBlock,
            QString ("setIlluminationLedEnabled"),
            enabled);
}

void
LedDBusInterface::eventsLedStateRequired ()
{
    SYS_DEBUG ("");

    m_DbusIf->callWithCallback (
            QString ("EventsLedEnabled"), QList<QVariant> (),
            this,
            SIGNAL (eventsLedStateReceived (bool)),
            SLOT (DBusMessagingFailure (QDBusError)));
}

void
LedDBusInterface::setEventsLedState (
        bool    enabled)
{
    SYS_DEBUG ("*** enabled = %s", enabled ? "yes" : "no");
    m_DbusIf->call (
            QDBus::NoBlock,
            QString ("setEventsLedEnabled"),
            enabled);
}
