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
#include <qmdisplaystate.h>
Maemo::QmDisplayState::DisplayState gDisplayState = Maemo::QmDisplayState::Unknown;
Maemo::QmDisplayState::DisplayState Maemo::QmDisplayState::get() const
{
    return gDisplayState;
}
#endif

void Ut_LockScreen::init()
{
    lockScreen = new LockScreen;
}

void Ut_LockScreen::cleanup()
{
    delete lockScreen;
    gDisplayState = Maemo::QmDisplayState::Unknown;
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

void Ut_LockScreen::testSliderUnlocked()
{
    connect(this, SIGNAL(unlocked()), lockScreen, SLOT(sliderUnlocked()));
    gDisplayState = Maemo::QmDisplayState::On;
    QSignalSpy spy(lockScreen, SIGNAL(unlocked()));
    emit unlocked();
    QCOMPARE(spy.count(), 1);

    gDisplayState = Maemo::QmDisplayState::Off;
    emit unlocked();
    QCOMPARE(spy.count(), 1);

    gDisplayState = Maemo::QmDisplayState::Dimmed;
    emit unlocked();
    QCOMPARE(spy.count(), 1);
}

void Ut_LockScreen::testWhenDisplayExitsLockScreenIsUnlocked()
{
    bool ret = disconnect(lockScreen, SIGNAL(displayExited()), lockScreen, SLOT(sliderUnlocked()));
    QCOMPARE(ret, true);
}

QTEST_APPLESS_MAIN(Ut_LockScreen)
