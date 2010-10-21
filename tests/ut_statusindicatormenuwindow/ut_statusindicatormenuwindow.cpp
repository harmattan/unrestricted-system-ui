
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
#include <MLocale>

#ifdef HAVE_QMSYSTEM
#include "qmlocks_stub.h"
#endif

// QWidget stubs
QPair<void*, bool> gSetVisible(0, false);
void MWindow::setVisible(bool visible)
{
    gSetVisible.first = this;
    gSetVisible.second = visible;
}

// MLocale stubs
QString gLanguage;
QString MLocale::language() const {
    return gLanguage;
}


void Ut_StatusIndicatorMenuWindow::init()
{
    gLanguage = "en";
    statusIndicatorMenuWindow = new StatusIndicatorMenuWindow;
    gSetVisible.first = 0;
    gSetVisible.second = false;
#ifdef HAVE_QMSYSTEM
    gQmLocksStub->stubSetReturnValue("getState", Maemo::QmLocks::Unlocked);
#endif
}

void Ut_StatusIndicatorMenuWindow::cleanup()
{
    delete statusIndicatorMenuWindow;
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
    QCOMPARE(statusIndicatorMenuWindow->menuWidget->sceneWindowState(), MSceneWindow::Appeared);
    QVERIFY(disconnect(statusIndicatorMenuWindow->menuWidget, SIGNAL(showRequested()), statusIndicatorMenuWindow, SLOT(makeVisible())));
    QVERIFY(disconnect(statusIndicatorMenuWindow->menuWidget, SIGNAL(hideRequested()), statusIndicatorMenuWindow, SLOT(hide())));
}

void Ut_StatusIndicatorMenuWindow::testMakeVisible()
{
    statusIndicatorMenuWindow->hide();
    QVERIFY(gSetVisible.first == statusIndicatorMenuWindow && !gSetVisible.second);
    statusIndicatorMenuWindow->makeVisible();

#ifdef HAVE_QMSYSTEM
    if (statusIndicatorMenuWindow->deviceLocked) {
        QVERIFY(gSetVisible.first == statusIndicatorMenuWindow && !gSetVisible.second);
        return;
    }
#endif

    QVERIFY(gSetVisible.first == statusIndicatorMenuWindow && gSetVisible.second);
}

void Ut_StatusIndicatorMenuWindow::testWindowType()
{
    QVERIFY(statusIndicatorMenuWindow->testAttribute(Qt::WA_X11NetWmWindowTypeMenu));
}

void Ut_StatusIndicatorMenuWindow::testWhenFullScreenWindowComesOnTopStatusMenuIsClosed()
{
    connect(this, SIGNAL(displayExited()), statusIndicatorMenuWindow, SLOT(displayInActive()));
    emit displayExited();
    QVERIFY(gSetVisible.first == statusIndicatorMenuWindow && !gSetVisible.second);
}

void Ut_StatusIndicatorMenuWindow::testWhenLanguageChangesThenMenuWidgetIsResetted()
{
    StatusIndicatorMenu *oldMenuWidget = statusIndicatorMenuWindow->menuWidget;

    gLanguage = "de";
    QEvent languageChangeEvent(QEvent::LanguageChange);
    statusIndicatorMenuWindow->event(&languageChangeEvent);

    QVERIFY(oldMenuWidget != statusIndicatorMenuWindow->menuWidget);
    testInitialization();
}

void Ut_StatusIndicatorMenuWindow::testWhenLanguageChangeEventWithoutLanguageChangingThenMenuWidgetIsNotResetted()
{
    StatusIndicatorMenu *oldMenuWidget = statusIndicatorMenuWindow->menuWidget;

    QEvent languageChangeEvent(QEvent::LanguageChange);
    statusIndicatorMenuWindow->event(&languageChangeEvent);

    QVERIFY(oldMenuWidget == statusIndicatorMenuWindow->menuWidget);
}

#ifdef HAVE_QMSYSTEM
void Ut_StatusIndicatorMenuWindow::testQmLocksSignalConnectionWhenDeviceLocked()
{
    bool connectionExisted = disconnect(&statusIndicatorMenuWindow->qmLocks, SIGNAL(stateChanged (Maemo::QmLocks::Lock, Maemo::QmLocks::State)), statusIndicatorMenuWindow,
                                        SLOT(setWindowStateAccordingToDeviceLockState(Maemo::QmLocks::Lock, Maemo::QmLocks::State)));
    QCOMPARE(connectionExisted, true);
}

void Ut_StatusIndicatorMenuWindow::testWhenDeviceLockedMenuIsNotVisible()
{
    // test when device lock state is locked
    statusIndicatorMenuWindow->hide();
    statusIndicatorMenuWindow->deviceLocked = true;
    QVERIFY(gSetVisible.first == statusIndicatorMenuWindow && !gSetVisible.second);
    statusIndicatorMenuWindow->makeVisible();
    QVERIFY(gSetVisible.first == statusIndicatorMenuWindow && !gSetVisible.second);
}

void Ut_StatusIndicatorMenuWindow::testWhenDeviceUnlockedMenuIsVisible()
{
    // test when device lock state is unlocked
    statusIndicatorMenuWindow->hide();
    statusIndicatorMenuWindow->deviceLocked = false;
    QVERIFY(gSetVisible.first == statusIndicatorMenuWindow && !gSetVisible.second);
    statusIndicatorMenuWindow->makeVisible();
    QVERIFY(gSetVisible.first == statusIndicatorMenuWindow && gSetVisible.second);
}

void Ut_StatusIndicatorMenuWindow::testWhenDeviceLockStateChangesFromLockedToUnlockedWindowActivates()
{
    // change status from locked to unlocked
    statusIndicatorMenuWindow->deviceLocked = true;
    statusIndicatorMenuWindow->setWindowStateAccordingToDeviceLockState(
            Maemo::QmLocks::Device, Maemo::QmLocks::Unlocked);
    QCOMPARE(statusIndicatorMenuWindow->deviceLocked, false);
}

void Ut_StatusIndicatorMenuWindow::testWhenDeviceLockStateChangesFromUnlockedToLockedWindowDeactivates()
{
    // change status from unlocked to locked and test if menu closes
    statusIndicatorMenuWindow->deviceLocked = false;
    statusIndicatorMenuWindow->setWindowStateAccordingToDeviceLockState(
            Maemo::QmLocks::Device, Maemo::QmLocks::Locked);
    QCOMPARE(statusIndicatorMenuWindow->deviceLocked, true);
    QCOMPARE(statusIndicatorMenuWindow->isVisible(), false);
}

#endif

QTEST_APPLESS_MAIN(Ut_StatusIndicatorMenuWindow)
