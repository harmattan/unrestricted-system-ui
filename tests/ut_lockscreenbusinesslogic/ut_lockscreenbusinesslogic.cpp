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
#include "screenlockbusinesslogic.h"
#include "lockscreenwindow_stub.h"
#include "eventeater_stub.h"
#include "sysuid_stub.h"
#include "eventeater_stub.h"
#include "closeeventeater_stub.h"
#include <MApplication>
#include <MApplicationWindow>

#ifdef HAVE_QMSYSTEM
#include <qmdisplaystate.h>
MeeGo::QmDisplayState::DisplayState qmDisplayState;
MeeGo::QmDisplayState::DisplayState MeeGo::QmDisplayState::get() const
{
    return qmDisplayState;
}
#endif

QMap<QWidget *, bool> gQWidgetVisible;
void QWidget::setVisible(bool visible)
{
    gQWidgetVisible[this] = visible;
    setAttribute(Qt::WA_WState_Visible, visible);
}

bool gQWidgetRaiseCalled = false;
void QWidget::raise()
{
    gQWidgetRaiseCalled = true;
}

void Ut_LockScreenBusinessLogic::init()
{
    gScreenLockWindowStub->stubReset();
    gEventEaterStub->stubReset();
#ifdef HAVE_QMSYSTEM
    qmDisplayState = MeeGo::QmDisplayState::On;
#endif
}

void Ut_LockScreenBusinessLogic::cleanup()
{
    gQWidgetRaiseCalled = false;
    gQWidgetVisible.clear();
}

void Ut_LockScreenBusinessLogic::initTestCase()
{
    static int argc = 1;
    static char *argv = (char *) "./ut_lockscreenbusinesslogic";
    m_App = new MApplication(argc, &argv);
    /* XXX: input context caused a crash :-S */
    m_App->setLoadMInputContext (false);
    m_App->setQuitOnLastWindowClosed (false);
}

void Ut_LockScreenBusinessLogic::cleanupTestCase()
{
    m_App->deleteLater ();
}

void Ut_LockScreenBusinessLogic::testToggleScreenLockUI()
{
    ScreenLockBusinessLogic logic;
    QSignalSpy spy(&logic, SIGNAL(screenIsLocked(bool)));

#ifdef HAVE_QMSYSTEM
    // First try with display off
    qmDisplayState = MeeGo::QmDisplayState::Off;
    logic.locksChanged (MeeGo::QmLocks::TouchAndKeyboard, MeeGo::QmLocks::Locked);
#endif

    // When the lock is toggled on, make sure the screen locking signals are sent and the lock UI is shown
    logic.toggleScreenLockUI(true);
#ifdef HAVE_QMSYSTEM
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), true);
#else
    spy.clear ();
#endif

    // The lock screen should now be visible
    QCOMPARE(gQWidgetVisible[logic.screenLockWindow], true);

    // Raising should happen
    QCOMPARE(gQWidgetRaiseCalled, true);

    // The lock screen needs to be reset
    QCOMPARE(gScreenLockWindowStub->stubCallCount("reset"), 1);

    // Reset the stubs
    gQWidgetVisible[logic.screenLockWindow] = false;
    gQWidgetRaiseCalled = false;

    // Lock the screen again
    logic.toggleScreenLockUI(true);

    // show() should not be called
    QCOMPARE(gQWidgetVisible[logic.screenLockWindow], false);

    // Raising should happen
    QCOMPARE(gQWidgetRaiseCalled, true);

    // The lock screen still needs to be reset
    QCOMPARE(gScreenLockWindowStub->stubCallCount("reset"), 2);

    // When the lock is toggled off, make sure the screen locking signals are sent and the lock UI is hidden
    logic.toggleScreenLockUI(false);
    QCOMPARE(logic.screenLockWindow->isVisible(), false);
}

void Ut_LockScreenBusinessLogic::testToggleEventEater()
{
    ScreenLockBusinessLogic logic;

    // Make sure the screen locking signals are sent and the eater UI is shown/hidden
    logic.toggleEventEater(true);
    QCOMPARE(gEventEaterStub->stubCallCount("show"), 1);

    logic.toggleEventEater(false);
    QCOMPARE(gEventEaterStub->stubCallCount("hide"), 1);
}

void Ut_LockScreenBusinessLogic::testUnlockScreen()
{
    ScreenLockBusinessLogic logic;
    QSignalSpy spy(&logic, SIGNAL(screenIsLocked(bool)));

    logic.unlockScreen();
#ifdef HAVE_QMSYSTEM
    logic.locksChanged (MeeGo::QmLocks::TouchAndKeyboard, MeeGo::QmLocks::Unlocked);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), false);
#else
    spy.clear ();
#endif
    QVERIFY(logic.screenLockWindow == NULL);
}

void Ut_LockScreenBusinessLogic::testHideEventEater()
{
    ScreenLockBusinessLogic logic;

    logic.hideEventEater();
    QVERIFY(logic.eventEaterWindow == NULL);
}

#ifdef HAVE_QMSYSTEM
void Ut_LockScreenBusinessLogic::testDisplayStateChanged()
{
    ScreenLockBusinessLogic logic;
    logic.toggleScreenLockUI(true);

    // When lock-screen-ui is shown reset should be called on it
    QCOMPARE(gScreenLockWindowStub->stubCallCount ("reset"), 1);

    logic.displayStateChanged(MeeGo::QmDisplayState::Off);
    logic.displayStateChanged(MeeGo::QmDisplayState::On);

    // Also check whether the reset called on the
    // lock-screen-ui (after display turn on)
    QCOMPARE(gScreenLockWindowStub->stubCallCount ("reset"), 2);
}
#endif

QTEST_APPLESS_MAIN(Ut_LockScreenBusinessLogic)
