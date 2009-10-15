#include "batterydbusinterface.h"
#include <QtDBus>
#include <QDebug>
#include <QVariant>

BatteryDBusInterface::BatteryDBusInterface()
{
    dbusIf = new QDBusInterface("com.nokia.systemui", "/", 
				"com.nokia.systemui.battery", 
				QDBusConnection::sessionBus());
    connect(dbusIf, SIGNAL(batteryCharging()), this, SIGNAL(batteryCharging()));
    connect(dbusIf, SIGNAL(batteryNotCharging()), this, SIGNAL(batteryNotCharging()));
    connect(dbusIf, SIGNAL(batteryLevelValueChanged(int)), this, SIGNAL(batteryLevelValueReceived(int)));
    connect(dbusIf, SIGNAL(PSMValueChanged(QString)), this, SIGNAL(PSMValueReceived(QString)));
    connect(dbusIf, SIGNAL(PSMAutoValueChanged(bool)), this, SIGNAL(PSMAutoValueReceived(bool)));
}

BatteryDBusInterface::~BatteryDBusInterface()
{
    delete dbusIf;
    dbusIf = NULL;
}

void BatteryDBusInterface::PSMValueRequired()
{
    qDebug() << "BatteryDBusInterface::PSMValueRequired()";
    QList<QVariant> list;    
    dbusIf->callWithCallback(QString("PSMValue"), list, this, SIGNAL(PSMValueReceived(QString)), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::PSMAutoValueRequired()
{
    qDebug() << "BatteryDBusInterface::PSMAutoValueRequired()";
    QList<QVariant> list;    
    dbusIf->callWithCallback(QString("PSMAutoValue"), list, this, SIGNAL(PSMAutoValueReceived(bool)), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::batteryChargingStateRequired()
{
    qDebug() << "BatteryDBusInterface::BatteryChargingValueRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("batteryChargingState"), list, this, SLOT(batteryChargingStateReceived(bool)), SLOT(DBusMessagingFailure()));
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

void BatteryDBusInterface::setPSMValue(const QString &value)
{
    qDebug() << "BatteryDBusInterface::setPSMValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setPSMValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::setPSMAutoValue(bool toggle)
{
    qDebug() << "BatteryDBusInterface::setPSMAutoValue(" << toggle << ")";
    QList<QVariant> list;
    list << QVariant(toggle);
    dbusIf->callWithCallback(QString("setPSMAutoValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::valueSet()
{
    qDebug() << "BatteryDBusInterface::valueSet()";
}

void BatteryDBusInterface::DBusMessagingFailure()
{
    qDebug() << "BatteryDBusInterface::DBusMessagingFailure()";
}
