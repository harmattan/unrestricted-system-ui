#include "batterydbusinterface.h"
#include <QtDBus>
#include <QDebug>
#include <QVariant>

BatteryDBusInterface::BatteryDBusInterface()
{
    dbusIf = new QDBusInterface("org.freedesktop.DBus.Battery", "/", "", QDBusConnection::sessionBus());
    connect(dbusIf, SIGNAL(batteryCharging()), this, SIGNAL(batteryCharging()));
    connect(dbusIf, SIGNAL(batteryNotCharging()), this, SIGNAL(batteryNotCharging()));
    connect(dbusIf, SIGNAL(batteryLevelValueChanged(int)), this, SIGNAL(batteryLevelValueReceived(int)));
    connect(dbusIf, SIGNAL(PSMToggleValueChanged(bool)), this, SIGNAL(PSMToggleValueReceived(bool)));
}

BatteryDBusInterface::~BatteryDBusInterface()
{
    delete dbusIf;
    dbusIf = NULL;
}

void BatteryDBusInterface::PSMToggleValueRequired()
{
    qDebug() << "BatteryDBusInterface::PSMToggleValueRequired()";
    QList<QVariant> list;    
    dbusIf->callWithCallback(QString("PSMToggleValue"), list, this, SIGNAL(PSMToggleValueReceived(bool)), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::PSMDisabledValueRequired()
{
    qDebug() << "BatteryDBusInterface::PSMDisabledValueRequired()";
    QList<QVariant> list;    
    dbusIf->callWithCallback(QString("PSMDisabledValue"), list, this, SIGNAL(PSMDisabledValueReceived(bool)), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::batteryChargingStateRequired()
{
    qDebug() << "BatteryDBusInterface::BatteryChargingValueRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("batteryChargingState"), list, this, SLOT(BatteryChargingStateReceived(bool)), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::batteryChargingStateReceived(bool state)
{
    qDebug() << "BatteryDBusInterface::BatteryChargingStateReceived(" << state << ")";
    if(state)
        emit batteryCharging();
    else
        emit batteryNotCharging();
}

void BatteryDBusInterface::PSMThresholdValuesRequired()
{
    qDebug() << "BatteryDBusInterface::PSMThresholdValuesRequired()";
    QList<QVariant> list;    
    dbusIf->callWithCallback(QString("PSMThresholdValues"), list, this, SIGNAL(PSMThresholdValuesReceived(QStringList)), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::PSMThresholdValueRequired()
{
    qDebug() << "BatteryDBusInterface::PSMThresholdValueRequired()";
    QList<QVariant> list;    
    dbusIf->callWithCallback(QString("PSMThresholdValue"), list, this, SIGNAL(PSMThresholdValueReceived(QString)), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::remainingTimeValuesRequired()
{
    qDebug() << "BatteryDBusInterface::remainingTimeValuesRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("remainingTimeValues"), list, this, SIGNAL(remainingTimeValuesReceived(QStringList)), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::batteryLevelValueRequired()
{
    qDebug() << "BatteryDBusInterface::BatteryLevelValueRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("batteryLevelValue"), list, this, SIGNAL(batteryLevelValueReceived(int)), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::setPSMThresholdValue(const QString &value)
{
    qDebug() << "BatteryDBusInterface::setPSMThresholdValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setPSMThresholdValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::setPSMToggleValue(bool toggle)
{
    qDebug() << "BatteryDBusInterface::setPSMToggleValue(" << toggle << ")";
    QList<QVariant> list;
    list << QVariant(toggle);
    dbusIf->callWithCallback(QString("setPSMToggleValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::setPSMDisabledValue(bool disabled)
{
    qDebug() << "BatteryDBusInterface::setPSMDisabledValue(" << disabled << ")";
    QList<QVariant> list;
    list << QVariant(disabled);
    dbusIf->callWithCallback(QString("setPSMDisabledValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::valueSet()
{
    qDebug() << "BatteryDBusInterface::valueSet()";
}

void BatteryDBusInterface::DBusMessagingFailure()
{
    qDebug() << "BatteryDBusInterface::DBusMessagingFailure()";
}
