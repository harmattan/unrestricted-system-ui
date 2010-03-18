/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "ut_lockscreenui.h"

//#include "lockscreenui.h"

#include <DuiApplication>
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
}

void Ut_LockScreenUI::cleanupTestCase()
{
    delete app;
}


QTEST_APPLESS_MAIN(Ut_LockScreenUI)
