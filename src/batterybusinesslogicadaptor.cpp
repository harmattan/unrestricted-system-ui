#include "batterybusinesslogicadaptor.h"
#include "batterybusinesslogic.h"

#include <QDebug>

BatteryBusinessLogicAdaptor::BatteryBusinessLogicAdaptor(QObject *obj, BatteryBusinessLogic *batteryLogic)
    : QDBusAbstractAdaptor(obj), batteryLogic(batteryLogic)
{
    connect(batteryLogic, SIGNAL(batteryCharging()), this, SIGNAL(batteryCharging()));
    connect(batteryLogic, SIGNAL(batteryNotCharging()), this, SIGNAL(batteryNotCharging()));
    connect(batteryLogic, SIGNAL(batteryLevelValueChanged(int)), this, SIGNAL(batteryLevelValueChanged(int)));
    connect(batteryLogic, SIGNAL(PSMValueChanged(bool)), this, SIGNAL(PSMValueChanged(bool)));
}

void BatteryBusinessLogicAdaptor::setPSMValue(bool toggle)
{    
    qDebug() << "BatteryBusinessLogicAdaptor::setPSMValue(" << toggle << ")";    
    batteryLogic->togglePSM(toggle);    
}

void BatteryBusinessLogicAdaptor::setPSMAutoValue(bool toggle)
{
    qDebug() << "BatteryBusinessLogicAdaptor::setPSMAutoValue(" << toggle << ")";
    batteryLogic->togglePSMAuto(toggle);
}

void BatteryBusinessLogicAdaptor::setPSMThresholdValue(const QString &value)
{
    qDebug() << "BatteryBusinessLogicAdaptor::setPSMThresholdValue(" << value << ")";
    batteryLogic->setPSMThreshold(value);   
}

bool BatteryBusinessLogicAdaptor::PSMValue()
{
    qDebug() << "BatteryBusinessLogicAdaptor::PSMValue()";
    return batteryLogic->PSMValue();
}

bool BatteryBusinessLogicAdaptor::PSMAutoValue()
{
    qDebug() << "BatteryBusinessLogicAdaptor::PSMDisabled()";
    return batteryLogic->GConfItemValue(SystemUIGConf::BatteryPSMAutoKey).toBool();
}

bool BatteryBusinessLogicAdaptor::batteryChargingState()
{
    qDebug() << "BatteryBusinessLogicAdaptor::batteryChargingState()";
    return batteryLogic->batteryChargingState();
}

QString BatteryBusinessLogicAdaptor::PSMThresholdValue()
{
    qDebug() << "BatteryBusinessLogicAdaptor::PSMThresholdValue(" << batteryLogic->GConfItemValue(SystemUIGConf::BatteryPSMThresholdKey).toString() << ")";
    return batteryLogic->GConfItemValue(SystemUIGConf::BatteryPSMThresholdKey).toString();
}

int BatteryBusinessLogicAdaptor::batteryLevelValue()
{
    qDebug () << "BatteryBusinessLogicAdaptor::batteryLevelValue()";
    return batteryLogic->batteryLevelValue();
}

QStringList BatteryBusinessLogicAdaptor::PSMThresholdValues()
{        
    qDebug() << "BatteryBusinessLogicAdaptor::PSMThresholdValues()";
    return batteryLogic->PSMThresholdValues();
}

QStringList BatteryBusinessLogicAdaptor::remainingTimeValues()
{
    qDebug() << "BatteryBusinessLogicAdaptor::remainingTimeValues()";
    return batteryLogic->remainingTimeValues();
}
