/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "batterydbusinterface.h"
#include <QtDBus>
#include <QDBusError>
#include <QDebug>
#include <QVariant>

#define DEBUG
#include "../debug.h"

BatteryDBusInterface::BatteryDBusInterface ()
{
    dbusIf = new QDBusInterface ("com.nokia.systemui", "/",
                                 "com.nokia.systemui.battery",
                                 QDBusConnection::sessionBus ());

    connect (dbusIf, SIGNAL (batteryCharging (int)),
             this, SIGNAL (batteryCharging (int)));
    connect (dbusIf, SIGNAL (batteryNotCharging ()),
             this, SIGNAL (batteryNotCharging()));
    connect (dbusIf, SIGNAL (batteryBarValueChanged (int)),
             this, SIGNAL (batteryBarValueReceived (int)));
    connect (dbusIf, SIGNAL (PSMValueChanged (bool)),
             this, SIGNAL (PSMValueReceived (bool)));
    connect (dbusIf, SIGNAL (PSMAutoDisabled ()),
             this, SIGNAL (PSMAutoDisabled ()));
    connect (dbusIf, SIGNAL (remainingTimeValuesChanged (QStringList)),
             this, SIGNAL (remainingTimeValuesReceived (QStringList)));
}

BatteryDBusInterface::~BatteryDBusInterface ()
{
    delete dbusIf;
    dbusIf = NULL;
}

void
BatteryDBusInterface::PSMValueRequired ()
{
    qDebug () << "BatteryDBusInterface::PSMValueRequired()";
    dbusIf->callWithCallback (QString ("PSMValue"), QList<QVariant> (), this,
                              SIGNAL (PSMValueReceived (bool)),
                              SLOT (DBusMessagingFailure (QDBusError)));
}

void
BatteryDBusInterface::PSMAutoValueRequired ()
{
    qDebug () << "BatteryDBusInterface::PSMAutoValueRequired()";
    dbusIf->callWithCallback (QString ("PSMAutoValue"), QList<QVariant> (), this,
                              SIGNAL (PSMAutoValueReceived (bool)),
                              SLOT (DBusMessagingFailure (QDBusError)));
}

void
BatteryDBusInterface::batteryChargingStateRequired ()
{
    qDebug () << "BatteryDBusInterface::BatteryChargingValueRequired()";
    dbusIf->call (QDBus::NoBlock, QString ("batteryChargingState"));
}

void
BatteryDBusInterface::PSMThresholdValuesRequired ()
{
    qDebug () << "BatteryDBusInterface::PSMThresholdValuesRequired()";
    dbusIf->callWithCallback (QString ("PSMThresholdValues"),
                              QList<QVariant> (), this,
                              SIGNAL (PSMThresholdValuesReceived (QStringList)),
                              SLOT (DBusMessagingFailure (QDBusError)));
}

void BatteryDBusInterface::PSMThresholdValueRequired()
{
    qDebug () << "BatteryDBusInterface::PSMThresholdValueRequired()";
    dbusIf->callWithCallback (QString ("PSMThresholdValue"),
                              QList<QVariant> (), this,
                              SIGNAL (PSMThresholdValueReceived (QString)),
                              SLOT (DBusMessagingFailure (QDBusError)));
}

void
BatteryDBusInterface::remainingTimeValuesRequired ()
{
    qDebug () << "BatteryDBusInterface::remainingTimeValuesRequired()";
    dbusIf->callWithCallback (QString ("remainingTimeValues"),
                              QList<QVariant> (), this,
                              SIGNAL (remainingTimeValuesReceived (QStringList)),
                              SLOT (DBusMessagingFailure (QDBusError)));
}

void
BatteryDBusInterface::batteryBarValueRequired ()
{
    qDebug () << "BatteryDBusInterface::batteryBarValueRequired()";
    dbusIf->callWithCallback (QString ("batteryBarValue"), QList<QVariant> (),
                              this, SIGNAL (batteryBarValueReceived (int)),
                              SLOT (DBusMessagingFailure (QDBusError)));
}

void
BatteryDBusInterface::setPSMThresholdValue (const QString &value)
{
    qDebug () << "BatteryDBusInterface::setPSMThresholdValue(" << value << ")";
    dbusIf->call (QDBus::NoBlock, QString ("setPSMThresholdValue"), value);
}

void
BatteryDBusInterface::setPSMValue (
        bool toggle)
{
    qDebug () << "BatteryDBusInterface::setPSMValue(" << toggle << ")";
    dbusIf->call (QDBus::NoBlock, QString ("setPSMValue"), toggle);

    QDBusError error = dbusIf->lastError();
    if (error.isValid()) {
        SYS_WARNING ("%s: %s", SYS_STR (error.name()), 
                SYS_STR (error.message()));
    }
}

void
BatteryDBusInterface::setPSMAutoValue (bool toggle)
{
    qDebug () << "BatteryDBusInterface::setPSMAutoValue(" << toggle << ")";
    dbusIf->call (QDBus::NoBlock, QString ("setPSMAutoValue"), toggle);
}

void
BatteryDBusInterface::querySent()
{
    qDebug () << "BatteryDBusInterface::querySent()";
}

void
BatteryDBusInterface::DBusMessagingFailure (
		QDBusError error)
{
    SYS_WARNING ("%s: %s", SYS_STR (error.name()), SYS_STR (error.message()));
    qWarning () << 
	    "BatteryDBusInterface::DBusMessagingFailure()" <<
	    error.name() <<
	    ": " <<
	    error.message();
}

