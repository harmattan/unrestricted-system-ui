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
#include "lockscreenwindow.h"
#include "lockscreenwindowstyle.h"
#include "lockscreen.h"
#include <QX11Info>
#include "x11wrapper.h"

LockScreenWindow::LockScreenWindow(QWidget *parent) :
    MWindow(parent),
    lockScreen(new LockScreen)
{
    setWindowTitle("LockScreenUI");
    setObjectName("LockScreenWindow");

    connect(lockScreen, SIGNAL(unlocked()), this, SIGNAL(unlocked()));
    lockScreen->setObjectName("LockScreen");
    lockScreen->appear(this);
}

LockScreenWindow::~LockScreenWindow()
{
    delete lockScreen;
}

void LockScreenWindow::applyStyle()
{
    const LockScreenWindowStyle *style = static_cast<const LockScreenWindowStyle *>(MTheme::style("LockScreenWindowStyle", "", "", "", M::Landscape, NULL));
    if (style->lockedOrientation() == "landscape") {
        setLandscapeOrientation();
        setOrientationLocked(true);
    } else if (style->lockedOrientation() == "portrait") {
        setPortraitOrientation();
        setOrientationLocked(true);
    } else {
        setOrientationLocked(false);
    }
}

void LockScreenWindow::reset()
{
    lockScreen->reset();

    // Ask the lock screen to redraw itself since it may be necessary if the display used to be turned off
    lockScreen->update();
}

void LockScreenWindow::showEvent(QShowEvent *event)
{
    MWindow::showEvent(event);

    // Set the stacking layer
    Display *display = QX11Info::display();
    Atom stackingLayerAtom = X11Wrapper::XInternAtom(display, "_MEEGO_STACKING_LAYER", False);
    if (stackingLayerAtom != None) {
        long layer = 2;
        X11Wrapper::XChangeProperty(display, internalWinId(), stackingLayerAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)&layer, 1);
    }

    excludeFromTaskBar();
    applyStyle();
}

void LockScreenWindow::excludeFromTaskBar()
{
    // Tell the window to not to be shown in the switcher
    Display *display = QX11Info::display();
    XEvent e;
    memset(&e, 0, sizeof(XEvent));
    e.xclient.type = ClientMessage;
    e.xclient.display = display;
    e.xclient.window = internalWinId();
    e.xclient.message_type = X11Wrapper::XInternAtom(display, "_NET_WM_STATE", False);
    e.xclient.format = 32;
    e.xclient.data.l[0] = 1;
    e.xclient.data.l[1] = X11Wrapper::XInternAtom(display, "_NET_WM_STATE_SKIP_TASKBAR", False);
    X11Wrapper::XSendEvent(display, RootWindow(display, x11Info().screen()), False, (SubstructureNotifyMask | SubstructureRedirectMask), &e);
    X11Wrapper::XSync(display, False);
}
