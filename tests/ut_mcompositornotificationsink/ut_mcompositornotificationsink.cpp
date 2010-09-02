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

#include "ut_mcompositornotificationsink.h"

#include <QtTest/QtTest>
#include <MRemoteAction>
#include <MBanner>
#include <MApplication>
#include <MScene>
#include <MSceneManager>
#include "mcompositornotificationsink.h"
#include "notificationwidgetparameterfactory.h"
#include "testnotificationparameters.h"
#include "eventtypestore_stub.h"
#include "notificationmanager_stub.h"
#include <QSettings>
#include "sysuid_stub.h"
#include "notificationmanager.h"
#include "eventtypestore.h"
#include "genericnotificationparameterfactory.h"
#include <QSettings>
#include <MOnDisplayChangeEvent>
#include <MGConfItem>

static const QString NOTIFICATION_PREVIEW_ENABLED = "/desktop/meego/notifications/previews_enabled";

// Mock notification manager (used by MCompositorNotificationSink)
MockNotificationManager::MockNotificationManager() :
    nextAvailableNotificationID(0)
{
}

uint MockNotificationManager::addNotification(uint, const NotificationParameters &parameters, uint groupId, int timeout)
{
    uint notificationId = nextAvailableNotificationID++;
    Notification notification;
    if (parameters.value("class") == QString("system")) {
        notification = Notification(notificationId, groupId, 0, parameters, Notification::SystemEvent, timeout);
    } else {
        notification = Notification(notificationId, groupId, 0, parameters, Notification::ApplicationEvent, timeout);
    }
    notifications.append(notification);
    emit notificationUpdated(notification);
    return notificationId;
}

uint MockNotificationManager::addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId)
{
    return addNotification(notificationUserId, parameters, groupId, 1000);
}

bool MockNotificationManager::updateNotification(uint, uint notificationId, const NotificationParameters &parameters)
{
    for (int i = 0; i < notifications.count(); ++i) {
        if (notifications.at(i).notificationId() == notificationId) {
            Notification oldNotification = notifications.at(i);
            Notification newNotification = Notification(notificationId, oldNotification.groupId(), oldNotification.userId(), parameters, oldNotification.type(), 1000);
            notifications[i] = newNotification;
            emit notificationUpdated(newNotification);
            return true;
        }
    }
    return false;
}

bool MockNotificationManager::removeNotification(uint, uint notificationId)
{
    bool removed = false;
    for (int i = 0; i < notifications.count(); ++i) {
        if (notifications.at(i).notificationId() == notificationId) {
            notifications.removeAt(i);
            removed = true;
            break;
        }
    }

    if (removed) {
        emit notificationRemoved(notificationId);
    }

    return removed;
}

uint MockNotificationManager::addGroup(uint, const NotificationParameters &)
{
    return 0;
}

bool MockNotificationManager::updateGroup(uint, uint, const NotificationParameters &)
{
    return false;
}

bool MockNotificationManager::removeGroup(uint, uint)
{
    return false;
}

uint MockNotificationManager::notificationUserId()
{
    return 0;
}

QList< uint > MockNotificationManager::notificationIdList(uint /*notificationUserId*/)
{
    QList<uint> tmp;
    return tmp;
}

QList<Notification> MockNotificationManager::notificationList(uint)
{
    return QList<Notification>();
}

QList<NotificationGroup> MockNotificationManager::notificationGroupList(uint)
{
    return QList<NotificationGroup>();
}


// QWidget stubs (used by MCompositorNotificationSink)
bool Ut_WindowShown = false;
QWidget* notificationWindow = NULL;
void QWidget::setVisible(bool visible)
{
    Ut_WindowShown = visible;
    notificationWindow = this;
}

// MGConfItem stub
QVariant gconfValue = QVariant();

QVariant MGConfItem::value() const {
    return gconfValue;
}

QVariant MGConfItem::value(const QVariant &def) const {
    if (gconfValue.isValid())
        return gconfValue;
    else
        return def;
}

void MGConfItem::set(const QVariant &val) {
    gconfValue = val;
}

void MGConfItem::unset() {
    gconfValue = QVariant();
}

MSceneWindow *infoBanner = NULL;
bool Ut_DestroyWhenDonePolicy = false;
void MSceneManager::appearSceneWindow(MSceneWindow *sceneWindow, MSceneWindow::DeletionPolicy policy)
{
    infoBanner = sceneWindow;
    if(policy == MSceneWindow::DestroyWhenDone) {
        Ut_DestroyWhenDonePolicy = true;
    }
}

bool Ut_DisappearSceneWindow= false;
MSceneWindow *disappearingWindow= NULL;
void MSceneManager::disappearSceneWindow(MSceneWindow *sceneWindow)
{
    Ut_DisappearSceneWindow= true;
    disappearingWindow = sceneWindow;
}

M::OrientationAngle gCurrentOrientationAngle;
M::OrientationAngle MSceneManager::orientationAngle() const
{
    return gCurrentOrientationAngle;
}

// QTimer stubs (used by MCompositorNotificationSink)
bool Ut_TimerStarted = false;
void QTimer::start(int msec)
{
    Ut_TimerStarted = true;
    Q_UNUSED(msec);
}

// MSceneWindow stubs (used by MCompositorNotificationSink)
void MSceneWindow::disappear()
{
    emit disappeared();
}

// Tests
void Ut_MCompositorNotificationSink::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_mcompositornotificationsink";
    app = new MApplication(argc, &app_name);
}

void Ut_MCompositorNotificationSink::cleanupTestCase()
{
    delete app;
}

void Ut_MCompositorNotificationSink::init()
{
    gconfValue = QVariant();
    notificationManager = new MockNotificationManager();
    sink = new MCompositorNotificationSink();
    connect(notificationManager, SIGNAL(notificationRemoved(uint)), sink, SLOT(removeNotification(uint)));
    connect(notificationManager, SIGNAL(notificationUpdated(Notification)), sink, SLOT(addNotification(Notification)));
    Ut_WindowShown = false;
    notificationWindow = NULL;
    infoBanner = NULL;
    Ut_DestroyWhenDonePolicy = false;
    Ut_TimerStarted = false;
    Ut_DisappearSceneWindow = false;
    disappearingWindow = NULL;
    gCurrentOrientationAngle = M::Angle0;
}

void Ut_MCompositorNotificationSink::cleanup()
{
    delete sink;
    delete notificationManager;
}

NotificationParameters Ut_MCompositorNotificationSink::setupSinkDisabledTests(bool isSystemEvent)
{
    connect(this, SIGNAL(statusIndictorMenuVisibilityChanged(bool)), sink, SLOT(setDisabled(bool)));
    emit statusIndictorMenuVisibilityChanged(true);
    // Create notification
    TestNotificationParameters parameters("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    if (isSystemEvent) {
        parameters.add(GenericNotificationParameterFactory::classKey(), "system");
    }
    notificationManager->addNotification(0, parameters);
    return parameters;
}

void Ut_MCompositorNotificationSink::testAddNotification()
{
    QSignalSpy spy(sink, SIGNAL(notificationAdded(const Notification&)));

    // Create a notification
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0, 0);

    // Check that a MWindow was created and shown(window->show() called)
    QCOMPARE(Ut_WindowShown, true);

    // Check that the window doesn't request focus
    QCOMPARE(notificationWindow->testAttribute(Qt::WA_X11DoNotAcceptFocus), true);

    MOnDisplayChangeEvent* event = new MOnDisplayChangeEvent(true, QRectF(0,0,1,1));
    QApplication::sendEvent(notificationWindow,event);

    // Check that a MBanner was created with the given parameters
    QCOMPARE(Ut_DestroyWhenDonePolicy, true);

    MBanner *banner = static_cast<MBanner*>(infoBanner);
    QCOMPARE(banner->iconID(), QString("buttonicon0"));
    QCOMPARE(banner->title(), QString("title0"));
    QCOMPARE(banner->subtitle(), QString("subtitle0"));

    // check that the timeout timer was started
    QCOMPARE(Ut_TimerStarted, true);

    // check that the transfer notification signal was emitted
    QCOMPARE(spy.count(), 1);
}

void Ut_MCompositorNotificationSink::testUpdateNotification()
{
    // Create two notifications
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0);
    MOnDisplayChangeEvent* event = new MOnDisplayChangeEvent(true, QRectF(0,0,1,1));
    QApplication::sendEvent(notificationWindow,event);
    MBanner *banner1 = static_cast<MBanner*>(infoBanner);

    TestNotificationParameters parameters1( "title1", "subtitle1", "buttonicon1", "content1 1 1 1");
    uint id = notificationManager->addNotification(0, parameters1);
    // Window will not drop to displaychange event slot unless window is explicitly hidden
    notificationWindow->hide();
    QApplication::sendEvent(notificationWindow,event);
    MBanner *banner2 = static_cast<MBanner*>(infoBanner);

    // Update the second notification
    TestNotificationParameters parametersX("titleX", "subtitleX", "buttoniconX", "contentX X X X");
    notificationManager->updateNotification(0, id, parametersX);

    QCOMPARE(banner2->title(), QString("titleX"));
    QCOMPARE(banner2->subtitle(), QString("subtitleX"));
    QCOMPARE(banner2->iconID(), QString("buttoniconX"));

    // Make sure the fist notification banner has still the same content
    QCOMPARE(banner1->title(), QString("title0"));
    QCOMPARE(banner1->subtitle(), QString("subtitle0"));
    QCOMPARE(banner1->iconID(), QString("buttonicon0"));
}

void Ut_MCompositorNotificationSink::testRemoveNotification()
{
    // Create three notifications
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    parameters0.add(GenericNotificationParameterFactory::classKey(), "system");
    notificationManager->addNotification(0, parameters0, 0);
    MOnDisplayChangeEvent* event = new MOnDisplayChangeEvent(true, QRectF(0,0,1,1));
    QApplication::sendEvent(notificationWindow,event);

    TestNotificationParameters parameters1("title1", "subtitle1", "buttonicon1", "content1 1 1 1");
    uint id = notificationManager->addNotification(0, parameters1);
    // Window will not drop to displaychange event slot unless window is explicitly hidden
    notificationWindow->hide();
    QApplication::sendEvent(notificationWindow,event);
    MBanner *banner = static_cast<MBanner*>(infoBanner);

    TestNotificationParameters parameters2("title2", "subtitle2", "buttonicon2", "");
    notificationManager->addNotification(0, parameters2);
    // Window will not drop to displaychange event slot unless window is explicitly hidden
    notificationWindow->hide();
    QApplication::sendEvent(notificationWindow,event);

    // Remove the second one
    notificationManager->removeNotification(0, id);
    // Check that the window was removed
    QCOMPARE(banner, disappearingWindow);
}

void Ut_MCompositorNotificationSink::testTimeout()
{
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    uint notificationId = notificationManager->addNotification(0, parameters0);
    MOnDisplayChangeEvent* event = new MOnDisplayChangeEvent(true, QRectF(0,0,1,1));
    QApplication::sendEvent(notificationWindow,event);

    connect(this, SIGNAL(timeout()), sink, SLOT(timeout()));
    setProperty("notificationId", notificationId);
    emit(timeout());
    // Check that the window disappeared
    QCOMPARE(infoBanner, disappearingWindow);
    QCOMPARE(Ut_DisappearSceneWindow, true);
}

void Ut_MCompositorNotificationSink::testNotificationWhileApplicationEventsDisabled()
{
    // Create notification
    TestNotificationParameters parameters("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    sink->setApplicationEventsEnabled(false);
    notificationManager->addNotification(0, parameters);
    QCOMPARE(Ut_WindowShown, false);

    sink->setApplicationEventsEnabled(true);
    notificationManager->addNotification(0, parameters);
    QCOMPARE(Ut_WindowShown, true);
}

void Ut_MCompositorNotificationSink::testWhenSinkDisableTrueNoBannerCreated()
{
    NotificationParameters parameters = setupSinkDisabledTests();
    QCOMPARE(Ut_WindowShown, false);
    emit statusIndictorMenuVisibilityChanged(false);
    notificationManager->addNotification(0, parameters);
    MOnDisplayChangeEvent* event = new MOnDisplayChangeEvent(true, QRectF(0,0,1,1));
    QApplication::sendEvent(notificationWindow,event);
    QCOMPARE(Ut_WindowShown, true);
}

void Ut_MCompositorNotificationSink::testWhenSinkIsSetToDisabledSystemNotificationsAreStillGenerated()
{
    setupSinkDisabledTests(true);
    // Check that notification is shown
    QCOMPARE(Ut_WindowShown, true);
}

void Ut_MCompositorNotificationSink::testNotificationPreviewsDisabled_data()
{
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<bool>("windowshown");

    QTest::newRow("key not set, notification shown") << QVariant() << true;
    QTest::newRow("key has garbage value, notification shown") << QVariant(QString("Garbage")) << true;
    QTest::newRow("key has has previews enabled, window shown") << QVariant(true) << true;
    QTest::newRow("key has has previews disabled, window not shown") << QVariant(false) << false;
}


void Ut_MCompositorNotificationSink::testNotificationPreviewsDisabled()
{
    // Reset previews as not disabled
    sink->allPreviewsDisabled = false;

    // Check that window is following right gconf key
    QVERIFY (sink->notificationPreviewMode->key() == NOTIFICATION_PREVIEW_ENABLED);

    QFETCH(QVariant, value);
    QFETCH(bool, windowshown);

    // Set value from test data to gconf key
    sink->notificationPreviewMode->set(value);
    sink->changeNotificationPreviewMode();

    // Create normal notification
    TestNotificationParameters parameters("title0", "subtitle0", "buttonicon0", "content0 0 0 0");

    // Check that notification is shown/not shown according to test data parameter
    notificationManager->addNotification(0, parameters);
    QCOMPARE(Ut_WindowShown, windowshown);

    // Create system notification
    parameters.add(GenericNotificationParameterFactory::classKey(), "system");
    notificationManager->addNotification(0, parameters);

    // Check that notification is not shown
    QCOMPARE(Ut_WindowShown, windowshown);
}

void Ut_MCompositorNotificationSink::testWindowMasking_data()
{
    QTest::addColumn<M::OrientationAngle>("angle");
    QTest::newRow("0")   << M::Angle0;
    QTest::newRow("90")  << M::Angle90;
    QTest::newRow("180") << M::Angle180;
    QTest::newRow("270") << M::Angle270;
}

void Ut_MCompositorNotificationSink::testWindowMasking()
{
    QFETCH(M::OrientationAngle, angle);
    gCurrentOrientationAngle = angle;

    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0, 0);

    MOnDisplayChangeEvent* event = new MOnDisplayChangeEvent(true, QRectF(0,0,1,1));
    QApplication::sendEvent(notificationWindow, event);

    QRect maskRect;
    switch(angle) {
    case M::Angle0:   maskRect = QRect(0, 0, infoBanner->preferredWidth(), infoBanner->preferredHeight()); break;
    case M::Angle90:  maskRect = QRect(notificationWindow->width() - infoBanner->preferredHeight(), 0, infoBanner->preferredHeight(), infoBanner->preferredWidth()); break;
    case M::Angle180: maskRect = QRect(0, notificationWindow->height() - infoBanner->preferredHeight(), infoBanner->preferredWidth(), infoBanner->preferredHeight()); break;
    case M::Angle270: maskRect = QRect(0, 0, infoBanner->preferredHeight(), infoBanner->preferredWidth()); break;
    }

    QCOMPARE(notificationWindow->mask().boundingRect(), maskRect);
}

QTEST_APPLESS_MAIN(Ut_MCompositorNotificationSink)
