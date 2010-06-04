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

#include "ft_lockscreenbusinesslogic.h"
#include "lockscreenbusinesslogic.h"
#include "lockscreenui.h"
#include "sysuid_stub.h"

#include <MApplication>
#include <MApplicationWindow>
#include <MSceneManager>
#include <MTheme>

#include <qmlocks.h>

using namespace Maemo;

#define DEBUG
#include "../../src/debug.h"

/*
 * We wait this much until we check if the window is actually appeared
 * disappeared from the stack of the window manager.
 */
static int WMDelay = 400;

/*
 * This much we wait between the tests.
 */
static int DelayBetweenTests = 1000;

void 
Ft_LockScreenBusinessLogic::init()
{
}

void 
Ft_LockScreenBusinessLogic::cleanup()
{
}

int   argc = 1;
char *argv[] = {
    (char *) "./ft_lockscreenbusinesslogic",
    NULL };

const QString themeDir = "/usr/share/themes/base/meegotouch/sysuid/";
const QString styleDir = themeDir + "style/";

void 
Ft_LockScreenBusinessLogic::initTestCase()
{
    m_MainWindow = 0;
    m_LockScreenBusinessLogic = 0;

    SYS_DEBUG ("+++ Creating application.");
    m_App = new MApplication(argc, argv);
    m_App->setQuitOnLastWindowClosed (false);

    SYS_DEBUG ("Initializing our own themes.");
    MTheme::addPixmapDirectory (themeDir, M::Recursive);
    MTheme::loadCSS (styleDir + "sysuid.css");
}

void 
Ft_LockScreenBusinessLogic::cleanupTestCase()
{
    if (m_LockScreenBusinessLogic)
        delete m_LockScreenBusinessLogic;

    if (m_MainWindow)
        delete m_MainWindow; 

    delete m_App;
}

void
Ft_LockScreenBusinessLogic::testLockScreenBusinessLogic ()
{
    Window LockScreenUIWindowID;
    Window EventEaterWindowID;

    m_LockScreenBusinessLogic = new LockScreenBusinessLogic;
    /*
     * Once it is constructed the two UI elements must already be there.
     */
    QVERIFY (m_LockScreenBusinessLogic->lockUI != NULL);
    QVERIFY (m_LockScreenBusinessLogic->eaterUI != NULL);

    /*
     *
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** calling toggleEventEater (true) ***************");
    SYS_DEBUG ("***************************************************");
    m_LockScreenBusinessLogic->toggleEventEater (true);
    QTest::qWait (WMDelay);
    LockScreenUIWindowID = m_LockScreenBusinessLogic->lockUI->internalWinId();
    EventEaterWindowID = m_LockScreenBusinessLogic->eaterUI->internalWinId();
    
    QVERIFY (m_XChecker.checkWindow (
                EventEaterWindowID, 
                XChecker::CheckIsVisible));
    QVERIFY (m_XChecker.checkWindow (
                EventEaterWindowID, 
                XChecker::CheckIsFullscreen));
    QVERIFY (m_XChecker.checkWindow (
                LockScreenUIWindowID, 
                XChecker::CheckIsInvisible));

    QTest::qWait (DelayBetweenTests);

    /*
     *
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** calling toggleEventEater (false) **************");
    SYS_DEBUG ("***************************************************");
    m_LockScreenBusinessLogic->toggleEventEater (false);
    QTest::qWait (WMDelay);
    LockScreenUIWindowID = m_LockScreenBusinessLogic->lockUI->internalWinId();
    EventEaterWindowID = m_LockScreenBusinessLogic->eaterUI->internalWinId();
    
    QVERIFY (m_XChecker.checkWindow(
                EventEaterWindowID, 
                XChecker::CheckIsInvisible));
    QVERIFY (m_XChecker.checkWindow(
                LockScreenUIWindowID, 
                XChecker::CheckIsInvisible));
    
    QTest::qWait (DelayBetweenTests);

    /*
     *
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** calling toggleScreenLockUI (true) **************");
    SYS_DEBUG ("***************************************************");
    m_LockScreenBusinessLogic->toggleScreenLockUI (true);
    QTest::qWait (WMDelay);
    LockScreenUIWindowID = m_LockScreenBusinessLogic->lockUI->internalWinId();
    EventEaterWindowID = m_LockScreenBusinessLogic->eaterUI->internalWinId();
    
    QVERIFY (m_XChecker.checkWindow(
                EventEaterWindowID, 
                XChecker::CheckIsInvisible));
    QVERIFY (m_XChecker.checkWindow(
                LockScreenUIWindowID, 
                XChecker::CheckIsVisible));
    QVERIFY (m_XChecker.checkWindow(
                LockScreenUIWindowID, 
                XChecker::CheckIsFullscreen));
    
    QTest::qWait (DelayBetweenTests);

    /*
     *
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** calling toggleScreenLockUI (false) ************");
    SYS_DEBUG ("***************************************************");
    m_LockScreenBusinessLogic->toggleScreenLockUI (false);
    QTest::qWait (WMDelay);
    LockScreenUIWindowID = m_LockScreenBusinessLogic->lockUI->internalWinId();
    EventEaterWindowID = m_LockScreenBusinessLogic->eaterUI->internalWinId();
    
    QVERIFY (m_XChecker.checkWindow(
                EventEaterWindowID, 
                XChecker::CheckIsInvisible));
    QVERIFY (m_XChecker.checkWindow(
                LockScreenUIWindowID, 
                XChecker::CheckIsInvisible));
    
    QTest::qWait (DelayBetweenTests);

    delete m_LockScreenBusinessLogic;
    m_LockScreenBusinessLogic = NULL;
}

void
Ft_LockScreenBusinessLogic::testLockScreenBusinessLogicWithMainWindow ()
{
    Window LockScreenUIWindowID;
    Window EventEaterWindowID;
    Window MWindowID;

    SYS_DEBUG ("Creating main window.");
    m_MainWindow = new MApplicationWindow;

    Qt::WindowFlags flags = 0;
    flags |= Qt::FramelessWindowHint;
    flags |= Qt::CustomizeWindowHint;
    flags |= Qt::WindowStaysOnTopHint;
    m_MainWindow->setWindowOpacity (0.0);
    m_MainWindow->setWindowFlags (flags);
    MWindowID = m_MainWindow->internalWinId ();
    SYS_DEBUG ("*** MainWindowID    = 0x%lx", MWindowID);

    m_LockScreenBusinessLogic = new LockScreenBusinessLogic;
    /*
     * Once it is constructed the two UI elements must already be there.
     */
    QVERIFY (m_LockScreenBusinessLogic->lockUI != NULL);
    QVERIFY (m_LockScreenBusinessLogic->eaterUI != NULL);

    /*
     *
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** calling toggleEventEater (true) ***************");
    SYS_DEBUG ("***************************************************");
    m_LockScreenBusinessLogic->toggleEventEater (true);
    QTest::qWait (WMDelay);
    LockScreenUIWindowID = m_LockScreenBusinessLogic->lockUI->internalWinId();
    EventEaterWindowID = m_LockScreenBusinessLogic->eaterUI->internalWinId();
    
    QVERIFY (m_XChecker.checkWindow(
                EventEaterWindowID, 
                XChecker::CheckIsVisible));
    QVERIFY (m_XChecker.checkWindow(
                EventEaterWindowID, 
                XChecker::CheckIsFullscreen));
    QVERIFY (m_XChecker.checkWindow(
                LockScreenUIWindowID, 
                XChecker::CheckIsInvisible));

    QTest::qWait (DelayBetweenTests);

    /*
     *
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** calling toggleEventEater (false) **************");
    SYS_DEBUG ("***************************************************");
    m_LockScreenBusinessLogic->toggleEventEater (false);
    QTest::qWait (WMDelay);
    LockScreenUIWindowID = m_LockScreenBusinessLogic->lockUI->internalWinId();
    EventEaterWindowID = m_LockScreenBusinessLogic->eaterUI->internalWinId();
    
    QVERIFY (m_XChecker.checkWindow(
                EventEaterWindowID, 
                XChecker::CheckIsInvisible));
    QVERIFY (m_XChecker.checkWindow(
                LockScreenUIWindowID, 
                XChecker::CheckIsInvisible));
    
    QTest::qWait (DelayBetweenTests);

    /*
     *
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** calling toggleScreenLockUI (true) **************");
    SYS_DEBUG ("***************************************************");
    m_LockScreenBusinessLogic->toggleScreenLockUI (true);
    QTest::qWait (WMDelay);
    LockScreenUIWindowID = m_LockScreenBusinessLogic->lockUI->internalWinId();
    EventEaterWindowID = m_LockScreenBusinessLogic->eaterUI->internalWinId();
    
    QVERIFY (m_XChecker.checkWindow(
                EventEaterWindowID, 
                XChecker::CheckIsInvisible));
    QVERIFY (m_XChecker.checkWindow(
                LockScreenUIWindowID, 
                XChecker::CheckIsVisible));
    QVERIFY (m_XChecker.checkWindow(
                LockScreenUIWindowID, 
                XChecker::CheckIsFullscreen));
    
    QTest::qWait (DelayBetweenTests);

    /*
     *
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** calling toggleScreenLockUI (false) ************");
    SYS_DEBUG ("***************************************************");
    m_LockScreenBusinessLogic->toggleScreenLockUI (false);
    QTest::qWait (WMDelay);
    LockScreenUIWindowID = m_LockScreenBusinessLogic->lockUI->internalWinId();
    EventEaterWindowID = m_LockScreenBusinessLogic->eaterUI->internalWinId();
    
    QVERIFY (m_XChecker.checkWindow(
                EventEaterWindowID, 
                XChecker::CheckIsInvisible));
    QVERIFY (m_XChecker.checkWindow(
                LockScreenUIWindowID, 
                XChecker::CheckIsInvisible));
    
    QTest::qWait (DelayBetweenTests);

    delete m_MainWindow;
    m_MainWindow = 0;

    delete m_LockScreenBusinessLogic;
    m_LockScreenBusinessLogic = NULL;
}

void 
Ft_LockScreenBusinessLogic::testLockScreenBusinessLogicMissedEvents ()
{
    bool missedEventsAreVisible; 

    m_LockScreenBusinessLogic = new LockScreenBusinessLogic;
    
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** toggleScreenLockUI (true) *********************");
    SYS_DEBUG ("***************************************************");
    m_LockScreenBusinessLogic->toggleScreenLockUI (true);
    QTest::qWait (DelayBetweenTests);
    QVERIFY (m_LockScreenBusinessLogic->lockUI->m_notificationArea != 0);

    m_LockScreenBusinessLogic->updateMissedEvents (0, 0, 0, 0);
    QTest::qWait (DelayBetweenTests);
    missedEventsAreVisible = 
        m_LockScreenBusinessLogic->lockUI->m_notificationArea->isVisible();
    QVERIFY (missedEventsAreVisible == false);
    
    m_LockScreenBusinessLogic->updateMissedEvents (1, 0, 0, 0);
    QTest::qWait (DelayBetweenTests);
    missedEventsAreVisible = 
        m_LockScreenBusinessLogic->lockUI->m_notificationArea->isVisible();
    QTest::qWait (DelayBetweenTests);
    QVERIFY (missedEventsAreVisible == true);
    
    m_LockScreenBusinessLogic->updateMissedEvents (2, 4, 0, 0);
    QTest::qWait (DelayBetweenTests);
    missedEventsAreVisible = 
        m_LockScreenBusinessLogic->lockUI->m_notificationArea->isVisible();
    QTest::qWait (DelayBetweenTests);
    QVERIFY (missedEventsAreVisible == true);
    
    m_LockScreenBusinessLogic->updateMissedEvents (2, 4, 5, 6);
    QTest::qWait (DelayBetweenTests);
    missedEventsAreVisible = 
        m_LockScreenBusinessLogic->lockUI->m_notificationArea->isVisible();
    QTest::qWait (DelayBetweenTests);
    QVERIFY (missedEventsAreVisible == true);
    
    m_LockScreenBusinessLogic->updateMissedEvents (0, 0, 0, 6);
    QTest::qWait (DelayBetweenTests);
    missedEventsAreVisible = 
        m_LockScreenBusinessLogic->lockUI->m_notificationArea->isVisible();
    QTest::qWait (DelayBetweenTests);
    QVERIFY (missedEventsAreVisible == true);
}

/*!
 * This test will try to emulate the situation when we lock the screen (touch
 * screen lock becomes black) show the lockscreenui and then we turn on the
 * touch screen again. 
 */
void 
Ft_LockScreenBusinessLogic::testLockScreenBusinessLogicWithLocking ()
{
    QmLocks locks;
    bool    lockingSuccess, unlockingSuccess;
    Window  LockScreenUIWindowID;
    Window  EventEaterWindowID;

    /*
     * Locking the screen first.
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** Locking the screen ****************************");
    SYS_DEBUG ("***************************************************");
    lockingSuccess = locks.setState (
            QmLocks::TouchAndKeyboard, QmLocks::Locked);
    #ifndef __i386__
    QVERIFY (lockingSuccess);
    #endif
    QTest::qWait (DelayBetweenTests);

    /*
     *
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** calling toggleScreenLockUI (true) **************");
    SYS_DEBUG ("***************************************************");
    m_LockScreenBusinessLogic->toggleScreenLockUI (true);
    QTest::qWait (WMDelay);

    /*
     * Then unlocking the screen again so the lockscreenUI will be visible.
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** Unlocking the screen **************************");
    SYS_DEBUG ("***************************************************");
    unlockingSuccess = locks.setState (
            QmLocks::TouchAndKeyboard, QmLocks::Unlocked);
    #ifndef __i386__
    QVERIFY (unlockingSuccess);
    #endif
    QTest::qWait (WMDelay);

    /*
     * Now that the screen is on the lockscreenUI should be up and visible.
     */
    LockScreenUIWindowID = m_LockScreenBusinessLogic->lockUI->internalWinId();
    EventEaterWindowID = m_LockScreenBusinessLogic->eaterUI->internalWinId();
    
    QVERIFY (m_XChecker.checkWindow(
                EventEaterWindowID, 
                XChecker::CheckIsInvisible));
    QVERIFY (m_XChecker.checkWindow(
                LockScreenUIWindowID, 
                XChecker::CheckIsVisible));
    QVERIFY (m_XChecker.checkWindow(
                LockScreenUIWindowID, 
                XChecker::CheckIsFullscreen));

    QTest::qWait (DelayBetweenTests);

    /*
     * We simply don't want to leave the window there, so let's hide it again,
     * and of course once we hide we can check if it is hidden.
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** calling toggleScreenLockUI (false) ************");
    SYS_DEBUG ("***************************************************");
    m_LockScreenBusinessLogic->toggleScreenLockUI (false);
    QTest::qWait (WMDelay);
    LockScreenUIWindowID = m_LockScreenBusinessLogic->lockUI->internalWinId();
    EventEaterWindowID = m_LockScreenBusinessLogic->eaterUI->internalWinId();
    
    QVERIFY (m_XChecker.checkWindow(
                EventEaterWindowID, 
                XChecker::CheckIsInvisible));
    QVERIFY (m_XChecker.checkWindow(
                LockScreenUIWindowID, 
                XChecker::CheckIsInvisible));
}

QTEST_APPLESS_MAIN(Ft_LockScreenBusinessLogic)

