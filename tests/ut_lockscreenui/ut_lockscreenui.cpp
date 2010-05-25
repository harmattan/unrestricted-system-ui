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

#include "ut_lockscreenui.h"
#include "xchecker.h"
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
static int WMDelay = 200;


void Ut_LockScreenUI::init()
{
}

void Ut_LockScreenUI::cleanup()
{
}


int   argc = 1;
char *argv[] = {
    (char *) "./ut_lockscreenui",
    NULL };

const QString themeDir = "/usr/share/themes/base/meegotouch/sysuid/";
const QString styleDir = themeDir + "style/";

void Ut_LockScreenUI::initTestCase()
{
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
Ut_LockScreenUI::cleanupTestCase()
{
    delete m_MainWindow; 
    delete m_App;
}

/*!
 * Will create a lockscreenUI window, show and hide it several times and check
 * if the window is actually shown by the XServer.
 */
void
Ut_LockScreenUI::testLockScreenUIShowHide ()
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
    
        QTest::qWait (500);
        m_XChecker.debug_dump_windows ();
        /*
        * Hiding the window again.
        */
        SYS_DEBUG ("***************************************************");
        SYS_DEBUG ("*** Hiding lockscreenUI ***************************");
        SYS_DEBUG ("***************************************************");
        m_LockScreenUI->hide();
        QTest::qWait (WMDelay);
        QVERIFY (!m_LockScreenUI->isVisible());
        QVERIFY (m_XChecker.check_window(WindowID, XChecker::CheckIsInvisible));
    }
}

/*!
 * Again we show/hide the lockscreenUI but before we do that we create an
 * MApplicationWindow.
 */
void
Ut_LockScreenUI::testLockScreenUIShowHideWithMainWindow ()
{
    Window WindowID;
    Window MWindowID;

    createLockScreenUI ();

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
    
        QTest::qWait (500);
        m_XChecker.debug_dump_windows ();
        /*
        * Hiding the window again.
        */
        SYS_DEBUG ("***************************************************");
        SYS_DEBUG ("*** Hiding lockscreenUI ***************************");
        SYS_DEBUG ("***************************************************");
        m_LockScreenUI->hide();
        QTest::qWait (WMDelay);
        QVERIFY (!m_LockScreenUI->isVisible());
        QVERIFY (m_XChecker.check_window(WindowID, XChecker::CheckIsInvisible));
        QVERIFY (m_XChecker.check_window(MWindowID, XChecker::CheckIsInvisible));
    }
}

void
Ut_LockScreenUI::createLockScreenUI ()
{
    if (!m_LockScreenUI)
        m_LockScreenUI = new LockScreenUI ();
}

QTEST_APPLESS_MAIN(Ut_LockScreenUI)
