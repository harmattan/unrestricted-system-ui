/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "batterybrief.h"
#include <dcpwidgettypes.h>

#include "batterydbusinterface.h"

#define DEBUG
#include "../debug.h"

BatteryBrief::BatteryBrief () :
    m_Charging (false),
    m_NotCharging (false)
{
    SYS_DEBUG ("");
    
    m_BatteryIf = new BatteryDBusInterface;
    connect (m_BatteryIf, SIGNAL (batteryCharging(int)),
            this, SLOT(batteryCharging(int)));
    connect (m_BatteryIf, SIGNAL (batteryNotCharging()),
            this, SLOT(batteryNotCharging()));

    m_BatteryIf->batteryChargingStateRequired ();
}

BatteryBrief::~BatteryBrief ()
{
    delete m_BatteryIf;
    m_BatteryIf = 0;
}


QString 
BatteryBrief::valueText() const
{
    if (m_Charging)
        return "Charging";
    else if (m_NotCharging)
        return "Not charging";

    return "N/A";
}


void 
BatteryBrief::batteryCharging (int n)
{
    SYS_DEBUG ("CHARGING *** n = %d", n);
    m_Charging = true;
    m_NotCharging = false;
    
    emit valuesChanged ();
}

void 
BatteryBrief::batteryNotCharging ()
{
    SYS_DEBUG ("NOT CHARGING");
    m_Charging = false;
    m_NotCharging = true;

    emit valuesChanged ();
}
