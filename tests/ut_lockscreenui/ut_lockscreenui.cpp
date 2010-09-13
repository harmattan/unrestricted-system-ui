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

#include "ut_lockscreenui.h"
#include "lockscreenui.h"
#include "sysuid_stub.h"
#include "unlockmissedevents_stub.h"
#include "lockscreenwindow_stub.h"

#include <MApplication>

#define DEBUG
#include "debug.h"

/******************************************************************************
 * The Ut_LockScreenUI implements the unit tests.
 */
void Ut_LockScreenUI::init()
{
}

void Ut_LockScreenUI::cleanup()
{
}


int   argc = 1;
char *argv[] = {
    (char *) "./ut_lockscreenui",
    NULL };

void Ut_LockScreenUI::initTestCase()
{
    m_LockScreenUI = 0;

    SYS_DEBUG ("+++ Creating application.");
    m_App = new MApplication(argc, argv);
    m_App->setQuitOnLastWindowClosed (false);
}

void Ut_LockScreenUI::cleanupTestCase()
{
    if (m_LockScreenUI)
        delete m_LockScreenUI;

    m_App->deleteLater ();
}

void Ut_LockScreenUI::testLockScreenUI ()
{
    Q_ASSERT (m_LockScreenUI == 0);

    /*
     * When the lockscreenUI is created it just delays its realize function. We
     * process the events to cause the realize to be called, then we check that
     * the internal widgets are created. For this delayed realization the widget
     * does not need to be shown, so the widget will be shown fast even at the first time.
     */
    m_LockScreenUI = new LockScreenUI;
    QCoreApplication::processEvents();

    QVERIFY (m_LockScreenUI->m_Realized);
    QVERIFY (m_LockScreenUI->m_policy != NULL);
    QVERIFY (m_LockScreenUI->m_LockLiftArea != NULL);
    QVERIFY (m_LockScreenUI->m_LockLandArea != NULL);
    QVERIFY (m_LockScreenUI->m_SceneWindow != NULL);

    delete m_LockScreenUI;

    /*
     * Now let's do it with the early manual realization. The createContent()
     * hopefully will be called when the window shown...
     */
    m_LockScreenUI = new LockScreenUI;
    m_LockScreenUI->createContent();
    QVERIFY (m_LockScreenUI->m_Realized);
    QVERIFY (m_LockScreenUI->m_policy != NULL);
    QVERIFY (m_LockScreenUI->m_LockLiftArea != NULL);
    QVERIFY (m_LockScreenUI->m_LockLandArea != NULL);
    QVERIFY (m_LockScreenUI->m_SceneWindow != NULL);

    // FIXME: I will enhance this part... 
    //m_LockScreenUI->showEvent (NULL);
    //considerLockScreenUIVisible = true;
    m_LockScreenUI->showHideNotifications (true);
    //QVERIFY (m_LockScreenUI->m_notificationArea->isVisible());

    m_LockScreenUI->showHideNotifications (false);
    //QVERIFY (!m_LockScreenUI->m_notificationArea->isVisible());
    delete m_LockScreenUI;

    /*
     * Test the extreme case of the early destruction of the object. We had a
     * creash in this case, this is what checked here.
     */
    m_LockScreenUI = new LockScreenUI;
    delete m_LockScreenUI;
}

/*
 * This test will check if the lockscreenui will send the signal when the
 * lockscreenwindow emits the unlocked() signal.
 */
void Ut_LockScreenUI::testLockScreenUISignals ()
{
    m_LockScreenUI = new LockScreenUI;
    m_LockScreenUI->createContent();

    QSignalSpy spy(m_LockScreenUI, SIGNAL(unlocked()));

    emit m_LockScreenUI->m_SceneWindow->unlocked();
    QCOMPARE(spy.count(), 1);

    QVERIFY(disconnect(m_LockScreenUI->m_SceneWindow, SIGNAL(unlocked()), m_LockScreenUI, SLOT (sliderUnlocked())));
    delete m_LockScreenUI;
}


void Ut_LockScreenUI::testLockScreenUIWindowName ()
{
    Q_ASSERT (m_LockScreenUI == 0);

    m_LockScreenUI = new LockScreenUI;

    /*
     * I'm not sure what could we test here except that the program will not
     * crash if the event is invalid and the internal window ID is also
     * invalid.
     */
    QShowEvent event;
    m_LockScreenUI->showEvent(&event);

    delete m_LockScreenUI;
}

QTEST_APPLESS_MAIN(Ut_LockScreenUI)
