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
    lockScreen->setParent(this);
    lockScreen->setObjectName("LockScreen");
    lockScreen->appear(this);
}

LockScreenWindow::~LockScreenWindow()
{
    delete lockScreen;
}

void LockScreenWindow::reset()
{
    lockScreen->reset();
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
}

void LockScreenWindow::excludeFromTaskBar()
{
    // Tell the window to not to be shown in the switcher
    Atom skipTaskbarAtom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_STATE_SKIP_TASKBAR", False);
    changeNetWmState(true, skipTaskbarAtom);

    Atom netWmStateAtom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_STATE", False);
    QVector<Atom> atoms;
    atoms.append(skipTaskbarAtom);
    X11Wrapper::XChangeProperty(QX11Info::display(), internalWinId(), netWmStateAtom, XA_ATOM, 32, PropModeReplace, (unsigned char *)atoms.data(), atoms.count());
 }

void LockScreenWindow::changeNetWmState(bool set, Atom one, Atom two)
{
    XEvent e;
    e.xclient.type = ClientMessage;
    Display *display = QX11Info::display();
    Atom netWmStateAtom = X11Wrapper::XInternAtom(display, "_NET_WM_STATE", FALSE);
    e.xclient.message_type = netWmStateAtom;
    e.xclient.display = display;
    e.xclient.window = internalWinId();
    e.xclient.format = 32;
    e.xclient.data.l[0] = set ? 1 : 0;
    e.xclient.data.l[1] = one;
    e.xclient.data.l[2] = two;
    e.xclient.data.l[3] = 0;
   e.xclient.data.l[4] = 0;
    X11Wrapper::XSendEvent(display, RootWindow(display, x11Info().screen()), FALSE, (SubstructureNotifyMask | SubstructureRedirectMask), &e);
    X11Wrapper::XSync(display, FALSE);
}

