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

// X stubs to avoid crashes
void XSetWMProperties(Display *, Window, XTextProperty *, XTextProperty *, char **, int, XSizeHints *, XWMHints *, XClassHint *)
{
}

bool sceneWindowAppearCalled = false;
void MSceneWindow::appear(MWindow *window, MSceneWindow::DeletionPolicy policy)
{
    Q_UNUSED(window);
    Q_UNUSED(policy);
    sceneWindowAppearCalled = true;
}

bool sceneWindowDisappearCalled = false;
void MSceneWindow::disappear()
{
    sceneWindowDisappearCalled = true;
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
    sceneWindowAppearCalled = false;
    sceneWindowDisappearCalled = false;
    delete lockScreenWindow;
}

void Ut_LockScreenWindow::testWhenWindowAppearIsCalledLockScreenIsAppeared()
{
    lockScreenWindow->appear();
    QCOMPARE(sceneWindowAppearCalled, true);
}

void Ut_LockScreenWindow::testWhenWindowDisappearIsCalledLockScreenIsDisappeared()
{
    lockScreenWindow->disappear();
    QCOMPARE(sceneWindowDisappearCalled, true);
}
void Ut_LockScreenWindow::testWhenDataAndTimeIsUpdatedLockScreenDateAndTimeIsUpdated()
{
    lockScreenWindow->updateDateTime();
    QCOMPARE(gLockScreenStub->stubCallCount("updateDateTime"), 1);
}

void Ut_LockScreenWindow::testWhenWindowIsCreatedUnlockedSignalFromLockScreenIsChainedToUnlockedSignal()
{
    bool result = disconnect(lockScreenWindow->lockScreen, SIGNAL(unlocked()), lockScreenWindow, SIGNAL(unlocked()));
    QCOMPARE(result, true);
}

QTEST_APPLESS_MAIN(Ut_LockScreenWindow)
