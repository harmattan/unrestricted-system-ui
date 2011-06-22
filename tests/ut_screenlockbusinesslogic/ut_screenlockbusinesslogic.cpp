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

#include <QtTest/QtTest>
#include <QDBusAbstractInterface>

#define TEST_SERVICE   QString("com.nokia.mcetest")
#define TEST_PATH      QString("/com/nokia/mcetest")
#define TEST_INTERFACE QString("com.nokia.mcetest")
#define TEST_METHOD    QString("testmethod")

#include "ut_screenlockbusinesslogic.h"
#include "screenlockbusinesslogic.h"
#include "lockscreenwindow_stub.h"
#include "eventeater_stub.h"
#include "sysuid_stub.h"
#include "eventeater_stub.h"
#include "closeeventeater_stub.h"
#include "notifiernotificationsink_stub.h"
#include "notificationsink_stub.h"
#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationExtensionArea>

#ifdef HAVE_QMSYSTEM
#include <qmdisplaystate.h>
#include <qmsystemstate.h>

MeeGo::QmDisplayState::DisplayState qmDisplayState;
MeeGo::QmDisplayState::DisplayState MeeGo::QmDisplayState::get() const
{
    return qmDisplayState;
}
#endif

QMap<QWidget *, bool> gQWidgetVisible;
void QWidget::setVisible(bool visible)
{
    gQWidgetVisible[this] = visible;
    setAttribute(Qt::WA_WState_Visible, visible);
}

bool gQWidgetRaiseCalled = false;
void QWidget::raise()
{
    gQWidgetRaiseCalled = true;
}

bool gQWidgetRepaintCalled = false;
void QWidget::repaint()
{
    gQWidgetRepaintCalled = true;
}

bool MApplicationExtensionArea::init()
{
    return true;
}

ScreenLockExtension::ScreenLockExtension() : widget_(NULL)
{
}
ScreenLockExtension::~ScreenLockExtension()
{
}
bool screenLockExtensionReset = false;
void ScreenLockExtension::reset()
{
    screenLockExtensionReset = true;
}
void ScreenLockExtension::setNotificationManagerInterface(NotificationManagerInterface &)
{
}
bool ScreenLockExtension::initialize(const QString &)
{
    widget_ = new LockScreen;
    return true;
}
QGraphicsWidget *ScreenLockExtension::widget()
{
    return widget_;
}
QObject *ScreenLockExtension::qObject()
{
    return this;
}
bool screenLockExtensionModeSet = false;
ScreenLockExtensionInterface::ScreenLockMode screenLockExtensionMode;
void ScreenLockExtension::setMode(ScreenLockExtensionInterface::ScreenLockMode mode)
{
    screenLockExtensionModeSet = true;
    screenLockExtensionMode = mode;
}

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

void Ut_ScreenLockBusinessLogic::init()
{
    m_subject = new ScreenLockBusinessLogic;
}

void Ut_ScreenLockBusinessLogic::cleanup()
{
    delete m_subject;
    qDbusAbstractInterfaceCallArg1.clear();
    qDbusAbstractInterfaceCallMethod.clear();
    qDbusAbstractInterfaceCallPath.clear();
    qDbusAbstractInterfaceCallService.clear();
    qDbusAbstractInterfaceCallInterface.clear();
    gQWidgetRaiseCalled = false;
    gQWidgetVisible.clear();
    gQWidgetRepaintCalled = false;
    screenLockExtensionReset = false;
    screenLockExtensionModeSet = false;
    gScreenLockWindowStub->stubReset();
    gEventEaterStub->stubReset();
#ifdef HAVE_QMSYSTEM
    qmDisplayState = MeeGo::QmDisplayState::On;
#endif
}

void Ut_ScreenLockBusinessLogic::initTestCase()
{
    static int argc = 1;
    static char *argv = (char *) "./ut_screenlockbusinesslogic";
    m_App = new MApplication(argc, &argv);
    /* XXX: input context caused a crash :-S */
    m_App->setLoadMInputContext (false);
    m_App->setQuitOnLastWindowClosed (false);
    notifierSink = new NotifierNotificationSink;
    gSysuidStub->stubSetReturnValue("notifierNotificationSink", notifierSink);
}

void Ut_ScreenLockBusinessLogic::cleanupTestCase()
{
    m_App->deleteLater ();
    delete notifierSink;
}

void Ut_ScreenLockBusinessLogic::testToggleScreenLockUI()
{
    ScreenLockExtension screenLockExtension;
    screenLockExtension.initialize("");
    m_subject->registerExtension(&screenLockExtension);

#ifdef HAVE_QMSYSTEM
    // First try with display off
    qmDisplayState = MeeGo::QmDisplayState::Off;
#endif

    // When the lock is toggled on, make sure the lock UI is shown
    m_subject->toggleScreenLockUI(true);

    // The lock screen should now be visible
    QCOMPARE(gQWidgetVisible[m_subject->screenLockWindow], true);

    // Raising should not happen
    QCOMPARE(gQWidgetRaiseCalled, false);

    // The lock screen needs to be reset
    QCOMPARE(screenLockExtensionReset, true);
    screenLockExtensionReset = false;

    // Reset the stubs
    gQWidgetVisible[m_subject->screenLockWindow] = false;

    // Lock the screen again
    m_subject->toggleScreenLockUI(true);

    // show() should not be called
    QCOMPARE(gQWidgetVisible[m_subject->screenLockWindow], false);

    // Raising should not happen
    QCOMPARE(gQWidgetRaiseCalled, false);

    // The lock screen still needs to be reset
    QCOMPARE(screenLockExtensionReset, true);

    // When the lock is toggled off, make sure the lock UI is hidden
    m_subject->toggleScreenLockUI(false);
    QCOMPARE(m_subject->screenLockWindow->isVisible(), false);

    // Extension mode should be set to normal mode
    QCOMPARE(screenLockExtensionModeSet, true);
    QCOMPARE(screenLockExtensionMode, ScreenLockExtensionInterface::NormalMode);
}

void Ut_ScreenLockBusinessLogic::testToggleEventEater()
{
    // Make sure the screen locking signals are sent and the eater UI is shown/hidden
    m_subject->toggleEventEater(true);
    QCOMPARE(gEventEaterStub->stubCallCount("show"), 1);

    m_subject->toggleEventEater(false);
    QCOMPARE(gEventEaterStub->stubCallCount("hide"), 1);
}

void Ut_ScreenLockBusinessLogic::testUnlockScreenWhenLocked()
{
    m_subject->tklock_open(TEST_SERVICE, TEST_PATH, TEST_INTERFACE, TEST_METHOD, ScreenLockBusinessLogic::TkLockModeNone, false, false);
    m_subject->toggleScreenLockUI(true);
    m_subject->toggleEventEater(true);
    m_subject->unlockScreen();

    QVERIFY(m_subject->screenLockWindow != NULL);
    QCOMPARE(m_subject->screenLockWindow->isVisible(), false);
    QCOMPARE(gEventEaterStub->stubCallCount("hide"), 1);

    QCOMPARE(qDbusAbstractInterfaceCallMethod, TEST_METHOD);
    QCOMPARE(qDbusAbstractInterfaceCallPath, TEST_PATH);
    QCOMPARE(qDbusAbstractInterfaceCallService, TEST_SERVICE);
    QCOMPARE(qDbusAbstractInterfaceCallInterface, TEST_INTERFACE);
    QCOMPARE(qDbusAbstractInterfaceCallMode, QDBus::NoBlock);
    QCOMPARE(qDbusAbstractInterfaceCallArg1.toInt(), (int)ScreenLockBusinessLogic::TkLockUnlock);
}

void Ut_ScreenLockBusinessLogic::testUnlockScreenWhenNotLocked()
{
    m_subject->unlockScreen();

    QCOMPARE(qDbusAbstractInterfaceCallMethod.isEmpty(), true);
    QCOMPARE(qDbusAbstractInterfaceCallPath.isEmpty(), true);
    QCOMPARE(qDbusAbstractInterfaceCallService.isEmpty(), true);
    QCOMPARE(qDbusAbstractInterfaceCallInterface.isEmpty(), true);
}

void Ut_ScreenLockBusinessLogic::testHideEventEater()
{
    m_subject->showEventEater();
    m_subject->hideEventEater();
    QCOMPARE(gEventEaterStub->stubCallCount("hide"), 1);
}

#ifdef HAVE_QMSYSTEM
void Ut_ScreenLockBusinessLogic::testDisplayStateChanged()
{
    ScreenLockExtension screenLockExtension;
    screenLockExtension.initialize("");
    m_subject->registerExtension(&screenLockExtension);

    m_subject->toggleScreenLockUI(true);

    // When lock screen UI is shown it should be reset
    QCOMPARE(screenLockExtensionReset, true);
    screenLockExtensionReset = false;

    m_subject->displayStateChanged(MeeGo::QmDisplayState::Off);
    m_subject->displayStateChanged(MeeGo::QmDisplayState::On);

    // It should also be reset after turning the display on
    QCOMPARE(screenLockExtensionReset, true);
}

void Ut_ScreenLockBusinessLogic::testSystemStateChanged()
{
    ScreenLockExtension screenLockExtension;
    screenLockExtension.initialize("");
    m_subject->registerExtension(&screenLockExtension);

    // Show the screen lock window and the event eater
    m_subject->showScreenLock();
    m_subject->showEventEater();

    // When system shuts down the window should be hidden
    m_subject->systemStateChanged(MeeGo::QmSystemState::Shutdown);

    // Both windows should be hidden
    QCOMPARE(gQWidgetVisible.contains(m_subject->screenLockWindow), true);
    QCOMPARE(gEventEaterStub->stubCallCount("hide"), 1);
    QCOMPARE(gQWidgetVisible[m_subject->screenLockWindow], false);

    // Extension mode should be set to normal mode
    QCOMPARE(screenLockExtensionModeSet, true);
    QCOMPARE(screenLockExtensionMode, ScreenLockExtensionInterface::NormalMode);

    // Further tklock_opens should be ignored
    screenLockExtensionModeSet = false;
    m_subject->tklock_open(TEST_SERVICE, TEST_PATH, TEST_INTERFACE, TEST_METHOD, ScreenLockBusinessLogic::TkLockEnableLowPowerMode, false, false);
    QCOMPARE(gQWidgetVisible[m_subject->screenLockWindow], false);
    QCOMPARE(screenLockExtensionModeSet, false);
}
#endif

void Ut_ScreenLockBusinessLogic::testReset()
{
    ScreenLockExtension screenLockExtension;
    screenLockExtension.initialize("");
    m_subject->registerExtension(&screenLockExtension);
    m_subject->reset();
    QVERIFY(screenLockExtensionReset);
}

void Ut_ScreenLockBusinessLogic::testWhenExtensionIsRegisteredSignalsAreConnected()
{
    ScreenLockExtension screenLockExtension;
    screenLockExtension.initialize("");
    m_subject->registerExtension(&screenLockExtension);

    QVERIFY(disconnect(screenLockExtension.qObject(), SIGNAL(unlocked()), m_subject, SLOT(unlockScreen())));
    QVERIFY(disconnect(&Sysuid::instance()->notifierNotificationSink(), SIGNAL(notifierSinkActive(bool)), screenLockExtension.qObject(), SIGNAL(notifierSinkActive(bool))));
}

void Ut_ScreenLockBusinessLogic::testRegisteringAndUnregisteringExtension()
{
    ScreenLockExtension screenLockExtension;
    screenLockExtension.initialize("");
    m_subject->registerExtension(&screenLockExtension);
    QCOMPARE(m_subject->screenLockExtensions.count(), 1);

    m_subject->unregisterExtension(&screenLockExtension);
    QCOMPARE(m_subject->screenLockExtensions.count(), 0);
}

void Ut_ScreenLockBusinessLogic::testTkLockOpen_data()
{
    QTest::addColumn<int>("mode");
    QTest::addColumn<bool>("screenLockWindowVisibilityModified");
    QTest::addColumn<bool>("screenLockWindowVisible");
    QTest::addColumn<bool>("eventEaterWindowVisibilityModified");
    QTest::addColumn<bool>("eventEaterWindowVisible");
    QTest::addColumn<bool>("screenLockModeSet");
    QTest::addColumn<int>("screenLockMode");
    QTest::addColumn<bool>("lowPowerModeWindow");

    QTest::newRow("TkLockModeNone") << (int)ScreenLockBusinessLogic::TkLockModeNone << false << false << false << false << false << 0 << false;
    QTest::newRow("TkLockModeEnable") << (int)ScreenLockBusinessLogic::TkLockModeEnable << true << true << true << false << true << (int)ScreenLockExtensionInterface::NormalMode << false;
    QTest::newRow("TkLockModeHelp") << (int)ScreenLockBusinessLogic::TkLockModeHelp << false << false << false << false << false << 0 << false;
    QTest::newRow("TkLockModeSelect") << (int)ScreenLockBusinessLogic::TkLockModeSelect << false << false << false << false << false << 0 << false;
    QTest::newRow("TkLockModeOneInput") << (int)ScreenLockBusinessLogic::TkLockModeOneInput << false << false << true << true << false << 0 << false;
    QTest::newRow("TkLockEnableVisual") << (int)ScreenLockBusinessLogic::TkLockEnableVisual << true << true << true << false << true << (int)ScreenLockExtensionInterface::NormalMode << false;
    QTest::newRow("TkLockEnableLowPowerMode") << (int)ScreenLockBusinessLogic::TkLockEnableLowPowerMode << true << true << true << false << true << (int)ScreenLockExtensionInterface::LowPowerMode << true;
    QTest::newRow("TkLockRealBlankMode") << (int)ScreenLockBusinessLogic::TkLockRealBlankMode << true << true << true << false << true << (int)ScreenLockExtensionInterface::DisplayOffMode << false;
}

void Ut_ScreenLockBusinessLogic::testTkLockOpen()
{
    QFETCH(int, mode);
    QFETCH(bool, screenLockWindowVisibilityModified);
    QFETCH(bool, screenLockWindowVisible);
    QFETCH(bool, eventEaterWindowVisibilityModified);
    QFETCH(bool, eventEaterWindowVisible);
    QFETCH(bool, screenLockModeSet);
    QFETCH(int, screenLockMode);
    QFETCH(bool, lowPowerModeWindow);

    ScreenLockExtension screenLockExtension;
    screenLockExtension.initialize("");
    m_subject->registerExtension(&screenLockExtension);

    // Make sure the event eater is visible so that it will be hidden if necessary
    m_subject->showEventEater();
    gEventEaterStub->stubReset();

    // Modify the state
    int reply = m_subject->tklock_open(TEST_SERVICE, TEST_PATH, TEST_INTERFACE, TEST_METHOD, mode, false, false);
    QCOMPARE(reply, (int)ScreenLockBusinessLogic::TkLockReplyOk);

    // Check that screen lock window visibility was only changed if needed (and to the correct state)
    QCOMPARE(gQWidgetVisible.contains(m_subject->screenLockWindow), screenLockWindowVisibilityModified);
    if (screenLockWindowVisibilityModified) {
        QCOMPARE(gQWidgetVisible[m_subject->screenLockWindow], screenLockWindowVisible);
    }

    // Check that event eater window visibility was only changed if needed (and to the correct state)
    if (eventEaterWindowVisibilityModified) {
        QCOMPARE(gEventEaterStub->stubCallCount(eventEaterWindowVisible ? "show" : "hide"), 1);
    } else {
        QCOMPARE(gEventEaterStub->stubCallCount("show"), 0);
        QCOMPARE(gEventEaterStub->stubCallCount("hide"), 0);
    }

    QCOMPARE(screenLockExtensionModeSet, screenLockModeSet);
    if (screenLockModeSet) {
        QCOMPARE((int)screenLockExtensionMode, screenLockMode);
    }

    if(screenLockMode >= ScreenLockBusinessLogic::TkLockEnableVisual && screenLockMode <= ScreenLockBusinessLogic::TkLockRealBlankMode) {
        QCOMPARE(gScreenLockWindowStub->stubLastCallTo("setLowPowerMode").parameter<bool>(0), lowPowerModeWindow);
    }
}

void Ut_ScreenLockBusinessLogic::testTkLockClose()
{
    // Show the screen lock window and the event eater
    m_subject->showScreenLock();
    m_subject->showEventEater();

    // Modify the state
    int reply = m_subject->tklock_close(false);
    QCOMPARE(reply, (int)ScreenLockBusinessLogic::TkLockReplyOk);

    // Both windows should be hidden
    QCOMPARE(gQWidgetVisible.contains(m_subject->screenLockWindow), true);
    QCOMPARE(gEventEaterStub->stubCallCount("hide"), 1);
    QCOMPARE(gQWidgetVisible[m_subject->screenLockWindow], false);
}

QTEST_APPLESS_MAIN(Ut_ScreenLockBusinessLogic)
