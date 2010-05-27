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

#include "ft_shutdownui.h"
#include "shutdownui.h"
#include "shutdownui.h"
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
static int DelayBetweenTests = 10000;

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
    m_App = new MApplication(argc, argv);
    m_App->setQuitOnLastWindowClosed (false);

    SYS_DEBUG ("Initializing our own themes.");
    MTheme::addPixmapDirectory (themeDir, M::Recursive);
    MTheme::loadCSS (styleDir + "sysuid.css");
    MTheme::loadCSS (styleDir + "unlockscreen.css");

    m_ShutDownUI = new ShutdownUI;
}

void 
Ft_ShutdownUI::cleanupTestCase()
{
    if (m_MainWindow)
        delete m_MainWindow; 

    delete m_App;
}

void 
Ft_ShutdownUI::testShutdownUIShowHide ()
{
    Window WindowID;

    SYS_DEBUG ("");
    
    SYS_DEBUG ("***************************************************");
    SYS_DEBUG ("*** Showing shutdownUI ****************************");
    SYS_DEBUG ("***************************************************");

    m_ShutDownUI->showWindow (
            "ft_shutdownUI functional test",
            "testShutdownUIShowHide() function",
            500);
    QTest::qWait (WMDelay);

    WindowID = m_ShutDownUI->internalWinId();
    m_XChecker.debug_dump_windows (WindowID);

    QTest::qWait (5000);
}

QTEST_APPLESS_MAIN(Ft_ShutdownUI)


