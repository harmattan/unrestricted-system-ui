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
#include <MApplication>
#include <MSceneManager>
#include "mcompositornotificationsink.h"
#include "mnotificationproxy.h"
#include "testnotificationparameters.h"
#include "genericnotificationparameterfactory.h"
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

QList<MNotificationProxy> MockNotificationManager::notificationList(uint)
{
    return QList<MNotificationProxy>();
}

QList<MNotificationGroupProxy> MockNotificationManager::notificationGroupList(uint)
{
    return QList<MNotificationGroupProxy>();
}

bool windowEventFilterCalled = false;
bool windowEventFilterBlock = false;
bool WindowEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    MOnDisplayChangeEvent *displayChangeEvent = dynamic_cast<MOnDisplayChangeEvent *>(event);
    if (displayChangeEvent != NULL) {
        windowEventFilterCalled = true;
        if (windowEventFilterBlock) {
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

// MWindow stubs (used by MCompositorNotificationSink)
bool mWindowSetVisibleValue = false;
QWidget* mWindowSetVisibleWidget = NULL;
void MWindow::setVisible(bool visible)
{
    mWindowSetVisibleValue = visible;
    mWindowSetVisibleWidget = this;

    setAttribute(visible ? Qt::WA_WState_Visible : Qt::WA_WState_Hidden);
}

// QWidget stubs (used by MCompositorNotificationSink)
QRegion mWindowMaskRegion;
void QWidget::setMask(const QRegion &region)
{
    MWindow *window = dynamic_cast<MWindow *>(this);
    if (window != NULL) {
        mWindowMaskRegion = region;
    }
}

// MGConfItem stub
QVariant gconfValue = QVariant();

QVariant MGConfItem::value() const {

    if (key() == "/meegotouch/target/name")
        return QVariant ("Default");

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

MSceneWindow *mSceneManagerAppearSceneWindowWindow = NULL;
bool mSceneManagerAppearSceneWindowPolicy = false;
void MSceneManager::appearSceneWindow(MSceneWindow *sceneWindow, MSceneWindow::DeletionPolicy policy)
{
    mSceneManagerAppearSceneWindowWindow = sceneWindow;
    if(policy == MSceneWindow::DestroyWhenDone) {
        mSceneManagerAppearSceneWindowPolicy = true;
    }
}

MSceneWindow *mSceneManagerDisappearSceneWindowWindow= NULL;
void MSceneManager::disappearSceneWindow(MSceneWindow *sceneWindow)
{
    mSceneManagerDisappearSceneWindowWindow = sceneWindow;
}

M::OrientationAngle gCurrentOrientationAngle;
M::OrientationAngle MSceneManager::orientationAngle() const
{
    return gCurrentOrientationAngle;
}

// QTimer stubs (used by MCompositorNotificationSink)
bool qTimerStarted = false;
void QTimer::start(int)
{
    qTimerStarted = true;
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
    gCurrentOrientationAngle = M::Angle0;
    mWindowSetVisibleValue = false;
    mWindowSetVisibleWidget = NULL;
    mSceneManagerAppearSceneWindowWindow = NULL;
    mSceneManagerAppearSceneWindowPolicy = false;
    qTimerStarted = false;
    mSceneManagerDisappearSceneWindowWindow = NULL;
    windowEventFilterCalled = false;
    windowEventFilterBlock = false;
    mWindowMaskRegion = QRegion();
}

void Ut_MCompositorNotificationSink::cleanup()
{
    delete sink;
    delete notificationManager;
}

void Ut_MCompositorNotificationSink::sendOnDisplayChangeEvent()
{
    // The rectangle does not make a difference - it just needs to be non-empty.
    MOnDisplayChangeEvent* event = new MOnDisplayChangeEvent(true, QRectF(0, 0, 1, 1));
    QApplication::sendEvent(sink->window, event);
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

void Ut_MCompositorNotificationSink::testNotificationWindowProperties()
{
    // Check that the window doesn't request focus
    QCOMPARE(sink->window->testAttribute(Qt::WA_X11DoNotAcceptFocus), true);
}

void Ut_MCompositorNotificationSink::testAddNotificationWhenWindowNotOpen()
{
    QSignalSpy spy(sink, SIGNAL(notificationAdded(const Notification&)));

    // Install an event filter for the window and block MOnDisplayChangeEvents but catch them
    QSharedPointer<WindowEventFilter> windowEventFilter(new WindowEventFilter);
    sink->window->installEventFilter(windowEventFilter.data());
    windowEventFilterBlock = true;

    // Create a notification
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0, 0);

    // Check that a MWindow was shown(window->show() called)
    QCOMPARE(mWindowSetVisibleWidget, sink->window);
    QCOMPARE(mWindowSetVisibleValue, true);

    // Check that the MBanner was not yet shown but an MOnDisplayChangeEvent was sent
    QCOMPARE(windowEventFilterCalled, true);
    QCOMPARE(mSceneManagerAppearSceneWindowWindow, (MSceneWindow *)NULL);

    // Unblock MOnDisplayChangeEvents and check that sending one will create the banner
    windowEventFilterBlock = false;
    sendOnDisplayChangeEvent();

    // Check that a MBanner was created with the given parameters
    QCOMPARE(mSceneManagerAppearSceneWindowPolicy, true);
    MBanner *banner = static_cast<MBanner*>(mSceneManagerAppearSceneWindowWindow);
    QCOMPARE(banner->iconID(), QString("buttonicon0"));
    QCOMPARE(banner->title(), QString("title0"));
    QCOMPARE(banner->subtitle(), QString("subtitle0"));

    // check that the timeout timer was started
    QCOMPARE(qTimerStarted, true);

    // check that the transfer notification signal was emitted
    QCOMPARE(spy.count(), 1);
}

void Ut_MCompositorNotificationSink::testAddNotificationWhenWindowAlreadyOpen()
{
    // Create a notification: this will create a window
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0, 0);

    // Create a banner for the first notification
    sendOnDisplayChangeEvent();

    // Reset stubs to see what happens next
    mWindowSetVisibleWidget = NULL;
    mWindowSetVisibleValue = false;
    qTimerStarted = false;
    mSceneManagerAppearSceneWindowWindow = NULL;
    mSceneManagerAppearSceneWindowPolicy = false;
    QSignalSpy spy(sink, SIGNAL(notificationAdded(const Notification&)));

    // Create another notification
    TestNotificationParameters parameters1("title1", "subtitle1", "buttonicon1", "content1 0 0 0");
    notificationManager->addNotification(1, parameters1, 0);

    // Check that the MWindow was not unnecessarily shown again
    QCOMPARE(mWindowSetVisibleWidget, (QWidget *)NULL);
    QCOMPARE(mWindowSetVisibleValue, false);

    // Check that a MBanner was created with the given parameters
    QCOMPARE(mSceneManagerAppearSceneWindowPolicy, true);
    MBanner *banner = static_cast<MBanner*>(mSceneManagerAppearSceneWindowWindow);
    QCOMPARE(banner->iconID(), QString("buttonicon1"));
    QCOMPARE(banner->title(), QString("title1"));
    QCOMPARE(banner->subtitle(), QString("subtitle1"));

    // check that the timeout timer was started
    QCOMPARE(qTimerStarted, true);

    // check that the transfer notification signal was emitted
    QCOMPARE(spy.count(), 1);
}

void Ut_MCompositorNotificationSink::testUpdateNotification()
{
    // Create two notifications
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0);
    sendOnDisplayChangeEvent();
    MBanner *banner1 = static_cast<MBanner*>(mSceneManagerAppearSceneWindowWindow);

    TestNotificationParameters parameters1( "title1", "subtitle1", "buttonicon1", "content1 1 1 1");
    uint id = notificationManager->addNotification(0, parameters1);
    // Window will not drop to displaychange event slot unless window is explicitly hidden
    mWindowSetVisibleWidget->hide();
    sendOnDisplayChangeEvent();
    MBanner *banner2 = static_cast<MBanner*>(mSceneManagerAppearSceneWindowWindow);

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
    sendOnDisplayChangeEvent();

    TestNotificationParameters parameters1("title1", "subtitle1", "buttonicon1", "content1 1 1 1");
    uint id = notificationManager->addNotification(0, parameters1);
    // Window will not drop to displaychange event slot unless window is explicitly hidden
    mWindowSetVisibleWidget->hide();
    sendOnDisplayChangeEvent();
    MBanner *banner = static_cast<MBanner*>(mSceneManagerAppearSceneWindowWindow);

    TestNotificationParameters parameters2("title2", "subtitle2", "buttonicon2", "");
    notificationManager->addNotification(0, parameters2);
    // Window will not drop to displaychange event slot unless window is explicitly hidden
    mWindowSetVisibleWidget->hide();
    sendOnDisplayChangeEvent();

    // Remove the second one
    notificationManager->removeNotification(0, id);
    // Check that the window was removed
    QCOMPARE(banner, mSceneManagerDisappearSceneWindowWindow);
}

void Ut_MCompositorNotificationSink::testTimeout()
{
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    uint notificationId = notificationManager->addNotification(0, parameters0);
    sendOnDisplayChangeEvent();

    connect(this, SIGNAL(timeout()), sink, SLOT(timeout()));
    setProperty("notificationId", notificationId);
    emit(timeout());

    // Check that the window disappeared
    QVERIFY(mSceneManagerDisappearSceneWindowWindow != NULL);
    QCOMPARE(mSceneManagerAppearSceneWindowWindow, mSceneManagerDisappearSceneWindowWindow);
}

void Ut_MCompositorNotificationSink::testNotificationWhileApplicationEventsDisabled()
{
    // Create notification
    TestNotificationParameters parameters("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    sink->setApplicationEventsEnabled(false);
    notificationManager->addNotification(0, parameters);
    QCOMPARE(mWindowSetVisibleValue, false);

    sink->setApplicationEventsEnabled(true);
    notificationManager->addNotification(0, parameters);
    QCOMPARE(mWindowSetVisibleValue, true);
}

void Ut_MCompositorNotificationSink::testWhenSinkDisableTrueNoBannerCreated()
{
    NotificationParameters parameters = setupSinkDisabledTests();
    QCOMPARE(mWindowSetVisibleValue, false);
    emit statusIndictorMenuVisibilityChanged(false);
    notificationManager->addNotification(0, parameters);
    sendOnDisplayChangeEvent();
    QCOMPARE(mWindowSetVisibleValue, true);
}

void Ut_MCompositorNotificationSink::testWhenSinkIsSetToDisabledSystemNotificationsAreStillGenerated()
{
    setupSinkDisabledTests(true);
    // Check that notification is shown
    QCOMPARE(mWindowSetVisibleValue, true);
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
    QCOMPARE(mWindowSetVisibleValue, windowshown);

    // Create system notification
    parameters.add(GenericNotificationParameterFactory::classKey(), "system");
    notificationManager->addNotification(0, parameters);

    // Check that notification is not shown
    QCOMPARE(mWindowSetVisibleValue, windowshown);
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

    sendOnDisplayChangeEvent();

    QRect maskRect;
    switch(angle) {
    case M::Angle0:   maskRect = QRect(0, 0, mSceneManagerAppearSceneWindowWindow->preferredWidth(), mSceneManagerAppearSceneWindowWindow->preferredHeight()); break;
    case M::Angle90:  maskRect = QRect(sink->window->width() - mSceneManagerAppearSceneWindowWindow->preferredHeight(), 0, mSceneManagerAppearSceneWindowWindow->preferredHeight(), mSceneManagerAppearSceneWindowWindow->preferredWidth()); break;
    case M::Angle180: maskRect = QRect(0, sink->window->height() - mSceneManagerAppearSceneWindowWindow->preferredHeight(), mSceneManagerAppearSceneWindowWindow->preferredWidth(), mSceneManagerAppearSceneWindowWindow->preferredHeight()); break;
    case M::Angle270: maskRect = QRect(0, 0, mSceneManagerAppearSceneWindowWindow->preferredHeight(), mSceneManagerAppearSceneWindowWindow->preferredWidth()); break;
    }

    QRegion region(maskRect, QRegion::Rectangle);
    QCOMPARE(mWindowMaskRegion, region);
}

QTEST_APPLESS_MAIN(Ut_MCompositorNotificationSink)
