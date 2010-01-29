/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef BATTERYDBUSINTERFACE_H
#define BATTERYDBUSINTERFACE_H

#include <QObject>
#include <QStringList>

class QDBusInterface;
#include <QDBusError>

/*!
 * Implements a DBus connection to the battery & power save mode API implemented
 * in the sysuid daemon. The other side of the DBus API is implemented in 
 * components/sysuid/batterybusinesslogicadaptor.cpp.
 */
class BatteryDBusInterface : public QObject
{
    Q_OBJECT

public:
    BatteryDBusInterface ();
    ~BatteryDBusInterface ();

    void PSMValueRequired ();
    void PSMAutoValueRequired ();
    void batteryChargingStateRequired ();
    void PSMThresholdValuesRequired ();
    void setPSMValue (bool toggle);

public slots:
    void remainingTimeValuesRequired ();
    void batteryBarValueRequired ();
    void PSMThresholdValueRequired ();
    void setPSMAutoValue (bool toggle);
    void setPSMThresholdValue (const QString &value);

public slots:
    void DBusMessagingFailure (QDBusError error);
    void valueSet ();
    void querySent ();

signals:
    void batteryCharging (int);
    void batteryNotCharging ();
    void PSMValueReceived (bool);
    void PSMAutoValueReceived (bool);
    void PSMAutoDisabled ();
    void PSMThresholdValuesReceived (QStringList);
    void PSMThresholdValueReceived (QString);
    void remainingTimeValuesReceived (QStringList);
    void batteryBarValueReceived (int);

private:
    QDBusInterface *dbusIf;
};

#endif // BATTERYDBUSINTERFACE_H

