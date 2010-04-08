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
#include <DuiTheme>

void Ut_LockScreenUI::init()
{
}

void Ut_LockScreenUI::cleanup()
{
}

DuiApplication *app;
void Ut_LockScreenUI::initTestCase()
{
    int argc = 1;
    char* app_name = (char*) "./Ut_LockScreenUI";
    app = new DuiApplication(argc, &app_name);

    DuiApplicationWindow *win = new DuiApplicationWindow;
    Qt::WindowFlags flags = 0;
    flags |= Qt::FramelessWindowHint;
    flags |= Qt::CustomizeWindowHint;
    flags |= Qt::WindowStaysOnTopHint;
    win->setWindowOpacity (0.0);
    win->setWindowFlags (flags);
}

void 
Ut_LockScreenUI::cleanupTestCase()
{
    delete app;
}

void
Ut_LockScreenUI::testLockScreenUI ()
{
    LockScreenUI *lockScreenUI;

    lockScreenUI = new LockScreenUI;

    DuiApplication::activeApplicationWindow ()->show ();
    DuiApplication::activeApplicationWindow ()->raise ();

    lockScreenUI->setOpacity (1.0);
    lockScreenUI->appearNow ();
    lockScreenUI->setActive (true);
 
    QTest::qWait (5000);
}

QTEST_APPLESS_MAIN(Ut_LockScreenUI)
