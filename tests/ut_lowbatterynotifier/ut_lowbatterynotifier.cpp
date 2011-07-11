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
#include "ut_lowbatterynotifier.h"
#include "lowbatterynotifier.h"

#ifdef HAVE_QMSYSTEM
#include "qmdisplaystate_stub.h"
#endif

void QTimer::start()
{
    id = 1;
}

int qTimerStartMsec = 0;
void QTimer::start(int msec)
{
    qTimerStartMsec = msec;
    id = 1;
}

int qTimeElapsed = 0;
int QTime::elapsed() const
{
    return qTimeElapsed;
}

void Ut_LowBatteryNotifier::init()
{
    m_subject = new LowBatteryNotifier;
}

void Ut_LowBatteryNotifier::cleanup()
{
    delete m_subject;
    qTimerStartMsec = 0;
    qTimeElapsed = 0;
}

void Ut_LowBatteryNotifier::initTestCase()
{
}

void Ut_LowBatteryNotifier::cleanupTestCase()
{
}

void Ut_LowBatteryNotifier::testSignalConnections()
{
#ifdef HAVE_QMSYSTEM
    QVERIFY(disconnect(m_subject->displayState, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)), m_subject, SLOT(setNotificationInterval())));
#endif
    QVERIFY(disconnect(m_subject->notificationTimer, SIGNAL(timeout()), m_subject, SLOT(sendLowBatteryAlert())));
}

void Ut_LowBatteryNotifier::testSendLowBatteryAlert_data()
{
    QTest::addColumn<bool>("deviceInactive");
    QTest::addColumn<int>("expectedTimerInterval");

    QTest::newRow("Active") << false << 5 * 60 * 1000;
    QTest::newRow("Inactive") << true << 30 * 60 * 1000;
}

void Ut_LowBatteryNotifier::testSendLowBatteryAlert()
{
    QFETCH(bool, deviceInactive);
    QFETCH(int, expectedTimerInterval);

    QSignalSpy spy(m_subject, SIGNAL(lowBatteryAlert()));
    m_subject->deviceInactive = deviceInactive;
    m_subject->sendLowBatteryAlert();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(qTimerStartMsec, expectedTimerInterval);
}

#ifdef HAVE_QMSYSTEM
Q_DECLARE_METATYPE(MeeGo::QmDisplayState::DisplayState)
#endif

void Ut_LowBatteryNotifier::testSetNotificationInterval_data()
{
    QTest::addColumn<bool>("deviceInactive");
    QTest::addColumn<bool>("touchScreenLockActive");
#ifdef HAVE_QMSYSTEM
    QTest::addColumn<MeeGo::QmDisplayState::DisplayState>("displayState");
#endif
    QTest::addColumn<int>("timeSincePreviousNotification");
    QTest::addColumn<int>("notificationTimerInterval");
    QTest::addColumn<int>("lowBatteryAlertCount");

#ifdef HAVE_QMSYSTEM
    QTest::newRow("Device active, becomes active") << false << false << MeeGo::QmDisplayState::On << 1000 << 0 << 0;
    QTest::newRow("Device inactive, becomes inactive") << true << true << MeeGo::QmDisplayState::Off << 1000 << 0 << 0;
    QTest::newRow("Device active, becomes inactive") << false << true << MeeGo::QmDisplayState::Off << 1000 << (30 * 60 * 1000 - 1000) << 0;
    QTest::newRow("Device inactive, becomes active, interval not elapsed") << true << false << MeeGo::QmDisplayState::Dimmed << 1000 << (5 * 60 * 1000 - 1000) << 0;
    QTest::newRow("Device inactive, becomes active, interval elapsed") << true << false << MeeGo::QmDisplayState::Dimmed << (5 * 60 * 1000) << 0 << 1;
#else
    QTest::newRow("Device active, becomes active") << false << false << 1000 << 0 << 0;
    QTest::newRow("Device inactive, becomes inactive") << true << true << 1000 << 0 << 0;
    QTest::newRow("Device active, becomes inactive") << false << true << 1000 << (30 * 60 * 1000 - 1000) << 0;
    QTest::newRow("Device inactive, becomes active, interval not elapsed") << true << false << 1000 << (5 * 60 * 1000 - 1000) << 0;
    QTest::newRow("Device inactive, becomes active, interval elapsed") << true << false << (5 * 60 * 1000) << 0 << 1;
#endif
}

void Ut_LowBatteryNotifier::testSetNotificationInterval()
{
    QFETCH(bool, deviceInactive);
    QFETCH(bool, touchScreenLockActive);
#ifdef HAVE_QMSYSTEM
    QFETCH(MeeGo::QmDisplayState::DisplayState, displayState);
#endif
    QFETCH(int, timeSincePreviousNotification);
    QFETCH(int, notificationTimerInterval);
    QFETCH(int, lowBatteryAlertCount);

    QSignalSpy spy(m_subject, SIGNAL(lowBatteryAlert()));
#ifdef HAVE_QMSYSTEM
    gQmDisplayStateStub->stubSetReturnValue("get", displayState);
#endif
    qTimeElapsed = timeSincePreviousNotification;
    m_subject->deviceInactive = deviceInactive;
    m_subject->setTouchScreenLockActive(touchScreenLockActive);
    QCOMPARE(m_subject->notificationTimer->interval(), notificationTimerInterval);
    QCOMPARE(spy.count(), lowBatteryAlertCount);
}

QTEST_MAIN(Ut_LowBatteryNotifier)
