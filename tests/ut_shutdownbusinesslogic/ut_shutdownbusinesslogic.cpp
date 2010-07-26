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

#include "ut_shutdownbusinesslogic.h"

#include "shutdownbusinesslogic.h"

#include <MApplication>
#include <MWindow>
#include <MNotification>
#include <MFeedback>

MApplication *exitPtr;

#define DEBUG
#include "../../src/debug.h"

/******************************************************************************
 * Stubbed version of th ShutdownUI class.
 */
ShutdownUI::ShutdownUI ()
{
    m_Delay = 0;
}

bool 
ShutdownUI::showWindow (
        const QString &text1, 
        const QString &text2, 
        int            delay)
{
    SYS_DEBUG ("*** text1 = %s", SYS_STR(text1));
    SYS_DEBUG ("*** text2 = %s", SYS_STR(text2));
    SYS_DEBUG ("*** delay = %d", delay);

    m_Text1 = text1;
    m_Text2 = text2;
    m_Delay = delay;

    return true;
}

/******************************************************************************
 * The stub for MNotification class.
 */
static QString bodyOfLastNotification;

bool
MNotification::publish ()
{
    SYS_DEBUG ("*** body = %s", SYS_STR(body()));
    bodyOfLastNotification = body();

    return true;
}

/******************************************************************************
 * The stub for MFeedback class.
 */
static QString nameOfLastFeedback;

void
MFeedback::play () const 
{
    SYS_DEBUG ("*** name = %s", SYS_STR(name()));
    nameOfLastFeedback = name();
}

/*******************************************************************************
 * The Ut_ShutdownBusinessLogic implements the unit tests.
 */
void Ut_ShutdownBusinessLogic::init()
{
}

void Ut_ShutdownBusinessLogic::cleanup()
{
}


int   argc = 1;
char *argv[] = {
    (char *) "./ut_shutdownbusinesslogic",
    NULL };

void Ut_ShutdownBusinessLogic::initTestCase()
{
    SYS_DEBUG ("+++ Creating application.");
    m_App = new MApplication(argc, argv);
    m_App->setQuitOnLastWindowClosed (false);

    m_Api = new ShutdownBusinessLogic;
}

void 
Ut_ShutdownBusinessLogic::cleanupTestCase()
{
    delete m_Api;
    m_App->deleteLater ();
}

void 
Ut_ShutdownBusinessLogic::testShowUI ()
{
    m_Api->showUI ("text1", "text2", 50);

    QVERIFY (m_Api->m_Ui->m_Text1 == "text1");
    QVERIFY (m_Api->m_Ui->m_Text2 == "text2");
    QVERIFY (m_Api->m_Ui->m_Delay == 50);
}

#ifdef HAVE_QMSYSTEM
void 
Ut_ShutdownBusinessLogic::testSystemStateChanged ()
{
    m_Api->systemStateChanged (Maemo::QmSystemState::BatteryStateEmpty);
    QVERIFY (bodyOfLastNotification == "qtn_shut_will_shut_down");
    QVERIFY (nameOfLastFeedback == "IDF_RECHARGE_BATTERY");
    
    m_Api->systemStateChanged (Maemo::QmSystemState::ShutdownDeniedUSB);
    QVERIFY (bodyOfLastNotification == "qtn_shut_unplug_usb");
    QVERIFY (nameOfLastFeedback == "IDF_INFORMATION_SOUND");
    
    m_Api->systemStateChanged (Maemo::QmSystemState::ThermalStateFatal);
    QVERIFY (bodyOfLastNotification == "qtn_shut_high_temp");
    QVERIFY (nameOfLastFeedback == "IDF_INFORMATION_STRONG");
    
    m_Api->systemStateChanged (Maemo::QmSystemState::Shutdown);
    QVERIFY (m_Api->m_Ui->m_Text1.isEmpty());
    QVERIFY (m_Api->m_Ui->m_Text2.isEmpty());
    QVERIFY (m_Api->m_Ui->m_Delay == 2000);
}
#endif

QTEST_APPLESS_MAIN(Ut_ShutdownBusinessLogic)

