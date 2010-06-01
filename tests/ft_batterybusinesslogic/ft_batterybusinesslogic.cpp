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
#include "batterystub.h"
#include "ft_batterybusinesslogic.h"

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
    m_NotificationCame = false;
    m_NotificationText = "";
    m_NotificationIcon = "";
    m_BatteryChargingCame = false;
    m_BatteryNotChargingCame = false;
    m_AnimationRate = -1;

    m_BatteryBarValueCame = false;
    m_BarValue = -1;
}

void
SignalSink::print() 
{
    SYS_DEBUG ("*** m_NotificationCame       = %s", 
            SYS_BOOL(m_NotificationCame));
    SYS_DEBUG ("*** m_NotificationText       = %s", 
            SYS_STR(m_NotificationText));
    SYS_DEBUG ("*** m_NotificationIcon       = %s", 
            SYS_STR(m_NotificationIcon));

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

void 
SignalSink::notificationSent (
        QString      text,
        QString      icon)
{
    SYS_DEBUG ("*** text = %s", SYS_STR(text));
    SYS_DEBUG ("*** icon = %s", SYS_STR(icon));

    m_NotificationCame = true;
    m_NotificationText = text;
    m_NotificationIcon = icon;
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
    int   argc = 1;
    char *app_name = (char*) "./ft_batterybusinesslogic";
    bool  connectSuccess;

    app = new MApplication(argc, &app_name);
    
    systemUIGConf = new SystemUIGConf();
   
    /*
     * Creating one batterybusinesslogic and connecting to its relevant signals.
     */
    SYS_DEBUG ("+++ Creating BatteryBusinessLogic");
    m_Subject = new BatteryBusinessLogic (systemUIGConf);

    connectSuccess = connect (
            m_Subject, SIGNAL(notificationSent(QString, QString)),
            &m_SignalSink, SLOT(notificationSent(QString, QString)));
    QVERIFY (connectSuccess);
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
Ft_BatteryBusinessLogic::testEnterPSM ()
{
    m_SignalSink.reset ();

    m_Subject->m_DeviceMode->setPSMState (QmDeviceMode::PSMStateOn);
    QVERIFY (m_SignalSink.m_NotificationCame);
    QVERIFY (m_SignalSink.m_NotificationText == "qtn_ener_ent_psnote");

    QTest::qWait (DelayBetweenTests);
}

void 
Ft_BatteryBusinessLogic::testLeavePSM ()
{
    m_SignalSink.reset ();

    m_Subject->m_DeviceMode->setPSMState (QmDeviceMode::PSMStateOff);
    QVERIFY (m_SignalSink.m_NotificationCame);
    QVERIFY (m_SignalSink.m_NotificationText == "qtn_ener_exit_psnote");

    QTest::qWait (DelayBetweenTests);
}

void
Ft_BatteryBusinessLogic::testChargingComplete ()
{
    m_SignalSink.reset ();

    m_Subject->m_Battery->emitBatteryStateChanged (QmBattery::StateFull);

    QVERIFY (m_SignalSink.m_NotificationCame);
    QVERIFY (m_SignalSink.m_NotificationText == "qtn_ener_charcomp");
    QVERIFY (m_SignalSink.m_NotificationIcon ==
            "icon-m-energy-management-charging-complete");
    
    QTest::qWait (DelayBetweenTests);
}

void
Ft_BatteryBusinessLogic::testCharging ()
{
    m_SignalSink.reset ();

    m_Subject->m_Battery->emitChargerEvent (QmBattery::Wall);
    m_Subject->m_Battery->emitChargingStateChanged (QmBattery::StateCharging);
    
    QVERIFY (m_SignalSink.m_NotificationCame);
    QVERIFY (m_SignalSink.m_NotificationText == "qtn_ener_charging");
    QVERIFY (m_SignalSink.m_NotificationIcon ==
            "icon-m-energy-management-charging");
    
    QTest::qWait (DelayBetweenTests);
}


void
Ft_BatteryBusinessLogic::testChargingFailed ()
{
    m_SignalSink.reset ();

    m_Subject->m_Battery->emitChargerEvent(QmBattery::Unknown);
    m_Subject->m_Battery->emitChargingStateChanged (
            QmBattery::StateChargingFailed);

    QVERIFY (m_SignalSink.m_NotificationCame);
    QVERIFY (m_SignalSink.m_NotificationText == "qtn_ener_repcharger");
    QVERIFY (m_SignalSink.m_NotificationIcon ==
            "icon-m-energy-management-replace-charger");

    QTest::qWait (DelayBetweenTests);
}

/*!
 * Tests the low battery notifications. Please note, that the 'very low battery'
 * condition is not handled here. 
 *
 * Please check NB#171837 for more details.
 */
void
Ft_BatteryBusinessLogic::testLowBattery ()
{
    m_SignalSink.reset ();

    m_Subject->m_Battery->emitChargerEvent(
            // FIXME: Maemo::QmBattery::None
            (Maemo::QmBattery::ChargerType) 0);
    m_Subject->m_Battery->emitBatteryStateChanged (QmBattery::StateLow);

    QVERIFY (m_SignalSink.m_NotificationCame);
    QVERIFY (m_SignalSink.m_NotificationText == "qtn_ener_lowbatt");
    QVERIFY (m_SignalSink.m_NotificationIcon ==
            "icon-m-energy-management-low-battery");

    QTest::qWait (DelayBetweenTests);
}

/*
 * After the user plugs out the charger from the device, this system banner is
 * displayed to remind the users to unplug charger from the power supply for
 * conserving energy.  Remove charger notification should not be shown in case
 * if USB cable is used for charging the device.
 */
void
Ft_BatteryBusinessLogic::testRemoveCharger ()
{
    /*
     * First we need a dedicated charger.
     */
    m_Subject->m_Battery->emitChargerEvent (QmBattery::Wall);
    QTest::qWait (DelayBetweenTests);

    m_SignalSink.reset ();
    m_Subject->m_Battery->emitChargerEvent (
            // FIXME: Maemo::QmBattery::None
            (Maemo::QmBattery::ChargerType) 0);

    QVERIFY (m_SignalSink.m_NotificationCame);
    QVERIFY (m_SignalSink.m_NotificationText == "qtn_ener_remcha");
    QVERIFY (m_SignalSink.m_NotificationIcon == "");
    
}

QTEST_APPLESS_MAIN(Ft_BatteryBusinessLogic)

