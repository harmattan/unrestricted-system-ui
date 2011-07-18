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
#ifdef HAVE_QMSYSTEM
#include <qmtime.h>
#include <qmheartbeat.h>
#endif
#include "ut_clock.h"
#include "clock.h"

#ifdef HAVE_QMSYSTEM
Q_DECLARE_METATYPE(MeeGo::QmTime::WhatChanged);
#endif

bool mWidgetIsOnDisplay = true;
bool MWidget::isOnDisplay() const
{
    return mWidgetIsOnDisplay;
}

QDateTime QDateTime::currentDateTime()
{
    return Ut_Clock::expectedDateTime;
}

void QTimer::start(int msec)
{
    Ut_Clock::timerTimeout = msec;
}

void QTimer::stop()
{
    Ut_Clock::timerTimeout = -1;
}

#ifdef HAVE_QMSYSTEM
int gMintime, gMaxtime;
bool gHeartBeatServiceOpened = false;
MeeGo::QmHeartbeat::SignalNeed gSignalNeed;
bool MeeGo::QmHeartbeat::open(MeeGo::QmHeartbeat::SignalNeed signalNeed) {
    gSignalNeed = signalNeed;
    return gHeartBeatServiceOpened = true;
}

bool gWakeUp = false;
bool MeeGo::QmHeartbeat::IWokeUp(void)
{
   return gWakeUp = true;
}

MeeGo::QmHeartbeat::WaitMode gWaitMode;
QTime MeeGo::QmHeartbeat::wait(unsigned short mintime, unsigned short maxtime, QmHeartbeat::WaitMode wait)
{
   gMintime = mintime;
   gMaxtime = maxtime;
   gWaitMode = wait;
   return QTime();
}
#endif

int Ut_Clock::timerTimeout = -1;
QDateTime Ut_Clock::expectedDateTime;

// Called before the first testfunction is executed
void Ut_Clock::initTestCase()
{
    expectedDateTime = QDateTime(QDate(2000, 1, 1));
}

// Called after the last testfunction was executed
void Ut_Clock::cleanupTestCase()
{
}

// Called before each testfunction is executed
void Ut_Clock::init()
{
#ifdef HAVE_QMSYSTEM
    gMintime = 0;
    gMaxtime = 0;
    gWakeUp = false;
    gWaitMode = MeeGo::QmHeartbeat::WaitHeartbeat;
    gSignalNeed = MeeGo::QmHeartbeat::NoSignalNeeded;
    gHeartBeatServiceOpened = false;
#endif
    m_subject = new Clock;
}

// Called after every testfunction
void Ut_Clock::cleanup()
{
    delete m_subject;
    mWidgetIsOnDisplay = true;
    timerTimeout = -1;
}

void Ut_Clock::testConstruction_data()
{
    QTest::addColumn<bool>("isOnDisplay");

    QTest::newRow("Is on display") << true;
    QTest::newRow("Is not on display") << false;
}

void Ut_Clock::testConstruction()
{
    QFETCH(bool, isOnDisplay);

    cleanup();

    mWidgetIsOnDisplay = isOnDisplay;
    m_subject = new Clock;

#ifdef HAVE_QMSYSTEM
    QCOMPARE(gHeartBeatServiceOpened, true);
    QCOMPARE(gSignalNeed, MeeGo::QmHeartbeat::SignalNeeded);
    QVERIFY(disconnect(&m_subject->qmHeartbeat, SIGNAL(wakeUp(QTime)), m_subject, SLOT(updateModelAndSetupTimer())));
#else
    QVERIFY(disconnect(&m_subject->timer, SIGNAL(timeout()), m_subject, SLOT(updateModelAndSetupTimer())));
    QVERIFY(m_subject->timer.isSingleShot());
#endif

    QDateTime nextUpdateTime = expectedDateTime.addSecs(60);
    QTime time = nextUpdateTime.time();
    time.setHMS(time.hour(), time.minute(), 0);
    nextUpdateTime.setTime(time);

    if (isOnDisplay) {
#ifdef HAVE_QMSYSTEM
        QCOMPARE(gMintime, expectedDateTime.secsTo(nextUpdateTime) + 1);
        QCOMPARE(gMaxtime, expectedDateTime.secsTo(nextUpdateTime) + 2);
        QCOMPARE(gWaitMode, MeeGo::QmHeartbeat::DoNotWaitHeartbeat);
#else
        QVERIFY(timerTimeout > expectedDateTime.msecsTo(nextUpdateTime));
#endif
    } else {
        QCOMPARE(timerTimeout, -1);
    }
}

void Ut_Clock::testTimeUpdate()
{
#ifdef HAVE_QMSYSTEM
    QVERIFY(disconnect(&m_subject->qmTime, SIGNAL(timeOrSettingsChanged(MeeGo::QmTime::WhatChanged)), m_subject, SLOT(updateModelAndSetupTimer())));
#endif
}

void Ut_Clock::testModelUpdates()
{
#ifdef HAVE_QMSYSTEM
    // qmHeartbeat.wait() is called with proper parameters
    // Seconds to next update time should be 60
    QCOMPARE(gMintime, 60 + 1);
    QCOMPARE(gMaxtime, 60 + 2);
    QCOMPARE(gWaitMode, MeeGo::QmHeartbeat::DoNotWaitHeartbeat);
#else
    // Check if the timer is running
    QVERIFY(timerTimeout >=0);
#endif
    // The model should contain the current time
    QCOMPARE(m_subject->model()->time(), expectedDateTime);

    // Check when the application becomes invisible
    m_subject->exitDisplayEvent();
#ifdef HAVE_QMSYSTEM
    // qmHeartbeat IWokeUp() is called
    QCOMPARE(gWakeUp, true);
#else
    // The timer should stop
    QCOMPARE(timerTimeout, -1);
#endif

    // Check when the application becomes visible
#ifdef HAVE_QMSYSTEM
    gWakeUp = false;
#endif
    expectedDateTime = QDateTime(QDate(2001, 1, 1));
    m_subject->enterDisplayEvent();

#ifdef HAVE_QMSYSTEM
    // qmHeartbeat IWokeUp() is called
    QCOMPARE(gWakeUp, true);
#else
    // The timer should start
    QVERIFY(timerTimeout >=0);
#endif
    //The model should be updated to contain the current time
    QCOMPARE(m_subject->model()->time(), expectedDateTime);
}

QTEST_MAIN(Ut_Clock)
