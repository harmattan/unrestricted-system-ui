#include "batterybusinesslogicadaptor.h"
#include "batterybusinesslogic.h"

#include <QDebug>

BatteryBusinessLogicAdaptor::BatteryBusinessLogicAdaptor(BatteryBusinessLogic *batteryLogic)
    : QDBusAbstractAdaptor(batteryLogic), batteryLogic(batteryLogic)
{
    connect(batteryLogic, SIGNAL(batteryCharging()), this, SIGNAL(batteryCharging()));
    connect(batteryLogic, SIGNAL(batteryNotCharging()), this, SIGNAL(batteryNotCharging()));
    connect(batteryLogic, SIGNAL(batteryLevelValueChanged(int)), this, SIGNAL(batteryLevelValueChanged(int)));
    connect(batteryLogic, SIGNAL(PSMToggleValueChanged(bool)), this, SIGNAL(PSMToggleValueChanged(bool)));
}

void BatteryBusinessLogicAdaptor::setPSMToggleValue(bool toggle)
{    
    qDebug() << "BatteryBusinessLogicAdaptor::setPSMToggleValue(" << toggle << ")";
    disconnect(batteryLogic, SIGNAL(PSMToggleValueChanged(bool)), this, SIGNAL(PSMToggleValueChanged(bool)));
    batteryLogic->togglePSM(toggle);
    connect(batteryLogic, SIGNAL(PSMToggleValueChanged(bool)), this, SIGNAL(PSMToggleValueChanged(bool)));
}

void BatteryBusinessLogicAdaptor::setPSMDisabledValue(bool disabled)
{
    qDebug() << "BatteryBusinessLogicAdaptor::setPSMDisabled(" << disabled << ")";
    batteryLogic->togglePSMDisabled(disabled);
}

void BatteryBusinessLogicAdaptor::setPSMThresholdValue(const QString &value)
{
    qDebug() << "BatteryBusinessLogicAdaptor::setPSMThresholdValue(" << value << ")";
    batteryLogic->setPSMThreshold(value);   
}

bool BatteryBusinessLogicAdaptor::PSMToggleValue()
{
    qDebug() << "BatteryBusinessLogicAdaptor::PSMToggleValue()";
    return batteryLogic->GConfItemValue(BatteryGConf::PSMToggleKey).toBool();
}

bool BatteryBusinessLogicAdaptor::PSMDisabledValue()
{
    qDebug() << "BatteryBusinessLogicAdaptor::PSMDisabled()";
    return batteryLogic->GConfItemValue(BatteryGConf::PSMDisabledKey).toBool();
}

bool BatteryBusinessLogicAdaptor::batteryChargingState()
{
    qDebug() << "BatteryBusinessLogicAdaptor::batteryChargingState()";
    return batteryLogic->batteryChargingState();
}

QString BatteryBusinessLogicAdaptor::PSMThresholdValue()
{
    qDebug() << "BatteryBusinessLogicAdaptor::PSMThresholdValue(" << batteryLogic->GConfItemValue(BatteryGConf::PSMThresholdKey).toString() << ")";
    return batteryLogic->GConfItemValue(BatteryGConf::PSMThresholdKey).toString();
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
