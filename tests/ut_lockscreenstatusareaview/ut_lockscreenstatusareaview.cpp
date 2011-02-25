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
#include <MApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <MSceneManager>
#include "ut_lockscreenstatusareaview.h"
#include "lockscreenstatusareaview.h"
#include "statusarea.h"
#include "clock_stub.h"
#include "statusindicator_stub.h"
#include "contextframeworkcontext_stub.h"
#include "alarmstatusindicator_stub.h"
#include "batterystatusindicator_stub.h"
#include "phonenetworksignalstrengthstatusindicator_stub.h"
#include "phonenetworktypestatusindicator_stub.h"
#include "bluetoothstatusindicator_stub.h"
#include "gpsstatusindicator_stub.h"
#include "presencestatusindicator_stub.h"
#include "statusindicator_stub.h"
#include "phonenetworkstatusindicator_stub.h"
#include "inputmethodstatusindicator_stub.h"
#include "callstatusindicator_stub.h"
#include "profilestatusindicator_stub.h"
#include "callforwardingstatusindicator_stub.h"
#include "notificationstatusindicator_stub.h"
#include "transferstatusindicator_stub.h"
#include "screenlockextension_stub.h"
#include "x11wrapper.h"
#include "notificationsink_stub.h"

WId qWidgetWinId = 0xdeadbeef;
WId QWidget::winId() const
{
    return qWidgetWinId;
}

MSceneManager *mWidgetSceneManager = NULL;
MSceneManager *MWidget::sceneManager() const
{
    return mWidgetSceneManager;
}

Atom X11Wrapper::XInternAtom(Display *, const char*, Bool)
{
    return 0;
}

int X11Wrapper::XSelectInput(Display *, Window , long)
{
    return 0;
}

Status X11Wrapper::XGetWindowAttributes(Display *, Window, XWindowAttributes *)
{
    return 0;
}

int X11Wrapper::XGetWindowProperty(Display *, Window , Atom, long , long , Bool , Atom , Atom *, int *, unsigned long *, unsigned long *, unsigned char **)
{
    return BadAtom;
}

int X11Wrapper::XFree(void *data)
{
    if (data != NULL) {
        delete [](unsigned char *)data;
    }
    return 0;
}

Status X11Wrapper::XGetWMName(Display *, Window, XTextProperty *)
{
    return 1;
}

Status X11Wrapper::XGetTextProperty(Display *, Window , XTextProperty *, Atom)
{
    return 0;
}

XWMHints *X11Wrapper::XGetWMHints(Display *, Window)
{
    return 0;
}

int X11Wrapper::XFreePixmap(Display *, Pixmap)
{
    return 0;
}

Damage X11Wrapper::XDamageCreate(Display *, Drawable, int)
{
    return 0;
}

void X11Wrapper::XDamageDestroy(Display *, Damage)
{
}

int X11Wrapper::XSync(Display *, Bool)
{
    return 0;
}

XErrorHandler X11Wrapper::XSetErrorHandler(XErrorHandler)
{
    return 0;
}

int xChangePropertyCallCount = 0;
Display *xChangePropertyDisplay = NULL;
Window xChangePropertyWindow = 0;
Atom xChangePropertyProperty = 0;
Atom xChangePropertyType = 0;
int xChangePropertyFormat = 0;
int xChangePropertyMode = 0;
unsigned char xChangePropertyData[16];
int xChangePropertyNElements = 0;
int X11Wrapper::XChangeProperty(Display *display, Window w, Atom property, Atom type, int format, int mode, unsigned char *data, int nelements)
{
    xChangePropertyCallCount++;
    xChangePropertyDisplay = display;
    xChangePropertyWindow = w;
    xChangePropertyProperty = property;
    xChangePropertyType = type;
    xChangePropertyFormat = format;
    xChangePropertyMode = mode;
    memcpy(xChangePropertyData, data, nelements * format / 8);
    xChangePropertyNElements = nelements;
    return 0;
}

Status X11Wrapper::XSendEvent(Display *, Window, Bool, long, XEvent *)
{
    return 0;
}

// Called before the first testfunction is executed
void Ut_LockScreenStatusAreaView::initTestCase()
{
    int argc = 1;
    static char *app_name = (char *)"./ut_lockscreenstatusareaview";
    app = new MApplication(argc, &app_name);
    extension = new ScreenLockExtension;
    gScreenLockExtensionStub->stubSetReturnValue("instance", extension);
}

// Called after the last testfunction was executed
void Ut_LockScreenStatusAreaView::cleanupTestCase()
{
    delete extension;
    delete app;
}

// Called before each testfunction is executed
void Ut_LockScreenStatusAreaView::init()
{
    statusArea = new StatusArea;
    m_subject = new LockScreenStatusAreaView(statusArea);
    statusArea->setView(m_subject);
}

// Called after every testfunction
void Ut_LockScreenStatusAreaView::cleanup()
{
    delete statusArea;
    statusArea = NULL;
    xChangePropertyCallCount = 0;
    xChangePropertyDisplay = NULL;
    xChangePropertyWindow = 0;
    xChangePropertyProperty = 0;
    xChangePropertyType = 0;
    xChangePropertyFormat = 0;
    xChangePropertyMode = 0;
    xChangePropertyNElements = 0;
    mWidgetSceneManager = NULL;
}

void Ut_LockScreenStatusAreaView::testSignalConnections()
{
    QVERIFY(disconnect(extension, SIGNAL(notifierSinkActive(bool)), m_subject->notifierIndicator, SLOT(setActive(bool))));
}

void Ut_LockScreenStatusAreaView::testStatusBarGeometryProperty()
{
    Display *display = QX11Info::display();
    QCOMPARE(xChangePropertyCallCount, 1);
    QCOMPARE(xChangePropertyDisplay, display);
    QCOMPARE(xChangePropertyWindow, qWidgetWinId);
    QCOMPARE(xChangePropertyProperty, X11Wrapper::XInternAtom(display, "_MEEGOTOUCH_MSTATUSBAR_GEOMETRY", False));
    QCOMPARE(xChangePropertyType, XA_CARDINAL);
    QCOMPARE(xChangePropertyFormat, 32);
    QCOMPARE(xChangePropertyMode, PropModeReplace);
    QCOMPARE(xChangePropertyNElements, 4);
    long *data = (long *)xChangePropertyData;
    QRectF statusBarGeometry = statusArea->mapRectToScene(QRectF(QPointF(), statusArea->geometry().size()));
    QCOMPARE(data[0], (long)statusBarGeometry.x());
    QCOMPARE(data[1], (long)statusBarGeometry.y());
    QCOMPARE(data[2], (long)statusBarGeometry.width());
    QCOMPARE(data[3], (long)statusBarGeometry.height());
}

void Ut_LockScreenStatusAreaView::testSetGeometryConnectsOrientationChangeSignal()
{
    QRectF geometry(0, 0, 15, 20);
    statusArea->setGeometry(geometry);
    QVERIFY(!disconnect(statusArea->sceneManager(), SIGNAL(orientationChangeFinished(M::Orientation)), m_subject, SLOT(updateStatusBarGeometryProperty())));
    QCOMPARE(xChangePropertyCallCount, 0);

    MSceneManager manager;
    mWidgetSceneManager = &manager;
    QGraphicsScene scene;
    QGraphicsView view;
    view.setScene(&scene);
    scene.addItem(statusArea);
    statusArea->setGeometry(geometry);
    QVERIFY(disconnect(statusArea->sceneManager(), SIGNAL(orientationChangeFinished(M::Orientation)), m_subject, SLOT(updateStatusBarGeometryProperty())));
    testStatusBarGeometryProperty();
    scene.removeItem(statusArea);
}

void Ut_LockScreenStatusAreaView::testOrientationChangeUpdatesStatusBarGeometry()
{
    QGraphicsScene scene;
    QGraphicsView view;
    view.setScene(&scene);
    scene.addItem(statusArea);
    statusArea->setGeometry(QRectF(5, 10, 15, 20));
    m_subject->updateStatusBarGeometryProperty();

    testStatusBarGeometryProperty();

    // Setting the same geometry should do nothing
    m_subject->updateStatusBarGeometryProperty();
    QCOMPARE(xChangePropertyCallCount, 1);

    scene.removeItem(statusArea);
}

QTEST_APPLESS_MAIN(Ut_LockScreenStatusAreaView)
