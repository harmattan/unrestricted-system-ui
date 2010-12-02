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

#include "lockscreen.h"
#include "ut_lockscreen.h"
#include <MApplication>
#include <MWindow>

#ifdef HAVE_QMSYSTEM
Q_DECLARE_METATYPE(MeeGo::QmDisplayState::DisplayState);

int qmDisplayStateGetCalled = 0;
MeeGo::QmDisplayState::DisplayState gDisplayState = MeeGo::QmDisplayState::Unknown;
MeeGo::QmDisplayState::DisplayState MeeGo::QmDisplayState::get() const
{
    qmDisplayStateGetCalled++;
    return gDisplayState;
}
#endif

void Ut_LockScreen::init()
{
#ifdef HAVE_QMSYSTEM
    qmDisplayStateGetCalled = 0;
    gDisplayState = MeeGo::QmDisplayState::Unknown;
#endif

    lockScreen = new LockScreen;
}

void Ut_LockScreen::cleanup()
{
    delete lockScreen;
    gDisplayState = MeeGo::QmDisplayState::Unknown;
}

void Ut_LockScreen::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_lockscreen";
    app = new MApplication(argc, &app_name);

}

void Ut_LockScreen::cleanupTestCase()
{
    delete app;
}

void Ut_LockScreen::testConstruction()
{
#ifdef HAVE_QMSYSTEM
    // Verify that QmDisplayState's displayStateChanged signal is tracked
    QVERIFY(disconnect(&lockScreen->qmDisplayState,
                       SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)),
                       lockScreen,
                       SLOT(updateDisplayState(MeeGo::QmDisplayState::DisplayState))));

    // Verify that Lockscreen queries the display state at creation
    QCOMPARE(qmDisplayStateGetCalled, 1);
#endif
}

#ifdef HAVE_QMSYSTEM
void Ut_LockScreen::testUnlock_data()
{
    QTest::addColumn<MeeGo::QmDisplayState::DisplayState>("displayState");
    QTest::addColumn<int>("expectedSignalCount");

    QTest::newRow("Display on, device unlocks")
        << MeeGo::QmDisplayState::On << 1;
    QTest::newRow("Display dimmed, device does not unlocks")
        << MeeGo::QmDisplayState::Dimmed << 0;
    QTest::newRow("Display off, device does not unlocks")
        << MeeGo::QmDisplayState::Off << 0;
}

void Ut_LockScreen::testUnlock()
{
    QFETCH(MeeGo::QmDisplayState::DisplayState, displayState);
    QFETCH(int, expectedSignalCount);

    connect(this,
            SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)),
            lockScreen,
            SLOT(updateDisplayState(MeeGo::QmDisplayState::DisplayState)));
    connect(this, SIGNAL(unlocked()), lockScreen, SLOT(unlock()));
    QSignalSpy spy(lockScreen, SIGNAL(unlocked()));

    emit displayStateChanged(displayState);
    emit unlocked();
    QCOMPARE(spy.count(), expectedSignalCount);
}
#else
void Ut_LockScreen::testUnlock()
{
    connect(this, SIGNAL(unlocked()), lockScreen, SLOT(unlock()));
    QSignalSpy spy(lockScreen, SIGNAL(unlocked()));

    emit unlocked();
    QCOMPARE(spy.count(), 1);
}
#endif

void Ut_LockScreen::testWhenDisplayExitsLockScreenIsUnlocked()
{
    bool ret = disconnect(lockScreen, SIGNAL(displayExited()), lockScreen, SLOT(unlock()));
    QCOMPARE(ret, true);
}

QTEST_APPLESS_MAIN(Ut_LockScreen)
