/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino=" (0,W2s,i2s,t0,l1,:0" */
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
#include "ft_batterybusinesslogic.h"
#include "batterystub.h"

#include <MApplication>
#include <MTheme>
#include <MLocale>

#include <QVariant>

#define DEBUG
#include "../../src/debug.h"

using namespace Maemo;

/*
 * This much we wait between the tests.
 */
static int DelayBetweenTests = 5000;

/******************************************************************************
 * SignalSink implementation.
 */
SignalSink::SignalSink() 
{
    reset ();
}

void
SignalSink::reset()
{
    m_BatteryChargingCame = false;
    m_BatteryNotChargingCame = false;
    m_AnimationRate = -1;

    m_BatteryBarValueCame = false;
    m_BarValue = -1;
}

void
SignalSink::print() 
{
    SYS_DEBUG ("*** m_BatteryChargingCame    = %s", 
            SYS_BOOL(m_BatteryChargingCame));
    SYS_DEBUG ("*** m_BatteryNotChargingCame = %s", 
            SYS_BOOL(m_BatteryNotChargingCame));
    SYS_DEBUG ("*** m_AnimationRate          = %d",
            m_AnimationRate);

    SYS_DEBUG ("*** m_BatteryBarValueCame    = %s",
            SYS_BOOL(m_BatteryBarValueCame));
    SYS_DEBUG ("*** m_BarValue               = %d",
            m_BarValue);
}

void 
SignalSink::batteryCharging (
        int animationLevel)
{
    SYS_DEBUG ("*** animationLevel = %d", animationLevel);
    m_BatteryChargingCame = true;
    m_AnimationRate = animationLevel;
}

void
SignalSink::batteryNotCharging ()
{
    SYS_DEBUG ("");
    m_BatteryNotChargingCame = true;
}

void
SignalSink::batteryBarValueChanged (
        int barValue)
{
    m_BatteryBarValueCame = true;
    m_BarValue = barValue;
}

/******************************************************************************
 * Ft_BatteryBusinessLogic implementation.
 */
void Ft_BatteryBusinessLogic::init()
{
}

void Ft_BatteryBusinessLogic::cleanup()
{
}

MApplication *app;
void Ft_BatteryBusinessLogic::initTestCase()
{
    int argc = 1;
    char* app_name = (char*) "./ft_batterybusinesslogic";
    app = new MApplication(argc, &app_name);
    
    systemUIGConf = new SystemUIGConf();
    
    SYS_DEBUG ("+++ Creating BatteryBusinessLogic");
    m_Subject = new BatteryBusinessLogic (systemUIGConf);
}

void 
Ft_BatteryBusinessLogic::cleanupTestCase()
{
    SYS_DEBUG ("+++ Cleaning up");
    delete m_Subject;
    m_Subject = NULL;

    delete systemUIGConf;
    systemUIGConf = NULL;

    delete app;
}

void
Ft_BatteryBusinessLogic::testChargingComplete ()
{
    m_Subject->m_Battery->emitBatteryStateChanged (QmBattery::StateFull);
    m_XChecker.debugDumpNotifications ();
    QTest::qWait (DelayBetweenTests);
}

void
Ft_BatteryBusinessLogic::testCharging ()
{
    m_Subject->m_Battery->emitChargerEvent (QmBattery::Wall);
    m_Subject->m_Battery->emitChargingStateChanged (QmBattery::StateCharging);
    m_XChecker.debugDumpNotifications ();
    QTest::qWait (DelayBetweenTests);
}


void
Ft_BatteryBusinessLogic::testChargingFailed ()
{
    m_Subject->m_Battery->emitChargerEvent(QmBattery::Unknown);
    m_Subject->m_Battery->emitChargingStateChanged (QmBattery::StateChargingFailed);
    m_XChecker.debugDumpNotifications ();
    QTest::qWait (DelayBetweenTests);
}

void
Ft_BatteryBusinessLogic::testLowBattery ()
{
    SYS_DEBUG ("====================");
    m_Subject->m_Battery->emitChargerEvent(QmBattery::Unknown);
    m_Subject->m_Battery->emitBatteryStateChanged (QmBattery::StateLow);
    m_XChecker.debugDumpNotifications ();
    QTest::qWait (DelayBetweenTests);
}


QTEST_APPLESS_MAIN(Ft_BatteryBusinessLogic)

