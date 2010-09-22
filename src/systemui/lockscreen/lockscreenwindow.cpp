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

void LockScreenWindow::showEvent(QShowEvent *)
{
    Display *display = QX11Info::display();
    Window windowID = internalWinId();

    // Set the stacking layer.
    Atom stackingLayerAtom = X11Wrapper::XInternAtom(display, "_MEEGO_STACKING_LAYER", False);
    if (stackingLayerAtom != None) {
        long layer = 2;
        X11Wrapper::XChangeProperty(display, windowID, stackingLayerAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)&layer, 1);
    }
}
