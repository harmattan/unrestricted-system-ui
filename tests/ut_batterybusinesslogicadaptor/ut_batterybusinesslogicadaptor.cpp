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

#include <QtTest/QtTest>
#include <QString>
#include <gconfstub.h>
#include "ut_batterybusinesslogicadaptor.h"
#include "batterybusinesslogicadaptor.h"
#include "batterybusinesslogic_stub.h"
#include "lowbatterynotifier_stub.h"

void
Ut_BatteryBusinessLogicAdaptor::initTestCase ()
{
    gBatteryBusinessLogicStub->stubReset();
    m_logic = new BatteryBusinessLogic (new SystemUIGConf, this);
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
    m_subject->setPSMValue(true);
    QCOMPARE(gBatteryBusinessLogicStub->stubCallCount("togglePSM"), 1);
    QCOMPARE(gBatteryBusinessLogicStub->stubLastCallTo("togglePSM").parameter<bool>(0), true);
}

void
Ut_BatteryBusinessLogicAdaptor::setPSMAutoValue ()
{
    m_subject->setPSMAutoValue (true);
    QCOMPARE(gBatteryBusinessLogicStub->stubCallCount("togglePSMAuto"), 1);
    QCOMPARE(gBatteryBusinessLogicStub->stubLastCallTo("togglePSMAuto").parameter<bool>(0), true);
}

void
Ut_BatteryBusinessLogicAdaptor::setPSMThresholdValue ()
{
    m_subject->setPSMThresholdValue (QString ("15"));
    QCOMPARE(gBatteryBusinessLogicStub->stubCallCount("setPSMThreshold"), 1);
    QCOMPARE(gBatteryBusinessLogicStub->stubLastCallTo("setPSMThreshold").parameter<QString>(0), QString("15"));
}

void
Ut_BatteryBusinessLogicAdaptor::remainingTimeValues ()
{
    QStringList list;
    list << "5" << "10";
    gBatteryBusinessLogicStub->stubSetReturnValue("remainingTimeValues", list);
    QStringList result = m_subject->remainingTimeValues ();
    QCOMPARE(result, list);
}

void
Ut_BatteryBusinessLogicAdaptor::PSMThresholdValues ()
{
    QStringList list;
    list << "5" << "10";
    gBatteryBusinessLogicStub->stubSetReturnValue("PSMThresholdValues", list);
    QStringList result = m_subject->PSMThresholdValues();
    QCOMPARE(result, list);
}

void
Ut_BatteryBusinessLogicAdaptor::batteryChargingState ()
{
    m_subject->batteryChargingState ();
    QCOMPARE(gBatteryBusinessLogicStub->stubCallCount("batteryStatus"), 1);
}

void
Ut_BatteryBusinessLogicAdaptor::batteryBarValue ()
{
    gBatteryBusinessLogicStub->stubSetReturnValue("batteryBarValue", 5);
    int result = m_subject->batteryBarValue();
    QCOMPARE(result, 5);
}

QTEST_MAIN(Ut_BatteryBusinessLogicAdaptor)
