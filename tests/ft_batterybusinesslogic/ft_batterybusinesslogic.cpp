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
#include "debug.h"

void
debugDumpNotifications ()
{
    QList<MNotification *> notifications;
    int                    n = 0;

    notifications = MNotification::notifications();
    foreach (MNotification *notification, notifications) {
        QString  body;
        QString  image;
        QString  summary;
        QString  eventType;

        eventType = notification->eventType ();
        body = notification->body ();
        image = notification->image ();
        summary = notification->summary ();
        SYS_DEBUG ("************ Notification %d *****************", n);
        SYS_DEBUG ("*** eventType   = %s", SYS_STR(eventType));
        SYS_DEBUG ("*** summary     = %s", SYS_STR(summary));
        SYS_DEBUG ("*** body        = %s", SYS_STR(body));
        SYS_DEBUG ("*** image       = %s", SYS_STR(image));
        SYS_DEBUG ("*** isPublished = %s",
                SYS_BOOL(notification->isPublished()));

        ++n;
    }
}

static bool
hasPublishedNotification (
        const QString    &checkEventType,
        const QString    &checkBody = QString(""),
        const QString    &checkIconName = QString(""),
        const QString    &checkSubject = QString(""))
{
    QList<MNotification *> notifications;

    notifications = MNotification::notifications();
    foreach (MNotification *notification, notifications) {
        QString  body;
        QString  image;
        QString  summary;
        QString  eventType;

        eventType = notification->eventType ();
        body = notification->body ();
        image = notification->image ();
        summary = notification->summary ();

        if (!notification->isPublished())
            continue;
        if (!checkEventType.isEmpty() && checkEventType != eventType)
            continue;
        if (!checkBody.isEmpty() && checkBody != body)
            continue;
        if (!checkIconName.isEmpty() && checkIconName != image)
            continue;
        if (!checkSubject.isEmpty() && checkSubject != summary)
            continue;

        /*
         * We found what we asked to search...
         */
        return true;
    }

    SYS_WARNING ("A notification with the following data should be published.");
    SYS_WARNING ("*** eventType = %s", SYS_STR(checkEventType));
    SYS_WARNING ("*** body      = %s", SYS_STR(checkBody));
    SYS_WARNING ("*** icon      = %s", SYS_STR(checkIconName));
    SYS_WARNING ("*** subject   = %s", SYS_STR(checkSubject));
    debugDumpNotifications ();
    return false;
}


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
Ft_BatteryBusinessLogic::testLowBatteryAlert ()
{
#ifdef HAVE_QMSYSTEM
    m_logic->lowBatteryAlert ();
    QTest::qWait (10);
    QVERIFY (hasPublishedNotification(
                "x-nokia.battery.lowbattery",
                qtTrId ("qtn_ener_lowbatt"))
            );
#endif
}

void
Ft_BatteryBusinessLogic::testBatteryStateChanged ()
{
#ifdef HAVE_QMSYSTEM
    gQmBatteryStub->stubReset ();
    gQmLEDStub->stubReset ();

    /* StateFull */
    m_logic->batteryStateChanged (MeeGo::QmBattery::StateFull);
    QTest::qWait (10);
    QVERIFY (hasPublishedNotification(
                "x-nokia.battery.chargingcomplete",
                qtTrId ("qtn_ener_charcomp"))
            );
    
    /* StateOK */
    m_logic->batteryStateChanged (MeeGo::QmBattery::StateOK);

    QTest::qWait (10);
    /* no signals/notifications should come, just silently no-op */

    /* StateEmpty */
    m_logic->batteryStateChanged (MeeGo::QmBattery::StateEmpty);
    QTest::qWait (10);
    QVERIFY (hasPublishedNotification (
                "x-nokia.battery.recharge",
                qtTrId ("qtn_ener_rebatt"))
            );

    /* StateError */
    m_logic->batteryStateChanged (MeeGo::QmBattery::StateError);

    QTest::qWait (10);
    /* no signals/notifications should come, just silently no-op */

    /* StateLow and charging */
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState> (
        "getChargingState", MeeGo::QmBattery::StateCharging);
    m_logic->batteryStateChanged (MeeGo::QmBattery::StateLow);
    QTest::qWait (10);
    /* no signals/notifications should come, because battery is charging... */

    /* StateLow and not charging */
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState> (
        "getChargingState", MeeGo::QmBattery::StateNotCharging);
    m_logic->batteryStateChanged (MeeGo::QmBattery::StateLow);
    QTest::qWait (10);
    QVERIFY (hasPublishedNotification (
                "x-nokia.battery.lowbattery",
                qtTrId ("qtn_ener_lowbatt"))
            );
#endif
}


void
Ft_BatteryBusinessLogic::testBatteryChargerEvent ()
{
#ifdef HAVE_QMSYSTEM
    /* Wall charger */
    m_logic->batteryChargerEvent (MeeGo::QmBattery::Wall);
    QCOMPARE (m_logic->m_ChargerType, MeeGo::QmBattery::Wall);

    /* Plug out : charger type = none */
    m_logic->batteryChargerEvent (MeeGo::QmBattery::None);
    QCOMPARE (m_logic->m_ChargerType, MeeGo::QmBattery::None);

    QTest::qWait (10);
    /* Look for the notification: "Disconnect the charger from..." */
    QVERIFY (hasPublishedNotification(
                "x-nokia.battery.removecharger",
                qtTrId ("qtn_ener_remcha"))
            );
    
    /* USB 500mA */
    m_logic->batteryChargerEvent (MeeGo::QmBattery::USB_500mA);
    QCOMPARE (m_logic->m_ChargerType, MeeGo::QmBattery::USB_500mA);

    /* USB 100mA */
    m_logic->batteryChargerEvent (MeeGo::QmBattery::USB_100mA);
    QCOMPARE (m_logic->m_ChargerType, MeeGo::QmBattery::USB_100mA);

    /* Unknown */
    m_logic->batteryChargerEvent (MeeGo::QmBattery::Unknown);
    QCOMPARE (m_logic->m_ChargerType, MeeGo::QmBattery::Unknown);
#endif
}

void
Ft_BatteryBusinessLogic::testPSMStateChanged ()
{
#ifdef HAVE_QMSYSTEM
    /* Entering to power-save mode */
    m_logic->devicePSMStateChanged (MeeGo::QmDeviceMode::PSMStateOn);
    QTest::qWait (10);
    QVERIFY (hasPublishedNotification(
                "x-nokia.battery.enterpsm",
                qtTrId ("qtn_ener_ent_psnote"))
            );

    /* Exiting from power-save mode */
    m_logic->devicePSMStateChanged (MeeGo::QmDeviceMode::PSMStateOff);
    QTest::qWait (10);
    QVERIFY (hasPublishedNotification(
                "x-nokia.battery.exitpsm",
                qtTrId ("qtn_ener_exit_psnote"))
            );
#endif
}

void
Ft_BatteryBusinessLogic::testLowBatteryNotifierConnection ()
{
#ifdef HAVE_QMSYSTEM
    /* LowBatteryNotifier shouldn't be instantiated at first */
    QVERIFY (m_logic->m_LowBatteryNotifier == 0);

    /* Simulate battery-state-low change */
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState> (
        "getChargingState", MeeGo::QmBattery::StateNotCharging);
    m_logic->batteryStateChanged (MeeGo::QmBattery::StateLow);

    /* LowBatteryNotifier should be exists now... */
    QVERIFY (m_logic->m_LowBatteryNotifier != 0);
    QTest::qWait (10);

    /* And should send a low-battery notification */
    QVERIFY (hasPublishedNotification(
                "x-nokia.battery.lowbattery",
                qtTrId ("qtn_ener_lowbatt"))
            );

    /* Simulate now a charging event */
    m_logic->chargingStateChanged (MeeGo::QmBattery::StateCharging);

    /* After this call LowBatteryNotifier should be destroyed */
    QVERIFY (m_logic->m_LowBatteryNotifier == 0);
#endif
}

QTEST_APPLESS_MAIN(Ft_BatteryBusinessLogic)

