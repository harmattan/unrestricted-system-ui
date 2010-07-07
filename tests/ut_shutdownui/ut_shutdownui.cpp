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

#include "ut_shutdownui.h"

#include "shutdownui.h"

#include <MApplication>
#include <MWindow>
#include <MNotification>
#include <MFeedback>

#ifdef HAVE_QMSYSTEM
#include <qmdisplaystate.h>
using namespace Maemo;
#endif

#define DEBUG
#include "../../src/debug.h"


#ifdef HAVE_QMSYSTEM
static bool dimmingShouldSucceed = true;

bool
QmDisplayState::set(
        DisplayState state)
{
    Q_UNUSED (state);

    SYS_DEBUG ("*************");
    //m_State = state;
    return dimmingShouldSucceed;
}
#endif
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

/******************************************************************************
 * Stubbing the QWidget, so the UI will not be really shown.
 */
void 
QWidget::setVisible (bool visible)
{
    SYS_DEBUG ("----------------------------------------------------------");
    SYS_DEBUG ("*** visible = %s", SYS_BOOL(visible));
    SYS_DEBUG ("*** this    = %p", this);
}

/*******************************************************************************
 * The Ut_ShutdownUI implements the unit tests.
 */
void Ut_ShutdownUI::init()
{
}

void Ut_ShutdownUI::cleanup()
{
}


int   argc = 1;
char *argv[] = {
    (char *) "./ut_shutdownui",
    NULL };

void Ut_ShutdownUI::initTestCase()
{
    SYS_DEBUG ("+++ Creating application.");
    m_App = new MApplication(argc, argv);
    m_App->setQuitOnLastWindowClosed (false);

    m_Api = new ShutdownUI;
    m_Api->realize ();
    m_Api->realize ();

    QVERIFY (m_Api->m_Realized);
    QVERIFY (m_Api->m_Feedback);
    QVERIFY (m_Api->m_Label1);
    QVERIFY (m_Api->m_Label2);
    QVERIFY (m_Api->m_Image);
    QVERIFY (m_Api->m_SceneWindow);

    /*
     * FIXME: What else should we check here? What about calling these methods?
     */
    m_Api->showLogo ();


    m_Api->turnOffScreen ();
    
    
    dimmingShouldSucceed = false;
    m_Api->turnOffScreen ();
}

void 
Ut_ShutdownUI::cleanupTestCase()
{
    delete m_Api;
    m_App->deleteLater ();
}

void 
Ut_ShutdownUI::testShowWindow ()
{
    if (m_Api)
        delete m_Api;

    m_Api = new ShutdownUI;
    m_Api->showWindow ("text1", "text2", 2000);
    
    QVERIFY (m_Api->m_Realized);
    QVERIFY (m_Api->m_Feedback);
    QVERIFY (m_Api->m_Label1);
    QVERIFY (m_Api->m_Label2);
    QVERIFY (m_Api->m_Image);
    QVERIFY (m_Api->m_SceneWindow);
}

QTEST_APPLESS_MAIN(Ut_ShutdownUI)

