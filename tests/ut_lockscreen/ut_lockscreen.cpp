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

// MWidget stubs
bool gMWidgetIsOnDisplay = false;
bool MWidget::isOnDisplay() const
{
    return gMWidgetIsOnDisplay;
}

void Ut_LockScreen::init()
{
    gMWidgetIsOnDisplay = false;
    lockScreen = new LockScreen;
}

void Ut_LockScreen::cleanup()
{
    delete lockScreen;
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
    // Verify that the MApplication's minimized slot is tracked
    QVERIFY(disconnect(MApplication::instance(),
                       SIGNAL(minimized()),
                       lockScreen,
                       SLOT(unlock())));
}

void Ut_LockScreen::testUnlock_data()
{
    QTest::addColumn<bool>("isOnDisplay");
    QTest::addColumn<int>("expectedSignalCount");

    QTest::newRow("Window on display, device unlocks") << true << 1;
    QTest::newRow("Window not on display, device does not unlock") << false << 0;
}

void Ut_LockScreen::testUnlock()
{
    QFETCH(bool, isOnDisplay);
    QFETCH(int, expectedSignalCount);

    connect(this, SIGNAL(minimized()), lockScreen, SLOT(unlock()));
    QSignalSpy spy(lockScreen, SIGNAL(unlocked()));

    gMWidgetIsOnDisplay = isOnDisplay;
    emit minimized();
    QCOMPARE(spy.count(), expectedSignalCount);
}

QTEST_APPLESS_MAIN(Ut_LockScreen)
