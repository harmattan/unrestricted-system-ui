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
#include "ut_lockscreenwindow.h"
#include "screenlockwindow.h"
#include "screenlockwindowstyle.h"
#include "notifiernotificationsink_stub.h"
#include "notificationsink_stub.h"
#include "sysuid_stub.h"
#include <X11/Xutil.h>
#include "x11wrapper_stub.h"

// X stubs to avoid crashes
void XSetWMProperties(Display *, Window, XTextProperty *, XTextProperty *, char **, int, XSizeHints *, XWMHints *, XClassHint *)
{
}

bool mWindowOrientationLocked = false;
void MWindow::setOrientationLocked(bool locked)
{
    mWindowOrientationLocked = locked;
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

bool MApplicationExtensionArea::init()
{
    return true;
}

MWindow* appearedWindow = NULL;
void MSceneWindow::appear(MWindow* window, MSceneWindow::DeletionPolicy policy)
{
    appearedWindow = window;
    Q_UNUSED(policy);
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

void Ut_LockScreenWindow::initTestCase()
{
    int   argc = 1;
    char *argv[] = {(char *) "./Ut_LockScreenWindow", NULL };
    app = new MApplication(argc, argv);
    notifierSink = new NotifierNotificationSink;
    gSysuidStub->stubSetReturnValue("notifierNotificationSink", notifierSink);
}

void Ut_LockScreenWindow::cleanupTestCase()
{
    delete notifierSink;
    delete app;
}

void Ut_LockScreenWindow::init()
{
    lockScreenWindow = new ScreenLockWindow;
}

void Ut_LockScreenWindow::cleanup()
{
    delete lockScreenWindow;
    lockScreenWindow = NULL;
    appearedWindow = NULL;
    mWindowOrientationLocked = false;
    mWindowOrientation.clear();
    gX11WrapperStub->stubReset();
    screenLockExtensionReset = false;
}

void Ut_LockScreenWindow::testWhenExtensionIsRegisteredSignalsAreConnected()
{
    ScreenLockExtension screenLockExtension;
    screenLockExtension.initialize("");
    lockScreenWindow->registerExtension(&screenLockExtension);
    QVERIFY(disconnect(screenLockExtension.qObject(), SIGNAL(unlocked()), lockScreenWindow, SIGNAL(unlocked())));
    QVERIFY(disconnect(&Sysuid::instance()->notifierNotificationSink(), SIGNAL(notifierSinkActive(bool)), screenLockExtension.qObject(), SIGNAL(notifierSinkActive(bool))));
}

void Ut_LockScreenWindow::testWhenWindowIsCreatedLockScreenAppears()
{
    QCOMPARE(appearedWindow, lockScreenWindow);
}

void Ut_LockScreenWindow::testWhenWindowIsShownItIsExcludedFromTaskbar()
{
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
    QCOMPARE(event.xclient.window, lockScreenWindow->internalWinId());
    QCOMPARE(event.xclient.message_type, X11Wrapper::XInternAtom(display, "_NET_WM_STATE", False));
    QCOMPARE(event.xclient.format, 32);
    QCOMPARE(event.xclient.data.l[0], (long)1);
    QCOMPARE(event.xclient.data.l[1], (long)X11Wrapper::XInternAtom(display, "_NET_WM_STATE_SKIP_TASKBAR", False));
    delete showEvent;
}

void Ut_LockScreenWindow::testOrientationLocking_data()
{
    QTest::addColumn<QString>("lockedOrientation");
    QTest::addColumn<bool>("orientationLocked");
    QTest::addColumn<QString>("expectedOrientation");

    QTest::newRow("No locking") << QString() << false << QString();
    QTest::newRow("Locked to landscape") << QString("landscape") << true << QString("landscape");
    QTest::newRow("Locked to portrait") << QString("portrait") << true << QString("portrait");
    QTest::newRow("Locked to something else") << QString("unknown") << false << QString();
}

void Ut_LockScreenWindow::testOrientationLocking()
{
    QFETCH(QString, lockedOrientation);
    QFETCH(bool, orientationLocked);
    QFETCH(QString, expectedOrientation);

    ScreenLockWindowStyle *style = const_cast<ScreenLockWindowStyle *>(static_cast<const ScreenLockWindowStyle *>(MTheme::style("ScreenLockWindowStyle", "", "", "", M::Landscape, NULL)));
    style->setLockedOrientation(lockedOrientation);
    QShowEvent event;
    lockScreenWindow->showEvent(&event);
    QCOMPARE(mWindowOrientationLocked, orientationLocked);
    QCOMPARE(mWindowOrientation, expectedOrientation);
}

void Ut_LockScreenWindow::testReset()
{
    ScreenLockExtension screenLockExtension;
    screenLockExtension.initialize("");
    lockScreenWindow->registerExtension(&screenLockExtension);
    lockScreenWindow->reset();
    QVERIFY(screenLockExtensionReset);
}

QTEST_APPLESS_MAIN(Ut_LockScreenWindow)
