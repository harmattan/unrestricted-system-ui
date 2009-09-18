#include "batterydbusinterface.h"
#include <QtDBus>
#include <QDebug>

BatteryDBusInterface::BatteryDBusInterface()
{
    dbusIf = new QDBusInterface("org.freedesktop.DBus.Battery", "/", "", QDBusConnection::sessionBus());
    connect(dbusIf, SIGNAL(batteryCharging()), this, SIGNAL(batteryCharging()));
    connect(dbusIf, SIGNAL(batteryNotCharging()), this, SIGNAL(batteryNotCharging()));
    connect(dbusIf, SIGNAL(batteryLevelValueChanged(int)), this, SIGNAL(batteryLevelValueChanged(int)));
    connect(dbusIf, SIGNAL(PSMToggleValueChanged(bool)), this, SIGNAL(PSMToggleValueChanged(bool)));
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
    dbusIf->callWithCallback(QString("PSMToggleValue"), list, this, SLOT(PSMToggleValueReceived(bool)), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::PSMToggleValueReceived(bool value)
{
    qDebug() << "BatteryDBusInterface::PSMToggleValueReceived(" << value << ")";
    emit PSMToggleValueChanged(value);
}

void BatteryDBusInterface::PSMDisabledValueRequired()
{
    qDebug() << "BatteryDBusInterface::PSMDisabledValueRequired()";
    QList<QVariant> list;    
    dbusIf->callWithCallback(QString("PSMDisabledValue"), list, this, SLOT(PSMDisabledValueReceived(bool)), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::PSMDisabledValueReceived(bool value)
{
    qDebug() << "BatteryDBusInterface::PSMDisabledValueReceived(" << value << ")";
    emit PSMDisabledValueChanged(value);
}

void BatteryDBusInterface::BatteryChargingStateRequired()
{
    qDebug() << "BatteryDBusInterface::BatteryChargingValueRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("batteryChargingState"), list, this, SLOT(BatteryChargingStateReceived(bool)), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::BatteryChargingStateReceived(bool state)
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
    dbusIf->callWithCallback(QString("PSMThresholdValues"), list, this, SLOT(PSMThresholdValuesReceived(QStringList)), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::PSMThresholdValuesReceived(const QStringList &values)
{
    qDebug() << "BatteryDBusInterface::PSMThresholdValuesReceived(QStringList values)";
    emit PSMThresholdValuesAvailable(values);
}

void BatteryDBusInterface::PSMThresholdValueRequired()
{
    qDebug() << "BatteryDBusInterface::PSMThresholdValueRequired()";
    QList<QVariant> list;    
    dbusIf->callWithCallback(QString("PSMThresholdValue"), list, this, SLOT(PSMThresholdValueReceived(QString)), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::PSMThresholdValueReceived(const QString &value)
{
    qDebug() << "BatteryDBusInterface::PSMThresholdValueReceived(" << value << ")";
    emit PSMThresholdValueAvailable(value);
}

void BatteryDBusInterface::remainingTimeValuesRequired()
{
    qDebug() << "BatteryDBusInterface::remainingTimeValuesRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("remainingTimeValues"), list, this, SLOT(remainingTimeValuesReceived(QStringList)), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::remainingTimeValuesReceived(const QStringList &values)
{
    qDebug() << "BatteryDBusInterface::remainingTimeValuesReceived(const QStringList &values)";
    emit remainingTimeValuesAvailable(values);
}

void BatteryDBusInterface::batteryLevelValueRequired()
{
    qDebug() << "BatteryDBusInterface::BatteryLevelValueRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("batteryLevelValue"), list, this, SLOT(batteryLevelValueReceived(QStringList)), SLOT(DBusMessagingFailure()));
}

void BatteryDBusInterface::batteryLevelValueReceived(int value)
{
    qDebug() << "BatteryDBusInterface::batteryLevelValueReceived(" << value << ")";
    emit batteryLevelValueAvailable(value);
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
//TODO when setting PSM DIsabled to true, set PSM Toggle to false

QVariant BatteryDBusInterface::call(const QString &method, BatteryDBusInterface::DBusDataType dataType, const QVariant &param1,
                                    const QVariant &param2, const QVariant &param3, const QVariant &param4)
{
    qDebug() << "BatteryDBusInterface::call(" << method << ", " << dataType << ", " << param1 << ", " << param2 << ", " << param3 << ", " << param4 << ")";
    switch(dataType) {
        case DBusDataTypeVoid: {
            dbusIf->call(method, param1, param2, param3, param4);
            return QVariant();
        }
        case DBusDataTypeBool: {            
            QDBusReply<bool> reply = dbusIf->call(method, param1, param2, param3, param4);            
            if(reply.isValid())
                return QVariant(reply.value());
            return QVariant();
        }
        case DBusDataTypeInt: {
            QDBusReply<int> reply = dbusIf->call(method, param1, param2, param3, param4);
            if(reply.isValid())
                return QVariant(reply.value());
            return QVariant();
        }
        case DBusDataTypeQString: {
            QDBusReply<QString> reply = dbusIf->call(method, param1, param2, param3, param4);
            if(reply.isValid())
                return QVariant(reply.value());
            return QVariant();
        }
        case DBusDataTypeQStringList: {
            QDBusReply<QStringList> reply = dbusIf->call(method, param1, param2, param3, param4);
            if(reply.isValid())
                return QVariant(reply.value());
            return QVariant();
        }
        default:
            return QVariant();
    }    
}
