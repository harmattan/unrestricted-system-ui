/****************************************************************************
**
** Copyright(C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation(directui@nokia.com)
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

#include <MApplication>
#include "ut_lockscreenwindow.h"
#include "lockscreenwindow.h"
#include "lockscreen_stub.h"
#include <X11/Xutil.h>
#include "x11wrapper_stub.h"

// X stubs to avoid crashes
void XSetWMProperties(Display *, Window, XTextProperty *, XTextProperty *, char **, int, XSizeHints *, XWMHints *, XClassHint *)
{
}

MWindow* appearedWindow = NULL;
void MSceneWindow::appear(MWindow* window, MSceneWindow::DeletionPolicy policy)
{
    appearedWindow = window;
    Q_UNUSED(policy);
}

void Ut_LockScreenWindow::initTestCase()
{
    int   argc = 1;
    char *argv[] = {(char *) "./Ut_LockScreenWindow", NULL };
    app = new MApplication(argc, argv);
}

void Ut_LockScreenWindow::cleanupTestCase()
{
    delete app;
}

void Ut_LockScreenWindow::init()
{
    lockScreenWindow = new LockScreenWindow;
}

void Ut_LockScreenWindow::cleanup()
{
    delete lockScreenWindow;
    appearedWindow = NULL;
}

void Ut_LockScreenWindow::testWhenWindowIsCreatedUnlockedSignalFromLockScreenIsChainedToUnlockedSignal()
{
    bool result = disconnect(lockScreenWindow->lockScreen, SIGNAL(unlocked()), lockScreenWindow, SIGNAL(unlocked()));
    QCOMPARE(result, true);
}

void Ut_LockScreenWindow::testWhenWindowIsCreatedLockScreenAppears()
{
    QCOMPARE(appearedWindow, lockScreenWindow);
}

QTEST_APPLESS_MAIN(Ut_LockScreenWindow)
