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
#include "screenlockwindow.h"
#include "screenlockwindowstyle.h"
#include <MApplicationExtensionArea>
#include <MSceneManager>
#include <MSceneWindow>
#include <QGraphicsLinearLayout>
#include <QX11Info>
#include "x11wrapper.h"

ScreenLockWindow::ScreenLockWindow(MApplicationExtensionArea *extensionArea, QWidget *parent) :
    MWindow(parent), lowPowerMode(false)
{
    setWindowTitle("Screen Lock");
    setSceneManager(new MSceneManager);

    applyStyle();

    // Create a layout for the extension area
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addItem(extensionArea);

    // Put the extension area into a full screen scene window
    MSceneWindow *screenLockWindow = new MSceneWindow;
    screenLockWindow->setStyleName("ScreenLockWindow");
    screenLockWindow->setLayout(layout);
    screenLockWindow->appear(this);
}

ScreenLockWindow::~ScreenLockWindow()
{
}

void ScreenLockWindow::applyStyle()
{
    const ScreenLockWindowStyle *style = static_cast<const ScreenLockWindowStyle *>(MTheme::style("ScreenLockWindowStyle"));

    // This must be set before setting the orientation. See bug #230352.
    setTranslucentBackground(style->translucent());

    if (!style->translucent()) {
        // Fill the window with black by default if it's not translucent
        setBackgroundBrush(Qt::black);
    }

    if (style->lockedOrientation() == "landscape") {
        setLandscapeOrientation();
        setOrientationAngle(M::Angle0);
        setOrientationLocked(true);
        setOrientationAngleLocked(true);
    } else if (style->lockedOrientation() == "portrait") {
        setPortraitOrientation();
        setOrientationAngle(M::Angle270);
        setOrientationLocked(true);
        setOrientationAngleLocked(true);
    } else {
        setOrientationLocked(false);
        setOrientationAngleLocked(false);
    }

    MTheme::releaseStyle(style);
}

void ScreenLockWindow::showEvent(QShowEvent *event)
{
    MWindow::showEvent(event);

    // The window properties cannot be set before the window is shown for the first time, so they're set here.
    setSkipTaskbarProperty();
    setOrientationAngleProperty();
    setLowPowerMode(lowPowerMode);
}

void ScreenLockWindow::setLowPowerMode(bool enable)
{
    lowPowerMode = enable;

    // Both the low power mode property and the stacking layer change when the low power mode changes
    setLowPowerModeProperty();
    setStackingLayerProperty();
}

void ScreenLockWindow::paintEvent(QPaintEvent *event)
{
    // Avoid running MWindow's paintEvent, because it stops painting when
    // display is off, but we need to paint the low power mode view even in that state
    QGraphicsView::paintEvent(event);
}

void ScreenLockWindow::setLowPowerModeProperty()
{
    Display *display = QX11Info::display();
    Atom lowPowerModeAtom = X11Wrapper::XInternAtom(display, "_MEEGO_LOW_POWER_MODE", False);
    if (lowPowerModeAtom != None) {
        long mode = lowPowerMode ? 1 : 0;
        X11Wrapper::XChangeProperty(display, effectiveWinId(), lowPowerModeAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)&mode, 1);
    }
}

void ScreenLockWindow::setStackingLayerProperty()
{
    Display *display = QX11Info::display();
    Atom stackingLayerAtom = X11Wrapper::XInternAtom(display, "_MEEGO_STACKING_LAYER", False);
    if (stackingLayerAtom != None) {
        long layer = lowPowerMode ? 7 : 5;
        X11Wrapper::XChangeProperty(display, effectiveWinId(), stackingLayerAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)&layer, 1);
    }
}

void ScreenLockWindow::setOrientationAngleProperty()
{
    Display *display = QX11Info::display();
    Atom orientationAngleAtom = X11Wrapper::XInternAtom(display, "_MEEGOTOUCH_ORIENTATION_ANGLE", False);
    if (orientationAngleAtom != None) {
        M::OrientationAngle angle = sceneManager()->orientationAngle();
        X11Wrapper::XChangeProperty(display, effectiveWinId(), orientationAngleAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)&angle, 1);
    }
}

void ScreenLockWindow::setSkipTaskbarProperty()
{
    Display *display = QX11Info::display();
    XEvent e;
    memset(&e, 0, sizeof(XEvent));
    e.xclient.type = ClientMessage;
    e.xclient.display = display;
    e.xclient.window = effectiveWinId();
    e.xclient.message_type = X11Wrapper::XInternAtom(display, "_NET_WM_STATE", False);
    e.xclient.format = 32;
    e.xclient.data.l[0] = 1;
    e.xclient.data.l[1] = X11Wrapper::XInternAtom(display, "_NET_WM_STATE_SKIP_TASKBAR", False);
    X11Wrapper::XSendEvent(display, RootWindow(display, x11Info().screen()), False, (SubstructureNotifyMask | SubstructureRedirectMask), &e);
    X11Wrapper::XSync(display, False);
}
