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
/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include <QString>
#include <gconfstub.h>
#include "ut_batterybusinesslogicadaptor.h"
#include <batterybusinesslogicadaptor.h>

MyBatteryBusinessLogic::MyBatteryBusinessLogic (
    SystemUIGConf *gconf, QObject *parent) 
    : BatteryBusinessLogic (gconf, parent)
{

}

void
MyBatteryBusinessLogic::setPSMThreshold (const QString &threshold)
{
    psmThresholdValue = threshold;
}

QString
MyBatteryBusinessLogic::PSMThresholdValue ()
{
    return psmThresholdValue;
}

void
MyBatteryBusinessLogic::togglePSMAuto (bool toggle)
{
    psmAutoValue = toggle;
}

void
MyBatteryBusinessLogic::togglePSM (bool toggle)
{
    psmValue = toggle;
}

bool
MyBatteryBusinessLogic::PSMValue ()
{
    return psmValue;
}

void
Ut_BatteryBusinessLogicAdaptor::initTestCase ()
{
    m_logic = new MyBatteryBusinessLogic (new SystemUIGConf, this);
    m_subject = new BatteryBusinessLogicAdaptor (this, m_logic);
}

void
Ut_BatteryBusinessLogicAdaptor::cleanupTestCase ()
{
    delete m_subject;
    delete m_logic;
}

void
Ut_BatteryBusinessLogicAdaptor::setPSMValue ()
{
    m_subject->setPSMValue (true);

    QVERIFY (m_logic->psmValue == true);
    QVERIFY (m_subject->PSMValue () == true);

    m_subject->setPSMValue (false);

    QVERIFY (m_logic->psmValue == false);
    QVERIFY (m_subject->PSMValue () == false);
}

void
Ut_BatteryBusinessLogicAdaptor::setPSMAutoValue ()
{
    m_subject->setPSMAutoValue (true);
    QVERIFY (m_logic->psmAutoValue == true);
    m_subject->setPSMAutoValue (false);
    QVERIFY (m_logic->psmAutoValue == false);
}

void
Ut_BatteryBusinessLogicAdaptor::setPSMThresholdValue ()
{
    m_subject->setPSMThresholdValue (QString ("15"));

    QVERIFY (m_logic->psmThresholdValue == "15");
    QVERIFY (m_subject->PSMThresholdValue () == "15");

    m_subject->setPSMThresholdValue (QString ("0"));

    QVERIFY (m_logic->psmThresholdValue == "0");
    QVERIFY (m_subject->PSMThresholdValue () == "0");
}

void
Ut_BatteryBusinessLogicAdaptor::remainingTimeValues ()
{
    QStringList toTest = m_subject->remainingTimeValues ();

    // This should return two strings in any case:
    QCOMPARE (toTest.count (), 2);
}

void
Ut_BatteryBusinessLogicAdaptor::PSMThresholdValues ()
{
    QStringList toTest = m_subject->PSMThresholdValues ();

    // This list should contains valid integers (> 0)
    for (int i = 0; i < toTest.count (); i++)
    {
        QVERIFY (toTest.at (i).toInt () > 0);
    }
}

void
Ut_BatteryBusinessLogicAdaptor::batteryChargingState ()
{
    QSignalSpy charging (m_subject, SIGNAL (batteryCharging (int)));
    QSignalSpy notcharging (m_subject, SIGNAL (batteryNotCharging ()));

    m_subject->batteryChargingState ();

    // Wait for some time (for event-handling, let some time for signals...)
    QTest::qWait (300);

    // One of those signals should be emitted
    QVERIFY (charging.count () + notcharging.count () == 1);
}

void
Ut_BatteryBusinessLogicAdaptor::batteryBarValue ()
{
    int toTest = m_subject->batteryBarValue ();

    // This value should be between 0 and 9
    QVERIFY ((toTest >= 0) && (toTest <= 9));
}

QTEST_MAIN(Ut_BatteryBusinessLogicAdaptor)
