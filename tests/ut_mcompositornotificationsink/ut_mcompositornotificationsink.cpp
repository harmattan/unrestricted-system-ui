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
#include <MOnDisplayChangeEvent>
#include "mcompositornotificationsink.h"
#include "mnotificationproxy.h"
#include "testnotificationparameters.h"
#include "genericnotificationparameterfactory.h"
#include <MGConfItem>
#include <X11/extensions/shape.h>
#include "x11wrapper.h"

#ifdef HAVE_QMSYSTEM
#include <qmdisplaystate.h>
#endif

static const QString NOTIFICATION_PREVIEW_ENABLED = "/desktop/meego/notifications/previews_enabled";

static QList<MSceneWindow*> gMSceneWindowsAppeared;
static QList<MSceneWindow::DeletionPolicy> gMSceneWindowDeletionPolicies;

MSceneWindowBridge::MSceneWindowBridge(QObject *parent): QObject(parent)
{
}

void MSceneWindowBridge::setSceneWindowState(MSceneWindow::SceneWindowState newState)
{
    QObject *testInterface = children()[0];

    QMetaObject::invokeMethod(testInterface, "setSceneWindowState", Qt::DirectConnection,
            Q_ARG(MSceneWindow::SceneWindowState, newState));
}

bool gMWindowIsOnDisplay = false;
bool MWindow::isOnDisplay() const
{
    return gMWindowIsOnDisplay;
}


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
    notificationContainer.append(notification);
    emit notificationUpdated(notification);
    return notificationId;
}

uint MockNotificationManager::addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId)
{
    return addNotification(notificationUserId, parameters, groupId, 1000);
}

bool MockNotificationManager::updateNotification(uint, uint notificationId, const NotificationParameters &parameters)
{
    for (int i = 0; i < notificationContainer.count(); ++i) {
        if (notificationContainer.at(i).notificationId() == notificationId) {
            Notification oldNotification = notificationContainer.at(i);
            Notification newNotification = Notification(notificationId, oldNotification.groupId(), oldNotification.userId(), parameters, oldNotification.type(), 1000);
            notificationContainer[i] = newNotification;
            emit notificationUpdated(newNotification);
            return true;
        }
    }
    return false;
}

bool MockNotificationManager::removeNotification(uint, uint notificationId)
{
    bool removed = false;
    for (int i = 0; i < notificationContainer.count(); ++i) {
        if (notificationContainer.at(i).notificationId() == notificationId) {
            notificationContainer.removeAt(i);
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

QList<Notification> MockNotificationManager::notificationListWithIdentifiers(uint)
{
    return QList<Notification>();
}

QList<NotificationGroup> MockNotificationManager::notificationGroupListWithIdentifiers(uint)
{
    return QList<NotificationGroup>();
}

QList<Notification> MockNotificationManager::notifications() const
{
    QList<Notification> tmp;
    return tmp;
}

QList<NotificationGroup> MockNotificationManager::groups() const
{
    QList<NotificationGroup> tmp;
    return tmp;
}

uint MockNotificationManager::notificationCountInGroup(uint, uint)
{
    return 0;
}

QObject* MockNotificationManager::qObject()
{
    return NULL;
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

    setAttribute(Qt::WA_WState_Visible, visible);
    setAttribute(Qt::WA_WState_Hidden, !visible);
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

#ifdef HAVE_QMSYSTEM
bool gQmDisplayStateOff = false;

namespace MeeGo
{
QmDisplayState::DisplayState QmDisplayState::get() const {
    return gQmDisplayStateOff ? QmDisplayState::Off : QmDisplayState::On;
}

bool QmDisplayState::set(QmDisplayState::DisplayState) {
    return false;
}
}
#endif

QList<QGraphicsItem *> QGraphicsScene::items () const
{
    QList<QGraphicsItem*> items;
    if (gMSceneWindowsAppeared.size() > 0) {
        items.append(gMSceneWindowsAppeared.at(0));
    }
    return items;
}

void MSceneManager::appearSceneWindow(MSceneWindow *sceneWindow, MSceneWindow::DeletionPolicy policy)
{
    gMSceneWindowsAppeared.append(sceneWindow);
    gMSceneWindowDeletionPolicies.append(policy);
    MSceneWindowBridge bridge;
    bridge.setObjectName("_m_testBridge");
    bridge.setParent(sceneWindow);
    bridge.setSceneWindowState(MSceneWindow::Appeared);
}

MSceneWindow *mSceneManagerDisappearSceneWindowWindow = NULL;
void MSceneManager::disappearSceneWindow(MSceneWindow *sceneWindow)
{
    mSceneManagerDisappearSceneWindowWindow = sceneWindow;
}

void MSceneManager::disappearSceneWindowNow(MSceneWindow *sceneWindow)
{
    mSceneManagerDisappearSceneWindowWindow = sceneWindow;

    MSceneWindowBridge bridge;
    bridge.setObjectName("_m_testBridge");
    bridge.setParent(sceneWindow);
    bridge.setSceneWindowState(MSceneWindow::Disappeared);
}

M::OrientationAngle gCurrentOrientationAngle;
M::OrientationAngle MSceneManager::orientationAngle() const
{
    return gCurrentOrientationAngle;
}

void MSceneManager::setOrientationAngle(M::OrientationAngle angle, TransitionMode mode)
{
    Q_UNUSED(mode);
    M::Orientation o = (angle == M::Angle0 || angle == M::Angle180) ? M::Landscape : M::Portrait;

    emit orientationAboutToChange(o);
    emit orientationChangeFinished(o);
}

// QTimer stubs (used by MCompositorNotificationSink)
bool qTimerStarted = false;
bool qQTimerEmitTimeoutImmediately = true;
void QTimer::start(int)
{
    qTimerStarted = true;
    if (qQTimerEmitTimeoutImmediately) {
        emit timeout();
    }
}

bool gQTimerStopped = false;
void QTimer::stop()
{
    gQTimerStopped = true;
}

QMap<Window, QMap<Atom, int> > gWindowPropertyMap;
QSet<int*> gXAllocs;

Atom _MEEGOTOUCH_CURRENT_APP_WINDOW = 1;
Atom _MEEGOTOUCH_NOTIFICATION_PREVIEWS_DISABLED = 2;
Window ROOT_WINDOW_ID = 0;

Qt::HANDLE QX11Info::appRootWindow(int screen)
{
    Q_UNUSED(screen);

    return ROOT_WINDOW_ID;
}

Atom X11Wrapper::XInternAtom(Display *display, const char *atom_name, Bool only_if_exists)
{
    Q_UNUSED(display);
    Q_UNUSED(only_if_exists);

    Atom atom = -1;

    if (QString(atom_name) == "_MEEGOTOUCH_CURRENT_APP_WINDOW") {
        atom = _MEEGOTOUCH_CURRENT_APP_WINDOW;
    }

    if (QString(atom_name) == "_MEEGOTOUCH_NOTIFICATION_PREVIEWS_DISABLED") {
        atom = _MEEGOTOUCH_NOTIFICATION_PREVIEWS_DISABLED;
    }

    return atom;
}

int X11Wrapper::XGetWindowProperty(Display *display, Window w, Atom property, long long_offset, long long_length, Bool del, Atom req_type, Atom *actual_type_return, int *actual_format_return, unsigned long *nitems_return, unsigned long *bytes_after_return, unsigned char **prop_return)
{
    Q_UNUSED(display);
    Q_UNUSED(long_offset);
    Q_UNUSED(long_length);
    Q_UNUSED(del);
    Q_UNUSED(req_type);
    Q_UNUSED(actual_type_return);
    Q_UNUSED(actual_format_return);
    Q_UNUSED(bytes_after_return);

    if (!gWindowPropertyMap.contains(w))
        return BadWindow;

    if (!gWindowPropertyMap[w].contains(property)) {
        *nitems_return = 0;
    } else {
        *nitems_return = 1;
        *(int**)prop_return = new int;
        gXAllocs.insert(*(int**)prop_return);
        **(int**)prop_return = gWindowPropertyMap[w][property];
    }

    return Success;
}

int X11Wrapper::XFree(void *data)
{
    gXAllocs.remove((int*)data);
    delete (int*)data;

    return Success;
}

QList<XRectangle> xFixesCreateRegionRectangles;
XserverRegion X11Wrapper::XFixesCreateRegion(Display *, XRectangle *rectangles, int nrectangles)
{
    for (int i = 0; i < nrectangles; i++) {
        xFixesCreateRegionRectangles.append(rectangles[i]);
    }
    return 1;
}

QList<Window> xFixesSetWindowShapeRegionWindow;
QList<int> xFixesSetWindowShapeRegionShapeKind;
QList<int> xFixesSetWindowShapeRegionXOff;
QList<int> xFixesSetWindowShapeRegionYOff;
QList<XserverRegion> xFixesSetWindowShapeRegionRegion;
void X11Wrapper::XFixesSetWindowShapeRegion(Display *, Window win, int shape_kind, int x_off, int y_off, XserverRegion region)
{
    xFixesSetWindowShapeRegionWindow.append(win);
    xFixesSetWindowShapeRegionShapeKind.append(shape_kind);
    xFixesSetWindowShapeRegionXOff.append(x_off);
    xFixesSetWindowShapeRegionYOff.append(y_off);
    xFixesSetWindowShapeRegionRegion.append(region);
}

QList<XserverRegion> xFixesDestroyRegionRegion;
void X11Wrapper::XFixesDestroyRegion(Display *, XserverRegion region)
{
    xFixesDestroyRegionRegion.append(region);
}

bool xSyncCalled = false;
int X11Wrapper::XSync(Display *, int)
{
    xSyncCalled = true;
    return 0;
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
    gMSceneWindowsAppeared.clear();
    gMSceneWindowDeletionPolicies.clear();

    qQTimerEmitTimeoutImmediately = true;
    qTimerStarted = false;
    gQTimerStopped = false;
    mSceneManagerDisappearSceneWindowWindow = NULL;
    windowEventFilterCalled = false;
    windowEventFilterBlock = false;
    gMWindowIsOnDisplay = false;

    gWindowPropertyMap.clear();
    gXAllocs.clear();
}

void Ut_MCompositorNotificationSink::cleanup()
{
    delete sink;
    delete notificationManager;
    xFixesCreateRegionRectangles.clear();
    xFixesSetWindowShapeRegionWindow.clear();
    xFixesSetWindowShapeRegionShapeKind.clear();
    xFixesSetWindowShapeRegionXOff.clear();
    xFixesSetWindowShapeRegionYOff.clear();
    xFixesSetWindowShapeRegionRegion.clear();
    xFixesDestroyRegionRegion.clear();
    xSyncCalled = false;
}

void Ut_MCompositorNotificationSink::emitDisplayEntered()
{
    disconnect(this, SIGNAL(displayEntered()), sink->window, SIGNAL(displayEntered()));
    connect(this, SIGNAL(displayEntered()), sink->window, SIGNAL(displayEntered()));
    emit displayEntered();
}

NotificationParameters Ut_MCompositorNotificationSink::setupSinkDisabledTests(bool isSystemEvent)
{
    sink->setApplicationEventsDisabled(true);
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
    // Check that the window is not automatically created
    QCOMPARE(sink->window, (MWindow *)NULL);

    // Check that the window is created when a notification is added
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0, 0);
    QVERIFY(sink->window != NULL);

    // Check that the window doesn't request focus
    QCOMPARE(sink->window->testAttribute(Qt::WA_X11DoNotAcceptFocus), true);
}

void Ut_MCompositorNotificationSink::testAddNotificationWhenWindowNotOpen()
{
    QSignalSpy spy(sink, SIGNAL(notificationAdded(const Notification&)));

    // Create a notification
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0, 0);
    emitDisplayEntered();

    // Check that a MWindow was shown(window->show() called)
    QCOMPARE(mWindowSetVisibleWidget, sink->window);
    QCOMPARE(mWindowSetVisibleValue, true);

    // Check that a MBanner was created with the given parameters
    MBanner* banner = static_cast<MBanner*>(gMSceneWindowsAppeared.at(0));
    QVERIFY(banner);
    QCOMPARE(banner->styleName(), QString(banner->objectName() == "EventBanner" ? "ShortEventBanner" : "SystemBanner"));
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
    qQTimerEmitTimeoutImmediately = false;
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0, 0);
    emitDisplayEntered();
    QVERIFY(mWindowSetVisibleWidget);
    QVERIFY(mWindowSetVisibleValue);
    QVERIFY(qTimerStarted);

    // Reset stubs to see what happens next
    mWindowSetVisibleWidget = NULL;
    mWindowSetVisibleValue = false;
    qTimerStarted = false;

    QSignalSpy spy(sink, SIGNAL(notificationAdded(const Notification&)));

    // Create another notification
    TestNotificationParameters parameters1("title1", "subtitle1", "buttonicon1", "content1 0 0 0");
    notificationManager->addNotification(1, parameters1, 0);

    // Disappear the first notification
    MBanner* first_banner = static_cast<MBanner*>(gMSceneWindowsAppeared.at(0));
    QVERIFY(first_banner);
    MSceneWindowBridge bridge;
    bridge.setObjectName("_m_testBridge");
    bridge.setParent(first_banner);
    bridge.setSceneWindowState(MSceneWindow::Disappeared);

    // Check that the MWindow was not unnecessarily shown again
    QVERIFY(!mWindowSetVisibleWidget);
    QVERIFY(!mWindowSetVisibleValue);

    // Check that a MBanner was created with the given parameters
    MBanner* banner = static_cast<MBanner*>(gMSceneWindowsAppeared.at(1));
    QVERIFY(banner);
    QCOMPARE(banner->iconID(), QString("buttonicon1"));
    QCOMPARE(banner->title(), QString("title1"));
    QCOMPARE(banner->subtitle(), QString("subtitle1"));

    // check that the timeout timer was started
    QCOMPARE(qTimerStarted, true);

    // check that the transfer notification signal was emitted
    QCOMPARE(spy.count(), 1);
}

void Ut_MCompositorNotificationSink::testWhenNotificationAlreadyOnDisplayNoNewNotificationIsShown()
{
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0, 0);
    emitDisplayEntered();

    sink->window->QGraphicsView::setVisible(false);
    gMWindowIsOnDisplay = true;

    TestNotificationParameters parameters1("title1", "subtitle1", "buttonicon1", "content1 0 0 0");
    notificationManager->addNotification(1, parameters1, 0);

    QCOMPARE(gMSceneWindowsAppeared.count(), 1);
}

void Ut_MCompositorNotificationSink::testWhenWindowOnDisplayThenNotificationAddedWithoutSignal()
{
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0, 0);
    emitDisplayEntered();

    // Disappear the first notification
    MBanner* first_banner = static_cast<MBanner*>(gMSceneWindowsAppeared.at(0));
    QVERIFY(first_banner);
    MSceneWindowBridge bridge;
    bridge.setObjectName("_m_testBridge");
    bridge.setParent(first_banner);
    bridge.setSceneWindowState(MSceneWindow::Disappeared);

    sink->window->QGraphicsView::setVisible(false);
    gMWindowIsOnDisplay = true;

    TestNotificationParameters parameters1("title1", "subtitle1", "buttonicon1", "content1 0 0 0");
    notificationManager->addNotification(1, parameters1, 0);

    QCOMPARE(gMSceneWindowsAppeared.count(), 2);
    QVERIFY(dynamic_cast<MBanner*>(gMSceneWindowsAppeared.at(1)));
}

void Ut_MCompositorNotificationSink::testWhenRemovingCurrentNotificationThenTimerStopsAndBannerDisappears()
{
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    uint id = notificationManager->addNotification(0, parameters0, 0);
    emitDisplayEntered();

    QCOMPARE(gMSceneWindowsAppeared.count(), 1);
    MBanner* banner = dynamic_cast<MBanner*>(gMSceneWindowsAppeared.at(0));
    QVERIFY(banner);

    notificationManager->removeNotification(0, id);
    QVERIFY(gQTimerStopped);

    QCOMPARE(mSceneManagerDisappearSceneWindowWindow, banner);
}

void Ut_MCompositorNotificationSink::testUpdateNotification()
{
    // Create two notifications
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0);
    emitDisplayEntered();
    MBanner* banner1 = static_cast<MBanner*>(gMSceneWindowsAppeared.at(0));

    TestNotificationParameters parameters1( "title1", "subtitle1", "buttonicon1", "content1 1 1 1");
    uint id = notificationManager->addNotification(0, parameters1);
    // Window will not drop to displaychange event slot unless window is explicitly hidden
    mWindowSetVisibleWidget->hide();
    emitDisplayEntered();

    // Update the second notification
    TestNotificationParameters parametersX("titleX", "subtitleX", "buttoniconX", "contentX X X X");
    notificationManager->updateNotification(0, id, parametersX);

    MSceneWindowBridge bridge;
    bridge.setObjectName("_m_testBridge");
    bridge.setParent(banner1);
    bridge.setSceneWindowState(MSceneWindow::Disappeared);
    emitDisplayEntered();
    MBanner* banner2 = static_cast<MBanner*>(gMSceneWindowsAppeared.at(1));

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
    emitDisplayEntered();

    // Make sure that the first one has been shown
    MBanner* banner = static_cast<MBanner*>(gMSceneWindowsAppeared.at(0));
    QVERIFY(banner);

    // Create and immediately remove a notification
    TestNotificationParameters parameters1("title1", "subtitle1", "buttonicon1", "content1 1 1 1");
    uint id = notificationManager->addNotification(0, parameters1);
    notificationManager->removeNotification(0, id);

    // Now make the first banner disappear
    MSceneWindowBridge bridge;
    bridge.setObjectName("_m_testBridge");
    bridge.setParent(banner);
    bridge.setSceneWindowState(MSceneWindow::Disappeared);

    QVERIFY(mWindowSetVisibleWidget);
    QVERIFY(!mWindowSetVisibleValue);
}

void Ut_MCompositorNotificationSink::testTimeout()
{
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0);
    emitDisplayEntered();
    MBanner* banner = static_cast<MBanner*>(gMSceneWindowsAppeared.at(0));
    MSceneWindowBridge bridge;
    bridge.setObjectName("_m_testBridge");
    bridge.setParent(banner);
    bridge.setSceneWindowState(MSceneWindow::Disappeared);

    // Check that the window disappeared
    QVERIFY(mSceneManagerDisappearSceneWindowWindow);
    QCOMPARE(gMSceneWindowsAppeared.at(0), mSceneManagerDisappearSceneWindowWindow);
}

void Ut_MCompositorNotificationSink::testNotificationWhileApplicationEventsDisabled()
{
    // Create notification
    TestNotificationParameters parameters("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    sink->setApplicationEventsEnabled(false);
    notificationManager->addNotification(0, parameters);
    emitDisplayEntered();
    QCOMPARE(mWindowSetVisibleValue, false);

    sink->setApplicationEventsEnabled(true);
    notificationManager->addNotification(0, parameters);
    QCOMPARE(mWindowSetVisibleValue, true);
}

void Ut_MCompositorNotificationSink::testWhenSinkDisableTrueNoBannerCreated()
{
    NotificationParameters parameters = setupSinkDisabledTests();
    QCOMPARE(mWindowSetVisibleValue, false);
    sink->setApplicationEventsDisabled(false);
    notificationManager->addNotification(0, parameters);
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
    QTest::addColumn<bool>("displayOff");
    QTest::addColumn<bool>("touchScreenLocked");
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<bool>("notificationshown");

    QTest::newRow("display off, touch screen locked, key not set, notification not shown") << true << true << QVariant() << false;
    QTest::newRow("display off, touch screen locked, key has garbage value, notification not shown") << true << true << QVariant(QString("Garbage")) << false;
    QTest::newRow("display off, touch screen locked, key has has previews enabled, notification not shown") << true << true << QVariant(true) << false;
    QTest::newRow("display off, touch screen locked, key has has previews disabled, notification not shown") << true << true << QVariant(false) << false;
    QTest::newRow("display off, touch screen unlocked, key not set, notification shown") << true << false << QVariant() << true;
    QTest::newRow("display off, touch screen unlocked, key has garbage value, notification shown") << true << false << QVariant(QString("Garbage")) << true;
    QTest::newRow("display off, touch screen unlocked, key has has previews enabled, notification shown") << true << false << QVariant(true) << true;
    QTest::newRow("display off, touch screen unlocked, key has has previews disabled, notification not shown") << true << false << QVariant(false) << false;
    QTest::newRow("display on, touch screen locked, key not set, notification shown") << false << true << QVariant() << true;
    QTest::newRow("display on, touch screen locked, key has garbage value, notification shown") << false << true << QVariant(QString("Garbage")) << true;
    QTest::newRow("display on, touch screen locked, key has has previews enabled, window shown") << false << true << QVariant(true) << true;
    QTest::newRow("display on, touch screen locked, key has has previews disabled, window not shown") << false << true << QVariant(false) << false;
}


void Ut_MCompositorNotificationSink::testNotificationPreviewsDisabled()
{
    // Reset previews as not disabled
    sink->allPreviewsDisabled = false;

    // Check that window is following right gconf key
    QCOMPARE(sink->notificationPreviewMode->key(), NOTIFICATION_PREVIEW_ENABLED);

    QFETCH(bool, displayOff);
    QFETCH(bool, touchScreenLocked);
    QFETCH(QVariant, value);
    QFETCH(bool, notificationshown);

    // Set value from test data to gconf key and qm variable
    sink->notificationPreviewMode->set(value);
#ifdef HAVE_QMSYSTEM
    gQmDisplayStateOff = displayOff;
#endif
    sink->setTouchScreenLockActive(touchScreenLocked);
    sink->changeNotificationPreviewMode();

    // Create normal notification
    TestNotificationParameters parameters("title0", "subtitle0", "buttonicon0", "content0 0 0 0");

    // Check that notification is shown/not shown according to test data parameter
    notificationManager->addNotification(0, parameters);
    QCOMPARE(mWindowSetVisibleValue, notificationshown);

    // Create system notification
    parameters.add(GenericNotificationParameterFactory::classKey(), "system");
    notificationManager->addNotification(0, parameters);

    // Check that notification is not shown
    QCOMPARE(mWindowSetVisibleValue, notificationshown);
}

void Ut_MCompositorNotificationSink::testWindowMasking_data()
{
    QTest::addColumn<M::OrientationAngle>("angle");
    QTest::newRow("0")   << M::Angle0;
    QTest::newRow("90")  << M::Angle90;
    QTest::newRow("180") << M::Angle180;
    QTest::newRow("270") << M::Angle270;
}

void Ut_MCompositorNotificationSink::testWindowShapeRegion(M::OrientationAngle angle, MSceneWindow* window)
{
    QRect rect;
    switch(angle) {
    case M::Angle0:   rect = QRect(0, 0, window->preferredWidth(), window->preferredHeight()); break;
    case M::Angle90:  rect = QRect(sink->window->width() - window->preferredHeight(), 0, window->preferredHeight(), window->preferredWidth()); break;
    case M::Angle180: rect = QRect(0, sink->window->height() - window->preferredHeight(), window->preferredWidth(), window->preferredHeight()); break;
    case M::Angle270: rect = QRect(0, 0, window->preferredHeight(), window->preferredWidth()); break;
    }

    QCOMPARE(xFixesCreateRegionRectangles.isEmpty(), false);
    QCOMPARE(xFixesCreateRegionRectangles.last().x, (short)rect.x());
    QCOMPARE(xFixesCreateRegionRectangles.last().y, (short)rect.y());
    QCOMPARE(xFixesCreateRegionRectangles.last().width, (unsigned short)rect.width());
    QCOMPARE(xFixesCreateRegionRectangles.last().height, (unsigned short)rect.height());
    QCOMPARE(xFixesSetWindowShapeRegionWindow.isEmpty(), false);
    QCOMPARE(xFixesSetWindowShapeRegionWindow.last(), sink->window->winId());
    QCOMPARE(xFixesSetWindowShapeRegionShapeKind.last(), ShapeInput);
    QCOMPARE(xFixesSetWindowShapeRegionXOff.last(), 0);
    QCOMPARE(xFixesSetWindowShapeRegionYOff.last(), 0);
    QCOMPARE(xFixesSetWindowShapeRegionRegion.last(), (XserverRegion)1);
    QCOMPARE(xFixesDestroyRegionRegion.isEmpty(), false);
    QCOMPARE(xFixesDestroyRegionRegion.last(), (XserverRegion)1);
    QCOMPARE(xSyncCalled, true);
}

void Ut_MCompositorNotificationSink::testWindowMasking()
{
    QFETCH(M::OrientationAngle, angle);
    gCurrentOrientationAngle = angle;
    qQTimerEmitTimeoutImmediately = false;

    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0, 0);
    emitDisplayEntered();

    MSceneWindow* window = gMSceneWindowsAppeared.at(0);

    testWindowShapeRegion(angle, window);
}

void Ut_MCompositorNotificationSink::testWindowMaskingWhenOrientationChangeSignalsEmitted()
{
    qQTimerEmitTimeoutImmediately = false;

    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0);
    emitDisplayEntered();
    int newAngle = sink->window->sceneManager()->orientationAngle() + M::Angle90;
    gCurrentOrientationAngle = (M::OrientationAngle)newAngle;

    sink->window->sceneManager()->setOrientationAngle(gCurrentOrientationAngle, MSceneManager::ImmediateTransition);
    QVERIFY(xFixesSetWindowShapeRegionWindow.count() > 1);
    int secondLast = xFixesSetWindowShapeRegionWindow.count() - 2;
    QCOMPARE(xFixesSetWindowShapeRegionWindow.at(secondLast), sink->window->winId());
    QCOMPARE(xFixesSetWindowShapeRegionShapeKind.at(secondLast), ShapeInput);
    QCOMPARE(xFixesSetWindowShapeRegionXOff.at(secondLast), 0);
    QCOMPARE(xFixesSetWindowShapeRegionYOff.at(secondLast), 0);
    QCOMPARE(xFixesSetWindowShapeRegionRegion.at(secondLast), (XserverRegion)0);
    QCOMPARE(xSyncCalled, true);

    MSceneWindow* window = gMSceneWindowsAppeared.at(0);
    testWindowShapeRegion(gCurrentOrientationAngle, window);
}

void Ut_MCompositorNotificationSink::testPreviewIconId()
{
    qQTimerEmitTimeoutImmediately = false;

    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    uint id = notificationManager->addNotification(0, parameters0);
    emitDisplayEntered();

    // Check default icon is used if preview icon id is not defined
    MBanner* banner = static_cast<MBanner*>(gMSceneWindowsAppeared.at(0));
    QCOMPARE(banner->iconID(), QString("buttonicon0"));

    // Check that preview icon id is used if defined
    parameters0.add("previewIconId", "previewicon0");
    notificationManager->updateNotification(0, id, parameters0);
    QCOMPARE(banner->iconID(), QString("previewicon0"));
}

void Ut_MCompositorNotificationSink::testNotificationPreviewsDisabledForApplication_data()
{
    QTest::addColumn<bool>("propertySet");
    QTest::addColumn<int>("value");
    QTest::addColumn<bool>("system");
    QTest::addColumn<bool>("windowshown");

    QTest::newRow("property not set, window shown for application notification") << false << 0 << false << true;
    QTest::newRow("property not set, window shown for system notification") << false << 0 << true << true;
    QTest::newRow("property set to 0, window shown for application notification") << true << 0 <<  false << true;
    QTest::newRow("property set to 0, window shown for system notification") << true << 0 <<  true << true;
    QTest::newRow("property set to 1, window not shown for application notification") << true << 1 <<  false << false;
    QTest::newRow("property set to 1, window shown for system notification") << true << 1 <<  true << true;
    QTest::newRow("property set to 2, window shown for application notification") << true << 2 <<  false << true;
    QTest::newRow("property set to 2, window not shown for system notification") << true << 2 <<  true << false;
    QTest::newRow("property set to 3, window not shown for application notification") << true << 3 <<  false << false;
    QTest::newRow("property set to 3, window not shown for system notification") << true << 3 <<  true << false;
}

void Ut_MCompositorNotificationSink::testNotificationPreviewsDisabledForApplication()
{
    QFETCH(bool, propertySet);
    QFETCH(int, value);

    // Set the current app root window property to point to a certain window
    gWindowPropertyMap[ROOT_WINDOW_ID][_MEEGOTOUCH_CURRENT_APP_WINDOW] = 100;
    if (propertySet) {
        // Set the property value for the window
        gWindowPropertyMap[100][_MEEGOTOUCH_NOTIFICATION_PREVIEWS_DISABLED] = value;
    }

    QFETCH(bool, system);
    QFETCH(bool, windowshown);

    // Create normal notification
    TestNotificationParameters parameters("title0", "subtitle0", "buttonicon0", "content0 0 0 0");

    if (system) {
        // Create system notification
        parameters.add(GenericNotificationParameterFactory::classKey(), "system");
    }

    // Check that notification is shown/not shown according to test data parameter
    notificationManager->addNotification(0, parameters);
    QCOMPARE(mWindowSetVisibleValue, windowshown);

    // Check that all requested property data was freed
    QCOMPARE(gXAllocs.count(), 0);
}

void Ut_MCompositorNotificationSink::updateNotificationDoesNotCreateWindowIfBannerNotOnDisplay()
{
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0);
    emitDisplayEntered();
    MBanner* banner = static_cast<MBanner*>(gMSceneWindowsAppeared.at(0));
    MSceneWindowBridge bridge;
    bridge.setObjectName("_m_testBridge");
    bridge.setParent(banner);
    bridge.setSceneWindowState(MSceneWindow::Disappeared);

    // At this point the window has disappeared and updating the notification should not bring it back
    TestNotificationParameters parameters1("title1", "subtitle1", "buttonicon1", "content1 1 1 1");
    notificationManager->updateNotification(0, 0, parameters1);
    QCOMPARE(mWindowSetVisibleValue, false);
}

void Ut_MCompositorNotificationSink::testCurrentBannerDoneDoesntRemoveOtherBanners()
{
    // Add a notification and display its banner
    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    notificationManager->addNotification(0, parameters0);
    emitDisplayEntered();
    MBanner* banner = static_cast<MBanner*>(gMSceneWindowsAppeared.at(0));

    // Simulate a case when a new banner with the same id is created
    // before the previous banner disappears
    sink->idToBanner.take(0);
    sink->notificationIds.remove(0);
    notificationManager->nextAvailableNotificationID = 0;
    notificationManager->addNotification(0, parameters0);

    MSceneWindowBridge bridge;
    bridge.setObjectName("_m_testBridge");
    bridge.setParent(banner);
    bridge.setSceneWindowState(MSceneWindow::Disappeared);

    // Verify that the initial banner disappeared but the new one was
    // not removed from the "id to banner" mapping.
    QVERIFY(sink->idToBanner.value(0) != NULL);
}

void Ut_MCompositorNotificationSink::testSystemNotificationIsRemovedWhenPreviewsAreDisabled()
{
    sink->notificationPreviewMode->set(false);
    sink->changeNotificationPreviewMode();

    QSignalSpy spy(sink, SIGNAL(notificationRemovalRequested(uint)));
    TestNotificationParameters parameters("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    parameters.add(GenericNotificationParameterFactory::classKey(), "system");
    uint id = notificationManager->addNotification(0, parameters);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().at(0).toUInt(), id);
}

void Ut_MCompositorNotificationSink::testSystemNotificationIsRemovedWhenBannerHasBeenShown()
{
    QSignalSpy spy(sink, SIGNAL(notificationRemovalRequested(uint)));
    TestNotificationParameters parameters("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    parameters.add(GenericNotificationParameterFactory::classKey(), "system");
    uint id = notificationManager->addNotification(0, parameters);
    emitDisplayEntered();

    MSceneWindowBridge bridge;
    bridge.setObjectName("_m_testBridge");
    bridge.setParent(static_cast<MBanner*>(gMSceneWindowsAppeared.at(0)));
    bridge.setSceneWindowState(MSceneWindow::Disappeared);

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.last().at(0).toUInt(), id);
}

void Ut_MCompositorNotificationSink::testWhenDisplayIsOffAndNotificationIsReceivedBannersAreRemovedFromQueue()
{
    qQTimerEmitTimeoutImmediately = false;

    TestNotificationParameters parameters0("title0", "subtitle0", "buttonicon0", "content0 0 0 0");
    TestNotificationParameters parameters1("title1", "subtitle1", "buttonicon1", "content1 1 1 1");
    TestNotificationParameters parameters2("title2", "subtitle2", "buttonicon2", "content2 2 2 2");
    TestNotificationParameters parameters3("title3", "subtitle3", "buttonicon3", "content3 3 3 3");
    TestNotificationParameters parameters4("title4", "subtitle4", "buttonicon3", "content4 4 4 4");
    parameters0.add(GenericNotificationParameterFactory::classKey(), "system");
    parameters1.add(GenericNotificationParameterFactory::classKey(), "system");
    parameters4.add(GenericNotificationParameterFactory::classKey(), "system");
    notificationManager->addNotification(0, parameters0, 0);
    notificationManager->addNotification(0, parameters1, 0);
    notificationManager->addNotification(0, parameters2, 0);
    notificationManager->addNotification(0, parameters3, 0);
    emitDisplayEntered();

#ifdef HAVE_QMSYSTEM
    gQmDisplayStateOff = true;
    notificationManager->addNotification(0, parameters4, 0);

    // The banner that was on screen should disappear and the queue should be emptied
    QCOMPARE(mSceneManagerDisappearSceneWindowWindow, gMSceneWindowsAppeared.at(0));
    QCOMPARE(gMSceneWindowsAppeared.count(), 1);

    // When the window is opened again the latest banner should appear
    emitDisplayEntered();
    QCOMPARE(gMSceneWindowsAppeared.count(), 2);
    QCOMPARE(static_cast<MBanner*>(gMSceneWindowsAppeared.at(1))->styleName(), QString("SystemBanner"));
    QCOMPARE(static_cast<MBanner*>(gMSceneWindowsAppeared.at(1))->title(), QString("subtitle4"));
#endif
}

QTEST_APPLESS_MAIN(Ut_MCompositorNotificationSink)
