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
/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "ut_lockscreenui.h"
#include "lockscreenui.h"
#include "sysuid_stub.h"

#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiSceneManager>
#include <DuiTheme>

void Ut_LockScreenUI::init()
{
}

void Ut_LockScreenUI::cleanup()
{
}


int   argc = 1;
char *argv[] = {
    "./ut_lockscreenui", 
    NULL };

const QString themeDir = "/usr/share/themes/base/dui/sysuid/";
const QString styleDir = themeDir + "style/";
const QString svgDir = themeDir + "svg/";

void Ut_LockScreenUI::initTestCase()
{
    qDebug() << "Creating application.";
    app = new DuiApplication(argc, argv);

    qDebug() << "Initializing our own themes.";
    DuiTheme::addPixmapDirectory (svgDir);
    DuiTheme::loadCSS (styleDir + "sysuid.css");
    DuiTheme::loadCSS (styleDir + "unlocksliderstyle.css");

    qDebug() << "Creating main window.";
    m_MainWindow = new DuiApplicationWindow;

#if 0
    Qt::WindowFlags flags = 0;
    flags |= Qt::FramelessWindowHint;
    flags |= Qt::CustomizeWindowHint;
    flags |= Qt::WindowStaysOnTopHint;
    m_MainWindow->setWindowOpacity (0.0);
    m_MainWindow->setWindowFlags (flags);
#endif
}

void 
Ut_LockScreenUI::cleanupTestCase()
{
    //delete m_LockScreenUI;
    delete m_MainWindow; 
    delete app;
}

void
Ut_LockScreenUI::testLockScreenBusinessLogic ()
{
}

/*!
 * This function will do the basic testing of the lockscreenui. It seems, that
 * we are not checking anything, but actually it is important to see if the test
 * program crashes or not. So consider this a 'compiled' and 'not crashed' test
 * case.
 */
void
Ut_LockScreenUI::testLockScreenUI ()
{
    m_LockScreenUI = new LockScreenUI ();

    DuiApplication::activeApplicationWindow ()->show ();
    //DuiApplication::activeApplicationWindow ()->raise ();

    //m_LockScreenUI->createContent ();
    //m_LockScreenUI->setOpacity (1.0);
    //m_LockScreenUI->show();
    //m_LockScreenUI->setActive (true);
    DuiApplication::activeApplicationWindow()->sceneManager()->showWindowNow(
    		    m_LockScreenUI);
    QTest::qWait (5000);
}

QTEST_APPLESS_MAIN(Ut_LockScreenUI)
