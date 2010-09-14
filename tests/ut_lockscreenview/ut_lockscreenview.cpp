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
****************************************************************************/

#include <MApplication>
#include "ut_lockscreenview.h"

void Ut_LockScreenView::init()
{
}

void Ut_LockScreenView::cleanup()
{
}

void Ut_LockScreenView::initTestCase()
{
    int   argc = 1;
    static char *app_name = (char *)"./ut_lockscreenview";
    app = new MApplication(argc, &app_name);
    lockScreenView = new LockScreenView(&controller);
}

void Ut_LockScreenView::cleanupTestCase()
{
}

QTEST_APPLESS_MAIN(Ut_LockScreenView)
