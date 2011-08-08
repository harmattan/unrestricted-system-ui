/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of system ui.
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

#include "ut_sysuid.h"

#include <QtTest/QtTest>
#include <QDBusConnection>
#include <MApplication>
#include <MLocale>
#include "batterybusinesslogic_stub.h"
#include "lockscreen_stub.h"
#include "statusarearenderer_stub.h"
#include "statusarearendereradaptor_stub.h"
#include "screenlockbusinesslogic_stub.h"
#include "screenlockbusinesslogicadaptor_stub.h"
#include "contextframeworkitem_stub.h"
#include "widgetnotificationsink_stub.h"
#include "eventtypestore_stub.h"
#include "notificationmanager_stub.h"
#include "notification_stub.h"
#include "notificationgroup_stub.h"
#include "unlockarea_stub.h"
#include "screenlockbusinesslogic_stub.h"
#include "shutdownbusinesslogic_stub.h"
#include "shutdownbusinesslogicadaptor_stub.h"
#include "statusindicatormenubusinesslogic_stub.h"
#include "statusindicatormenuadaptor_stub.h"
#include "mcompositornotificationsink_stub.h"
#include "unlocknotifications_stub.h"
#include "unlockmissedevents_stub.h"
#include "unlocknotificationsinkstub.h"
#include "notificationstatusindicatorsink_stub.h"
#include "shutdownui_stub.h"
#include "usbui_stub.h"
#include "lockscreenwindow_stub.h"
#include "x11wrapper_modified_stub.h"
#include "eventeater_stub.h"
#include "closeeventeater_stub.h"
#include "diskspacenotifier_stub.h"
#include "lowmemorynotifier_stub.h"
#include "ngfnotificationsink.h"
#include "testcontextitem.h"
#include "sysuid.h"

bool gQmLocksDeviceLock = false;
bool gQmLocksScreenLock = false;
bool gQmDisplayStateOff = false;

#ifdef HAVE_QMSYSTEM
namespace MeeGo
{

QmLocks::State QmLocks::getState(QmLocks::Lock what) const {
    if(what == QmLocks::Device)
        return gQmLocksDeviceLock ? QmLocks::Locked : QmLocks::Unlocked;
    else if(what == QmLocks::TouchAndKeyboard)
        return gQmLocksScreenLock ? QmLocks::Locked : QmLocks::Unlocked;
    else
        return QmLocks::Unknown;
}

bool QmLocks::setState(QmLocks::Lock, QmLocks::State) {
    return false;
}

bool QmLocks::setDeviceAutolockTime(int) {
    return false;
}

int QmLocks::getDeviceAutolockTime() {
    return 0;
}
}
#endif

NotificationParameters::NotificationParameters()
{
}

NotificationParameters::~NotificationParameters()
{
}

// NotificationSink stubs (used by MCompositorNotificationSink)
NotificationSink::NotificationSink(QObject *)
{
}

void NotificationSink::addGroup(uint, const NotificationParameters &)
{
}

void NotificationSink::removeGroup(uint)
{
}

bool Ut_SysuidCompositorNotificationState = false;
bool Ut_SysuidFeedbackNotificationState = false;

void NotificationSink::setApplicationEventsEnabled(bool enabled)
{
    if(dynamic_cast<MCompositorNotificationSink*>(this) != NULL) {
        Ut_SysuidCompositorNotificationState = enabled;
    } else if(dynamic_cast<NGFNotificationSink*>(this) != NULL) {
        Ut_SysuidFeedbackNotificationState = enabled;
    }
}

// NGFNotificationSink stubs (used by Sysuid)
NGFNotificationSink *mNGFNotificationSink = 0;
NGFNotificationSink::NGFNotificationSink(QObject *parent) : NotificationSink(parent), adapter(NULL)
{
    mNGFNotificationSink = this;
}

NGFNotificationSink::~NGFNotificationSink()
{
}

void NGFNotificationSink::addNotification(const Notification &)
{
}

void NGFNotificationSink::removeNotification(uint)
{
}

// QDBusConnection stubs (used by Sysuid)
bool QDBusConnection::registerService(QString const &)
{
    return true;
}

bool QDBusConnection::registerObject(QString const &, QObject *, QFlags<QDBusConnection::RegisterOption>)
{
    return true;
}

QHash<const MLocale*, QSet<QString> > gInstalledTranslationCatalogs;
void MLocale::installTrCatalog(const QString &name)
{
    gInstalledTranslationCatalogs[this].insert(name);
}
const MLocale *gDefaultLocale;
void MLocale::setDefault(const MLocale &locale)
{
    gDefaultLocale = &locale;
}

// ContextFrameworkContext stub
TestContextItem *testContextItem;
ContextItem *ContextFrameworkContext::createContextItem(const QString&)
{
    testContextItem = new TestContextItem;
    return testContextItem;
}

void Ut_Sysuid::initTestCase()
{
    static char *args[] = {(char *) "./ut_sysuid"};
    static int argc = sizeof(args) / sizeof(char *);
    app = new MApplication(argc, args);
}

void Ut_Sysuid::cleanupTestCase()
{
    delete app;
}

void Ut_Sysuid::init()
{
    gInstalledTranslationCatalogs.clear();
    gDefaultLocale = NULL;
    sysuid = new Sysuid(NULL);
    Ut_SysuidCompositorNotificationState = false;
    Ut_SysuidFeedbackNotificationState = false;
    gQmLocksDeviceLock = false;
    gQmLocksScreenLock = false;
    gMCompositorNotificationSinkStub->stubReset();
}

void Ut_Sysuid::cleanup()
{
    delete sysuid;
    gStatusIndicatorMenuBusinessLogicStub->stubReset();
}

void Ut_Sysuid::testInitialization()
{
    QCOMPARE(gNotificationManagerStub->stubCallCount("initializeStore"), 1);
}

void Ut_Sysuid::testSignalConnections()
{
    QVERIFY(disconnect(sysuid->statusIndicatorMenuBusinessLogic, SIGNAL(statusIndicatorMenuVisibilityChanged(bool)), sysuid, SLOT(updateCompositorNotificationSinkEnabledStatus())));
    QVERIFY(disconnect(sysuid->notificationManager, SIGNAL(notificationUpdated (const Notification &)), sysuid->mCompositorNotificationSink, SLOT(addNotification (const Notification &))));
    QVERIFY(disconnect(sysuid->notificationManager, SIGNAL(notificationRemoved(uint)), sysuid->mCompositorNotificationSink, SLOT(removeNotification(uint))));
    QVERIFY(disconnect(sysuid->mCompositorNotificationSink, SIGNAL(notificationRemovalRequested(uint)), sysuid->notificationManager, SLOT(removeNotification(uint))));
    QVERIFY(disconnect(sysuid->notificationManager, SIGNAL(notificationUpdated (const Notification &)), sysuid->ngfNotificationSink, SLOT(addNotification (const Notification &))));
    QVERIFY(disconnect(sysuid->notificationManager, SIGNAL(notificationRemoved(uint)), sysuid->ngfNotificationSink, SLOT(removeNotification(uint))));
    QVERIFY(disconnect(sysuid->notificationManager, SIGNAL(notificationUpdated(const Notification &)), sysuid->notificationStatusIndicatorSink_, SLOT(addNotification(const Notification &))));
    QVERIFY(disconnect(sysuid->notificationManager, SIGNAL(notificationRemoved(uint)), sysuid->notificationStatusIndicatorSink_, SLOT(removeNotification(uint))));
    QVERIFY(disconnect(sysuid->notificationManager, SIGNAL(notificationRestored(const Notification &)), sysuid->notificationStatusIndicatorSink_, SLOT(addNotification(const Notification &))));
    QVERIFY(disconnect(sysuid->notificationManager, SIGNAL(groupUpdated(uint, const NotificationParameters &)), sysuid->notificationStatusIndicatorSink_, SLOT(addGroup(uint, const NotificationParameters &))));
    QVERIFY(disconnect(sysuid->screenLockBusinessLogic, SIGNAL(screenIsLocked(bool)), sysuid, SLOT(updateCompositorNotificationSinkEnabledStatus())));
    QVERIFY(disconnect(sysuid->screenLockBusinessLogic, SIGNAL(screenIsLocked(bool)), sysuid->mCompositorNotificationSink, SLOT(setTouchScreenLockActive(bool))));
    QVERIFY(disconnect(sysuid->screenLockBusinessLogic, SIGNAL(screenIsLocked(bool)), sysuid->batteryBusinessLogic, SLOT(setTouchScreenLockActive(bool))));
    QVERIFY(disconnect(sysuid->usbUi, SIGNAL(dialogShown()), sysuid->screenLockBusinessLogic, SLOT(unlockScreen())));

#ifdef HAVE_QMSYSTEM
    QVERIFY(disconnect(&sysuid->qmLocks, SIGNAL(stateChanged (MeeGo::QmLocks::Lock, MeeGo::QmLocks::State)), sysuid, SLOT(updateCompositorNotificationSinkEnabledStatus())));
#endif
}

void Ut_Sysuid::testUseMode()
{
    testContextItem->setValue("");
    QVERIFY(Ut_SysuidCompositorNotificationState);
    QVERIFY(Ut_SysuidFeedbackNotificationState);
    testContextItem->setValue("videorecording");
    QVERIFY(!Ut_SysuidCompositorNotificationState);
    QVERIFY(!Ut_SysuidFeedbackNotificationState);
}

void Ut_Sysuid::testLocaleContainsNotificationCatalog()
{
    QVERIFY(gDefaultLocale != NULL);
    QCOMPARE(gInstalledTranslationCatalogs.contains(gDefaultLocale), true);
    QCOMPARE(gInstalledTranslationCatalogs[gDefaultLocale].contains("notification"), true);
}

void Ut_Sysuid::testWhenLockStateOrStatusIndicatorMenuVisibilityChangesThenCompositorSinkIsDisabled_data()
{
    QTest::addColumn<int>("state");
    QTest::addColumn<bool>("sinkDisabled");

#ifdef HAVE_QMSYSTEM
    int rows = 8;
#else
    int rows = 2;
#endif

    for(int i = 0; i < rows; i++) {
        QTest::newRow(QString::number(i).toAscii()) << i << (i != 0);
    }
}

void Ut_Sysuid::testWhenLockStateOrStatusIndicatorMenuVisibilityChangesThenCompositorSinkIsDisabled()
{
    QFETCH(int, state);
    QFETCH(bool, sinkDisabled);

    gStatusIndicatorMenuBusinessLogicStub->stubSetReturnValue("isStatusIndicatorMenuVisible", (state & 1) != 0);
    gQmLocksDeviceLock = state & 2;
    gScreenLockBusinessLogicStub->stubSetReturnValue("isScreenLocked", (state & 4) != 0);

    sysuid->updateCompositorNotificationSinkEnabledStatus();
    QCOMPARE(gMCompositorNotificationSinkStub->stubCallCount("setApplicationEventsDisabled"), 1);
    QCOMPARE(gMCompositorNotificationSinkStub->stubLastCallTo("setApplicationEventsDisabled").parameter<bool>(0), sinkDisabled);
}

QTEST_APPLESS_MAIN(Ut_Sysuid)
