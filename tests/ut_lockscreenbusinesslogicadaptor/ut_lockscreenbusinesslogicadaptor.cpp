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

#include "ut_lockscreenbusinesslogicadaptor.h"
#include "screenlockbusinesslogic_stub.h"
#include "screenlockbusinesslogicadaptor.h"

#include <QDBusAbstractInterface>

#define TEST_SERVICE   QString ("com.nokia.mcetest")
#define TEST_PATH      QString ("/com/nokia/mcetest")
#define TEST_INTERFACE QString ("com.nokia.mcetest")
#define TEST_METHOD    QString ("testmethod")

QString lastCalledMethod;
QString lastCalledPath;
QString lastCalledService;
QString lastCalledInterface;

QDBusMessage QDBusAbstractInterface::call(QDBus::CallMode mode, const QString & method, const QVariant & arg1, const QVariant & arg2, const QVariant & arg3, const QVariant & arg4, const QVariant & arg5, const QVariant & arg6, const QVariant & arg7, const QVariant & arg8)
{
    Q_UNUSED (mode);
    Q_UNUSED (arg1);
    Q_UNUSED (arg2);
    Q_UNUSED (arg3);
    Q_UNUSED (arg4);
    Q_UNUSED (arg5);
    Q_UNUSED (arg6);
    Q_UNUSED (arg7);
    Q_UNUSED (arg8);

    QDBusMessage message;

    lastCalledMethod = method;
    lastCalledPath = path();
    lastCalledService = service();
    lastCalledInterface = interface();

    return message;
}

void Ut_LockScreenBusinessLogicAdaptor::initTestCase()
{
    m_logic = new ScreenLockBusinessLogic;
}

void Ut_LockScreenBusinessLogicAdaptor::cleanupTestCase()
{
    delete m_logic;
}

void Ut_LockScreenBusinessLogicAdaptor::init()
{
    m_subject = new ScreenLockBusinessLogicAdaptor(this, m_logic);
}

void Ut_LockScreenBusinessLogicAdaptor::cleanup()
{
    delete m_subject;
}

/*
 * This function will call the tklock_open method that is exported on the DBus
 * for the MCE to call.
 */
void Ut_LockScreenBusinessLogicAdaptor::testTklockOpen()
{
    int reply;

    /*
     * This should not do nothing...
     */
    reply = m_subject->tklock_open(TEST_SERVICE, TEST_PATH, TEST_INTERFACE, TEST_METHOD, ScreenLockBusinessLogicAdaptor::TkLockModeNone, false, false);

    QVERIFY (reply == ScreenLockBusinessLogicAdaptor::TkLockReplyOk);
    QCOMPARE (m_subject->m_MCECallbackService, TEST_SERVICE);
    QCOMPARE (m_subject->m_MCECallbackPath, TEST_PATH);
    QCOMPARE (m_subject->m_MCECallbackInterface, TEST_INTERFACE);
    QCOMPARE (m_subject->m_MCECallbackMethod, TEST_METHOD);

    /*
     * Neither this one.
     */
    reply = m_subject->tklock_open(TEST_SERVICE, TEST_PATH, TEST_INTERFACE, TEST_METHOD, ScreenLockBusinessLogicAdaptor::TkLockModeHelp, false, false);

    QVERIFY (reply == ScreenLockBusinessLogicAdaptor::TkLockReplyOk);
    QCOMPARE (m_subject->m_MCECallbackService, TEST_SERVICE);
    QCOMPARE (m_subject->m_MCECallbackPath, TEST_PATH);
    QCOMPARE (m_subject->m_MCECallbackInterface, TEST_INTERFACE);
    QCOMPARE (m_subject->m_MCECallbackMethod, TEST_METHOD);

    /*
     * Neither this.
     */
    reply = m_subject->tklock_open(TEST_SERVICE, TEST_PATH, TEST_INTERFACE, TEST_METHOD, ScreenLockBusinessLogicAdaptor::TkLockModeSelect, false, false);

    QVERIFY (reply == ScreenLockBusinessLogicAdaptor::TkLockReplyOk);
    QCOMPARE (m_subject->m_MCECallbackService, TEST_SERVICE);
    QCOMPARE (m_subject->m_MCECallbackPath, TEST_PATH);
    QCOMPARE (m_subject->m_MCECallbackInterface, TEST_INTERFACE);
    QCOMPARE (m_subject->m_MCECallbackMethod, TEST_METHOD);
}

void Ut_LockScreenBusinessLogicAdaptor::testUnlockConfirmed()
{
    int reply;

    /*
     * This should not do nothing...
     */
    reply = m_subject->tklock_open(TEST_SERVICE, TEST_PATH, TEST_INTERFACE, TEST_METHOD, ScreenLockBusinessLogicAdaptor::TkLockModeNone, false, false);

    QVERIFY (reply == ScreenLockBusinessLogicAdaptor::TkLockReplyOk);

    /*
     * Calling the unlockConfirmed() twice intentionaly.
     */
    m_subject->unlockConfirmed();
    QVERIFY (lastCalledMethod == TEST_METHOD);
    QVERIFY (lastCalledPath == TEST_PATH);
    QVERIFY (lastCalledService == TEST_SERVICE);
    QVERIFY (lastCalledInterface == TEST_INTERFACE);

    m_subject->unlockConfirmed();
    QVERIFY (lastCalledMethod == TEST_METHOD);
    QVERIFY (lastCalledPath == TEST_PATH);
    QVERIFY (lastCalledService == TEST_SERVICE);
    QVERIFY (lastCalledInterface == TEST_INTERFACE);
}

void Ut_LockScreenBusinessLogicAdaptor::testEventEaterIf()
{
    int reply;

    // Call the interface like MCE to show the event eater
    reply = m_subject->tklock_open(TEST_SERVICE, TEST_PATH, TEST_INTERFACE, TEST_METHOD, ScreenLockBusinessLogicAdaptor::TkLockModeOneInput, false, false);
    QTest::qWait(100);

    // Check the reply
    QVERIFY (reply == ScreenLockBusinessLogicAdaptor::TkLockReplyOk);

    // Check the visibility of the event-eater ui
    QCOMPARE (gScreenLockBusinessLogicStub->stubLastCallTo("toggleEventEater").parameter<bool>(0), true);

    // Call the interface like MCE to hide the event eater
    reply = m_subject->tklock_close(false);

    QTest::qWait(100);

    // Check the reply
    QVERIFY (reply == ScreenLockBusinessLogicAdaptor::TkLockReplyOk);

    // Check the visibility of the event-eater ui
    QCOMPARE (gScreenLockBusinessLogicStub->stubLastCallTo("toggleEventEater").parameter<bool>(0), false);
}

void Ut_LockScreenBusinessLogicAdaptor::testUnlockScreenIf()
{
    int reply;

    // Call the interface like MCE to show the unlock screen
    reply = m_subject->tklock_open(TEST_SERVICE, TEST_PATH, TEST_INTERFACE, TEST_METHOD, ScreenLockBusinessLogicAdaptor::TkLockEnableVisual, false, false);
    QTest::qWait(100);

    // Check the reply
    QVERIFY (reply == ScreenLockBusinessLogicAdaptor::TkLockReplyOk);

    // Check the visibility of the tklock ui
    QCOMPARE (gScreenLockBusinessLogicStub->stubLastCallTo("toggleScreenLockUI").parameter<bool>(0), true);

    // Call the interface like MCE to hide the lockscreen ui
    reply = m_subject->tklock_close(false);

    QTest::qWait(100);

    // Check the reply
    QVERIFY (reply == ScreenLockBusinessLogicAdaptor::TkLockReplyOk);

    // Check the visibility of the tklock ui
    QCOMPARE (gScreenLockBusinessLogicStub->stubLastCallTo("toggleScreenLockUI").parameter<bool>(0), false);
}

QTEST_MAIN(Ut_LockScreenBusinessLogicAdaptor)
