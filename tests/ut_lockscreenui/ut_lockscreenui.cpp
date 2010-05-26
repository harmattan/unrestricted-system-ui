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
#include <MGConfItem>
#include <MTheme>

#define GCONF_BG_LANDSCAPE \
    "/desktop/meego/background/landscape/picture_filename"
#define GCONF_BG_PORTRAIT \
    "/desktop/meego/background/portrait/picture_filename"


#define DEBUG
#include "../../src/debug.h"

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
    m_EventEaterUI = 0;
    m_LockScreenUI = 0;
    m_MainWindow = 0;

    SYS_DEBUG ("+++ Creating application.");
    m_App = new MApplication(argc, argv);
    m_App->setQuitOnLastWindowClosed (false);

    SYS_DEBUG ("+++ Initializing our own themes.");
    MTheme::addPixmapDirectory (themeDir, M::Recursive);
    MTheme::loadCSS (styleDir + "sysuid.css");
    MTheme::loadCSS (styleDir + "unlockscreen.css");
}

void 
Ut_LockScreenUI::cleanupTestCase()
{
    if (m_LockScreenUI)
        delete m_LockScreenUI;

    if (m_EventEaterUI)
        delete m_EventEaterUI;

    if (m_MainWindow)
        delete m_MainWindow; 

    delete m_App;
}

void 
Ut_LockScreenUI::testLockScreenWindow ()
{
    LockScreenWindow *window;

    window = new LockScreenWindow;

    /*
     * Checking if the window is watching the right gconf keys.
     */
    QVERIFY (window->m_confBgLandscape != NULL);
    QVERIFY (window->m_confBgPortrait != NULL);
    QVERIFY (window->m_confBgLandscape->key() == GCONF_BG_LANDSCAPE);
    QVERIFY (window->m_confBgPortrait->key() == GCONF_BG_PORTRAIT);

    /*
     * Checking if the background images are loaded with the right size.
     */
    SYS_DEBUG ("*** m_bgLandscape size = %dx%d", 
            window->m_bgLandscape.width(),
            window->m_bgLandscape.height());
    SYS_DEBUG ("*** m_bgPortrait  size = %dx%d", 
            window->m_bgPortrait.width(),
            window->m_bgPortrait.height());

    QVERIFY (window->m_bgLandscape.width() == 864);
    QVERIFY (window->m_bgLandscape.height() == 480);
    
    QVERIFY (window->m_bgPortrait.width() == 480);
    QVERIFY (window->m_bgPortrait.height() == 864);

    delete window;
}

void
Ut_LockScreenUI::createLockScreenUI ()
{
    if (!m_LockScreenUI)
        m_LockScreenUI = new LockScreenUI ();
}

void
Ut_LockScreenUI::createEventEaterUI ()
{
    if (!m_EventEaterUI)
        m_EventEaterUI = new EventEaterUI ();
}

QTEST_APPLESS_MAIN(Ut_LockScreenUI)
