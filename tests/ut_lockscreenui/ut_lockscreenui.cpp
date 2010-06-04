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

static int normalDelay = 200;

#define DEBUG
#include "../../src/debug.h"

/*********************************************************************************
 * The helper class to watch the signals.
 */
LockScreenUIEventSink::LockScreenUIEventSink() :
    m_OneInputCame (false)
{
}

void
LockScreenUIEventSink::OneInput ()
{
    m_OneInputCame = true;
}

/*********************************************************************************
 * The Ut_LockScreenUI implements the unit tests.
 */
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

/*
 * The LockScreenWindow is a simple window that shows two sperate images, one
 * for portrait and one for landscape. The images are set by using a pair of
 * GConf keys and a wired in default. Running the test on a clean device the
 * GConf items are probable empty, but the default image should be loaded with
 * the right size.
 */
void 
Ut_LockScreenUI::testLockScreenWindow ()
{
    LockScreenWindow *window;

    window = new LockScreenWindow (0, 0, 0);

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
Ut_LockScreenUI::testLockScreenUI ()
{
    Q_ASSERT (m_LockScreenUI == 0);
    
    /*
     * When the lockscreenUI is created it just delays its realize function. We
     * wait a small amount of time then we chack that the internal widgets are
     * created. For this delayed realization the widget does not need to be
     * shown, so the widget will be shown fast even at the first time.
     */
    m_LockScreenUI = new LockScreenUI;
    QTest::qWait (normalDelay);

    QVERIFY (m_LockScreenUI->m_Realized);
    QVERIFY (m_LockScreenUI->m_policy != NULL);
    QVERIFY (m_LockScreenUI->m_LockLiftArea != NULL);
    QVERIFY (m_LockScreenUI->m_LockLandArea != NULL);
    QVERIFY (m_LockScreenUI->m_SceneWindow != NULL);

    delete m_LockScreenUI;
    m_LockScreenUI = 0;
}

void
Ut_LockScreenUI::testEventEaterUI ()
{
    bool connectSuccess;

    /*
     * We need a new event eater widget.
     */
    Q_ASSERT (m_EventEaterUI == 0);
    m_EventEaterUI = new EventEaterUI;
    
    /*
     * We test if we can connect to the OneInput() signal.
     */
    connectSuccess = connect (m_EventEaterUI, SIGNAL(OneInput()),
            &m_EventSink, SLOT(OneInput()));
    QVERIFY (connectSuccess);
    
    /*
     * Let's see if the event is actually generates a signal. 
     */
    m_EventSink.m_OneInputCame = false;
    m_EventEaterUI->mousePressEvent ((QMouseEvent *)NULL);
    QVERIFY (m_EventSink.m_OneInputCame);
    
    m_EventSink.m_OneInputCame = false;
    m_EventEaterUI->mouseReleaseEvent ((QMouseEvent *)NULL);
    QVERIFY (m_EventSink.m_OneInputCame);

    delete m_EventEaterUI;
    m_EventEaterUI = 0;
}

QTEST_APPLESS_MAIN(Ut_LockScreenUI)
