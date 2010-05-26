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
static int DelayBetweenTests = 5000;

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
    MTheme::loadCSS (styleDir + "unlockscreen.css");
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
    SYS_DEBUG ("*** LockScreenUIWindowID = 0x%lx", LockScreenUIWindowID);
    SYS_DEBUG ("*** EventEaterWindowID   = 0x%lx", EventEaterWindowID);

    m_XChecker.debug_dump_windows (EventEaterWindowID);
    QVERIFY (m_XChecker.check_window(
                EventEaterWindowID, 
                XChecker::CheckIsVisible));
    QVERIFY (m_XChecker.check_window(
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
    SYS_DEBUG ("*** LockScreenUIWindowID = 0x%lx", LockScreenUIWindowID);
    SYS_DEBUG ("*** EventEaterWindowID   = 0x%lx", EventEaterWindowID);

    m_XChecker.debug_dump_windows (EventEaterWindowID);
    QVERIFY (m_XChecker.check_window(
                EventEaterWindowID, 
                XChecker::CheckIsInvisible));
    QVERIFY (m_XChecker.check_window(
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
    SYS_DEBUG ("*** LockScreenUIWindowID = 0x%lx", LockScreenUIWindowID);
    SYS_DEBUG ("*** EventEaterWindowID   = 0x%lx", EventEaterWindowID);

    m_XChecker.debug_dump_windows (LockScreenUIWindowID);
    QVERIFY (m_XChecker.check_window(
                EventEaterWindowID, 
                XChecker::CheckIsInvisible));
    QVERIFY (m_XChecker.check_window(
                LockScreenUIWindowID, 
                XChecker::CheckIsVisible));
    
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
    SYS_DEBUG ("*** LockScreenUIWindowID = 0x%lx", LockScreenUIWindowID);
    SYS_DEBUG ("*** EventEaterWindowID   = 0x%lx", EventEaterWindowID);

    m_XChecker.debug_dump_windows (LockScreenUIWindowID);
    QVERIFY (m_XChecker.check_window(
                EventEaterWindowID, 
                XChecker::CheckIsInvisible));
    QVERIFY (m_XChecker.check_window(
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
    SYS_DEBUG ("*** LockScreenUIWindowID = 0x%lx", LockScreenUIWindowID);
    SYS_DEBUG ("*** EventEaterWindowID   = 0x%lx", EventEaterWindowID);

    m_XChecker.debug_dump_windows (EventEaterWindowID);
    QVERIFY (m_XChecker.check_window(
                EventEaterWindowID, 
                XChecker::CheckIsVisible));
    QVERIFY (m_XChecker.check_window(
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
    SYS_DEBUG ("*** LockScreenUIWindowID = 0x%lx", LockScreenUIWindowID);
    SYS_DEBUG ("*** EventEaterWindowID   = 0x%lx", EventEaterWindowID);

    m_XChecker.debug_dump_windows (EventEaterWindowID);
    QVERIFY (m_XChecker.check_window(
                EventEaterWindowID, 
                XChecker::CheckIsInvisible));
    QVERIFY (m_XChecker.check_window(
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
    SYS_DEBUG ("*** LockScreenUIWindowID = 0x%lx", LockScreenUIWindowID);
    SYS_DEBUG ("*** EventEaterWindowID   = 0x%lx", EventEaterWindowID);

    m_XChecker.debug_dump_windows (LockScreenUIWindowID);
    QVERIFY (m_XChecker.check_window(
                EventEaterWindowID, 
                XChecker::CheckIsInvisible));
    QVERIFY (m_XChecker.check_window(
                LockScreenUIWindowID, 
                XChecker::CheckIsVisible));
    
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
    SYS_DEBUG ("*** LockScreenUIWindowID = 0x%lx", LockScreenUIWindowID);
    SYS_DEBUG ("*** EventEaterWindowID   = 0x%lx", EventEaterWindowID);

    m_XChecker.debug_dump_windows (LockScreenUIWindowID);
    QVERIFY (m_XChecker.check_window(
                EventEaterWindowID, 
                XChecker::CheckIsInvisible));
    QVERIFY (m_XChecker.check_window(
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

QTEST_APPLESS_MAIN(Ft_LockScreenBusinessLogic)

