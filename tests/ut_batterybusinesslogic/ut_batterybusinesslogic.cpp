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

/*
 * TODO:
 *  - test the feedback playing also
 *  [XXX: we need to know first, how to play those correctly...]
 */
#include "ut_batterybusinesslogic.h"
#include "batterybusinesslogic.h"

#ifdef HAVE_QMSYSTEM
#include "qmled_stub.h"
#include "qmbattery_stub.h"
#include "qmdevicemode_stub.h"
#include "qmdisplaystate_stub.h"
#endif

#include <QtTest/QtTest>
#include <MNotification>
#include <MFeedback>

// MFeedback stub
static QString nameOfLastFeedback;
MFeedback::MFeedback(const QString &name, QObject *) :
    d_ptr(NULL)
{
    nameOfLastFeedback = name;
}

void MFeedback::play () const
{
}

QList<MNotification*> gMNotificationPublish;
bool MNotification::publish ()
{
    gMNotificationPublish.append(this);
    return false;
}

QList<QString> gMNotificationRemoveEventType;
bool MNotification::remove ()
{
    gMNotificationRemoveEventType.append(eventType());
    return false;
}

struct QTimerSingleShotParams
{
    int msec;
    QObject *receiver;
    QString member;
};

QHash<QObject*, QTimerSingleShotParams> qTimerSingleShotParams;
void QTimer::singleShot(int msec, QObject *receiver, const char * member)
{
    QTimerSingleShotParams params;
    params.msec = msec;
    params.receiver = receiver;
    params.member = member;
    qTimerSingleShotParams[receiver] = params;
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
}

void Ut_BatteryBusinessLogic::cleanup()
{
    delete m_logic;
    m_logic = NULL;
    gMNotificationPublish.clear();
    gMNotificationRemoveEventType.clear();
    qTimerSingleShotParams.clear();
}

void Ut_BatteryBusinessLogic::testInitBattery()
{
#ifdef HAVE_QMSYSTEM
    QSignalSpy spy(m_logic, SIGNAL(notificationSent(QString, QString, QString)));

    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState>("getChargingState", MeeGo::QmBattery::StateNotCharging);
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::BatteryState>("getBatteryState", MeeGo::QmBattery::StateOK);

    // no notification should be shown and battery charging pattern should be deactivated
    m_logic->initBattery();
    QTest::qWait(10);

    QCOMPARE(spy.count(), 0);
    QCOMPARE(gQmLEDStub->stubLastCallTo("deactivate").parameter<QString>(0), QString("PatternBatteryCharging"));
#endif
}

void Ut_BatteryBusinessLogic::testLowBatteryAlert()
{
#ifdef HAVE_QMSYSTEM
    QSignalSpy spy(m_logic, SIGNAL(notificationSent(QString, QString, QString)));

    m_logic->lowBatteryAlert();

    QTest::qWait(10);
    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();

    QCOMPARE(arguments.at(0).toString(), QString("x-nokia.battery.lowbattery"));
    QCOMPARE(arguments.at(1).toString(), qtTrId("qtn_ener_lowbatt"));
    QCOMPARE(arguments.at(2).toString(), QString());
#endif
}

void Ut_BatteryBusinessLogic::testBatteryStateChanged()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy(m_logic, SIGNAL(notificationSent(QString, QString, QString)));

    gQmBatteryStub->stubReset();
    gQmLEDStub->stubReset();

    /* StateFull */
    spy.clear();
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateFull);

    QTest::qWait(10);

    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("x-nokia.battery.chargingcomplete"));
    QCOMPARE(arguments.at(1).toString(), qtTrId("qtn_ener_charcomp"));
    QCOMPARE(arguments.at(2).toString(), QString());
    QCOMPARE(gQmLEDStub->stubLastCallTo("activate").parameter<QString>(0), QString("PatternBatteryFull"));

    /* StateOK */
    spy.clear();
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateOK);

    QTest::qWait(10);
    /* no signals/notifications should come, just silently no-op */
    QCOMPARE(spy.count(), 0);

    /* StateEmpty */
    spy.clear();
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateEmpty);

    QTest::qWait(10);
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("x-nokia.battery.recharge"));
    QCOMPARE(arguments.at(1).toString(), qtTrId("qtn_ener_rebatt"));
    QCOMPARE(arguments.at(2).toString(), QString());

    /* StateError */
    spy.clear();
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateError);

    QTest::qWait(10);
    /* no signals/notifications should come, just silently no-op */
    QCOMPARE(spy.count(), 0);

    /* StateLow and charging */
    spy.clear();
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState>("getChargingState", MeeGo::QmBattery::StateCharging);
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateLow);

    QTest::qWait(10);
    /* no signals/notifications should come, because battery is charging... */
    QCOMPARE(spy.count(), 0);

    /* StateLow and not charging */
    spy.clear();
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState>("getChargingState", MeeGo::QmBattery::StateNotCharging);
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateLow);

    QTest::qWait(10);

    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("x-nokia.battery.lowbattery"));
    QCOMPARE(arguments.at(1).toString(), qtTrId("qtn_ener_lowbatt"));
    QCOMPARE(arguments.at(2).toString(), QString());
#endif
}

void Ut_BatteryBusinessLogic::testChargingStateChanged()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy(m_logic, SIGNAL(notificationSent(QString, QString, QString)));

    gQmBatteryStub->stubReset();
    gQmLEDStub->stubReset();

    gQmBatteryStub->stubSetReturnValue("getChargerType", MeeGo::QmBattery::Wall);

    for(int i = 0; i <= 100; i += 5) {
        /* StateCharging */
        gQmBatteryStub->stubSetReturnValue<int>("getRemainingCapacityPct", i);
        m_logic->chargingStateChanged(MeeGo::QmBattery::StateCharging);

        QTest::qWait(1);
        QCOMPARE(spy.count(), 1);
        arguments = spy.takeFirst();
        QCOMPARE(arguments.at(0).toString(), QString("x-nokia.battery"));
        QCOMPARE(arguments.at(1).toString(), qtTrId("qtn_ener_charging"));
        QCOMPARE(arguments.at(2).toString(), m_logic->chargingImageId());
        QCOMPARE(gQmLEDStub->stubLastCallTo("activate").parameter<QString>(0), QString("PatternBatteryCharging"));
        spy.clear();
    }

    /* StateNotCharging */
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateNotCharging);

    QTest::qWait(10);
    QCOMPARE(spy.count(), 0);
    QCOMPARE(gQmLEDStub->stubLastCallTo("deactivate").parameter<QString>(0), QString("PatternBatteryCharging"));
    spy.clear();

    /* StateChargingFailed */
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateChargingFailed);

    QTest::qWait(10);
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("x-nokia.battery.chargingnotstarted"));
    QCOMPARE(arguments.at(1).toString(), qtTrId("qtn_ener_repcharger"));
    QCOMPARE(arguments.at(2).toString(), QString());
    spy.clear();

    /* Test "not enough power to charge" situation... */
    gQmBatteryStub->stubSetReturnValue("getChargerType", MeeGo::QmBattery::USB_100mA);
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateCharging);

    QTest::qWait(10);
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("x-nokia.battery.notenoughpower"));
    QCOMPARE(arguments.at(1).toString(), qtTrId("qtn_ener_nopowcharge"));
    QCOMPARE(arguments.at(2).toString(), QString("icon-m-energy-management-insufficient-power"));
    spy.clear();
#endif
}

void Ut_BatteryBusinessLogic::testBatteryChargerEvent()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy(m_logic, SIGNAL(notificationSent(QString, QString, QString)));

    /* Wall charger */
    m_logic->batteryChargerEvent(MeeGo::QmBattery::Wall);
    QCOMPARE(m_logic->m_ChargerType, MeeGo::QmBattery::Wall);

    /* Plug out : charger type = none */
    m_logic->batteryChargerEvent(MeeGo::QmBattery::None);
    QCOMPARE(m_logic->m_ChargerType, MeeGo::QmBattery::None);

    QTest::qWait(10);
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    /* Look for the notification: "Disconnect the charger from..." */
    QCOMPARE(arguments.at(0).toString(), QString("x-nokia.battery.removecharger"));
    QCOMPARE(arguments.at(1).toString(), qtTrId("qtn_ener_remcha"));
    QCOMPARE(arguments.at(2).toString(), QString());
    spy.clear();

    /* USB 500mA */
    m_logic->batteryChargerEvent(MeeGo::QmBattery::USB_500mA);
    QCOMPARE(m_logic->m_ChargerType, MeeGo::QmBattery::USB_500mA);

    /* USB 100mA */
    m_logic->batteryChargerEvent(MeeGo::QmBattery::USB_100mA);
    QCOMPARE(m_logic->m_ChargerType, MeeGo::QmBattery::USB_100mA);

    /* Unknown */
    m_logic->batteryChargerEvent(MeeGo::QmBattery::Unknown);
    QCOMPARE(m_logic->m_ChargerType, MeeGo::QmBattery::Unknown);
#endif
}

void Ut_BatteryBusinessLogic::testPSMStateChanged()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy(m_logic, SIGNAL(notificationSent(QString, QString, QString)));

    /* Entering to power-save mode */
    m_logic->devicePSMStateChanged(MeeGo::QmDeviceMode::PSMStateOn);

    QTest::qWait(10);
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("x-nokia.battery.enterpsm"));
    QCOMPARE(arguments.at(1).toString(), qtTrId("qtn_ener_ent_psnote"));
    QCOMPARE(arguments.at(2).toString(), m_logic->chargingImageId());

    spy.clear();

    /* Exiting from power-save mode */
    m_logic->devicePSMStateChanged(MeeGo::QmDeviceMode::PSMStateOff);

    QTest::qWait(10);
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("x-nokia.battery.exitpsm"));
    QCOMPARE(arguments.at(1).toString(), qtTrId("qtn_ener_exit_psnote"));
    QCOMPARE(arguments.at(2).toString(), m_logic->chargingImageId());
#endif
}

void Ut_BatteryBusinessLogic::testLowBatteryNotifierConnection()
{
#ifdef HAVE_QMSYSTEM
    QList<QVariant> arguments;
    QSignalSpy spy(m_logic, SIGNAL(notificationSent(QString, QString, QString)));

    gQmBatteryStub->stubSetReturnValue("getChargerType", MeeGo::QmBattery::USB_500mA);

    /* LowBatteryNotifier shouldn't be instantiated at first */
    QCOMPARE(m_logic->m_LowBatteryNotifier, (LowBatteryNotifier *)NULL);

    /* Simulate battery-state-low change */
    gQmBatteryStub->stubSetReturnValue<MeeGo::QmBattery::ChargingState>("getChargingState", MeeGo::QmBattery::StateNotCharging);
    m_logic->batteryStateChanged(MeeGo::QmBattery::StateLow);

    /* LowBatteryNotifier should be exists now... */
    QVERIFY(m_logic->m_LowBatteryNotifier != NULL);
    QTest::qWait(10);

    /* And should send a low-battery notification */
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toString(), QString("x-nokia.battery.lowbattery"));
    QCOMPARE(arguments.at(1).toString(), qtTrId("qtn_ener_lowbatt"));
    QCOMPARE(arguments.at(2).toString(), QString());

    /* Simulate now a charging event */
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateCharging);

    /* After this call LowBatteryNotifier should be destroyed */
    QCOMPARE(m_logic->m_LowBatteryNotifier, (LowBatteryNotifier *)NULL);
#endif
}

void Ut_BatteryBusinessLogic::testWhenChargingStopsThenNotificationRemoved()
{
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateCharging);

    //Test that the single-shot timer is called for publishing notification
    QVERIFY(qTimerSingleShotParams.contains(m_logic));
    const QTimerSingleShotParams &params(qTimerSingleShotParams.value(m_logic));
    QCOMPARE(params.msec, 0);
    QCOMPARE(params.receiver, m_logic);
    QString timedSlotName(params.member);
    QVERIFY2(timedSlotName.endsWith(QString("publishNotification()")), qPrintable(QString("Actual timed slot name was: ") + timedSlotName));

    m_logic->publishNotification();

    QVERIFY(gMNotificationPublish.count() > 0);
    QCOMPARE(gMNotificationPublish.last()->eventType(), QString("x-nokia.battery"));
    m_logic->chargingStateChanged(MeeGo::QmBattery::StateNotCharging);
    QVERIFY(gMNotificationRemoveEventType.count() > 0);
    QCOMPARE(gMNotificationRemoveEventType.last(), QString("x-nokia.battery"));
}

QTEST_MAIN(Ut_BatteryBusinessLogic)
