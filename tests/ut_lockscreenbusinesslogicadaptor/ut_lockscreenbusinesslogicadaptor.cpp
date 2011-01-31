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

#include <QtTest/QtTest>
#include <QDBusAbstractInterface>

#define TEST_SERVICE   QString("com.nokia.mcetest")
#define TEST_PATH      QString("/com/nokia/mcetest")
#define TEST_INTERFACE QString("com.nokia.mcetest")
#define TEST_METHOD    QString("testmethod")

QDBus::CallMode qDbusAbstractInterfaceCallMode;
QVariant qDbusAbstractInterfaceCallArg1;
QString qDbusAbstractInterfaceCallMethod;
QString qDbusAbstractInterfaceCallPath;
QString qDbusAbstractInterfaceCallService;
QString qDbusAbstractInterfaceCallInterface;
QDBusMessage QDBusAbstractInterface::call(QDBus::CallMode mode, const QString & method, const QVariant & arg1, const QVariant & arg2, const QVariant & arg3, const QVariant & arg4, const QVariant & arg5, const QVariant & arg6, const QVariant & arg7, const QVariant & arg8)
{
    Q_UNUSED(arg2);
    Q_UNUSED(arg3);
    Q_UNUSED(arg4);
    Q_UNUSED(arg5);
    Q_UNUSED(arg6);
    Q_UNUSED(arg7);
    Q_UNUSED(arg8);

    qDbusAbstractInterfaceCallMode = mode;
    qDbusAbstractInterfaceCallArg1 = arg1;
    qDbusAbstractInterfaceCallMethod = method;
    qDbusAbstractInterfaceCallPath = path();
    qDbusAbstractInterfaceCallService = service();
    qDbusAbstractInterfaceCallInterface = interface();

    return QDBusMessage();
}

void QTimer::singleShot(int, QObject *receiver, const char *member)
{
    // The "member" string is of form "1member()", so remove the trailing 1 and the ()
    int memberLength = strlen(member) - 3;
    char modifiedMember[memberLength + 1];
    strncpy(modifiedMember, member + 1, memberLength);
    modifiedMember[memberLength] = 0;
    QMetaObject::invokeMethod(receiver, modifiedMember, Qt::DirectConnection);
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
    m_subject = new ScreenLockBusinessLogicAdaptor(this, *m_logic);
}

void Ut_LockScreenBusinessLogicAdaptor::cleanup()
{
    delete m_subject;
    qDbusAbstractInterfaceCallMode = QDBus::Block;
    qDbusAbstractInterfaceCallArg1.clear();
    qDbusAbstractInterfaceCallMethod.clear();
    qDbusAbstractInterfaceCallPath.clear();
    qDbusAbstractInterfaceCallService.clear();
    qDbusAbstractInterfaceCallInterface.clear();
    gScreenLockBusinessLogicStub->stubReset();
}

void Ut_LockScreenBusinessLogicAdaptor::testTkLockOpen_data()
{
    QTest::addColumn<int>("mode");
    QTest::addColumn<int>("toggleScreenLockUICount");
    QTest::addColumn<bool>("toggleScreenLockUIParameter");
    QTest::addColumn<int>("toggleEventEaterCount");
    QTest::addColumn<bool>("toggleEventEaterParameter");

    QTest::newRow("TkLockModeNone") << (int)ScreenLockBusinessLogicAdaptor::TkLockModeNone << 0 << false << 0 << false;
    QTest::newRow("TkLockModeEnable") << (int)ScreenLockBusinessLogicAdaptor::TkLockModeEnable << 1 << true << 1 << false;
    QTest::newRow("TkLockModeHelp") << (int)ScreenLockBusinessLogicAdaptor::TkLockModeHelp << 0 << false << 0 << false;
    QTest::newRow("TkLockModeSelect") << (int)ScreenLockBusinessLogicAdaptor::TkLockModeSelect << 0 << false << 0 << false;
    QTest::newRow("TkLockModeOneInput") << (int)ScreenLockBusinessLogicAdaptor::TkLockModeOneInput << 0 << false << 1 << true;
    QTest::newRow("TkLockEnableVisual") << (int)ScreenLockBusinessLogicAdaptor::TkLockEnableVisual << 1 << true << 1 << false;
}

void Ut_LockScreenBusinessLogicAdaptor::testTkLockOpen()
{
    QFETCH(int, mode);
    QFETCH(int, toggleScreenLockUICount);
    QFETCH(bool, toggleScreenLockUIParameter);
    QFETCH(int, toggleEventEaterCount);
    QFETCH(bool, toggleEventEaterParameter);

    int reply = m_subject->tklock_open(TEST_SERVICE, TEST_PATH, TEST_INTERFACE, TEST_METHOD, mode, false, false);
    QCOMPARE(reply, (int)ScreenLockBusinessLogicAdaptor::TkLockReplyOk);

    QCOMPARE(gScreenLockBusinessLogicStub->stubCallCount("toggleScreenLockUI"), toggleScreenLockUICount);
    if (toggleScreenLockUICount > 0) {
        QCOMPARE(gScreenLockBusinessLogicStub->stubLastCallTo("toggleScreenLockUI").parameter<bool>(0), toggleScreenLockUIParameter);
    }

    QCOMPARE(gScreenLockBusinessLogicStub->stubCallCount("toggleEventEater"), toggleEventEaterCount);
    if (toggleEventEaterCount > 0) {
        QCOMPARE(gScreenLockBusinessLogicStub->stubLastCallTo("toggleEventEater").parameter<bool>(0), toggleEventEaterParameter);
    }
}

void Ut_LockScreenBusinessLogicAdaptor::testTkLockClose()
{
    int reply = m_subject->tklock_close(false);
    QCOMPARE(reply, (int)ScreenLockBusinessLogicAdaptor::TkLockReplyOk);

    QCOMPARE(gScreenLockBusinessLogicStub->stubCallCount("toggleScreenLockUI"), 1);
    QCOMPARE(gScreenLockBusinessLogicStub->stubLastCallTo("toggleScreenLockUI").parameter<bool>(0), false);
    QCOMPARE(gScreenLockBusinessLogicStub->stubCallCount("toggleEventEater"), 1);
    QCOMPARE(gScreenLockBusinessLogicStub->stubLastCallTo("toggleEventEater").parameter<bool>(0), false);
}

void Ut_LockScreenBusinessLogicAdaptor::testUnlockCallback()
{
    m_subject->tklock_open(TEST_SERVICE, TEST_PATH, TEST_INTERFACE, TEST_METHOD, ScreenLockBusinessLogicAdaptor::TkLockModeNone, false, false);
    m_subject->callUnlockCallback();
    QCOMPARE(qDbusAbstractInterfaceCallMethod, TEST_METHOD);
    QCOMPARE(qDbusAbstractInterfaceCallPath, TEST_PATH);
    QCOMPARE(qDbusAbstractInterfaceCallService, TEST_SERVICE);
    QCOMPARE(qDbusAbstractInterfaceCallInterface, TEST_INTERFACE);
    QCOMPARE(qDbusAbstractInterfaceCallMode, QDBus::NoBlock);
    QCOMPARE(qDbusAbstractInterfaceCallArg1.toInt(), (int)ScreenLockBusinessLogicAdaptor::TkLockUnlock);
}

QTEST_MAIN(Ut_LockScreenBusinessLogicAdaptor)
