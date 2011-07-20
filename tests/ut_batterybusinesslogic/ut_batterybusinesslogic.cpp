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
#include <MNotification>

#include "lowbatterynotifier_stub.h"
#include "contextframeworkcontext_stub.h"
#include "contextframeworkitem_stub.h"
#ifdef HAVE_QMSYSTEM
#include "qmled_stub.h"
#include "qmbattery_stub.h"
#include "qmdevicemode_stub.h"
#include "qmdisplaystate_stub.h"
#endif
#include "batterybusinesslogic.h"
#include "ut_batterybusinesslogic.h"

QStringList mNotificationEventTypes;
QStringList mNotificationSummaries;
QStringList mNotificationBodies;
QStringList mNotificationImages;
bool MNotification::publish()
{
    mNotificationEventTypes.append(eventType());
    mNotificationSummaries.append(summary());
    mNotificationBodies.append(body());
    mNotificationImages.append(image());
    return false;
}

QList<QString> gMNotificationRemoveEventType;
bool MNotification::remove()
{
    gMNotificationRemoveEventType.append(eventType());
    return false;
}

void QTimer::start()
{
    id = 1;
}

void Ut_BatteryBusinessLogic::initTestCase()
{
}

void Ut_BatteryBusinessLogic::cleanupTestCase()
{
}

void Ut_BatteryBusinessLogic::init()
{
    m_logic = new BatteryBusinessLogic;

    mNotificationEventTypes.clear();
    mNotificationBodies.clear();
    mNotificationSummaries.clear();
    mNotificationImages.clear();
}

void Ut_BatteryBusinessLogic::cleanup()
{
    delete m_logic;
    m_logic = NULL;
    mNotificationEventTypes.clear();
    mNotificationBodies.clear();
    mNotificationSummaries.clear();
    mNotificationImages.clear();
    gMNotificationRemoveEventType.clear();
    gLowBatteryNotifierStub->stubReset();
}

void Ut_BatteryBusinessLogic::testInitBattery()
{
#ifdef HAVE_QMSYSTEM
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState>("getChargingState", MeeGo::QmBattery::StateNotCharging);
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::BatteryState>("getBatteryState", MeeGo::QmBattery::StateOK);

    // no notification should be shown and battery charging pattern should be deactivated
    m_logic->initBattery();

    QCOMPARE(mNotificationEventTypes.count(), 0);
    QCOMPARE(gQmLEDStub->stubLastCallTo("deactivate").parameter<QString>(0), QString("PatternBatteryCharging"));
#endif
}

void Ut_BatteryBusinessLogic::testLowBatteryAlert()
{
#ifdef HAVE_QMSYSTEM
    m_logic->lowBatteryAlert();

    QCOMPARE(mNotificationEventTypes.count(), 1);
    QCOMPARE(mNotificationEventTypes.at(0), QString("x-nokia.battery.lowbattery"));
    QCOMPARE(mNotificationBodies.at(0), qtTrId("qtn_ener_lowbatt"));
    QCOMPARE(mNotificationSummaries.at(0), QString());
    QCOMPARE(mNotificationImages.at(0), QString());
#endif
}

void Ut_BatteryBusinessLogic::testBatteryStateChanged()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;

    gQmBatteryStub->stubReset();
    gQmLEDStub->stubReset();

    /* StateFull */
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateFull);

    QCOMPARE(mNotificationEventTypes.count(), 1);
    QCOMPARE(mNotificationEventTypes.at(0), QString("x-nokia.battery.chargingcomplete"));
    QCOMPARE(mNotificationBodies.at(0), qtTrId("qtn_ener_charcomp"));
    QCOMPARE(mNotificationSummaries.at(0), QString());
    QCOMPARE(mNotificationImages.at(0), QString());
    QCOMPARE(gQmLEDStub->stubLastCallTo("activate").parameter<QString>(0), QString("PatternBatteryFull"));

    /* StateOK */
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateOK);

    /* no notifications should be published, just silently no-op */
    QCOMPARE(mNotificationEventTypes.count(), 1);

    /* StateEmpty */
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateEmpty);

    QCOMPARE(mNotificationEventTypes.count(), 2);
    QCOMPARE(mNotificationEventTypes.at(1), QString("x-nokia.battery.recharge"));
    QCOMPARE(mNotificationBodies.at(1), qtTrId("qtn_ener_rebatt"));
    QCOMPARE(mNotificationSummaries.at(1), QString());
    QCOMPARE(mNotificationImages.at(1), QString());

    /* StateError */
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateError);

    /* no notifications should be published, just silently no-op */
    QCOMPARE(mNotificationEventTypes.count(), 2);

    /* StateLow and charging */
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState>("getChargingState", MeeGo::QmBattery::StateCharging);
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateLow);

    /* no notifications should be published, because battery is charging... */
    QCOMPARE(mNotificationEventTypes.count(), 2);

    /* StateLow and not charging */
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState>("getChargingState", MeeGo::QmBattery::StateNotCharging);
    m_logic->setTouchScreenLockActive(true);
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateLow);

    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 1);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), true);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("sendLowBatteryAlert"), 1);
#endif
}

void Ut_BatteryBusinessLogic::testChargingStateChanged()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;

    gQmBatteryStub->stubReset();
    gQmLEDStub->stubReset();

    gQmBatteryStub->stubSetReturnValue("getChargerType", MeeGo::QmBattery::Wall);

    for(int i = 0; i <= 100; i += 5) {
        /* StateCharging */
        gQmBatteryStub->stubSetReturnValue<int>("getRemainingCapacityPct", i);
        m_logic->chargingStateChanged(MeeGo::QmBattery::StateCharging);

        QCOMPARE(mNotificationEventTypes.count(), 1);
        QCOMPARE(mNotificationEventTypes.at(0), QString("x-nokia.battery"));
        QCOMPARE(mNotificationBodies.at(0), qtTrId("qtn_ener_charging"));
        QCOMPARE(mNotificationSummaries.at(0), QString());
        QCOMPARE(mNotificationImages.at(0), m_logic->chargingImageId());
        QCOMPARE(gQmLEDStub->stubLastCallTo("activate").parameter<QString>(0), QString("PatternBatteryCharging"));

        mNotificationEventTypes.clear();
        mNotificationBodies.clear();
        mNotificationSummaries.clear();
        mNotificationImages.clear();
    }

    /* StateNotCharging */
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateNotCharging);

    QCOMPARE(mNotificationEventTypes.count(), 0);
    QCOMPARE(gQmLEDStub->stubLastCallTo("deactivate").parameter<QString>(0), QString("PatternBatteryCharging"));

    /* StateChargingFailed */
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateChargingFailed);

    QCOMPARE(mNotificationEventTypes.count(), 1);
    QCOMPARE(mNotificationEventTypes.at(0), QString("x-nokia.battery.chargingnotstarted"));
    QCOMPARE(mNotificationBodies.at(0), qtTrId("qtn_ener_repcharger"));
    QCOMPARE(mNotificationSummaries.at(0), QString());
    QCOMPARE(mNotificationImages.at(0), QString());

    /* Test "not enough power to charge" situation... */
    gQmBatteryStub->stubSetReturnValue("getChargerType", MeeGo::QmBattery::USB_100mA);
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateCharging);

    QCOMPARE(mNotificationEventTypes.count(), 2);
    QCOMPARE(mNotificationEventTypes.at(1), QString("x-nokia.battery.notenoughpower"));
    QCOMPARE(mNotificationBodies.at(1), qtTrId("qtn_ener_nopowcharge"));
    QCOMPARE(mNotificationSummaries.at(1), QString());
    QCOMPARE(mNotificationImages.at(1), QString("icon-m-energy-management-insufficient-power"));
#endif
}

void Ut_BatteryBusinessLogic::testBatteryChargerEvent()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;

    /* Wall charger */
    m_logic->batteryChargerEvent(MeeGo::QmBattery::Wall);
    QCOMPARE(m_logic->chargerType, MeeGo::QmBattery::Wall);

    /* Plug out : charger type = none */
    m_logic->batteryChargerEvent(MeeGo::QmBattery::None);
    QCOMPARE(m_logic->chargerType, MeeGo::QmBattery::None);

    /* Look for the notification: "Disconnect the charger from..." */
    QCOMPARE(mNotificationEventTypes.count(), 1);
    QCOMPARE(mNotificationEventTypes.at(0), QString("x-nokia.battery.removecharger"));
    QCOMPARE(mNotificationBodies.at(0), qtTrId("qtn_ener_remcha"));
    QCOMPARE(mNotificationSummaries.at(0), QString());
    QCOMPARE(mNotificationImages.at(0), QString());

    /* USB 500mA */
    m_logic->batteryChargerEvent(MeeGo::QmBattery::USB_500mA);
    QCOMPARE(m_logic->chargerType, MeeGo::QmBattery::USB_500mA);

    /* USB 100mA */
    m_logic->batteryChargerEvent(MeeGo::QmBattery::USB_100mA);
    QCOMPARE(m_logic->chargerType, MeeGo::QmBattery::USB_100mA);

    /* Unknown */
    m_logic->batteryChargerEvent(MeeGo::QmBattery::Unknown);
    QCOMPARE(m_logic->chargerType, MeeGo::QmBattery::Unknown);
#endif
}

void Ut_BatteryBusinessLogic::testPSMStateChanged()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy(m_logic, SIGNAL(notificationSent(QString, QString, QString)));

    /* Entering to power-save mode */
    m_logic->devicePSMStateChanged(MeeGo::QmDeviceMode::PSMStateOn);

    QCOMPARE(mNotificationEventTypes.count(), 1);
    QCOMPARE(mNotificationEventTypes.at(0), QString("x-nokia.battery.enterpsm"));
    QCOMPARE(mNotificationBodies.at(0), qtTrId("qtn_ener_ent_psnote"));
    QCOMPARE(mNotificationSummaries.at(0), QString());
    QCOMPARE(mNotificationImages.at(0), m_logic->chargingImageId());

    /* Exiting from power-save mode */
    m_logic->devicePSMStateChanged(MeeGo::QmDeviceMode::PSMStateOff);

    QCOMPARE(mNotificationEventTypes.count(), 2);
    QCOMPARE(mNotificationEventTypes.at(1), QString("x-nokia.battery.exitpsm"));
    QCOMPARE(mNotificationBodies.at(1), qtTrId("qtn_ener_exit_psnote"));
    QCOMPARE(mNotificationSummaries.at(1), QString());
    QCOMPARE(mNotificationImages.at(1), m_logic->chargingImageId());
#endif
}

void Ut_BatteryBusinessLogic::testLowBatteryNotifierConnection()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;

    gQmBatteryStub->stubSetReturnValue("getChargerType", MeeGo::QmBattery::USB_500mA);

    /* LowBatteryNotifier shouldn't be instantiated at first */
    QCOMPARE(m_logic->lowBatteryNotifier, (LowBatteryNotifier *)NULL);

    /* Simulate battery-state-low change */
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState>("getChargingState", MeeGo::QmBattery::StateNotCharging);
    m_logic->setTouchScreenLockActive(true);
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateLow);

    /* LowBatteryNotifier should be exists now... */
    QVERIFY(m_logic->lowBatteryNotifier != NULL);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 1);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), true);

    /* And should send a low-battery notification */
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("sendLowBatteryAlert"), 1);

    /* Simulate now a charging event */
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateCharging);

    /* After this call LowBatteryNotifier should be destroyed */
    QCOMPARE(m_logic->lowBatteryNotifier, (LowBatteryNotifier *)NULL);
#endif
}

void Ut_BatteryBusinessLogic::testWhenChargingStopsThenNotificationRemoved()
{
#ifdef HAVE_QMSYSTEM
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateCharging);
    QCOMPARE(mNotificationEventTypes.count(), 1);
    QCOMPARE(mNotificationEventTypes.at(0), QString("x-nokia.battery"));
    QCOMPARE(mNotificationBodies.at(0), qtTrId("qtn_ener_charging"));
    QCOMPARE(mNotificationSummaries.at(0), QString());
    QCOMPARE(mNotificationImages.at(0), m_logic->chargingImageId());

    m_logic->chargingStateChanged(MeeGo::QmBattery::StateNotCharging);
    QVERIFY(gMNotificationRemoveEventType.count() > 0);
    QCOMPARE(gMNotificationRemoveEventType.last(), QString("x-nokia.battery"));
    QCOMPARE(m_logic->notificationTimer.isActive(), false);
#endif
}

void Ut_BatteryBusinessLogic::testWhenChargingStopsWhenConnectedToWallChargerThenNotificationRemoved()
{
#ifdef HAVE_QMSYSTEM
    m_logic->batteryChargerEvent(MeeGo::QmBattery::Wall);
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateCharging);
    m_logic->batteryChargerEvent(MeeGo::QmBattery::None);
    QVERIFY(gMNotificationRemoveEventType.count() > 0);
    QCOMPARE(gMNotificationRemoveEventType.last(), QString("x-nokia.battery"));

    m_logic->batteryChargerEvent(MeeGo::QmBattery::Wall);
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateCharging);
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateFull);
    m_logic->batteryChargerEvent(MeeGo::QmBattery::None);
    QVERIFY(gMNotificationRemoveEventType.count() > 0);
    QCOMPARE(gMNotificationRemoveEventType.last(), QString("x-nokia.battery.chargingcomplete"));
#endif
}

void Ut_BatteryBusinessLogic::testWhenChargingStopsMoreThanNSecondAfterBeingStartedThenNotificationNotRemoved()
{
#ifdef HAVE_QMSYSTEM
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateCharging);
    m_logic->notificationTimer.stop();
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateNotCharging);
    QCOMPARE(gMNotificationRemoveEventType.count(), 0);
#endif
}

void Ut_BatteryBusinessLogic::testWhenChargingStartsWhenRemoveChargerNotifiedThenNotificationRemoved()
{
#ifdef HAVE_QMSYSTEM
    m_logic->batteryChargerEvent(MeeGo::QmBattery::Wall);
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateCharging);
    m_logic->batteryChargerEvent(MeeGo::QmBattery::None);
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateCharging);
    QVERIFY(gMNotificationRemoveEventType.count() > 0);
    QCOMPARE(gMNotificationRemoveEventType.last(), QString("x-nokia.battery.removecharger"));
#endif
}

void Ut_BatteryBusinessLogic::testWhenBatteryFullWhenChargingNotifiedThenNotificationRemoved()
{
#ifdef HAVE_QMSYSTEM
    m_logic->batteryChargerEvent(MeeGo::QmBattery::Wall);
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateCharging);
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateFull);
    QVERIFY(gMNotificationRemoveEventType.count() > 0);
    QCOMPARE(gMNotificationRemoveEventType.last(), QString("x-nokia.battery"));
#endif
}

void Ut_BatteryBusinessLogic::testSetTouchScreenLockActive()
{
    m_logic->setTouchScreenLockActive(true);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 0);

#ifdef HAVE_QMSYSTEM
    gQmBatteryStub->stubSetReturnValue("getChargerType", MeeGo::QmBattery::USB_500mA);
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState>("getChargingState", MeeGo::QmBattery::StateNotCharging);
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateLow);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 1);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), true);

    m_logic->setTouchScreenLockActive(false);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 2);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), false);

    m_logic->setTouchScreenLockActive(true);
    QCOMPARE(gLowBatteryNotifierStub->stubCallCount("setTouchScreenLockActive"), 3);
    QCOMPARE(gLowBatteryNotifierStub->stubLastCallTo("setTouchScreenLockActive").parameter<bool>(0), true);
#endif
}

QTEST_MAIN(Ut_BatteryBusinessLogic)
