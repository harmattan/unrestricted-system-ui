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

#include "ft_shutdownui.h"
#include "shutdownui.h"
#include "sysuid_stub.h"

#include <MApplication>
#include <MApplicationWindow>
#include <MSceneManager>
#include <MTheme>
#include <MLabel>
#include <MStylableWidget>

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
//static int DelayBetweenTests = 10000;

void 
Ft_ShutdownUI::init()
{
}

void 
Ft_ShutdownUI::cleanup()
{
}

int   argc = 1;
char *argv[] = {
    (char *) "./Ft_ShutdownUI",
    NULL };

const QString themeDir = "/usr/share/themes/base/meegotouch/sysuid/";
const QString styleDir = themeDir + "style/";

void 
Ft_ShutdownUI::initTestCase()
{
    m_MainWindow = 0;
    m_ShutDownUI = 0;

    SYS_DEBUG ("+++ Creating application.");
    m_App = new MApplication (argc, argv);
    m_App->setQuitOnLastWindowClosed (false);

    SYS_DEBUG ("Initializing our own themes.");
    MTheme::addPixmapDirectory (themeDir, M::Recursive);
    MTheme::loadCSS (styleDir + "sysuid.css");
}

void 
Ft_ShutdownUI::cleanupTestCase()
{
    if (m_ShutDownUI) 
        delete m_ShutDownUI;

    if (m_MainWindow) 
        delete m_MainWindow; 

    m_App->deleteLater ();
}

/*
 * We had some crashes because the delayed initialization (realization). It is
 * tested here.
 */
void
Ft_ShutdownUI::testConstructDestruct ()
{
    m_ShutDownUI = new ShutdownUI;
    QVERIFY (!m_ShutDownUI->realized);
    delete m_ShutDownUI;
    m_ShutDownUI = 0;
}

void 
Ft_ShutdownUI::testShutdownUIShowHide ()
{
    Window WindowID;

    SYS_DEBUG ("");

    m_ShutDownUI = new ShutdownUI;
    
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** Showing shutdownUI ****************************");
    SYS_DEBUG ("***************************************************");
    m_ShutDownUI->showWindow (
            "ft_shutdownUI functional test",
            "testShutdownUIShowHide() function",
            2000);
    QTest::qWait (WMDelay);

    /*
     * After the WMDelay the window has to be visible.
     */
    WindowID = m_ShutDownUI->internalWinId();
    QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsVisible));
    QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsFullscreen));
    m_XChecker.debug_dump_windows (WindowID);

    /*
     * Also the labels has to be visible. 
     */
    QVERIFY (m_ShutDownUI->label1 != 0 && 
            m_ShutDownUI->label1->isVisible());
    QVERIFY (m_ShutDownUI->label2 != 0 && 
            m_ShutDownUI->label2->isVisible());

    /*
     * Then we wait a little bit more than the reguiested delay while the labels
     * disappear and the image is shown.
     */
    QTest::qWait (2500);
    QVERIFY (m_ShutDownUI->logo != 0 && 
            m_ShutDownUI->logo->isVisible());

    /*
     * A bit more to see the screen dimming.
     */
    QTest::qWait (3000);

    /*
     * Then we hide the window and so we can see if it is really gone.
     */
    m_ShutDownUI->hide ();
    QTest::qWait (WMDelay);
    QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsInvisible));

    /*
     * Cleaning up.
     */
    delete m_ShutDownUI;
    m_ShutDownUI = 0;

    /*
     * We want to be sure we leave the screen on...
     */
    QTest::qWait (WMDelay);
    QVERIFY(m_XChecker.turnOnDisplay ());
}

QTEST_APPLESS_MAIN(Ft_ShutdownUI)
