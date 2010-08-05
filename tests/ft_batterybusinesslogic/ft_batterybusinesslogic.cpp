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
#include "ft_batterybusinesslogic.h"

#ifdef HAVE_QMSYSTEM
#include "qmled_stub.h"
#include "qmbattery_stub.h"
#include "qmdevicemode_stub.h"
#endif

#include <MApplication>
#include <MNotification>
#include <MFeedback>

#define DEBUG
#include "debug.h"

/******************************************************************************
 * The stub for MFeedback class.
 */
static QString nameOfLastFeedback;

MFeedback::MFeedback (
		const QString &name, 
		QObject *parent) :
	d_ptr (0)
{
    Q_UNUSED (parent);

    SYS_DEBUG ("*** name = %s", SYS_STR(name));
    nameOfLastFeedback = name;
}

void
MFeedback::play () const 
{
    SYS_DEBUG ("Playing...");
}

int   argc = 1;
char *argv[] = {
    (char *) "./ft_batterybusinesslogic",
    NULL };

void
Ft_BatteryBusinessLogic::initTestCase ()
{
    m_App = new MApplication(argc, argv);
    m_App->setQuitOnLastWindowClosed (false);
}

void
Ft_BatteryBusinessLogic::cleanupTestCase ()
{
    m_App->deleteLater ();
}


void
Ft_BatteryBusinessLogic::init ()
{
    /* Testcase initialization... */
    m_logic = new BatteryBusinessLogic;
}

void
Ft_BatteryBusinessLogic::cleanup ()
{
    /* Testcase deinitialization... */
    delete m_logic;
    m_logic = NULL;
}

void
Ft_BatteryBusinessLogic::testInitBattery ()
{
#ifdef HAVE_QMSYSTEM
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString, QString)));

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
Ft_BatteryBusinessLogic::testLowBatteryAlert ()
{
#ifdef HAVE_QMSYSTEM
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString, QString)));

    m_logic->lowBatteryAlert ();

    QTest::qWait (10);

    QCOMPARE (spy.count (), 1);

    QList<QVariant> arguments = spy.takeFirst ();

    QVERIFY (arguments.at (0).toString () == "x-nokia.battery.lowbattery");
    QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_lowbatt"));
    QVERIFY (arguments.at (2).toString () == "");
#endif
}

void
Ft_BatteryBusinessLogic::testBatteryStateChanged ()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString, QString)));

    gQmBatteryStub->stubReset ();
    gQmLEDStub->stubReset ();

    /* StateFull */
    spy.clear ();
    m_logic->batteryStateChanged (Maemo::QmBattery::StateFull);

    QTest::qWait (10);

    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    QVERIFY (arguments.at (0).toString () == "x-nokia.battery.chargingcomplete");
    QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_charcomp"));
    QVERIFY (arguments.at (2).toString () == "");
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
    QVERIFY (arguments.at (0).toString () == "x-nokia.battery.recharge");
    QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_rebatt"));
    QVERIFY (arguments.at (2).toString () == "");

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
    QVERIFY (arguments.at (0).toString () == "x-nokia.battery.lowbattery");
    QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_lowbatt"));
    QVERIFY (arguments.at (2).toString () == "");
#endif
}

#if 0
void
Ft_BatteryBusinessLogic::testChargingStateChanged ()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString, QString)));

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
      QVERIFY (arguments.at (0).toString () == "x-nokia.battery");
      QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_charging"));
      QVERIFY (arguments.at (2).toString () == m_logic->chargingImageId ());
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
    QVERIFY (arguments.at (0).toString () == "x-nokia.battery.chargingnotstarted");
    QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_repcharger"));
    QVERIFY (arguments.at (2).toString () == "");
#endif
}
#endif

void
Ft_BatteryBusinessLogic::testBatteryChargerEvent ()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString, QString)));

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
    QVERIFY (arguments.at (0).toString () == "x-nokia.battery");
    QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_remcha"));
    QVERIFY (arguments.at (2).toString () == "");
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
Ft_BatteryBusinessLogic::testPSMStateChanged ()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString, QString)));

    /* Entering to power-save mode */
    m_logic->devicePSMStateChanged (Maemo::QmDeviceMode::PSMStateOn);

    QTest::qWait (10);
    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    QVERIFY (arguments.at (0).toString () == "x-nokia.battery.enterpsm");
    QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_ent_psnote"));
    QVERIFY (arguments.at (2).toString () == "");

    spy.clear ();

    /* Exiting from power-save mode */
    m_logic->devicePSMStateChanged (Maemo::QmDeviceMode::PSMStateOff);

    QTest::qWait (10);
    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    QVERIFY (arguments.at (0).toString () == "x-nokia.battery.exitpsm");
    QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_exit_psnote"));
    QVERIFY (arguments.at (2).toString () == "");
#endif
}

void
Ft_BatteryBusinessLogic::testLowBatteryNotifierConnection ()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy (m_logic, SIGNAL (notificationSent (QString, QString, QString)));

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
    QVERIFY (arguments.at (0).toString () == "x-nokia.battery.lowbattery");
    QVERIFY (arguments.at (1).toString () == qtTrId ("qtn_ener_lowbatt"));
    QVERIFY (arguments.at (2).toString () == "");

    /* Simulate now a charging event */
    m_logic->chargingStateChanged (Maemo::QmBattery::StateCharging);

    /* After this call LowBatteryNotifier should be destroyed */
    QVERIFY (m_logic->m_LowBatteryNotifier == 0);
#endif
}

QTEST_APPLESS_MAIN(Ft_BatteryBusinessLogic)

