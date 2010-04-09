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
    m_LockScreenUI = 0;

    qDebug() << "Creating application.";
    m_App = new DuiApplication(argc, argv);

    qDebug() << "Initializing our own themes.";
    DuiTheme::addPixmapDirectory (svgDir);
    DuiTheme::loadCSS (styleDir + "sysuid.css");
    DuiTheme::loadCSS (styleDir + "unlocksliderstyle.css");

    qDebug() << "Creating main window.";
    m_MainWindow = new DuiApplicationWindow;

    Qt::WindowFlags flags = 0;
    flags |= Qt::FramelessWindowHint;
    flags |= Qt::CustomizeWindowHint;
    flags |= Qt::WindowStaysOnTopHint;
    m_MainWindow->setWindowOpacity (0.0);
    m_MainWindow->setWindowFlags (flags);
}

void 
Ut_LockScreenUI::cleanupTestCase()
{
    delete m_MainWindow; 
    delete m_App;
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
    showLockScreenUI ();
    QTest::qWait (2000);
    hideLockScreenUI ();
    QTest::qWait (2000);
    
    showLockScreenUI ();
    QTest::qWait (2000);
    hideLockScreenUI ();
    QTest::qWait (2000);
}

void
Ut_LockScreenUI::showLockScreenUI ()
{
    qDebug() << "Showing LockScreenUI";
    if (!m_LockScreenUI)
        m_LockScreenUI = new LockScreenUI ();

    m_MainWindow->show ();
    m_MainWindow->raise ();

    m_LockScreenUI->setOpacity (1.0);
    m_LockScreenUI->setActive (true);

    m_MainWindow->sceneManager()->showWindowNow (m_LockScreenUI);
}

void
Ut_LockScreenUI::hideLockScreenUI ()
{
    QVERIFY (m_LockScreenUI != 0);

    qDebug() << "Hiding LockScreenUI";
    m_LockScreenUI->hide();
    m_MainWindow->hide ();
}

QTEST_APPLESS_MAIN(Ut_LockScreenUI)
