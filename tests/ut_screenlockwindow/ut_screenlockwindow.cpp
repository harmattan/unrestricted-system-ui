/****************************************************************************
**
** Copyright(C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation(directui@nokia.com)
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
#include <MApplication>
#include <MSceneWindow>
#include <MApplicationExtensionArea>
#include <QShowEvent>
#include "ut_screenlockwindow.h"
#include "screenlockwindow.h"
#include "screenlockwindowstyle.h"
#include "notificationstatusindicatorsink_stub.h"
#include "notificationsink_stub.h"
#include "sysuid_stub.h"
#include <X11/Xutil.h>
#include "x11wrapper_modified_stub.h"

// X stubs to avoid crashes
void XSetWMProperties(Display *, Window, XTextProperty *, XTextProperty *, char **, int, XSizeHints *, XWMHints *, XClassHint *)
{
}

bool mWindowOrientationLocked = false;
void MWindow::setOrientationLocked(bool locked)
{
    mWindowOrientationLocked = locked;
}

bool mWindowOrientationAngleLocked = false;
void MWindow::setOrientationAngleLocked(bool locked)
{
    mWindowOrientationAngleLocked = locked;
}

QString mWindowOrientation;
void MWindow::setLandscapeOrientation()
{
    mWindowOrientation = "landscape";
}

void MWindow::setPortraitOrientation()
{
    mWindowOrientation = "portrait";
}

M::OrientationAngle mWindowOrientationAngle;
void MWindow::setOrientationAngle(M::OrientationAngle angle)
{
    mWindowOrientationAngle = angle;
}

MWindow* appearedWindow = NULL;
void MSceneWindow::appear(MWindow* window, MSceneWindow::DeletionPolicy policy)
{
    appearedWindow = window;
    Q_UNUSED(policy);
}


void Ut_ScreenLockWindow::initTestCase()
{
    int   argc = 1;
    char *argv[] = {(char *) "./ut_screenlockwindow", NULL };
    app = new MApplication(argc, argv);
}

void Ut_ScreenLockWindow::cleanupTestCase()
{
    delete app;
}

void Ut_ScreenLockWindow::init()
{
    lockScreenWindow = new ScreenLockWindow(new MApplicationExtensionArea(""));
}

void Ut_ScreenLockWindow::cleanup()
{
    delete lockScreenWindow;
    lockScreenWindow = NULL;
    appearedWindow = NULL;
    mWindowOrientationLocked = false;
    mWindowOrientationAngleLocked = false;
    mWindowOrientation.clear();
    mWindowOrientationAngle = M::Angle180;
    gX11WrapperStub->stubReset();
}

void Ut_ScreenLockWindow::testWhenWindowIsCreatedLockScreenAppears()
{
    QCOMPARE(appearedWindow, lockScreenWindow);
}

void Ut_ScreenLockWindow::testWhenWindowIsShownItIsExcludedFromTaskbar()
{
    // The stub needs to be reset so that things done at construction time won't affect the test
    gX11WrapperStub->stubReset();

    Display *display = QX11Info::display();

    QShowEvent *showEvent = new QShowEvent;
    lockScreenWindow->showEvent(showEvent);

    QCOMPARE(gX11WrapperStub->stubCallCount("XSendEvent"), 1);
    QCOMPARE(gX11WrapperStub->stubLastCallTo("XSendEvent").parameter<Display *>(0), display);
    QCOMPARE(gX11WrapperStub->stubLastCallTo("XSendEvent").parameter<Window>(1), RootWindow(display, lockScreenWindow->x11Info().screen()));
    QCOMPARE(gX11WrapperStub->stubLastCallTo("XSendEvent").parameter<Bool>(2), (Bool)False);
    QCOMPARE(gX11WrapperStub->stubLastCallTo("XSendEvent").parameter<long>(3), (long)(SubstructureNotifyMask | SubstructureRedirectMask));
    XEvent event = gX11WrapperStub->stubLastCallTo("XSendEvent").parameter<XEvent>(4);
    QCOMPARE(event.xclient.type, ClientMessage);
    QCOMPARE(event.xclient.display, display);
    QCOMPARE(event.xclient.window, lockScreenWindow->effectiveWinId());
    QCOMPARE(event.xclient.message_type, X11Wrapper::XInternAtom(display, "_NET_WM_STATE", False));
    QCOMPARE(event.xclient.format, 32);
    QCOMPARE(event.xclient.data.l[0], (long)1);
    QCOMPARE(event.xclient.data.l[1], (long)X11Wrapper::XInternAtom(display, "_NET_WM_STATE_SKIP_TASKBAR", False));
    delete showEvent;
}

void Ut_ScreenLockWindow::testOrientationLocking_data()
{
    QTest::addColumn<QString>("lockedOrientation");
    QTest::addColumn<bool>("orientationLocked");
    QTest::addColumn<QString>("expectedOrientation");
    QTest::addColumn<int>("expectedOrientationAngle");

    QTest::newRow("No locking") << QString() << false << QString() << (int)M::Angle180;
    QTest::newRow("Locked to landscape") << QString("landscape") << true << QString("landscape") << (int)M::Angle0;
    QTest::newRow("Locked to portrait") << QString("portrait") << true << QString("portrait") << (int)M::Angle270;
    QTest::newRow("Locked to something else") << QString("unknown") << false << QString() << (int)M::Angle180;
}

void Ut_ScreenLockWindow::testOrientationLocking()
{
    QFETCH(QString, lockedOrientation);
    QFETCH(bool, orientationLocked);
    QFETCH(QString, expectedOrientation);
    QFETCH(int, expectedOrientationAngle);

    // Reset the stubs
    mWindowOrientationLocked = false;
    mWindowOrientation = QString();
    mWindowOrientationAngle = M::Angle180;

    // Set the style
    ScreenLockWindowStyle *style = const_cast<ScreenLockWindowStyle *>(static_cast<const ScreenLockWindowStyle *>(MTheme::style("ScreenLockWindowStyle", "", "", "", M::Landscape, NULL)));
    style->setLockedOrientation(lockedOrientation);

    // Create a new window
    delete lockScreenWindow;
    lockScreenWindow = new ScreenLockWindow(new MApplicationExtensionArea(""));
    QCOMPARE(mWindowOrientationLocked, orientationLocked);
    QCOMPARE(mWindowOrientationAngleLocked, orientationLocked);
    QCOMPARE(mWindowOrientation, expectedOrientation);
    QCOMPARE(mWindowOrientationAngle, (M::OrientationAngle)expectedOrientationAngle);
}

void Ut_ScreenLockWindow::testTranslucency_data()
{
    QTest::addColumn<bool>("translucent");

    QTest::newRow("Translucent") << true;
    QTest::newRow("Not translucent") << false;
}

void Ut_ScreenLockWindow::testTranslucency()
{
    QFETCH(bool, translucent);

    // Set the style
    ScreenLockWindowStyle *style = const_cast<ScreenLockWindowStyle *>(static_cast<const ScreenLockWindowStyle *>(MTheme::style("ScreenLockWindowStyle", "", "", "", M::Landscape, NULL)));
    style->setTranslucent(translucent);

    // Create a new window
    delete lockScreenWindow;
    lockScreenWindow = new ScreenLockWindow(new MApplicationExtensionArea(""));
    QCOMPARE(lockScreenWindow->testAttribute(Qt::WA_TranslucentBackground), translucent);
}

void Ut_ScreenLockWindow::testLowPowerMode_data()
{
    QTest::addColumn<bool>("lowPowerModeEnabled");
    QTest::addColumn<int>("lowPowerModeProperty");
    QTest::addColumn<int>("stackingLayerProperty");

    QTest::newRow("Low power mode enabled") << true << 1 << 7;
    QTest::newRow("Low power mode disabled") << false << 0 << 5;
}

void Ut_ScreenLockWindow::testLowPowerMode()
{
    QFETCH(bool, lowPowerModeEnabled);
    QFETCH(int, lowPowerModeProperty);
    QFETCH(int, stackingLayerProperty);

    lockScreenWindow->setLowPowerMode(lowPowerModeEnabled);

    Display *display = QX11Info::display();
    QCOMPARE(gX11WrapperStub->stubCallCount("XChangeProperty"), 2);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(0)->parameter<Display *>(0), display);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(0)->parameter<Window>(1), lockScreenWindow->effectiveWinId());
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(0)->parameter<Atom>(2), X11Wrapper::XInternAtom(display, "_MEEGO_LOW_POWER_MODE", False));
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(0)->parameter<Atom>(3), XA_CARDINAL);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(0)->parameter<int>(4), 32);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(0)->parameter<int>(5), PropModeReplace);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(0)->parameter<QByteArray>(6).constData()[0], (char)lowPowerModeProperty);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(0)->parameter<int>(7), 1);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(1)->parameter<Display *>(0), display);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(1)->parameter<Window>(1), lockScreenWindow->effectiveWinId());
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(1)->parameter<Atom>(2), X11Wrapper::XInternAtom(display, "_MEEGO_STACKING_LAYER", False));
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(1)->parameter<Atom>(3), XA_CARDINAL);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(1)->parameter<int>(4), 32);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(1)->parameter<int>(5), PropModeReplace);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(1)->parameter<QByteArray>(6).constData()[0], (char)stackingLayerProperty);
    QCOMPARE(gX11WrapperStub->stubCallsTo("XChangeProperty").at(1)->parameter<int>(7), 1);
}

QTEST_APPLESS_MAIN(Ut_ScreenLockWindow)
