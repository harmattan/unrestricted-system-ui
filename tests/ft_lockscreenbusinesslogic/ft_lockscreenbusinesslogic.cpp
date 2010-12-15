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
#include "screenlockbusinesslogic.h"
#include "screenlockwindow.h"
#include "eventeater.h"
#include "lockscreen.h"
#include "sysuid_stub.h"

#include <MApplication>
#include <MApplicationWindow>
#include <MSceneManager>
#include <MTheme>

#ifdef HAVE_QMSYSTEM
#include <qmlocks.h>
#endif

#define DEBUG
#include "debug.h"

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
    m_LockScreenBusinessLogic = new ScreenLockBusinessLogic;
}

void
Ft_LockScreenBusinessLogic::cleanup()
{
    delete m_LockScreenBusinessLogic;
    m_LockScreenBusinessLogic = 0;
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

    m_App->deleteLater ();
}

void
Ft_LockScreenBusinessLogic::testLockScreenBusinessLogic ()
{
    Window LockScreenUIWindowID;
    Window EventEaterWindowID;

    /*
     * Once it is constructed the two UI elements should not be there.
     */
    QVERIFY (m_LockScreenBusinessLogic->screenLockWindow == NULL);
    QVERIFY (m_LockScreenBusinessLogic->eventEaterWindow == NULL);

    /*
     *
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** calling toggleEventEater (true) ***************");
    SYS_DEBUG ("***************************************************");
    m_LockScreenBusinessLogic->toggleEventEater (true);
    QTest::qWait (WMDelay);
    EventEaterWindowID = m_LockScreenBusinessLogic->eventEaterWindow->window;

    QVERIFY (m_XChecker.checkWindow (
                EventEaterWindowID,
                XChecker::CheckIsVisible));
    QVERIFY (m_XChecker.checkWindow (
                EventEaterWindowID,
                XChecker::CheckIsFullscreen));

    QTest::qWait (DelayBetweenTests);

    /*
     *
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** calling toggleEventEater (false) **************");
    SYS_DEBUG ("***************************************************");
    m_LockScreenBusinessLogic->toggleEventEater (false);
    QTest::qWait (WMDelay);
    EventEaterWindowID = m_LockScreenBusinessLogic->eventEaterWindow->window;

    QVERIFY (m_XChecker.checkWindow(
                EventEaterWindowID,
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
    LockScreenUIWindowID = m_LockScreenBusinessLogic->screenLockWindow->internalWinId();

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
    LockScreenUIWindowID = m_LockScreenBusinessLogic->screenLockWindow->internalWinId();

    QVERIFY (m_XChecker.checkWindow(
                LockScreenUIWindowID,
                XChecker::CheckIsInvisible));

    QTest::qWait (DelayBetweenTests);
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

    /*
     * Once it is constructed the two UI elements should not be there.
     */
    QVERIFY (m_LockScreenBusinessLogic->screenLockWindow == NULL);
    QVERIFY (m_LockScreenBusinessLogic->eventEaterWindow == NULL);

    /*
     *
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** calling toggleEventEater (true) ***************");
    SYS_DEBUG ("***************************************************");
    m_LockScreenBusinessLogic->toggleEventEater (true);
    QTest::qWait (WMDelay);
    EventEaterWindowID = m_LockScreenBusinessLogic->eventEaterWindow->window;

    QVERIFY (m_XChecker.checkWindow(
                EventEaterWindowID,
                XChecker::CheckIsVisible));
    QVERIFY (m_XChecker.checkWindow(
                EventEaterWindowID,
                XChecker::CheckIsFullscreen));

    QTest::qWait (DelayBetweenTests);

    /*
     *
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** calling toggleEventEater (false) **************");
    SYS_DEBUG ("***************************************************");
    m_LockScreenBusinessLogic->toggleEventEater (false);
    QTest::qWait (WMDelay);
    EventEaterWindowID = m_LockScreenBusinessLogic->eventEaterWindow->window;

    QVERIFY (m_XChecker.checkWindow(
                EventEaterWindowID,
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
    LockScreenUIWindowID = m_LockScreenBusinessLogic->screenLockWindow->internalWinId();

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
    LockScreenUIWindowID = m_LockScreenBusinessLogic->screenLockWindow->internalWinId();

    QVERIFY (m_XChecker.checkWindow(
                LockScreenUIWindowID,
                XChecker::CheckIsInvisible));

    QTest::qWait (DelayBetweenTests);

    delete m_MainWindow;
    m_MainWindow = 0;
}

#ifdef HAVE_QMSYSTEM
/*!
 * This test will try to emulate the situation when we lock the screen (touch
 * screen lock becomes black) show the lockscreenui and then we turn on the
 * touch screen again.
 */
void
Ft_LockScreenBusinessLogic::testLockScreenBusinessLogicWithLocking ()
{
    MeeGo::QmLocks locks;
    Window  LockScreenUIWindowID;

    /*
     * Locking the screen first.
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** Locking the screen ****************************");
    SYS_DEBUG ("***************************************************");
    locks.setState (MeeGo::QmLocks::TouchAndKeyboard, MeeGo::QmLocks::Locked);
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
    locks.setState (MeeGo::QmLocks::TouchAndKeyboard, MeeGo::QmLocks::Unlocked);
    QTest::qWait (WMDelay);

    /*
     * Now that the screen is on the lockscreenUI should be up and visible.
     */
    LockScreenUIWindowID = m_LockScreenBusinessLogic->screenLockWindow->internalWinId();

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
    LockScreenUIWindowID = m_LockScreenBusinessLogic->screenLockWindow->internalWinId();

    QVERIFY (m_XChecker.checkWindow(
                LockScreenUIWindowID,
                XChecker::CheckIsInvisible));
}
#endif

QTEST_APPLESS_MAIN(Ft_LockScreenBusinessLogic)

