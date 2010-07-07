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

#include "ft_shutdownbusinesslogic.h"
#include "shutdownbusinesslogic.h"
#include "shutdownui.h"
#include "sysuid_stub.h"

#ifdef HAVE_QMSYSTEM
#include <qmsystemstate.h>
#endif

#include <MApplication>
#include <MApplicationWindow>
#include <MSceneManager>
#include <MTheme>
#include <MLabel>
#include <MImageWidget>

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
Ft_ShutdownBusinessLogic::init()
{
}

void 
Ft_ShutdownBusinessLogic::cleanup()
{
}

int   argc = 1;
char *argv[] = {
    (char *) "./ft_shutdownbusinesslogic",
    NULL };

const QString themeDir = "/usr/share/themes/base/meegotouch/sysuid/";
const QString styleDir = themeDir + "style/";

void 
Ft_ShutdownBusinessLogic::initTestCase()
{
    SYS_DEBUG ("+++ Creating application.");
    m_App = new MApplication (argc, argv);
    m_App->setQuitOnLastWindowClosed (false);

    SYS_DEBUG ("Initializing our own themes.");
    MTheme::addPixmapDirectory (themeDir, M::Recursive);
    MTheme::loadCSS (styleDir + "sysuid.css");
    MTheme::loadCSS (styleDir + "unlockscreen.css");
}

void 
Ft_ShutdownBusinessLogic::cleanupTestCase()
{
    SYS_DEBUG ("");
    m_App->deleteLater ();
}

#ifdef HAVE_QMSYSTEM
void 
Ft_ShutdownBusinessLogic::testThermalState ()
{
    ShutdownBusinessLogic *logic;
    logic = new ShutdownBusinessLogic;

    SYS_DEBUG ("*********************************************");
    SYS_DEBUG ("*** Simulating ThermalStateFatal state ******");
    SYS_DEBUG ("*********************************************");
    logic->systemStateChanged (Maemo::QmSystemState::ThermalStateFatal);
    QTest::qWait (WMDelay);
    
    /*
     * FIXME: We should check here if the notification is published but it seems
     * that m_XChecker is failed to get the notification data. It might be a bug
     * in Meego.
     */

    QTest::qWait (DelayBetweenTests);
    
    SYS_DEBUG ("*********************************************");
    SYS_DEBUG ("*** Destroying ShutdownBusinessLogic ********");
    SYS_DEBUG ("*********************************************");
    delete logic;
}

void 
Ft_ShutdownBusinessLogic::testShutDownDenied ()
{
    ShutdownBusinessLogic *logic;
    logic = new ShutdownBusinessLogic;

    SYS_DEBUG ("*********************************************");
    SYS_DEBUG ("*** Simulating ShutdownDeniedUSB ************");
    SYS_DEBUG ("*********************************************");
    logic->systemStateChanged (Maemo::QmSystemState::ShutdownDeniedUSB);
    QTest::qWait (WMDelay);
    
    /*
     * FIXME: We should check here if the notification is published but it seems
     * that m_XChecker is failed to get the notification data. It might be a bug
     * in Meego.
     */

    QTest::qWait (DelayBetweenTests);
    
    SYS_DEBUG ("*********************************************");
    SYS_DEBUG ("*** Destroying ShutdownBusinessLogic ********");
    SYS_DEBUG ("*********************************************");
    delete logic;
}

void 
Ft_ShutdownBusinessLogic::testBatteryStateEmpty ()
{
    ShutdownBusinessLogic *logic;
    logic = new ShutdownBusinessLogic;

    SYS_DEBUG ("*********************************************");
    SYS_DEBUG ("*** Simulating BatteryStateEmpty ************");
    SYS_DEBUG ("*********************************************");
    logic->systemStateChanged (Maemo::QmSystemState::BatteryStateEmpty);
    QTest::qWait (WMDelay);
    
    /*
     * FIXME: We should check here if the notification is published but it seems
     * that m_XChecker is failed to get the notification data. It might be a bug
     * in Meego.
     */

    QTest::qWait (DelayBetweenTests);
    
    SYS_DEBUG ("*********************************************");
    SYS_DEBUG ("*** Destroying ShutdownBusinessLogic ********");
    SYS_DEBUG ("*********************************************");
    delete logic;
}

void 
Ft_ShutdownBusinessLogic::testShutDown ()
{
    ShutdownBusinessLogic *logic;
    Window                 WindowID;
    logic = new ShutdownBusinessLogic;

    /*
     * If we change the system to shutdown, the shutdown full screen window must
     * be shown.
     */
    logic->systemStateChanged (Maemo::QmSystemState::Shutdown);
    QTest::qWait (WMDelay);

    /*
     * This time the window has to be there.
     */
    QVERIFY (logic->m_Ui != 0);
    WindowID = logic->m_Ui->internalWinId();
    SYS_DEBUG ("*** WindowID = 0x%lx", WindowID);
    QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsVisible));
    QVERIFY (m_XChecker.checkWindow(WindowID, XChecker::CheckIsFullscreen));

    QTest::qWait (5000);

    SYS_DEBUG ("*********************************************");
    SYS_DEBUG ("*** Destroying ShutdownBusinessLogic ********");
    SYS_DEBUG ("*********************************************");
    delete logic;

    /*
     * We want to be sure we leave the screen on...
     */
    QTest::qWait (WMDelay);
    QVERIFY(m_XChecker.turnOnDisplay ());
}
#endif

QTEST_APPLESS_MAIN(Ft_ShutdownBusinessLogic)
