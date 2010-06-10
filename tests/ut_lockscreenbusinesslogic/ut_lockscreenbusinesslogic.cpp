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
#include "lockscreenui_stub.h"
#include "lockscreenwindow_stub.h"
#include "eventeaterui_stub.h"
#include "sysuid_stub.h"
#include <qmdisplaystate.h>
#include <MApplication>
#include <MApplicationWindow>

Maemo::QmDisplayState::DisplayState qmDisplayState;
Maemo::QmDisplayState::DisplayState QmDisplayState::get() const
{
    return qmDisplayState;
}

// QTimer stubs
int qTimerStart = -1;
void QTimer::start(int msec)
{
    qTimerStart = msec;
    id = 1;
}

void QTimer::stop()
{
    qTimerStart = -1;
    id = -1;
}

void Ut_LockScreenBusinessLogic::init()
{
    qTimerStart = -1;
    gLockScreenUIStub->stubReset();
    qmDisplayState = Maemo::QmDisplayState::On;
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

    // First try with display off
    qmDisplayState = Maemo::QmDisplayState::Off;

    // When the lock is toggled on, make sure the screen locking signals are sent and the lock UI is shown
    logic.toggleScreenLockUI(true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);
    QCOMPARE(logic.lockUI->isVisible(), true);

#ifndef __i386__
    // The timer should not be started if the display is off
    QCOMPARE(qTimerStart, -1);
    QCOMPARE(gLockScreenUIStub->stubCallCount("updateDateTime"), 0);

    // Then try with display on: the timer should be started
    qmDisplayState = Maemo::QmDisplayState::On;
    logic.toggleScreenLockUI(true);
#endif

    QCOMPARE(qTimerStart, 1000);
    QCOMPARE(gLockScreenUIStub->stubCallCount("updateDateTime"), 1);

    // When the lock is toggled off, make sure the screen locking signals are sent and the lock UI is hidden
    spy.clear();
    logic.toggleScreenLockUI(false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), false);
    QCOMPARE(logic.lockUI->isVisible(), false);
    QCOMPARE(qTimerStart, -1);
}

void Ut_LockScreenBusinessLogic::testToggleEventEater()
{
    LockScreenBusinessLogic logic;
    QSignalSpy spy(&logic, SIGNAL(screenIsLocked(bool)));

    // Make sure the screen locking signals are sent and the eater UI is shown/hidden
    logic.toggleEventEater(true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);
    QCOMPARE(logic.eaterUI->isVisible(), true);
    QCOMPARE(logic.eaterUI->isFullScreen(), true);

    spy.clear();
    logic.toggleEventEater(false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), false);
    QCOMPARE(logic.eaterUI->isVisible(), false);
}

void Ut_LockScreenBusinessLogic::testUnlockScreen()
{
    LockScreenBusinessLogic logic;
    QSignalSpy spy(&logic, SIGNAL(screenIsLocked(bool)));

    logic.unlockScreen();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), false);
    QCOMPARE(logic.lockUI->isVisible(), false);
    QCOMPARE(qTimerStart, -1);
}

void Ut_LockScreenBusinessLogic::testHideEventEater()
{
    LockScreenBusinessLogic logic;
    QSignalSpy spy(&logic, SIGNAL(screenIsLocked(bool)));

    logic.hideEventEater();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), false);
    QCOMPARE(logic.eaterUI->isVisible(), false);
}

void Ut_LockScreenBusinessLogic::testDisplayStateChanged()
{
    LockScreenBusinessLogic logic;
    logic.toggleScreenLockUI(true);
    logic.displayStateChanged(Maemo::QmDisplayState::Off);
    QCOMPARE(qTimerStart, -1);

    logic.toggleEventEater(true);
    logic.displayStateChanged(Maemo::QmDisplayState::On);
    QCOMPARE(qTimerStart, 1000);
}

QTEST_APPLESS_MAIN(Ut_LockScreenBusinessLogic)
