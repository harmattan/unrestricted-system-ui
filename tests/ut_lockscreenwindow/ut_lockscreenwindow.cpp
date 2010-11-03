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

#include <MApplication>
#include <QShowEvent>
#include "ut_lockscreenwindow.h"
#include "lockscreenwindow.h"
#include "lockscreenwindowstyle.h"
#include "lockscreen_stub.h"
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

MWindow* appearedWindow = NULL;
void MSceneWindow::appear(MWindow* window, MSceneWindow::DeletionPolicy policy)
{
    appearedWindow = window;
    Q_UNUSED(policy);
}

QPair<QGraphicsItem *, QRectF> qGraphicsItemUpdate(NULL, QRectF());
void QGraphicsItem::update(const QRectF &rect)
{
    qGraphicsItemUpdate.first = this;
    qGraphicsItemUpdate.second = rect;
}

void Ut_LockScreenWindow::initTestCase()
{
    int   argc = 1;
    char *argv[] = {(char *) "./Ut_LockScreenWindow", NULL };
    app = new MApplication(argc, argv);
}

void Ut_LockScreenWindow::cleanupTestCase()
{
    delete app;
}

void Ut_LockScreenWindow::init()
{
    lockScreenWindow = new LockScreenWindow;
}

void Ut_LockScreenWindow::cleanup()
{
    delete lockScreenWindow;
    lockScreenWindow = NULL;
    appearedWindow = NULL;
    mWindowOrientationLocked = false;
    mWindowOrientation.clear();
    gX11WrapperStub->stubReset();
    gLockScreenStub->stubReset();
    qGraphicsItemUpdate.first = NULL;
    qGraphicsItemUpdate.second = QRectF();
}

void Ut_LockScreenWindow::testWhenWindowIsCreatedUnlockedSignalFromLockScreenIsChainedToUnlockedSignal()
{
    bool result = disconnect(lockScreenWindow->lockScreen, SIGNAL(unlocked()), lockScreenWindow, SIGNAL(unlocked()));
    QCOMPARE(result, true);
}

void Ut_LockScreenWindow::testWhenWindowIsCreatedLockScreenAppears()
{
    QCOMPARE(appearedWindow, lockScreenWindow);
}

void Ut_LockScreenWindow::testWhenWindowIsShownItIsExcludedFromTaskbar()
{
    Display *display = QX11Info::display();

    lockScreenWindow->show();
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

    LockScreenWindowStyle *style = const_cast<LockScreenWindowStyle *>(static_cast<const LockScreenWindowStyle *>(MTheme::style("LockScreenWindowStyle", "", "", "", M::Landscape, NULL)));
    style->setLockedOrientation(lockedOrientation);
    QShowEvent event;
    lockScreenWindow->showEvent(&event);
    QCOMPARE(mWindowOrientationLocked, orientationLocked);
    QCOMPARE(mWindowOrientation, expectedOrientation);
}

void Ut_LockScreenWindow::testReset()
{
    lockScreenWindow->reset();

    QCOMPARE(gLockScreenStub->stubCallCount("reset"), 1);
    QCOMPARE(qGraphicsItemUpdate.first, lockScreenWindow->lockScreen);
    QCOMPARE(qGraphicsItemUpdate.second, QRectF());
}

QTEST_APPLESS_MAIN(Ut_LockScreenWindow)
