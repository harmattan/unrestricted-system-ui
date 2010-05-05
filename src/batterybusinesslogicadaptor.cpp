/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "batterybusinesslogicadaptor.h"
#include "batterybusinesslogic.h"

#define DEBUG
#include "debug.h"

BatteryBusinessLogicAdaptor::BatteryBusinessLogicAdaptor (
		QObject              *obj, 
		BatteryBusinessLogic *batteryLogic) : 
	QDBusAbstractAdaptor (obj), 
    m_BatteryLogic (batteryLogic)
{
    SYS_DEBUG ("");
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
    connect (m_BatteryLogic, SIGNAL (batteryFullyCharged ()),
             this, SIGNAL (batteryFull ()));
}


void 
BatteryBusinessLogicAdaptor::setPSMValue (
        bool toggle)
{
    SYS_DEBUG ("*** toggle = %s", SYS_BOOL(toggle));
    m_BatteryLogic->togglePSM (toggle);
}

void
BatteryBusinessLogicAdaptor::setPSMAutoValue (
        bool toggle)
{
    SYS_DEBUG ("");
    m_BatteryLogic->togglePSMAuto(toggle);
}

void
BatteryBusinessLogicAdaptor::setPSMThresholdValue (
        const QString &value)
{
    SYS_DEBUG ("");
    m_BatteryLogic->setPSMThreshold(value);
}

bool
BatteryBusinessLogicAdaptor::PSMValue ()
{
    SYS_DEBUG ("Returning %s", SYS_BOOL(m_BatteryLogic->PSMValue()));
    return m_BatteryLogic->PSMValue();
}

bool 
BatteryBusinessLogicAdaptor::PSMAutoValue ()
{
    bool retval = m_BatteryLogic->GConfItemValue (
            SystemUIGConf::BatteryPSMAutoKey).toBool();

    SYS_DEBUG ("Returning %s", SYS_BOOL(retval));
    return retval;
}

void 
BatteryBusinessLogicAdaptor::batteryChargingState ()
{
    SYS_DEBUG ("");
    m_BatteryLogic->batteryStatus();
}

QString 
BatteryBusinessLogicAdaptor::PSMThresholdValue ()
{
    SYS_DEBUG ("Returning %s", SYS_STR(m_BatteryLogic->PSMThresholdValue()));
    return m_BatteryLogic->PSMThresholdValue();
}

int 
BatteryBusinessLogicAdaptor::batteryBarValue ()
{
    SYS_DEBUG ("Returning %d", m_BatteryLogic->batteryBarValue());
    return m_BatteryLogic->batteryBarValue();
}

QStringList 
BatteryBusinessLogicAdaptor::PSMThresholdValues ()
{
    SYS_DEBUG ("");
    return m_BatteryLogic->PSMThresholdValues();
}

QStringList 
BatteryBusinessLogicAdaptor::remainingTimeValues ()
{
    SYS_DEBUG ("");
    return m_BatteryLogic->remainingTimeValues();
}
