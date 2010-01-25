/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "batterybusinesslogicadaptor.h"
#include "batterybusinesslogic.h"

#define DEBUG
#include "../debug.h"

BatteryBusinessLogicAdaptor::BatteryBusinessLogicAdaptor (
		QObject              *obj, 
		BatteryBusinessLogic *batteryLogic) : 
	QDBusAbstractAdaptor (obj), 
    m_BatteryLogic (batteryLogic)
{
    connect (m_BatteryLogic, SIGNAL(batteryCharging(int)), 
            this, SIGNAL(batteryCharging(int)));
    connect (m_BatteryLogic, SIGNAL(batteryNotCharging()), 
            this, SIGNAL(batteryNotCharging()));
    connect (m_BatteryLogic, SIGNAL(batteryBarValueChanged(int)), 
            this, SIGNAL(batteryBarValueChanged(int)));
    connect (m_BatteryLogic, SIGNAL(PSMValueChanged(bool)), 
            this, SIGNAL(PSMValueChanged(bool)));
    connect (m_BatteryLogic, SIGNAL(PSMAutoDisabled()), 
            this, SIGNAL(PSMAutoDisabled()));
    connect (m_BatteryLogic, SIGNAL(remainingTimeValuesChanged(QStringList)), 
            this, SIGNAL(remainingTimeValuesChanged(QStringList)));
}


void 
BatteryBusinessLogicAdaptor::setPSMValue (
        bool toggle)
{
    SYS_DEBUG ("*** toggle = %s", toggle ? "yes" : "no");
    m_BatteryLogic->togglePSM (toggle);
}

void
BatteryBusinessLogicAdaptor::setPSMAutoValue (
        bool toggle)
{
    qDebug() << 
        "BatteryBusinessLogicAdaptor::setPSMAutoValue(" << 
        toggle << 
        ")";
    m_BatteryLogic->togglePSMAuto(toggle);
}

void
BatteryBusinessLogicAdaptor::setPSMThresholdValue (
        const QString &value)
{
    qDebug() << 
        "BatteryBusinessLogicAdaptor::setPSMThresholdValue(" << 
        value << 
        ")";
    m_BatteryLogic->setPSMThreshold(value);
}

bool
BatteryBusinessLogicAdaptor::PSMValue ()
{
    qDebug() << "BatteryBusinessLogicAdaptor::PSMValue()";
    return m_BatteryLogic->PSMValue();
}

bool 
BatteryBusinessLogicAdaptor::PSMAutoValue ()
{
    qDebug() << "BatteryBusinessLogicAdaptor::PSMDisabled()";
    return m_BatteryLogic->GConfItemValue (
            SystemUIGConf::BatteryPSMAutoKey).toBool();
}

void 
BatteryBusinessLogicAdaptor::batteryChargingState ()
{
    qDebug() << "BatteryBusinessLogicAdaptor::batteryChargingState()";
    m_BatteryLogic->batteryStatus();
}

QString 
BatteryBusinessLogicAdaptor::PSMThresholdValue ()
{
    qDebug() << "BatteryBusinessLogicAdaptor::PSMThresholdValue()";
    return m_BatteryLogic->PSMThresholdValue();
}

int 
BatteryBusinessLogicAdaptor::batteryBarValue ()
{
    qDebug() << "BatteryBusinessLogicAdaptor::batteryBarValue()";
    return m_BatteryLogic->batteryBarValue();
}

QStringList 
BatteryBusinessLogicAdaptor::PSMThresholdValues ()
{
    qDebug() << "BatteryBusinessLogicAdaptor::PSMThresholdValues()";
    return m_BatteryLogic->PSMThresholdValues();
}

QStringList 
BatteryBusinessLogicAdaptor::remainingTimeValues ()
{
    qDebug() << "BatteryBusinessLogicAdaptor::remainingTimeValues()";
    return m_BatteryLogic->remainingTimeValues();
}
