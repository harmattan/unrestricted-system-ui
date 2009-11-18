#include "batterydbusinterface.h"
#include <QtDBus>
#include <QDebug>
#include <QVariant>

BatteryDBusInterface::BatteryDBusInterface()
{
    dbusIf = new QDBusInterface("com.nokia.systemui", "/", 
				"com.nokia.systemui.battery", 
				QDBusConnection::sessionBus());
    connect(dbusIf, SIGNAL(batteryCharging(int)), this, SIGNAL(batteryCharging(int)));
    connect(dbusIf, SIGNAL(batteryNotCharging()), this, SIGNAL(batteryNotCharging()));
    connect(dbusIf, SIGNAL(batteryBarValueChanged(int)), this, SIGNAL(batteryBarValueReceived(int)));
    connect(dbusIf, SIGNAL(PSMValueChanged(bool)), this, SIGNAL(PSMValueReceived(bool)));
    connect(dbusIf, SIGNAL(PSMAutoDisabled()), this, SIGNAL(PSMAutoDisabled()));
    connect(dbusIf, SIGNAL(remainingTimeValuesChanged(QStringList)), this, SIGNAL(remainingTimeValuesReceived(QStringList)));
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
    dbusIf->callWithCallback(QString("PSMValue"), list, this, SIGNAL(PSMValueReceived(bool)), SLOT(DBusMessagingFailure()));
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
    dbusIf->callWithCallback(QString("batteryChargingState"), list, this, SLOT(querySent()), SLOT(DBusMessagingFailure()));
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

void BatteryDBusInterface::batteryBarValueRequired()
{
    qDebug() << "BatteryDBusInterface::batteryBarValueRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("batteryBarValue"), list, this, SIGNAL(batteryBarValueReceived(int)), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::setPSMThresholdValue(const QString &value)
{
    qDebug() << "BatteryDBusInterface::setPSMThresholdValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setPSMThresholdValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::setPSMValue(bool toggle)
{
    qDebug() << "BatteryDBusInterface::setPSMValue(" << toggle << ")";
    QList<QVariant> list;
    list << QVariant(toggle);
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

void BatteryDBusInterface::querySent()
{
    qDebug() << "BatteryDBusInterface::querySent()";
}

void BatteryDBusInterface::DBusMessagingFailure()
{
    qDebug() << "BatteryDBusInterface::DBusMessagingFailure()";
}
