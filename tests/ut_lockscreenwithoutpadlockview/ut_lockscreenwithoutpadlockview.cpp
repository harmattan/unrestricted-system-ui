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
****************************************************************************/

#include <MApplication>
#include "ut_lockscreenwithoutpadlockview.h"
#include "lockscreenwithoutpadlockview.h"
#include "lockscreen_stub.h"
#include "notificationarea_stub.h"

void Ut_LockScreenWithoutPadlockView::init()
{
    controller = new LockScreen(NULL);
    m_subject = new LockScreenWithoutPadlockView(controller);
    controller->setView(m_subject);
}

void Ut_LockScreenWithoutPadlockView::cleanup()
{
    gNotificationAreaStub->stubReset();
    delete controller;
}

void Ut_LockScreenWithoutPadlockView::initTestCase()
{
    int   argc = 1;
    static char *app_name = (char *)"./ut_lockscreenwithoutpadlockview";
    app = new MApplication(argc, &app_name);
}

void Ut_LockScreenWithoutPadlockView::cleanupTestCase()
{
    delete app;
}

void Ut_LockScreenWithoutPadlockView::testNotificationArea()
{
    QCOMPARE(gNotificationAreaStub->stubCallCount("setHonorPrivacySetting"), 1);
    QCOMPARE(gNotificationAreaStub->stubLastCallTo("setHonorPrivacySetting").parameter<bool>(0), true);
}

void Ut_LockScreenWithoutPadlockView::testCreatedItemsAreRemovedFromTheControllerAndTheScene()
{
    controller->setView(NULL);

    // All the items added by the view should have disappeared
    QCOMPARE(controller->childItems().count(), 0);
}

QTEST_APPLESS_MAIN(Ut_LockScreenWithoutPadlockView)
