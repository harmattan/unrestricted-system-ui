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

#ifdef HAVE_QMSYSTEM
#include "qmled_stub.h"
#include "qmbattery_stub.h"
#include "qmdevicemode_stub.h"
#endif

#include <MNotification>

#define DEBUG
#include "debug.h"

/*
 * TODO:
 *  - test the feedback playing also
 *  [XXX: we need to know first, how to play those correctly...]
 */

bool
MNotification::publish ()
{
/*
 * XXX: We don't want to show real notifications
 * in unit-test:
 */
    return false;
}

bool
MNotification::remove ()
{
    return false;
}


void
Ut_BatteryBusinessLogic::initTestCase ()
{
    /* Add testsuite initialization here */
}

void
Ut_BatteryBusinessLogic::cleanupTestCase ()
{
    /* Add testsuite deinitialization here */
}


void
Ut_BatteryBusinessLogic::init ()
{
    /* Testcase initialization... */
    m_logic = new BatteryBusinessLogic;
}

void
Ut_BatteryBusinessLogic::cleanup ()
{
    /* Testcase deinitialization... */
    delete m_logic;
    m_logic = NULL;
}

void
Ut_BatteryBusinessLogic::testInitBattery ()
{
#ifdef HAVE_QMSYSTEM
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString)));

    /* Set some ret. values for stubs */
    gQmBatteryStub->stubSetReturnValue<Maemo::QmBattery::ChargingState> (
        "getChargingState", Maemo::QmBattery::StateNotCharging);
    gQmBatteryStub->stubSetReturnValue<Maemo::QmBattery::BatteryState> (
        "getBatteryState", Maemo::QmBattery::StateOK);

    /* no notification should be shown,
     * and battery-charging pattern should be deactivated:
     */
    m_logic->initBattery ();
    /* wait for signals... if any... */
    QTest::qWait (10);

    QCOMPARE (spy.count (), 0);
    QVERIFY (gQmLEDStub->stubLastCallTo ("deactivate").parameter<QString>(0)
             == QString ("PatternBatteryCharging"));
#endif
}

void
Ut_BatteryBusinessLogic::testLowBatteryAlert ()
{
#ifdef HAVE_QMSYSTEM
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString)));

    m_logic->lowBatteryAlert ();

    QTest::qWait (10);

    QCOMPARE (spy.count (), 1);

    QList<QVariant> arguments = spy.takeFirst ();

    QVERIFY (arguments.at (0).toString () == qtTrId ("qtn_ener_lowbatt"));
    QVERIFY (arguments.at (1).toString () == "icon-m-energy-management-low-battery");
#endif
}

void
Ut_BatteryBusinessLogic::testBatteryStateChanged ()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString)));

    gQmBatteryStub->stubReset ();
    gQmLEDStub->stubReset ();

    /* StateFull */
    spy.clear ();
    m_logic->batteryStateChanged (Maemo::QmBattery::StateFull);

    QTest::qWait (10);

    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    QVERIFY (arguments.at (0).toString () == qtTrId ("qtn_ener_charcomp"));
    QVERIFY (arguments.at (1).toString () == "icon-m-energy-management-remove-charger");
    QVERIFY (gQmLEDStub->stubLastCallTo ("activate").parameter<QString>(0)
             == QString ("PatternBatteryFull"));

    /* StateOK */
    spy.clear ();
    m_logic->batteryStateChanged (Maemo::QmBattery::StateOK);

    QTest::qWait (10);
    /* no signals/notifications should come, just silently no-op */
    QCOMPARE (spy.count (), 0);

    /* StateEmpty */
    spy.clear ();
    m_logic->batteryStateChanged (Maemo::QmBattery::StateEmpty);

    QTest::qWait (10);
    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    QVERIFY (arguments.at (0).toString () == qtTrId ("qtn_ener_rebatt"));
    QVERIFY (arguments.at (1).toString () == "icon-m-energy-management-recharge");

    /* StateError */
    spy.clear ();
    m_logic->batteryStateChanged (Maemo::QmBattery::StateError);

    QTest::qWait (10);
    /* no signals/notifications should come, just silently no-op */
    QCOMPARE (spy.count (), 0);

    /* StateLow and charging */
    spy.clear ();
    gQmBatteryStub->stubSetReturnValue<Maemo::QmBattery::ChargingState> (
        "getChargingState", Maemo::QmBattery::StateCharging);
    m_logic->batteryStateChanged (Maemo::QmBattery::StateLow);

    QTest::qWait (10);
    /* no signals/notifications should come, because battery is charging... */
    QCOMPARE (spy.count (), 0);

    /* StateLow and not charging */
    spy.clear ();
    gQmBatteryStub->stubSetReturnValue<Maemo::QmBattery::ChargingState> (
        "getChargingState", Maemo::QmBattery::StateNotCharging);
    m_logic->batteryStateChanged (Maemo::QmBattery::StateLow);

    QTest::qWait (10);

    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    QVERIFY (arguments.at (0).toString () == qtTrId ("qtn_ener_lowbatt"));
    QVERIFY (arguments.at (1).toString () == "icon-m-energy-management-low-battery");
#endif
}

void
Ut_BatteryBusinessLogic::testChargingStateChanged ()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString)));

    gQmBatteryStub->stubReset ();
    gQmLEDStub->stubReset ();

    for (int i = 0; i <= 100; i += 5)
    {
      /* StateCharging */
      gQmBatteryStub->stubSetReturnValue<int> ("getRemainingCapacityPct", i);
      m_logic->chargingStateChanged (Maemo::QmBattery::StateCharging);

      QTest::qWait (1);
      QCOMPARE (spy.count (), 1);
      arguments = spy.takeFirst ();
      QVERIFY (arguments.at (0).toString () == qtTrId ("qtn_ener_charging"));
      QVERIFY (arguments.at (1).toString () == m_logic->chargingImageId ());
      QVERIFY (gQmLEDStub->stubLastCallTo ("activate").parameter<QString>(0)
               == QString ("PatternBatteryCharging"));
      spy.clear ();
    }

    /* StateNotCharging */
    m_logic->chargingStateChanged (Maemo::QmBattery::StateNotCharging);

    QTest::qWait (10);
    QCOMPARE (spy.count (), 0);
    QVERIFY (gQmLEDStub->stubLastCallTo ("deactivate").parameter<QString>(0)
             == QString ("PatternBatteryCharging"));
    spy.clear ();

    /* StateChargingFailed */
    m_logic->chargingStateChanged (Maemo::QmBattery::StateChargingFailed);

    QTest::qWait (10);
    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    QVERIFY (arguments.at (0).toString () == qtTrId ("qtn_ener_repcharger"));
    QVERIFY (arguments.at (1).toString () == "icon-m-energy-management-replace-charger");
#endif
}

void
Ut_BatteryBusinessLogic::testBatteryChargerEvent ()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString)));

    /* Wall charger */
    m_logic->batteryChargerEvent (Maemo::QmBattery::Wall);
    QCOMPARE (m_logic->m_ChargerType, Maemo::QmBattery::Wall);

    /* Plug out : charger type = none */
    m_logic->batteryChargerEvent (Maemo::QmBattery::None);
    QCOMPARE (m_logic->m_ChargerType, Maemo::QmBattery::None);

    QTest::qWait (10);
    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    /* Look for the notification: "Disconnect the charger from..." */
    QVERIFY (arguments.at (0).toString () == qtTrId ("qtn_ener_remcha"));
    spy.clear ();

    /* USB 500mA */
    m_logic->batteryChargerEvent (Maemo::QmBattery::USB_500mA);
    QCOMPARE (m_logic->m_ChargerType, Maemo::QmBattery::USB_500mA);

    /* USB 100mA */
    m_logic->batteryChargerEvent (Maemo::QmBattery::USB_100mA);
    QCOMPARE (m_logic->m_ChargerType, Maemo::QmBattery::USB_100mA);

    /* Unknown */
    m_logic->batteryChargerEvent (Maemo::QmBattery::Unknown);
    QCOMPARE (m_logic->m_ChargerType, Maemo::QmBattery::Unknown);
#endif
}

void
Ut_BatteryBusinessLogic::testPSMStateChanged ()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString)));

    /* Entering to power-save mode */
    m_logic->devicePSMStateChanged (Maemo::QmDeviceMode::PSMStateOn);

    QTest::qWait (10);
    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    QVERIFY (arguments.at (0).toString () == qtTrId ("qtn_ener_ent_psnote"));

    spy.clear ();

    /* Exiting from power-save mode */
    m_logic->devicePSMStateChanged (Maemo::QmDeviceMode::PSMStateOff);

    QTest::qWait (10);
    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    QVERIFY (arguments.at (0).toString () == qtTrId ("qtn_ener_exit_psnote"));
#endif
}

void
Ut_BatteryBusinessLogic::testLowBatteryNotifierConnection ()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString)));

    /* LowBatteryNotifier shouldn't be instantiated at first */
    QVERIFY (m_logic->m_LowBatteryNotifier == 0);

    /* Simulate battery-state-low change */
    gQmBatteryStub->stubSetReturnValue<Maemo::QmBattery::ChargingState> (
        "getChargingState", Maemo::QmBattery::StateNotCharging);
    m_logic->batteryStateChanged (Maemo::QmBattery::StateLow);

    /* LowBatteryNotifier should be exists now... */
    QVERIFY (m_logic->m_LowBatteryNotifier != 0);
    QTest::qWait (10);

    /* And should send a low-battery notification */
    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    QVERIFY (arguments.at (0).toString () == qtTrId ("qtn_ener_lowbatt"));
    QVERIFY (arguments.at (1).toString () == "icon-m-energy-management-low-battery");

    /* Simulate now a charging event */
    m_logic->chargingStateChanged (Maemo::QmBattery::StateCharging);

    /* After this call LowBatteryNotifier should be destroyed */
    QVERIFY (m_logic->m_LowBatteryNotifier == 0);
#endif
}

QTEST_MAIN(Ut_BatteryBusinessLogic)
