#include "batterybusinesslogicadaptor.h"
#include "batterybusinesslogic.h"

//#include <QDebug>

BatteryBusinessLogicAdaptor::BatteryBusinessLogicAdaptor(QObject *obj, BatteryBusinessLogic *batteryLogic)
    : QDBusAbstractAdaptor(obj), batteryLogic(batteryLogic)
{
    connect(batteryLogic, SIGNAL(batteryCharging(int)), this, SIGNAL(batteryCharging(int)));
    connect(batteryLogic, SIGNAL(batteryNotCharging()), this, SIGNAL(batteryNotCharging()));
    connect(batteryLogic, SIGNAL(batteryBarValueChanged(int)), this, SIGNAL(batteryBarValueChanged(int)));
    connect(batteryLogic, SIGNAL(PSMValueChanged(QString)), this, SIGNAL(PSMValueChanged(QString)));
    connect(batteryLogic, SIGNAL(PSMAutoValueChanged(bool)), this, SIGNAL(PSMAutoValueChanged(bool)));
    connect(batteryLogic, SIGNAL(remainingTimeValuesChanged(QStringList)), this, SIGNAL(remainingTimeValuesChanged(QStringList)));
}

BatteryBusinessLogicAdaptor::~BatteryBusinessLogicAdaptor()
{
}

void BatteryBusinessLogicAdaptor::setPSMValue(const QString &value)
{    
    qDebug() << "BatteryBusinessLogicAdaptor::setPSMValue(" << value << ")";
    batteryLogic->togglePSM(value);
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

QString BatteryBusinessLogicAdaptor::PSMValue()
{
    qDebug() << "BatteryBusinessLogicAdaptor::PSMValue()";
    return batteryLogic->PSMValue();
}

bool BatteryBusinessLogicAdaptor::PSMAutoValue()
{
    qDebug() << "BatteryBusinessLogicAdaptor::PSMDisabled()";
    return batteryLogic->GConfItemValue(SystemUIGConf::BatteryPSMAutoKey).toBool();
}

void BatteryBusinessLogicAdaptor::batteryChargingState()
{
    qDebug() << "BatteryBusinessLogicAdaptor::batteryChargingState()";
    batteryLogic->batteryStatus();
}

QString BatteryBusinessLogicAdaptor::PSMThresholdValue()
{
    qDebug() << "BatteryBusinessLogicAdaptor::PSMThresholdValue(" << batteryLogic->GConfItemValue(SystemUIGConf::BatteryPSMThresholdKey).toString() << ")";
    return batteryLogic->PSMThresholdValue();
}

int BatteryBusinessLogicAdaptor::batteryBarValue()
{
    qDebug () << "BatteryBusinessLogicAdaptor::batteryBarValue()";
    return batteryLogic->batteryBarValue();
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
