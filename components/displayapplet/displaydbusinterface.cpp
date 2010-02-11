/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "displaydbusinterface.h"
#include <QtDBus>
#include <QDebug>

#define DEBUG
#define WARNING
#include "../debug.h"
#if 0
DisplayDBusInterface::DisplayDBusInterface()
{
    SYS_DEBUG ("");
    dbusIf = new QDBusInterface (
		    "com.nokia.systemui", "/", "com.nokia.systemui.display",
		    QDBusConnection::sessionBus());

    connect (dbusIf, SIGNAL(brightnessValuesReceived(int, QStringList)), 
            this, SIGNAL(brightnessValuesReceived(int, QStringList)));
    connect (dbusIf, SIGNAL(screenLightsValuesReceived(int, QStringList)), 
            this, SIGNAL(screenLightsValuesReceived(int, QStringList)));
}

DisplayDBusInterface::~DisplayDBusInterface()
{
    delete dbusIf;
    dbusIf = NULL;
}

void DisplayDBusInterface::brightnessValuesRequired()
{
    SYS_DEBUG (""); 

    QList<QVariant> list;
    dbusIf->callWithCallback (QString("brightnessValues"), list, 
            this, SLOT(querySent()), SLOT(DBusMessagingFailure(QDBusError)));
}

void DisplayDBusInterface::screenLightsValuesRequired()
{
    SYS_DEBUG (""); 

    QList<QVariant> list;
    dbusIf->callWithCallback(QString("screenLightsValues"), list, 
            this, SLOT(querySent()), SLOT(DBusMessagingFailure(QDBusError)));
}

void DisplayDBusInterface::blankInhibitValueRequired()
{
    SYS_DEBUG (""); 
    
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("blankInhibitValue"), list, 
            this, SIGNAL(blankInhibitValueReceived(bool)), 
            SLOT(DBusMessagingFailure(QDBusError)));
}

void DisplayDBusInterface::setBrightnessValue(const QString &value)
{
    SYS_DEBUG ("*** value = %s", SYS_STR (value)); 

    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setBrightnessValue"), list, 
            this, SLOT(valueSet()), SLOT(DBusMessagingFailure(QDBusError)));
}

void DisplayDBusInterface::setScreenLightsValue(const QString &value)
{
    SYS_DEBUG ("*** value = %s", SYS_STR (value)); 

    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setScreenLightsValue"), list, 
            this, SLOT(valueSet()), SLOT(DBusMessagingFailure(QDBusError)));
}

void DisplayDBusInterface::setBlankInhibitValue(bool value)
{
    SYS_DEBUG ("*** value = %s", SYS_STR (value)); 
    
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setBlankInhibitValue"), list, 
            this, SLOT(valueSet()), SLOT(DBusMessagingFailure(QDBusError)));
}

void DisplayDBusInterface::valueSet ()
{
    SYS_DEBUG (""); 
}

void 
DisplayDBusInterface::DBusMessagingFailure(
        		QDBusError error)
{
    SYS_WARNING ("%s: %s", SYS_STR (error.name()), SYS_STR (error.message()));
    qWarning () << 
	    "BatteryDBusInterface::DBusMessagingFailure()" <<
	    error.name() <<
	    ": " <<
	    error.message();
}
#endif
