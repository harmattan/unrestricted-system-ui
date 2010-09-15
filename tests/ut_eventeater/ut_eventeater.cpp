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

#include "ut_eventeater.h"
#include "eventeater.h"

#include <QEvent>
#include <QShowEvent>

#include <MApplication>
#include <MWindow>

#define DEBUG
#include "debug.h"


/******************************************************************************
 * The helper class to watch the signals.
 */
LockScreenUIEventSink::LockScreenUIEventSink() :
        m_OneInputCame(false),
        m_UnlockedCame(false)
{
}

void LockScreenUIEventSink::OneInput()
{
    m_OneInputCame = true;
}

void LockScreenUIEventSink::unlocked()
{
    m_UnlockedCame = true;
}


/******************************************************************************
 * The Ut_EventEater implements the unit tests.
 */
void Ut_EventEater::init()
{
}

void Ut_EventEater::cleanup()
{
}


int   argc = 1;
char *argv[] = {
    (char *) "./ut_eventeater",
    NULL };

void Ut_EventEater::initTestCase()
{
    m_EventEater = 0;

    SYS_DEBUG ("+++ Creating application.");
    m_App = new MApplication(argc, argv);
    m_App->setQuitOnLastWindowClosed (false);
}

void
Ut_EventEater::cleanupTestCase()
{
    if (m_EventEater)
        delete m_EventEater;

    m_App->deleteLater ();
}

void
Ut_EventEater::testEventEater()
{
    bool connectSuccess;

    /*
     * We need a new event eater widget.
     */
    Q_ASSERT (m_EventEater == 0);
    m_EventEater = new EventEater;

    /*
     * We test if we can connect to the OneInput() signal.
     */
    connectSuccess = connect (m_EventEater, SIGNAL(OneInput()),
            &m_EventSink, SLOT(OneInput()));
    QVERIFY (connectSuccess);

    /*
     * Let's see if the event is actually generates a signal.
     */
    m_EventSink.m_OneInputCame = false;
    m_EventEater->mousePressEvent ((QMouseEvent *)NULL);
    QVERIFY (m_EventSink.m_OneInputCame);

    m_EventSink.m_OneInputCame = false;
    m_EventEater->mouseReleaseEvent ((QMouseEvent *)NULL);
    QVERIFY (m_EventSink.m_OneInputCame);

    delete m_EventEater;
    m_EventEater = 0;
}

void
Ut_EventEater::testEventEaterWindowName ()
{
    /*
     * We need a new event eater widget.
     */
    Q_ASSERT (m_EventEater == 0);
    m_EventEater = new EventEater;

    /*
     * I'm not sure what could we test here except that the program will not
     * crash if the event is invalid and the internal window ID is also
     * invalid.
     */
    QShowEvent event;
    m_EventEater->showEvent(&event);

    delete m_EventEater;
    m_EventEater = 0;
}

QTEST_APPLESS_MAIN(Ut_EventEater)
