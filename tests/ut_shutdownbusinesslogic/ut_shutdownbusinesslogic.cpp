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

#include "ut_shutdownbusinesslogic.h"
#include "shutdownbusinesslogic.h"
#include <signal.h>
#include <MApplication>
#include <MWindow>
#include <MNotification>

void restoreSignalHandlers()
{
}

// Stubbed version of th ShutdownUI class.
ShutdownUI::ShutdownUI()
{
    m_Delay = 0;
}

bool ShutdownUI::showWindow(const QString &text1, const QString &text2, int delay)
{
    m_Text1 = text1;
    m_Text2 = text2;
    m_Delay = delay;

    return true;
}

// The stub for MNotification class.
static QString bodyOfLastNotification;

bool MNotification::publish()
{
    bodyOfLastNotification = body();
    return true;
}

void Ut_ShutdownBusinessLogic::init()
{
    m_Api = new ShutdownBusinessLogic;
}

void Ut_ShutdownBusinessLogic::cleanup()
{
    delete m_Api;
}

int argc = 1;
char *argv[] = { (char *) "./ut_shutdownbusinesslogic", NULL };

void Ut_ShutdownBusinessLogic::initTestCase()
{
    m_App = new MApplication(argc, argv);
    m_App->setQuitOnLastWindowClosed(false);
}

void Ut_ShutdownBusinessLogic::cleanupTestCase()
{
    m_App->deleteLater();
}

void Ut_ShutdownBusinessLogic::testShowUI()
{
    m_Api->showUI("text1", "text2", 50);

    QCOMPARE(m_Api->shutdownUi->m_Text1, QString("text1"));
    QCOMPARE(m_Api->shutdownUi->m_Text2, QString("text2"));
    QCOMPARE(m_Api->shutdownUi->m_Delay, 50);

    m_Api->showUI("text3", "text4", 150);

    QCOMPARE(m_Api->shutdownUi->m_Text1, QString("text3"));
    QCOMPARE(m_Api->shutdownUi->m_Text2, QString("text4"));
    QCOMPARE(m_Api->shutdownUi->m_Delay, 150);
}

#ifdef HAVE_QMSYSTEM
void Ut_ShutdownBusinessLogic::testSystemStateChanged()
{
    m_Api->systemStateChanged(MeeGo::QmSystemState::BatteryStateEmpty);
    QCOMPARE(bodyOfLastNotification, QString("qtn_shut_batt_empty"));

    m_Api->systemStateChanged(MeeGo::QmSystemState::ShutdownDeniedUSB);
    QCOMPARE(bodyOfLastNotification, QString("qtn_shut_unplug_usb"));

    m_Api->systemStateChanged(MeeGo::QmSystemState::ThermalStateFatal);
    QCOMPARE(bodyOfLastNotification, QString("qtn_shut_high_temp"));
}

void Ut_ShutdownBusinessLogic::testShutdownWindow()
{
    m_Api->systemStateChanged(MeeGo::QmSystemState::Shutdown);
    QVERIFY(m_Api->shutdownUi->m_Text1.isEmpty());
    QVERIFY(m_Api->shutdownUi->m_Text2.isEmpty());
    QCOMPARE(m_Api->shutdownUi->m_Delay, 2000);
}
#endif

QTEST_APPLESS_MAIN(Ut_ShutdownBusinessLogic)
