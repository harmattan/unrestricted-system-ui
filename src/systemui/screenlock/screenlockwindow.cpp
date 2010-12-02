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
#include "screenlockextensioninterface.h"
#include "sysuid.h"
#include <MApplicationExtensionArea>
#include <MSceneWindow>
#include <QGraphicsLinearLayout>
#include <QX11Info>
#include "x11wrapper.h"

ScreenLockWindow::ScreenLockWindow(QWidget *parent) :
    MWindow(parent)
{
    setWindowTitle("Screen Lock");

    // Create an extension area for the screen lock
    MApplicationExtensionArea *extensionArea = new MApplicationExtensionArea("com.meego.core.ScreenLockExtensionInterface/1.0");
    connect(extensionArea, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)), this, SLOT(registerExtension(MApplicationExtensionInterface*)));
    connect(extensionArea, SIGNAL(extensionRemoved(MApplicationExtensionInterface*)), this, SLOT(unregisterExtension(MApplicationExtensionInterface*)));
    extensionArea->setStyleName("ScreenLockExtensionArea");
    extensionArea->setInProcessFilter(QRegExp("/sysuid-screenlock.desktop$"));
    extensionArea->setOutOfProcessFilter(QRegExp("$^"));
    extensionArea->init();

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
    const ScreenLockWindowStyle *style = static_cast<const ScreenLockWindowStyle *>(MTheme::style("ScreenLockWindowStyle", "", "", "", M::Landscape, NULL));
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

void ScreenLockWindow::reset()
{
    foreach (ScreenLockExtensionInterface *screenLockExtension, screenLockExtensions) {
        screenLockExtension->reset();
    }
}

void ScreenLockWindow::registerExtension(MApplicationExtensionInterface *extension)
{
    ScreenLockExtensionInterface *screenLockExtension = static_cast<ScreenLockExtensionInterface *>(extension);
    screenLockExtensions.append(screenLockExtension);
    screenLockExtension->setNotificationManagerInterface(Sysuid::instance()->notificationManagerInterface());
    connect(screenLockExtension->widget(), SIGNAL(unlocked()), this, SIGNAL(unlocked()));
}

void ScreenLockWindow::unregisterExtension(MApplicationExtensionInterface *extension)
{
    ScreenLockExtensionInterface *screenLockExtension = dynamic_cast<ScreenLockExtensionInterface *>(extension);

    if (screenLockExtension != NULL) {
        screenLockExtensions.removeAll(screenLockExtension);
    }
}

void ScreenLockWindow::showEvent(QShowEvent *event)
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

void ScreenLockWindow::excludeFromTaskBar()
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
