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

#ifdef HAVE_QMSYSTEM
#include <qmdisplaystate.h>
using namespace MeeGo;
#endif

#ifdef HAVE_QMSYSTEM
static bool dimmingShouldSucceed = true;

bool
QmDisplayState::set(
        DisplayState state)
{
    Q_UNUSED (state);

    //m_State = state;
    return dimmingShouldSucceed;
}
#endif

/******************************************************************************
 * The stub for MFeedback class.
 */
#include <MFeedback>

static QString nameOfLastFeedback;

void
MFeedback::play () const 
{
    nameOfLastFeedback = name();
}

/******************************************************************************
 * Stubbing the QWidget, so the UI will not be really shown.
 */
void 
QWidget::setVisible (bool)
{
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
    m_App = new MApplication(argc, argv);
    m_App->setQuitOnLastWindowClosed (false);

    m_Api = new ShutdownUI;
    m_Api->realize ();
    m_Api->realize ();

    QVERIFY (m_Api->m_Realized);
    QVERIFY (m_Api->m_Feedback);
    QVERIFY (m_Api->m_Label1);
    QVERIFY (m_Api->m_Label2);
    QVERIFY (m_Api->m_logo);
    QVERIFY (m_Api->m_SceneWindow);

    /*
     * FIXME: What else should we check here? What about calling these methods?
     */
    m_Api->showLogo ();


    m_Api->turnOffScreen ();
    
    
#ifdef HAVE_QMSYSTEM
    dimmingShouldSucceed = false;
#endif
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
    QVERIFY (m_Api->m_logo);
    QVERIFY (m_Api->m_SceneWindow);
    QVERIFY (nameOfLastFeedback == "power-off");
}

QTEST_APPLESS_MAIN(Ut_ShutdownUI)

