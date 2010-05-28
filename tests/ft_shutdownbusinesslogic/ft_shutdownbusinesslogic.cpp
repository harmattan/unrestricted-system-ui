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

#include "ft_shutdownbusinesslogic.h"
#include "shutdownbusinesslogic.h"
#include "shutdownui.h"
#include "sysuid_stub.h"

#include <qmsystem/qmsystemstate.h>

#include <MApplication>
#include <MApplicationWindow>
#include <MSceneManager>
#include <MTheme>
#include <MLabel>
#include <MImageWidget>

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
//static int DelayBetweenTests = 10000;

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
    m_MainWindow = 0;

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
    if (m_MainWindow) 
        delete m_MainWindow; 

    delete m_App;
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
    logic->systemStateChanged (QmSystemState::Shutdown);
    QTest::qWait (WMDelay);

    /*
     * This time the window has to be there.
     */
    QVERIFY (logic->m_Ui != 0);
    WindowID = logic->m_Ui->internalWinId();
    SYS_DEBUG ("*** WindowID = 0x%lx", WindowID);
    QVERIFY (m_XChecker.check_window(WindowID, XChecker::CheckIsVisible));
    m_XChecker.debug_dump_windows (WindowID);

    QTest::qWait (3000);

    delete logic;
}

QTEST_APPLESS_MAIN(Ft_ShutdownBusinessLogic)



