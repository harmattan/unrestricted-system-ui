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
