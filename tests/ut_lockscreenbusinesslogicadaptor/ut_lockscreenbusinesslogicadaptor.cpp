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

#include "lockscreenbusinesslogic_stub.h"
#include "ut_lockscreenbusinesslogicadaptor.h"

#define TEST_SERVICE   QString ("com.nokia.mcetest")
#define TEST_PATH      QString ("/com/nokia/mcetest")
#define TEST_INTERFACE QString ("com.nokia.mcetest")
#define TEST_METHOD    QString ("testmethod")

void
Ut_LockScreenBusinessLogicAdaptor::initTestCase ()
{
    m_logic = new LockScreenBusinessLogic;
    m_subject = new LockScreenBusinessLogicAdaptor (this, m_logic);
}

void
Ut_LockScreenBusinessLogicAdaptor::cleanupTestCase ()
{
    delete m_subject;
    delete m_logic;
}

void
Ut_LockScreenBusinessLogicAdaptor::MCECallBack ()
{
    int reply;

    reply = m_subject->tklock_open (TEST_SERVICE,
                                    TEST_PATH,
                                    TEST_INTERFACE,
                                    TEST_METHOD,
                                    LockScreenBusinessLogicAdaptor::TkLockModeNone,
                                    false,
                                    false);
    
    QVERIFY (reply == LockScreenBusinessLogicAdaptor::TkLockReplyOk);

    QCOMPARE (m_subject->m_MCECallbackService,   TEST_SERVICE);
    QCOMPARE (m_subject->m_MCECallbackPath,      TEST_PATH);
    QCOMPARE (m_subject->m_MCECallbackInterface, TEST_INTERFACE);
    QCOMPARE (m_subject->m_MCECallbackMethod,    TEST_METHOD);
}

void
Ut_LockScreenBusinessLogicAdaptor::EventEaterIf ()
{
    int reply;

    // Check the visibility of the event-eater ui
    QCOMPARE (m_logic->eaterUIvisible, false);

    // Call the interface like MCE to show the event eater
    reply = m_subject->tklock_open (TEST_SERVICE,
                                    TEST_PATH,
                                    TEST_INTERFACE,
                                    TEST_METHOD,
                                    LockScreenBusinessLogicAdaptor::TkLockModeOneInput,
                                    false,
                                    false);
    QTest::qWait (100);

    // Check the reply
    QVERIFY (reply == LockScreenBusinessLogicAdaptor::TkLockReplyOk);

    // Check the visibility of the event-eater ui
    QCOMPARE (m_logic->eaterUIvisible, true);

    // Call the interface like MCE to hide the event eater
    reply = m_subject->tklock_close (false);

    QTest::qWait (100);

    // Check the reply
    QVERIFY (reply == LockScreenBusinessLogicAdaptor::TkLockReplyOk);

    // Check the visibility of the event-eater ui
    QCOMPARE (m_logic->eaterUIvisible, false);
}

void
Ut_LockScreenBusinessLogicAdaptor::UnlockScreenIf ()
{
    int reply;

    // Check the visibility of the tklock ui
    QCOMPARE (m_logic->lockUIvisible, false);

    // Call the interface like MCE to show the unlock screen
    reply = m_subject->tklock_open (TEST_SERVICE,
                                    TEST_PATH,
                                    TEST_INTERFACE,
                                    TEST_METHOD,
                                    LockScreenBusinessLogicAdaptor::TkLockEnableVisual,
                                    false,
                                    false);
    QTest::qWait (100);

    // Check the reply
    QVERIFY (reply == LockScreenBusinessLogicAdaptor::TkLockReplyOk);

    // Check the visibility of the tklock ui
    QCOMPARE (m_logic->lockUIvisible, true);

    // Call the interface like MCE to hide the lockscreen ui
    reply = m_subject->tklock_close (false);

    QTest::qWait (100);

    // Check the reply
    QVERIFY (reply == LockScreenBusinessLogicAdaptor::TkLockReplyOk);

    // Check the visibility of the tklock ui
    QCOMPARE (m_logic->lockUIvisible, false);
}

QTEST_MAIN(Ut_LockScreenBusinessLogicAdaptor)
