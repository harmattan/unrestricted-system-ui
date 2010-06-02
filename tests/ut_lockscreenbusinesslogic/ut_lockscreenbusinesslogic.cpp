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

#include "ut_lockscreenbusinesslogic.h"
#include "lockscreenbusinesslogic.h"
#include "lockscreenui.h"
#include "sysuid_stub.h"

#include <MApplication>
#include <MApplicationWindow>
#include <MGConfItem>
#include <MTheme>

#define DEBUG
#include "../../src/debug.h"

/******************************************************************************
 * EventSink implementation.
 */
EventSink::EventSink() :
    m_ScreenIsLockedCame (false),
    m_ScreenIsLocked (false),
    m_Timeouts (0)
{
}
 
void 
EventSink::screenIsLocked (
        bool toggle)
{
    SYS_DEBUG ("*** toggle = %s", SYS_BOOL(toggle));
    m_ScreenIsLockedCame = true;
    m_ScreenIsLocked = toggle;
}

void 
EventSink::timeout ()
{
    ++m_Timeouts;

    SYS_DEBUG ("*** m_Timeouts = %d", m_Timeouts);
}

/*******************************************************************************
 * The Ut_LockScreenBusinessLogic implements the unit tests.
 */
void Ut_LockScreenBusinessLogic::init()
{
}

void Ut_LockScreenBusinessLogic::cleanup()
{
}

int   argc = 1;
char *argv[] = {
    (char *) "./ut_lockscreenbusinesslogic",
    NULL };

const QString themeDir = "/usr/share/themes/base/meegotouch/sysuid/";
const QString styleDir = themeDir + "style/";

void Ut_LockScreenBusinessLogic::initTestCase()
{
    m_MainWindow = 0;
    m_LockScreenBusinessLogic = 0;

    SYS_DEBUG ("+++ Creating application.");
    m_App = new MApplication(argc, argv);
    m_App->setQuitOnLastWindowClosed (false);

    SYS_DEBUG ("+++ Initializing our own themes.");
    MTheme::addPixmapDirectory (themeDir, M::Recursive);
    MTheme::loadCSS (styleDir + "sysuid.css");
    MTheme::loadCSS (styleDir + "unlockscreen.css");
}

void 
Ut_LockScreenBusinessLogic::cleanupTestCase()
{
    if (m_LockScreenBusinessLogic)
        delete m_LockScreenBusinessLogic;

    if (m_MainWindow)
        delete m_MainWindow; 

    delete m_App;
}

/*
 * This test will check if the signal that indicates the screen
 * locking/unlocking is there and properly sent.
 */
void 
Ut_LockScreenBusinessLogic::testLockScreenBusinessLogicSignals ()
{
    bool connectSuccess;

    m_LockScreenBusinessLogic = new LockScreenBusinessLogic;
    
    connectSuccess = connect (
            m_LockScreenBusinessLogic, SIGNAL(screenIsLocked(bool)),
            &m_EventSink, SLOT(screenIsLocked(bool)));
    QVERIFY (connectSuccess);

    /*
     *
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** toggleScreenLockUI (true) *********************");
    SYS_DEBUG ("***************************************************");
    m_EventSink.m_ScreenIsLockedCame = false;
    m_EventSink.m_ScreenIsLocked = false;
    m_LockScreenBusinessLogic->toggleScreenLockUI (true);

    QVERIFY (m_EventSink.m_ScreenIsLockedCame == true);
    QVERIFY (m_EventSink.m_ScreenIsLocked == true);

    /*
     *
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** toggleScreenLockUI (false) ********************");
    SYS_DEBUG ("***************************************************");
    m_EventSink.m_ScreenIsLockedCame = false;
    m_EventSink.m_ScreenIsLocked = false;
    m_LockScreenBusinessLogic->toggleScreenLockUI (false);
    
    QVERIFY (m_EventSink.m_ScreenIsLockedCame == true);
    QVERIFY (m_EventSink.m_ScreenIsLocked == false);
    
    /*
     *
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** toggleEventEater (true) ***********************");
    SYS_DEBUG ("***************************************************");
    m_EventSink.m_ScreenIsLockedCame = false;
    m_EventSink.m_ScreenIsLocked = false;
    m_LockScreenBusinessLogic->toggleEventEater (true);

    QVERIFY (m_EventSink.m_ScreenIsLockedCame == true);
    QVERIFY (m_EventSink.m_ScreenIsLocked == true);

    /*
     *
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** toggleEventEater (false) **********************");
    SYS_DEBUG ("***************************************************");
    m_EventSink.m_ScreenIsLockedCame = false;
    m_EventSink.m_ScreenIsLocked = false;
    m_LockScreenBusinessLogic->toggleEventEater (false);

    QVERIFY (m_EventSink.m_ScreenIsLockedCame == true);
    QVERIFY (m_EventSink.m_ScreenIsLocked == false);

    delete m_LockScreenBusinessLogic;
    m_LockScreenBusinessLogic = 0;
}

/*
 * This test will check if the timer in the screenlokui is started when the
 * screen lock is shown and it is stopped when the screen lock is hidden.
 */
void 
Ut_LockScreenBusinessLogic::testLockScreenBusinessLogicTimer ()
{
    bool connectSuccess;

    m_LockScreenBusinessLogic = new LockScreenBusinessLogic;
    connectSuccess = connect (
            &m_LockScreenBusinessLogic->timer, SIGNAL (timeout()),
            &m_EventSink, SLOT(timeout()));
    QVERIFY (connectSuccess);

    /*
     * When the screen is locked the timer should be started. 
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** toggleEventEater (true) ***********************");
    SYS_DEBUG ("***************************************************");
    m_EventSink.m_Timeouts = 0;
    m_LockScreenBusinessLogic->toggleScreenLockUI (true);
    QTest::qWait (5000);

    SYS_DEBUG ("came %d", m_EventSink.m_Timeouts);
    // FIXME: We got 2 refreshes in 5 seconds?
    QVERIFY (m_EventSink.m_Timeouts >= 2);

    /*
     * When the screen is unlocked the timer should be stopped. 
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** toggleEventEater (false) ***********************");
    SYS_DEBUG ("***************************************************");
    m_LockScreenBusinessLogic->toggleScreenLockUI (false);
    m_EventSink.m_Timeouts = 0;
    QTest::qWait (5000);

    QVERIFY (m_EventSink.m_Timeouts == 0);

    delete m_LockScreenBusinessLogic;
    m_LockScreenBusinessLogic = 0;
}

/*
 * This function is testing if the lockscreenbusinesslogic actually connected to
 * the unlocked() signal of the unlockui.
 */
void 
Ut_LockScreenBusinessLogic::testLockScreenBusinessLogicUnlock ()
{
    bool connectSuccess;

    m_LockScreenBusinessLogic = new LockScreenBusinessLogic;
    connectSuccess = connect (
            m_LockScreenBusinessLogic, SIGNAL(screenIsLocked(bool)),
            &m_EventSink, SLOT(screenIsLocked(bool)));
    QVERIFY (connectSuccess);

    /*
     * First we lock the screen and check if it is really locked.
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** toggleScreenLockUI (true) *********************");
    SYS_DEBUG ("***************************************************");
    m_EventSink.m_ScreenIsLockedCame = false;
    m_EventSink.m_ScreenIsLocked = false;
    m_LockScreenBusinessLogic->toggleScreenLockUI (true);

    QVERIFY (m_EventSink.m_ScreenIsLockedCame == true);
    QVERIFY (m_EventSink.m_ScreenIsLocked == true);

    /*
     *
     */
    m_EventSink.m_ScreenIsLockedCame = false;
    emit m_LockScreenBusinessLogic->lockUI->unlocked();

    QVERIFY (m_EventSink.m_ScreenIsLockedCame == true);
    QVERIFY (m_EventSink.m_ScreenIsLocked == false);
    
    delete m_LockScreenBusinessLogic;
    m_LockScreenBusinessLogic = 0;
}


QTEST_APPLESS_MAIN(Ut_LockScreenBusinessLogic)

