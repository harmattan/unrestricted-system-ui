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

#include "ft_lockscreenui.h"
#include "xchecker.h"
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


void Ft_LockScreenUI::init()
{
}

void Ft_LockScreenUI::cleanup()
{
}


int   argc = 1;
char *argv[] = {
    (char *) "./ft_lockscreenui",
    NULL };

const QString themeDir = "/usr/share/themes/base/meegotouch/sysuid/";
const QString styleDir = themeDir + "style/";

void Ft_LockScreenUI::initTestCase()
{
    m_EventEaterUI = 0;
    m_LockScreenUI = 0;
    m_MainWindow = 0;

    SYS_DEBUG ("+++ Creating application.");
    m_App = new MApplication(argc, argv);
    m_App->setQuitOnLastWindowClosed (false);

    SYS_DEBUG ("Initializing our own themes.");
    MTheme::addPixmapDirectory (themeDir, M::Recursive);
    MTheme::loadCSS (styleDir + "sysuid.css");
    MTheme::loadCSS (styleDir + "unlockscreen.css");
}

void 
Ft_LockScreenUI::cleanupTestCase()
{
    delete m_LockScreenUI;
    delete m_EventEaterUI;
    delete m_MainWindow; 
    delete m_App;
}

void
Ft_LockScreenUI::testEventEaterUIShowHide ()
{
    Window WindowID;

    createEventEaterUI ();

    SYS_DEBUG ("*** m_EventEaterUI = %p", m_EventEaterUI);
    for (int i = 0; i < 3; ++i) {
        /*
         * Showing the eventEaterUI window.
         */
        SYS_DEBUG ("***************************************************");
        SYS_DEBUG ("*** Showing eventeaterUI **************************");
        SYS_DEBUG ("***************************************************");
        m_EventEaterUI->show ();
        m_EventEaterUI->showFullScreen ();
        QTest::qWait (WMDelay);
        
        /*
         * From this point the event eater must be shown and visible.
         */
        WindowID = m_EventEaterUI->internalWinId();
        QVERIFY (m_EventEaterUI->isVisible());
        QVERIFY (m_XChecker.check_window(WindowID, XChecker::CheckIsVisible));

        m_XChecker.debug_dump_windows (WindowID);
        QTest::qWait (DelayBetweenTests);
        /*
         * Hiding the window again.
         */
        SYS_DEBUG ("***************************************************");
        SYS_DEBUG ("*** Hiding eventeaterUI ***************************");
        SYS_DEBUG ("***************************************************");
        m_EventEaterUI->hide();
        QTest::qWait (WMDelay);
        WindowID = m_EventEaterUI->internalWinId();
        SYS_DEBUG ("*** WindowID = 0x%lx", WindowID);
        QVERIFY (!m_EventEaterUI->isVisible());
        QVERIFY (m_XChecker.check_window(WindowID, XChecker::CheckIsInvisible));
    }
}

/*!
 * Will create a lockscreenUI window, show and hide it several times and check
 * if the window is actually shown by the XServer.
 */
void
Ft_LockScreenUI::testLockScreenUIShowHide ()
{
    Window WindowID;

    createLockScreenUI ();
   
    for (int n = 0; n < 2; ++n) {
        /*
         * Showing the lockscreenUI window.
         */
        SYS_DEBUG ("***************************************************");
        SYS_DEBUG ("*** Showing lockscreenUI **************************");
        SYS_DEBUG ("***************************************************");
        m_LockScreenUI->show ();
        QTest::qWait (WMDelay);
        /*
         * From this point the lock screen should be realized, shown and visible.
         */
        QVERIFY (m_LockScreenUI->m_Realized);
        QVERIFY (m_LockScreenUI->m_SceneWindow != NULL);
        QVERIFY (m_LockScreenUI->isVisible());
        WindowID = m_LockScreenUI->internalWinId();
        QVERIFY (m_XChecker.check_window(WindowID, XChecker::CheckIsVisible));
    
        QTest::qWait (DelayBetweenTests);
        m_XChecker.debug_dump_windows (WindowID);
        /*
        * Hiding the window again.
        */
        SYS_DEBUG ("***************************************************");
        SYS_DEBUG ("*** Hiding lockscreenUI ***************************");
        SYS_DEBUG ("***************************************************");
        m_LockScreenUI->hide();
        QTest::qWait (WMDelay);
        WindowID = m_LockScreenUI->internalWinId();
        QVERIFY (!m_LockScreenUI->isVisible());
        QVERIFY (m_XChecker.check_window(WindowID, XChecker::CheckIsInvisible));
    }
}

/*!
 * Again we show/hide the lockscreenUI but before we do that we create an
 * MApplicationWindow.
 */
void
Ft_LockScreenUI::testLockScreenUIShowHideWithMainWindow ()
{
    Window WindowID;
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
    
    SYS_DEBUG ("*** MWindowID    = 0x%lx", MWindowID);
    
    createLockScreenUI ();

    for (int n = 0; n < 2; ++n) {
        /*
         * Showing the lockscreenUI window.
         */
        SYS_DEBUG ("***************************************************");
        SYS_DEBUG ("*** Showing lockscreenUI **************************");
        SYS_DEBUG ("***************************************************");
        m_LockScreenUI->show ();
        QTest::qWait (WMDelay);
        /*
         * From this point the lock screen should be realized, shown and visible.
         */
        QVERIFY (m_LockScreenUI->m_Realized);
        QVERIFY (m_LockScreenUI->m_SceneWindow != NULL);
        QVERIFY (m_LockScreenUI->isVisible());
        WindowID = m_LockScreenUI->internalWinId();
        QVERIFY (m_XChecker.check_window(WindowID, XChecker::CheckIsVisible));
        QVERIFY (m_XChecker.check_window(MWindowID, XChecker::CheckIsInvisible));
        SYS_DEBUG ("*** lockScreenUI = 0x%lx", WindowID);
    
        QTest::qWait (DelayBetweenTests);
        m_XChecker.debug_dump_windows (WindowID);
        /*
        * Hiding the window again.
        */
        SYS_DEBUG ("***************************************************");
        SYS_DEBUG ("*** Hiding lockscreenUI ***************************");
        SYS_DEBUG ("***************************************************");
        m_LockScreenUI->hide();
        QTest::qWait (WMDelay);
        WindowID = m_LockScreenUI->internalWinId();
        QVERIFY (!m_LockScreenUI->isVisible());
        QVERIFY (m_XChecker.check_window(WindowID, XChecker::CheckIsInvisible));
        QVERIFY (m_XChecker.check_window(MWindowID, XChecker::CheckIsInvisible));
    }

    delete m_MainWindow;
    m_MainWindow = 0;
}

/*!
 * When this test is executed the main window is already created, so this test
 * will show/hide the event eater again with the main window created and
 * registered.
 */
void
Ft_LockScreenUI::testEventEaterUIShowHideWithMainWindow ()
{
    Window WindowID;
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
    SYS_DEBUG ("*** MWindowID    = 0x%lx", MWindowID);

    createEventEaterUI ();

    SYS_DEBUG ("*** m_EventEaterUI = %p", m_EventEaterUI);
    for (int i = 0; i < 3; ++i) {
        /*
         * Showing the eventEaterUI window.
         */
        SYS_DEBUG ("***************************************************");
        SYS_DEBUG ("*** Showing eventeaterUI **************************");
        SYS_DEBUG ("***************************************************");
        m_EventEaterUI->show ();
        m_EventEaterUI->showFullScreen ();
        QTest::qWait (WMDelay);
        
        /*
         * From this point the event eater must be shown and visible.
         */
        WindowID = m_EventEaterUI->internalWinId();
        QVERIFY (m_EventEaterUI->isVisible());
        QVERIFY (m_XChecker.check_window(WindowID, XChecker::CheckIsVisible));

        m_XChecker.debug_dump_windows (WindowID);
        QTest::qWait (DelayBetweenTests);
        /*
         * Hiding the window again.
         */
        SYS_DEBUG ("***************************************************");
        SYS_DEBUG ("*** Hiding eventeaterUI ***************************");
        SYS_DEBUG ("***************************************************");
        m_EventEaterUI->hide();
        QTest::qWait (WMDelay);
        WindowID = m_EventEaterUI->internalWinId();
        SYS_DEBUG ("*** WindowID = 0x%lx", WindowID);
        QVERIFY (!m_EventEaterUI->isVisible());
        QVERIFY (m_XChecker.check_window(WindowID, XChecker::CheckIsInvisible));
    }

    delete m_MainWindow;
    m_MainWindow = 0;
}

void 
Ft_LockScreenUI::testLockScreenUIWithLocking ()
{
    QmLocks locks;
    bool    lockingSuccess, unlockingSuccess;
    Window  WindowID;

    createEventEaterUI ();
    /*
     * Locking the screen first.
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** Locking the screen ****************************");
    SYS_DEBUG ("***************************************************");
    lockingSuccess = locks.setState (QmLocks::TouchAndKeyboard, QmLocks::Locked);
    #ifndef __i386__
    QVERIFY (lockingSuccess);
    #endif
    QTest::qWait (DelayBetweenTests);

    /*
     * Then we show the lockscreenUI
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** Showing lockscreenUI **************************");
    SYS_DEBUG ("***************************************************");
    m_LockScreenUI->show ();
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

    /*
     * From this point the lock screen should be realized, shown and visible.
     */
    QVERIFY (m_LockScreenUI->m_Realized);
    QVERIFY (m_LockScreenUI->m_SceneWindow != NULL);
    QVERIFY (m_LockScreenUI->isVisible());
    WindowID = m_LockScreenUI->internalWinId();
    QVERIFY (m_XChecker.check_window(WindowID, XChecker::CheckIsVisible));
}


void
Ft_LockScreenUI::createLockScreenUI ()
{
    if (!m_LockScreenUI)
        m_LockScreenUI = new LockScreenUI ();
}

void
Ft_LockScreenUI::createEventEaterUI ()
{
    if (!m_EventEaterUI)
        m_EventEaterUI = new EventEaterUI ();
}

QTEST_APPLESS_MAIN(Ft_LockScreenUI)
