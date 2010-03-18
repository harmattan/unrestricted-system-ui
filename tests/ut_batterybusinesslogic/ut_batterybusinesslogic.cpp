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

#include <DuiApplication>
#include <DuiTheme>
#include <DuiLocale>

#include <QVariant>

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

DuiApplication *app;
void Ut_BatteryBusinessLogic::initTestCase()
{
    int argc = 1;
    char* app_name = (char*) "./ut_batterybusinesslogic";
    app = new DuiApplication(argc, &app_name);
}

void Ut_BatteryBusinessLogic::cleanupTestCase()
{
    delete app;
}

void Ut_BatteryBusinessLogic::testSetPSMThreshold()
{
    // set the battery level to 15%
    m_subject->m_Battery->setBatteryEnergyLevel(15);

    // set the threshold level to 10%
    const QString test = "10";
    m_subject->setPSMThreshold(test);

    // check that the value is set
    QCOMPARE(m_subject->PSMThresholdValue(), test);

    // set the PSM state to Off (just to make sure)
    m_subject->m_DeviceMode->setPSMState(Maemo::QmDeviceMode::PSMStateOff);

    // set the PSM auto on
    systemUIGConf->setValue(SystemUIGConf::BatteryPSMAutoKey, QVariant(true));

    // set the threshold level to 25%
    const QString test2 = "25";
    m_subject->setPSMThreshold(test2);

    // check that the value is set
    QCOMPARE(m_subject->PSMThresholdValue(), test2);

    // check that the PSM is toggled on
    QCOMPARE(m_subject->m_DeviceMode->getPSMState(), Maemo::QmDeviceMode::PSMStateOn);

    // set the threshold level to 5%
    const QString test3 = "5";
    m_subject->setPSMThreshold(test3);

    // check that the value is set
    QCOMPARE(m_subject->PSMThresholdValue(), test3);

    // check that the PSM is toggled off
    QCOMPARE(m_subject->m_DeviceMode->getPSMState(), Maemo::QmDeviceMode::PSMStateOff);

}

void Ut_BatteryBusinessLogic::testTogglePSM()
{
    QSignalSpy spy(m_subject, SIGNAL(PSMAutoDisabled()));

    // set the PSM auto on
    systemUIGConf->setValue(SystemUIGConf::BatteryPSMAutoKey, QVariant(true));

    // turn off the PSM
    m_subject->togglePSM(false);

    // check that the PSM auto is turned off
    QCOMPARE(systemUIGConf->value(SystemUIGConf::BatteryPSMAutoKey).toBool(), false);

    // check that the PSM auto turn off signal is emitted
    QCOMPARE(spy.count(), 1);
    spy.takeFirst();

    // set the PSM auto on
    systemUIGConf->setValue(SystemUIGConf::BatteryPSMAutoKey, QVariant(true));

    // turn on the PSM
    m_subject->togglePSM(true);

    // check that the PSM auto is turned off
    QCOMPARE(systemUIGConf->value(SystemUIGConf::BatteryPSMAutoKey).toBool(), false);

    // check that the PSM auto turn off signal is emitted
    QCOMPARE(spy.count(), 1);
}

void Ut_BatteryBusinessLogic::testTogglePSMAuto()
{
    // set the PSM auto on
    systemUIGConf->setValue(SystemUIGConf::BatteryPSMAutoKey, QVariant(true));

    // set the PSM on
    m_subject->m_DeviceMode->setPSMState(Maemo::QmDeviceMode::PSMStateOn);

    // toggle the PSM auto off
    m_subject->togglePSMAuto(false);

    // check that the PSM auto is toggled off
    QCOMPARE(systemUIGConf->value(SystemUIGConf::BatteryPSMAutoKey).toBool(), false);

    // check that the PSM is toggled off
    QCOMPARE(m_subject->m_DeviceMode->getPSMState(), Maemo::QmDeviceMode::PSMStateOff);

    // set the battery level to 5%
    m_subject->m_Battery->setBatteryEnergyLevel(5);

    // set the threshold level to 10%
    systemUIGConf->setValue(SystemUIGConf::BatteryPSMThresholdKey, QVariant("10"));

    // toggle the PSM auto on
    m_subject->togglePSMAuto(true);

    // check that the PSM is toggled on
    QCOMPARE(m_subject->m_DeviceMode->getPSMState(), Maemo::QmDeviceMode::PSMStateOn);

}

QTEST_APPLESS_MAIN(Ut_BatteryBusinessLogic)
