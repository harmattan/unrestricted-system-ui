/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
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

#include <QtTest/QtTest>
#include <MApplication>
#include "statusindicatormenubusinesslogic.h"
#include "statusindicatormenuwindow_stub.h"
#include "closeeventeater_stub.h"
#include "ut_statusindicatormenubusinesslogic.h"

void Ut_StatusIndicatorMenuBusinessLogic::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_statusindicatormenubusinesslogic" };
    app = new MApplication(argc, app_name);
}

void Ut_StatusIndicatorMenuBusinessLogic::cleanupTestCase()
{
    delete app;
}

void Ut_StatusIndicatorMenuBusinessLogic::init()
{
    m_subject = new StatusIndicatorMenuBusinessLogic();
}

void Ut_StatusIndicatorMenuBusinessLogic::cleanup()
{
    delete m_subject;
}

void Ut_StatusIndicatorMenuBusinessLogic::testShowStatusIndicatorMenu()
{
    // There should not be a window by default
    QCOMPARE(m_subject->statusIndicatorMenuWindow, (StatusIndicatorMenuWindow *)NULL);

    // Calling for the first time should create the window and make the window visible
    m_subject->showStatusIndicatorMenu();
    QVERIFY(m_subject->statusIndicatorMenuWindow != NULL);
    QVERIFY(disconnect(m_subject->statusIndicatorMenuWindow, SIGNAL(visibilityChanged(bool)), m_subject, SIGNAL(statusIndicatorMenuVisibilityChanged(bool))));
    QCOMPARE(gStatusIndicatorMenuWindowStub->stubCallCount("StatusIndicatorMenuWindow"), 1);
    QCOMPARE(gStatusIndicatorMenuWindowStub->stubCallCount("makeVisible"), 1);

    // Calling again should not create a new window but make the window visible
    m_subject->showStatusIndicatorMenu();
    QCOMPARE(gStatusIndicatorMenuWindowStub->stubCallCount("StatusIndicatorMenuWindow"), 1);
    QCOMPARE(gStatusIndicatorMenuWindowStub->stubCallCount("makeVisible"), 2);
}

void Ut_StatusIndicatorMenuBusinessLogic::testIsStatusIndicatorMenuVisible()
{
    // Window should not be visible if it doesn't exist
    QCOMPARE(m_subject->isStatusIndicatorMenuVisible(), false);

    // When the window exists its visibility should be reflected
    m_subject->showStatusIndicatorMenu();
    m_subject->statusIndicatorMenuWindow->show();
    QCOMPARE(m_subject->isStatusIndicatorMenuVisible(), true);
    m_subject->statusIndicatorMenuWindow->hide();
    QCOMPARE(m_subject->isStatusIndicatorMenuVisible(), false);
}

QTEST_APPLESS_MAIN(Ut_StatusIndicatorMenuBusinessLogic)
