
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
#include "ut_statusindicatormenuwindow.h"
#include "statusindicatormenuwindow.h"
#include <MApplication>
#include <MSceneManager>
#include <QtTest/QtTest>
#include "statusindicatormenu_stub.h"
#include "sysuid_stub.h"
#include <MLocale>
#include <QMouseEvent>
#include <mstatusbar.h>

#ifdef HAVE_QMSYSTEM
#include "qmlocks_stub.h"
#endif

// QWidget stubs
QPair<void*, bool> mWindowSetVisible(0, false);
void MWindow::setVisible(bool visible)
{
    setAttribute(Qt::WA_WState_Visible, visible);
    mWindowSetVisible.first = this;
    mWindowSetVisible.second = visible;
}

bool gMWindowIsOnDisplay = false;
bool MWindow::isOnDisplay() const
{
    return gMWindowIsOnDisplay;
}

QWidget *qWidgetRaise = NULL;
void QWidget::raise()
{
    qWidgetRaise = this;
}

// MLocale stubs
QString gLanguage;
QString MLocale::language() const {
    return gLanguage;
}

QMap<const MSceneWindow*, MSceneWindow::SceneWindowState> gSceneWindowStateMap;

void MSceneManager::appearSceneWindow(MSceneWindow *sceneWindow, MSceneWindow::DeletionPolicy)
{
    gSceneWindowStateMap[sceneWindow] = MSceneWindow::Appeared;
}

void MSceneManager::disappearSceneWindow(MSceneWindow *sceneWindow)
{
    gSceneWindowStateMap[sceneWindow] = MSceneWindow::Disappeared;
}

void MSceneManager::disappearSceneWindowNow(MSceneWindow *sceneWindow)
{
    gSceneWindowStateMap[sceneWindow] = MSceneWindow::Disappeared;
}

void MSceneWindow::disappear()
{
    gSceneWindowStateMap[this] = MSceneWindow::Disappeared;
}

MSceneWindow::SceneWindowState MSceneWindow::sceneWindowState() const
{
    if (!gSceneWindowStateMap.contains(this)) {
        gSceneWindowStateMap[this] = MSceneWindow::Disappeared;
    }

    return gSceneWindowStateMap[this];
}

void Ut_StatusIndicatorMenuWindow::init()
{
    gStatusIndicatorMenuStub->stubReset();
    gLanguage = "en";
    statusIndicatorMenuWindow = new StatusIndicatorMenuWindow;
#ifdef HAVE_QMSYSTEM
    gQmLocksStub->stubSetReturnValue("getState", MeeGo::QmLocks::Unlocked);
#endif
    gMWindowIsOnDisplay = false;
}

void Ut_StatusIndicatorMenuWindow::cleanup()
{
    delete statusIndicatorMenuWindow;
    mWindowSetVisible.first = 0;
    mWindowSetVisible.second = false;
    qWidgetRaise = NULL;
}

void Ut_StatusIndicatorMenuWindow::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_statusindicatormenuwindow";
    app = new MApplication(argc, &app_name);
}

void Ut_StatusIndicatorMenuWindow::cleanupTestCase()
{
    delete app;
}

void Ut_StatusIndicatorMenuWindow::testInitialization()
{
    QVERIFY(statusIndicatorMenuWindow->menuWidget);

    QVERIFY(disconnect(statusIndicatorMenuWindow, SIGNAL(displayExited()), statusIndicatorMenuWindow, SLOT(displayInActive())));
    QVERIFY(disconnect(statusIndicatorMenuWindow, SIGNAL(displayEntered()), statusIndicatorMenuWindow, SLOT(displayActive())));

    QVERIFY(disconnect(statusIndicatorMenuWindow->menuWidget, SIGNAL(showRequested()), statusIndicatorMenuWindow, SLOT(makeVisible())));
    QVERIFY(disconnect(statusIndicatorMenuWindow->menuWidget, SIGNAL(hideRequested()), statusIndicatorMenuWindow->menuWidget, SLOT(disappear())));
    QVERIFY(disconnect(statusIndicatorMenuWindow->menuWidget, SIGNAL(disappeared()), statusIndicatorMenuWindow, SLOT(hideWindow())));
}

void Ut_StatusIndicatorMenuWindow::testMakeVisible_data()
{
    QTest::addColumn<bool>("deviceLocked");
    QTest::addColumn<bool>("visible");
    QTest::addColumn<bool>("showCalled");
    QTest::addColumn<bool>("raiseCalled");

    QTest::newRow("Device locked") << true << false << false << false;
    QTest::newRow("Window not visible") << false << false << true << true;
    QTest::newRow("Window already visible") << false << true << false << true;
}

void Ut_StatusIndicatorMenuWindow::testMakeVisible()
{
    QFETCH(bool, deviceLocked);
    QFETCH(bool, visible);
    QFETCH(bool, showCalled);
    QFETCH(bool, raiseCalled);

    // Set up the original window state
#ifdef HAVE_QMSYSTEM
    statusIndicatorMenuWindow->deviceLocked = deviceLocked;
#endif
    statusIndicatorMenuWindow->setVisible(visible);
    mWindowSetVisible.first = NULL;
    mWindowSetVisible.second = false;

    // Try to make it visible
    statusIndicatorMenuWindow->makeVisible();
    QCOMPARE(mWindowSetVisible.first, showCalled ? statusIndicatorMenuWindow : NULL);
    QCOMPARE(mWindowSetVisible.second, showCalled);
    QCOMPARE(qWidgetRaise, raiseCalled ? statusIndicatorMenuWindow : NULL);
}

void Ut_StatusIndicatorMenuWindow::testWhenWindowAlreadyOnDisplayThenMenuWidgetAppearsWithoutDisplayEnterSignal()
{
    gMWindowIsOnDisplay = true;

    statusIndicatorMenuWindow->makeVisible();

    QVERIFY(gSceneWindowStateMap.contains(statusIndicatorMenuWindow->menuWidget));
    QCOMPARE(gSceneWindowStateMap[statusIndicatorMenuWindow->menuWidget], MSceneWindow::Appeared);
}

void Ut_StatusIndicatorMenuWindow::testWindowType()
{
    QVERIFY(statusIndicatorMenuWindow->testAttribute(Qt::WA_X11NetWmWindowTypeMenu));
}

void Ut_StatusIndicatorMenuWindow::testWhenFullScreenWindowComesOnTopStatusMenuIsClosed()
{
    statusIndicatorMenuWindow->displayActive();

    connect(this, SIGNAL(displayExited()), statusIndicatorMenuWindow, SLOT(displayInActive()));
    emit displayExited();
    QVERIFY(mWindowSetVisible.first == statusIndicatorMenuWindow && !mWindowSetVisible.second);

    QCOMPARE(statusIndicatorMenuWindow->menuWidget->sceneWindowState(), MSceneWindow::Disappeared);
}

void Ut_StatusIndicatorMenuWindow::testWhenLanguageChangesThenMenuWidgetIsResetted()
{
    gLanguage = "de";

    QEvent languageChangeEvent(QEvent::LanguageChange);
    statusIndicatorMenuWindow->event(&languageChangeEvent);

    QCOMPARE(gStatusIndicatorMenuStub->stubCallCount("StatusIndicatorMenuConstructor"), 2);
    QCOMPARE(gStatusIndicatorMenuStub->stubCallCount("StatusIndicatorMenuDestructor"), 1);

    testInitialization();
}

void Ut_StatusIndicatorMenuWindow::testWhenLanguageChangeEventWithoutLanguageChangingThenMenuWidgetIsNotResetted()
{
    QEvent languageChangeEvent(QEvent::LanguageChange);
    statusIndicatorMenuWindow->event(&languageChangeEvent);

    QCOMPARE(gStatusIndicatorMenuStub->stubCallCount("StatusIndicatorMenuConstructor"), 1);
    QCOMPARE(gStatusIndicatorMenuStub->stubCallCount("StatusIndicatorMenuDestructor"), 0);
}

void Ut_StatusIndicatorMenuWindow::testStatusIndicatorMenuAppearsAfterEnteringDisplay()
{
    QCOMPARE(statusIndicatorMenuWindow->menuWidget->sceneWindowState(), MSceneWindow::Disappeared);
    statusIndicatorMenuWindow->displayActive();
    QCOMPARE(statusIndicatorMenuWindow->menuWidget->sceneWindowState(), MSceneWindow::Appeared);
}


void Ut_StatusIndicatorMenuWindow::testStatusIndicatorMenuIsClosedWhenStatusBarIsTapped()
{
    statusIndicatorMenuWindow->displayActive();

    // Map the bounding rect to the scene
    QRectF statusBarGeo(statusIndicatorMenuWindow->statusBar->sceneBoundingRect());

    QPoint inside(statusBarGeo.x()+statusBarGeo.width()/2,
                  statusBarGeo.y()+statusBarGeo.height()/2);

    // Then map a point inside it to the viewport
    inside = statusIndicatorMenuWindow->mapFromScene(inside);

    QMouseEvent pressInside(QEvent::MouseButtonPress,
                             inside,
                             Qt::LeftButton,
                             Qt::MouseButtons(Qt::LeftButton),
                             Qt::KeyboardModifiers(Qt::NoModifier));

    QMouseEvent releaseInside(QEvent::MouseButtonRelease,
                              inside,
                              Qt::LeftButton,
                              Qt::MouseButtons(Qt::LeftButton),
                              Qt::KeyboardModifiers(Qt::NoModifier));

    statusIndicatorMenuWindow->mousePressEvent(&pressInside);
    statusIndicatorMenuWindow->mouseReleaseEvent(&releaseInside);

    QCOMPARE(statusIndicatorMenuWindow->menuWidget->sceneWindowState(), MSceneWindow::Disappeared);
}

void Ut_StatusIndicatorMenuWindow::testWhenStatusIndicatorMenuIsDisappearedThenWindowIsHidden()
{
    QSignalSpy spy(statusIndicatorMenuWindow, SIGNAL(visibilityChanged(bool)));
    QVERIFY(disconnect(statusIndicatorMenuWindow->menuWidget, SIGNAL(disappeared()), statusIndicatorMenuWindow, SLOT(hideWindow())));
    statusIndicatorMenuWindow->hideWindow();

    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toBool(), false);
    QCOMPARE(statusIndicatorMenuWindow->isVisible(), false);
}

#ifdef HAVE_QMSYSTEM
void Ut_StatusIndicatorMenuWindow::testQmLocksSignalConnectionWhenDeviceLocked()
{
    bool connectionExisted = disconnect(&statusIndicatorMenuWindow->qmLocks, SIGNAL(stateChanged (MeeGo::QmLocks::Lock, MeeGo::QmLocks::State)), statusIndicatorMenuWindow,
                                        SLOT(setWindowStateAccordingToDeviceLockState(MeeGo::QmLocks::Lock, MeeGo::QmLocks::State)));
    QCOMPARE(connectionExisted, true);
}

void Ut_StatusIndicatorMenuWindow::testWhenDeviceLockedMenuIsNotVisible()
{
    // test when device lock state is locked
    statusIndicatorMenuWindow->hide();
    statusIndicatorMenuWindow->deviceLocked = true;
    QVERIFY(mWindowSetVisible.first == statusIndicatorMenuWindow && !mWindowSetVisible.second);
    statusIndicatorMenuWindow->makeVisible();
    QVERIFY(mWindowSetVisible.first == statusIndicatorMenuWindow && !mWindowSetVisible.second);
}

void Ut_StatusIndicatorMenuWindow::testWhenDeviceUnlockedMenuIsVisible()
{
    // test when device lock state is unlocked
    statusIndicatorMenuWindow->hide();
    statusIndicatorMenuWindow->deviceLocked = false;
    QVERIFY(mWindowSetVisible.first == statusIndicatorMenuWindow && !mWindowSetVisible.second);
    statusIndicatorMenuWindow->makeVisible();
    QVERIFY(mWindowSetVisible.first == statusIndicatorMenuWindow && mWindowSetVisible.second);
}

void Ut_StatusIndicatorMenuWindow::testWhenDeviceLockStateChangesFromLockedToUnlockedWindowActivates()
{
    // change status from locked to unlocked
    statusIndicatorMenuWindow->deviceLocked = true;
    statusIndicatorMenuWindow->setWindowStateAccordingToDeviceLockState(
            MeeGo::QmLocks::Device, MeeGo::QmLocks::Unlocked);
    QCOMPARE(statusIndicatorMenuWindow->deviceLocked, false);
}

void Ut_StatusIndicatorMenuWindow::testWhenDeviceLockStateChangesFromUnlockedToLockedWindowDeactivates()
{
    // change status from unlocked to locked and test if menu closes
    statusIndicatorMenuWindow->deviceLocked = false;
    statusIndicatorMenuWindow->setWindowStateAccordingToDeviceLockState(
            MeeGo::QmLocks::Device, MeeGo::QmLocks::Locked);
    QCOMPARE(statusIndicatorMenuWindow->deviceLocked, true);
    QCOMPARE(statusIndicatorMenuWindow->isVisible(), false);
}

#endif

QTEST_APPLESS_MAIN(Ut_StatusIndicatorMenuWindow)
