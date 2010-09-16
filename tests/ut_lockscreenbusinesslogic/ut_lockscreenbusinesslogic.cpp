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

#include <QtTest/QtTest>

#include "ut_lockscreenbusinesslogic.h"
#include "lockscreenbusinesslogic.h"
#include "lockscreen_stub.h"
#include "lockscreenwindow_stub.h"
#include "eventeater.h"
#include "sysuid_stub.h"
#include "eventeater_stub.h"
#include <MApplication>
#include <MApplicationWindow>

#define DEBUG
#include "debug.h"

#ifdef HAVE_QMSYSTEM
#include <qmdisplaystate.h>
Maemo::QmDisplayState::DisplayState qmDisplayState;
Maemo::QmDisplayState::DisplayState Maemo::QmDisplayState::get() const
{
    return qmDisplayState;
}
#endif

void Ut_LockScreenBusinessLogic::init()
{
    gLockScreenWindowStub->stubReset();
#ifdef HAVE_QMSYSTEM
    qmDisplayState = Maemo::QmDisplayState::On;
#endif
}

void Ut_LockScreenBusinessLogic::cleanup()
{
}

void Ut_LockScreenBusinessLogic::initTestCase()
{
    m_MainWindow = 0;

    static int argc = 1;
    static char *argv = (char *) "./ut_lockscreenbusinesslogic";
    m_App = new MApplication(argc, &argv);
    /* XXX: input context caused a crash :-S */
    m_App->setLoadMInputContext (false);
    m_App->setQuitOnLastWindowClosed (false);
}

void Ut_LockScreenBusinessLogic::cleanupTestCase()
{
    if (m_MainWindow)
        delete m_MainWindow;

    m_App->deleteLater ();
}

void Ut_LockScreenBusinessLogic::testToggleScreenLockUI()
{
    LockScreenBusinessLogic logic;
    QSignalSpy spy(&logic, SIGNAL(screenIsLocked(bool)));

#ifdef HAVE_QMSYSTEM
    // First try with display off
    qmDisplayState = Maemo::QmDisplayState::Off;
    logic.locksChanged (Maemo::QmLocks::TouchAndKeyboard, Maemo::QmLocks::Locked);
#endif

    // When the lock is toggled on, make sure the screen locking signals are sent and the lock UI is shown
    logic.toggleScreenLockUI(true);
    QTest::qWait (10);
#ifdef HAVE_QMSYSTEM
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);
#else
    spy.clear ();
#endif
    QCOMPARE(logic.lockScreenWindow->isVisible(), true);

    // Check whether the lock-screen-ui state has be reset to defaults
    QCOMPARE(gLockScreenWindowStub->stubCallCount("reset"), 1);

#if !defined(__i386__) && defined(HAVE_QMSYSTEM)
    // The timer should not be started if the display is off
    QCOMPARE (logic.timer.isActive (), false);
    QCOMPARE(gLockScreenWindowStub->stubCallCount("updateDateTime"), 0);

    // Then try with display on: the timer should be started
    qmDisplayState = Maemo::QmDisplayState::On;
    logic.toggleScreenLockUI(true);
    QTest::qWait (10);
#endif

    QCOMPARE (logic.timer.isActive (), true);
    QCOMPARE (logic.timer.interval (), 1000);
    QCOMPARE(gLockScreenWindowStub->stubCallCount("updateDateTime"), 1);

    // When the lock is toggled off, make sure the screen locking signals are sent and the lock UI is hidden
    logic.toggleScreenLockUI(false);
    QTest::qWait (10);
    QCOMPARE(logic.lockScreenWindow->isVisible(), false);
    QCOMPARE (logic.timer.isActive (), false);
}

void Ut_LockScreenBusinessLogic::testToggleEventEater()
{
    LockScreenBusinessLogic logic;

    // Make sure the screen locking signals are sent and the eater UI is shown/hidden
    logic.toggleEventEater(true);
    QTest::qWait (10);
    QCOMPARE(logic.eaterUI->isVisible(), true);
    // XXX: Stub not really allows us to test this:
    // QCOMPARE(logic.eaterUI->isFullScreen(), true);

    logic.toggleEventEater(false);
    QTest::qWait (10);
    QCOMPARE(logic.eaterUI->isVisible(), false);
}

void Ut_LockScreenBusinessLogic::testUnlockScreen()
{
    LockScreenBusinessLogic logic;
    QSignalSpy spy(&logic, SIGNAL(screenIsLocked(bool)));

    logic.unlockScreen();
#ifdef HAVE_QMSYSTEM
    logic.locksChanged (Maemo::QmLocks::TouchAndKeyboard, Maemo::QmLocks::Unlocked);
    QTest::qWait (10);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), false);
#else
    spy.clear ();
#endif
    QCOMPARE(logic.lockScreenWindow->isVisible(), false);
    QCOMPARE (logic.timer.isActive (), false);
}

void Ut_LockScreenBusinessLogic::testHideEventEater()
{
    LockScreenBusinessLogic logic;

    logic.hideEventEater();
    QTest::qWait (10);
    QCOMPARE(logic.eaterUI->isVisible(), false);
}

#ifdef HAVE_QMSYSTEM
void Ut_LockScreenBusinessLogic::testDisplayStateChanged()
{
    LockScreenBusinessLogic logic;
    logic.toggleScreenLockUI(true);

    // When lock-screen-ui is shown reset should be called on it
    QCOMPARE(gLockScreenWindowStub->stubCallCount ("reset"), 1);

    logic.displayStateChanged(Maemo::QmDisplayState::Off);
    QCOMPARE (logic.timer.isActive (), false);

    logic.displayStateChanged(Maemo::QmDisplayState::On);
    QCOMPARE (logic.timer.isActive (), true);
    QCOMPARE (logic.timer.interval (), 1000);

    // Also check whether the reset called on the
    // lock-screen-ui (after display turn on)
    QCOMPARE(gLockScreenWindowStub->stubCallCount ("reset"), 2);
}
#endif

QTEST_APPLESS_MAIN(Ut_LockScreenBusinessLogic)
