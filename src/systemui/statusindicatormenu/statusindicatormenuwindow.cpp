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
#include <MSceneManager>
#include <MScene>
#include "statusindicatormenuwindow.h"
#include "statusindicatormenu.h"


StatusIndicatorMenuWindow::StatusIndicatorMenuWindow(QWidget *parent) :
    MWindow(parent),
    menuWidget(new StatusIndicatorMenu())
{
    setTranslucentBackground(true);
    setWindowTitle("Status Indicator Menu");
    setProperty("followsCurrentApplicationWindowOrientation", true);
    connect(this, SIGNAL(displayEntered()), this, SLOT(displayActive()));
    connect(this, SIGNAL(displayExited()), this, SLOT(displayInActive()));

#ifdef HAVE_QMSYSTEM
    /*
     * We need to receive updates when device lock state changes
     * to prevent status indicator menu opening when device lock is on
     */
    connect (&qmLocks, SIGNAL(stateChanged (Maemo::QmLocks::Lock, Maemo::QmLocks::State)), this,
                                   SLOT(setWindowStateAccordingToDeviceLockState(Maemo::QmLocks::Lock, Maemo::QmLocks::State)));
    if (qmLocks.getState(Maemo::QmLocks::Device) != Maemo::QmLocks::Locked) {
        deviceLocked = false;
    } else {
        deviceLocked = true;
    }
#endif

    // Set the X window type, so that the window does not appear in the switcher and
    // home screen can provide the correct UI flow
    setAttribute(Qt::WA_X11NetWmWindowTypeMenu);

    // The scene takes ownership of menuWidget
    sceneManager()->appearSceneWindowNow(menuWidget);

    connect(menuWidget, SIGNAL(showRequested()), this, SLOT(makeVisible()));
    connect(menuWidget, SIGNAL(hideRequested()), this, SLOT(hide()));
}

StatusIndicatorMenuWindow::~StatusIndicatorMenuWindow()
{
}


void StatusIndicatorMenuWindow::displayActive()
{
    emit visibilityChanged(true);
}

void StatusIndicatorMenuWindow::displayInActive()
{
    emit visibilityChanged(false);
    // Hide the window when the it is obscured by another view
    // Note: Dialogs and notifications won't close it anyways,
    // as they are not supposed to be full screen and don't completely
    // obstruct the status menu window fully.
    hide();
}

void StatusIndicatorMenuWindow::makeVisible()
{
#ifdef HAVE_QMSYSTEM
    if (deviceLocked) {
        return;
    }
#endif
    if (!isVisible()) {
        // If status indicator window is not visible, then show it
        show();
    } else {
        // Otherwise, raise it
        raise();
    }
}

#ifdef HAVE_QMSYSTEM
void StatusIndicatorMenuWindow::setWindowStateAccordingToDeviceLockState(Maemo::QmLocks::Lock what, Maemo::QmLocks::State how)
{
    if (what == Maemo::QmLocks::Device) {
        if (how == Maemo::QmLocks::Unlocked) {
            deviceLocked = false;
        } else {
            deviceLocked = true;
            if (isVisible()) {
                hide();
            }
        }
    }
}

#endif
