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
#include "ut_batterybusinesslogic.h"

#include <MApplication>
#include <MTheme>
#include <MLocale>

#include <QVariant>

#define DEBUG
#include "../../src/debug.h"

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
 * Ut_BatteryBusinessLogic implementation.
 */
void Ut_BatteryBusinessLogic::init()
{
    systemUIGConf = new SystemUIGConf();
    m_subject = new BatteryBusinessLogic(systemUIGConf);
}

void Ut_BatteryBusinessLogic::cleanup()
{
    delete m_subject;
    m_subject = NULL;
    delete systemUIGConf;
    systemUIGConf = NULL;
}

MApplication *app;
void Ut_BatteryBusinessLogic::initTestCase()
{
    static int argc = 1;
    static char* app_name = (char*) "./ut_batterybusinesslogic";
    app = new MApplication(argc, &app_name);
}

void
Ut_BatteryBusinessLogic::cleanupTestCase()
{
    delete app;
}

/*!
 * Asks the battery applet for the battery status, checks one of the
 * batteryCharging(int) and batteryNotCharging() signals arrive.
 */
void
Ut_BatteryBusinessLogic::testBatteryChargingSignal ()
{
    bool connectSuccess;

    /*
     * We connect to the relevant signals and check if the signals are there.
     */
    connectSuccess = connect (
            m_subject, SIGNAL(batteryCharging(int)),
            &m_SignalSink, SLOT(batteryCharging(int)));
    QVERIFY (connectSuccess);

    connectSuccess = connect (
            m_subject, SIGNAL(batteryNotCharging()),
            &m_SignalSink, SLOT(batteryNotCharging()));
    QVERIFY (connectSuccess);

    SYS_DEBUG ("*************************************************");
    SYS_DEBUG ("*** calling batteryStatus() *********************");
    SYS_DEBUG ("*************************************************");
    m_SignalSink.reset();
    m_subject->batteryStatus ();
    m_SignalSink.print();

    // Battery can not be charging and not charging in the same time.
    QVERIFY (!(m_SignalSink.m_BatteryChargingCame &&
                m_SignalSink.m_BatteryNotChargingCame));
    // But it has to do somethhing!
    QVERIFY (!(!m_SignalSink.m_BatteryChargingCame &&
                !m_SignalSink.m_BatteryNotChargingCame));
}

void
Ut_BatteryBusinessLogic::testBatteryBarValueChangedSignal ()
{
    bool connectSuccess;

    /*
     * We connect to the relevant signals and check if the signals are there.
     */
    connectSuccess = connect (
            m_subject, SIGNAL(batteryBarValueChanged(int)),
            &m_SignalSink, SLOT(batteryBarValueChanged(int)));
    QVERIFY (connectSuccess);

    /*
     * Let's try on 3%...
     */
    m_SignalSink.reset();
    m_subject->batteryEnergyLevelChanged (3);
    m_SignalSink.print();

    QVERIFY (m_SignalSink.m_BatteryBarValueCame);
    QVERIFY (m_SignalSink.m_BarValue == 1);

    /*
     * And then on 97%
     */
    m_SignalSink.reset();
    m_subject->batteryEnergyLevelChanged (97);
    m_SignalSink.print();

    QVERIFY (m_SignalSink.m_BatteryBarValueCame);
    QVERIFY (m_SignalSink.m_BarValue == 9);
}

void
Ut_BatteryBusinessLogic::testSetPSMThreshold()
{
    // set the battery level to 15%
    m_subject->m_Battery->setBatteryEnergyLevel (14);

    // set the threshold level to 20%
    const QString test = "20";
    m_subject->setPSMThreshold(test);

    // check that the value is set
    QCOMPARE(m_subject->PSMThresholdValue(), test);

    // set the PSM state to Off (just to make sure)
    m_subject->m_DeviceMode->setPSMState(Maemo::QmDeviceMode::PSMStateOff);

    // set the PSM auto on
    systemUIGConf->setValue (SystemUIGConf::BatteryPSMAutoKey, true);

    // set the threshold level to 30%
    const QString test2 = "30";
    m_subject->setPSMThreshold(test2);

    // check that the value is set
    QCOMPARE(m_subject->PSMThresholdValue(), test2);

    // check that the PSM is toggled on
    // Well, we are not doing in the sysuid any more.
    // See NB#169777 - Power save mode logic problems
    #if 0
    QCOMPARE(
            m_subject->m_DeviceMode->getPSMState(),
            Maemo::QmDeviceMode::PSMStateOn);
    #endif

    // set the threshold level to 10%
    const QString test3 = "10";
    m_subject->setPSMThreshold(test3);

    // check that the value is set
    QCOMPARE(m_subject->PSMThresholdValue(), test3);

    // check that the PSM is toggled off
    // Well, we are not doing in the sysuid any more.
    // See NB#169777 - Power save mode logic problems
    #if 0
    QCOMPARE(
            m_subject->m_DeviceMode->getPSMState(),
            Maemo::QmDeviceMode::PSMStateOn);
    #endif
    QCOMPARE(
            m_subject->m_DeviceMode->getPSMState(),
            Maemo::QmDeviceMode::PSMStateOff);
}

void Ut_BatteryBusinessLogic::testTogglePSMAuto()
{
    // set the PSM auto on
    systemUIGConf->setValue(SystemUIGConf::BatteryPSMAutoKey, QVariant(true));

    // set the PSM on
    m_subject->m_DeviceMode->setPSMState(Maemo::QmDeviceMode::PSMStateOn);
    // toggle the PSM auto off
    m_subject->togglePSMAuto(false);

    // check that the PSM is toggled off
    QCOMPARE(
            m_subject->m_DeviceMode->getPSMState(),
            Maemo::QmDeviceMode::PSMStateOn);

    // Check that the psm auto is off.
    QCOMPARE(
            systemUIGConf->value(SystemUIGConf::BatteryPSMAutoKey).toBool(),
            false);

    // set the battery level to 5%
    m_subject->m_Battery->setBatteryEnergyLevel(5);

    // set the threshold level to 10%
    m_subject->setPSMThreshold ("10");

    // toggle the PSM auto on
    m_subject->togglePSMAuto(true);

    // check that the PSM is toggled on
    // Well, we are not doing in the sysuid any more.
    // See NB#169777 - Power save mode logic problems
    #if 0
    QCOMPARE(
            m_subject->m_DeviceMode->getPSMState(),
            Maemo::QmDeviceMode::PSMStateOn);
    #endif
}

void
Ut_BatteryBusinessLogic::testBatteryBarValue()
{
    // Test invalid values:
    QVERIFY (m_subject->batteryBarValue (111) == 2);

    // Test percentage values in valid range
    // >= 84
    QVERIFY (m_subject->batteryBarValue (99) == 9);
    // 73 <= value < 84
    QVERIFY (m_subject->batteryBarValue (77) == 8);
    // 62 <= value < 73
    QVERIFY (m_subject->batteryBarValue (66) == 7);
    // 51 <= value < 62
    QVERIFY (m_subject->batteryBarValue (55) == 6);
    // 39 <= value < 51
    QVERIFY (m_subject->batteryBarValue (44) == 5);
    // 28 <= value < 39
    QVERIFY (m_subject->batteryBarValue (33) == 4);
    // 17 <= value < 28
    QVERIFY (m_subject->batteryBarValue (22) == 3);
    //  5 <= value < 17
    QVERIFY (m_subject->batteryBarValue (11) == 2);
    //  0 <  value < 5
    QVERIFY (m_subject->batteryBarValue (3)  == 1);
    //  0
    QVERIFY (m_subject->batteryBarValue (0)  == 0);
}

QTEST_APPLESS_MAIN(Ut_BatteryBusinessLogic)
