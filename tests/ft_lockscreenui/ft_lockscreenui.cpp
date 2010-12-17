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
#include "screenlockwindow.h"
#include "eventeater.h"
#include "sysuid_stub.h"
#include "unlockmissedevents.h"
#include <MApplication>
#include <MApplicationWindow>
#include <MSceneManager>
#include <MTheme>
#include <MApplicationExtensionArea>

#define DEBUG
#include "debug.h"

/*
 * We wait this much until we check if the window is actually appeared
 * disappeared from the stack of the window manager.
 */
static int WMDelay = 1000;

/*
 * This much we wait between the tests.
 */
static int DelayBetweenTests = 2500;


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
    m_LockScreenWindow = 0;
    m_MainWindow = 0;

    SYS_DEBUG ("+++ Creating application.");
    m_App = new MApplication(argc, argv);
    m_App->setQuitOnLastWindowClosed (false);

    SYS_DEBUG ("Initializing our own themes.");
    MTheme::addPixmapDirectory (themeDir, M::Recursive);
    MTheme::loadCSS (styleDir + "sysuid.css");
}

void 
Ft_LockScreenUI::cleanupTestCase()
{
    delete m_LockScreenWindow;
    delete m_EventEaterUI;
    delete m_MainWindow; 

    m_App->deleteLater ();
}

void
Ft_LockScreenUI::testEventEaterUIShowHide ()
{
#if 0
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
        QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsVisible));
        QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsFullscreen));

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
        QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsInvisible));
    }
#endif
}

/*!
 * Will create a lockscreenUI window, show and hide it several times and check
 * if the window is actually shown by the XServer.
 */
void
Ft_LockScreenUI::testLockScreenUIShowHide ()
{
#if 0
    Window WindowID;

    createLockScreenUI ();
   
    for (int n = 0; n < 2; ++n) {
        /*
         * Showing the lockscreenUI window.
         */
        SYS_DEBUG ("***************************************************");
        SYS_DEBUG ("*** Showing lockscreenUI **************************");
        SYS_DEBUG ("***************************************************");
        m_LockScreenWindow->show ();
        QTest::qWait (WMDelay);
        /*
         * From this point the lock screen should be realized, shown and visible.
         */
        QVERIFY (m_LockScreenWindow->m_Realized);
        QVERIFY (m_LockScreenWindow->m_SceneWindow != NULL);
        QVERIFY (m_LockScreenWindow->isVisible());
        WindowID = m_LockScreenWindow->internalWinId();
        QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsVisible));
        QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsFullscreen));
    
        QTest::qWait (DelayBetweenTests);
        /*
        * Hiding the window again.
        */
        SYS_DEBUG ("***************************************************");
        SYS_DEBUG ("*** Hiding lockscreenUI ***************************");
        SYS_DEBUG ("***************************************************");
        m_LockScreenWindow->hide();
        QTest::qWait (WMDelay);
        WindowID = m_LockScreenWindow->internalWinId();
        QVERIFY (!m_LockScreenWindow->isVisible());
        QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsInvisible));
    }
#endif
}

/*!
 * Again we show/hide the lockscreenUI but before we do that we create an
 * MApplicationWindow.
 */
void
Ft_LockScreenUI::testLockScreenUIShowHideWithMainWindow ()
{
#if 0
    Window WindowID;
    Window MWindowID;

    SYS_DEBUG ("Creating main window.");
    m_MainWindow = new MApplicationWindow;

    Qt::WindowFlags flags = 0;
    flags |= Qt::FramelessWindowHint;
    flags |= Qt::CustomizeWindowHint;
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
        m_LockScreenWindow->show ();
        QTest::qWait (WMDelay);
        /*
         * From this point the lock screen should be realized, shown and visible.
         */
        QVERIFY (m_LockScreenWindow->m_Realized);
        QVERIFY (m_LockScreenWindow->m_SceneWindow != NULL);
        QVERIFY (m_LockScreenWindow->isVisible());
        WindowID = m_LockScreenWindow->internalWinId();
        QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsVisible));
        QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsFullscreen));
        QVERIFY (m_XChecker.checkWindow(MWindowID, XChecker::CheckIsInvisible));
        SYS_DEBUG ("*** lockScreenUI = 0x%lx", WindowID);
    
        QTest::qWait (DelayBetweenTests);
        /*
        * Hiding the window again.
        */
        SYS_DEBUG ("***************************************************");
        SYS_DEBUG ("*** Hiding lockscreenUI ***************************");
        SYS_DEBUG ("***************************************************");
        m_LockScreenWindow->hide();
        QTest::qWait (WMDelay);
        WindowID = m_LockScreenWindow->internalWinId();
        QVERIFY (!m_LockScreenWindow->isVisible());
        QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsInvisible));
        QVERIFY (m_XChecker.checkWindow(MWindowID, XChecker::CheckIsInvisible));
    }

    delete m_MainWindow;
    m_MainWindow = 0;
#endif
}

/*!
 * When this test is executed the main window is already created, so this test
 * will show/hide the event eater again with the main window created and
 * registered.
 */
void
Ft_LockScreenUI::testEventEaterUIShowHideWithMainWindow ()
{
#if 0
    Window WindowID;
    Window MWindowID;

    SYS_DEBUG ("Creating main window.");
    m_MainWindow = new MApplicationWindow;

    Qt::WindowFlags flags = 0;
    flags |= Qt::FramelessWindowHint;
    flags |= Qt::CustomizeWindowHint;
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
        QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsVisible));
        QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsFullscreen));

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
        QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsInvisible));
    }

    delete m_MainWindow;
    m_MainWindow = 0;
#endif
}

void 
Ft_LockScreenUI::testLockScreenUIWithTSOff ()
{
#if 0
    Window  WindowID;
    /*
     * Turning off the screen first.
     */
    QVERIFY(m_XChecker.turnOffDisplay ());

    createEventEaterUI ();
    QTest::qWait (DelayBetweenTests);

    /*
     * Then we show the lockscreenUI
     */
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** Showing lockscreenUI **************************");
    SYS_DEBUG ("***************************************************");
    m_LockScreenWindow->show ();
    QTest::qWait (WMDelay);

    /*
     * Then turning on the screen again so the lockscreenUI will be visible.
     */
    QVERIFY(m_XChecker.turnOnDisplay ());
    QTest::qWait (DelayBetweenTests);

     /*
     * From this point the lock screen should be realized, shown and visible.
     */
    QVERIFY (m_LockScreenWindow->m_Realized);
    QVERIFY (m_LockScreenWindow->m_SceneWindow != NULL);
    QVERIFY (m_LockScreenWindow->isVisible());
    WindowID = m_LockScreenWindow->internalWinId();
    QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsVisible));
    QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsFullscreen));
#endif
}

/*
 * FIXME: This test is not ready yet. Should check the following bug(s):
 * 176358 -  missed events representation in notification area of lock ui is not proper in portrait orientation.
 */
#if 0
void
Ft_LockScreenUI::testMissedEvents ()
{
    Window WindowID;

    createLockScreenUI ();
   
    /*
    * Showing the lockscreenUI window.
    */
   SYS_DEBUG ("***************************************************");
   SYS_DEBUG ("*** Showing lockscreenUI **************************");
   SYS_DEBUG ("***************************************************");
   m_LockScreenWindow->show ();
   QTest::qWait (WMDelay);
   /*
    * From this point the lock screen should be realized, shown and visible.
    */
    UnlockMissedEvents &missedEvents = UnlockMissedEvents::getInstance();

    for (int i = 0; i < 5; ++i) {
        missedEvents.addNotification (
                UnlockMissedEvents::NotifyEmail,
                QString("mailmessage subject"));
        QTest::qWait (2000);
    }

    for (int i = 0; i < 5; ++i) {
        missedEvents.addNotification (
                UnlockMissedEvents::NotifySms,
                QString("sms subject"));
        QTest::qWait (500);
    }
    
    for (int i = 0; i < 3; ++i) {
        missedEvents.addNotification (
                UnlockMissedEvents::NotifyCall,
                QString("sms subject"));
        QTest::qWait (500);
    }
    
    for (int i = 0; i < 3; ++i) {
        missedEvents.addNotification (
                UnlockMissedEvents::NotifyMessage,
                QString("sms subject"));
        QTest::qWait (500);
    }
    
    for (int i = 0; i < 3; ++i) {
        missedEvents.addNotification (
                UnlockMissedEvents::NotifyOther,
                QString("sms subject"));
        QTest::qWait (500);
    }
   
   QTest::qWait (DelayBetweenTests);
   /*
   * Hiding the window again.
   */
   SYS_DEBUG ("***************************************************");
   SYS_DEBUG ("*** Hiding lockscreenUI ***************************");
   SYS_DEBUG ("***************************************************");
   m_LockScreenWindow->hide();
   QTest::qWait (WMDelay);
   WindowID = m_LockScreenWindow->internalWinId();
   QVERIFY (!m_LockScreenWindow->isVisible());
   QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsInvisible));
}
#endif

void
Ft_LockScreenUI::createLockScreenUI ()
{
    if (!m_LockScreenWindow)
        m_LockScreenWindow = new ScreenLockWindow (new MApplicationExtensionArea(""));
}

void
Ft_LockScreenUI::createEventEaterUI ()
{
    if (!m_EventEaterUI)
        m_EventEaterUI = new EventEater ();
}

QTEST_APPLESS_MAIN(Ft_LockScreenUI)
