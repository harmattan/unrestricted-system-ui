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
#include <MLocale>

StatusIndicatorMenuWindow::StatusIndicatorMenuWindow(QWidget *parent) :
    MWindow(parent),
    menuWidget(NULL)
{
    currentLanguage = MLocale().language();

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
    connect (&qmLocks, SIGNAL(stateChanged (MeeGo::QmLocks::Lock, MeeGo::QmLocks::State)), this,
                                   SLOT(setWindowStateAccordingToDeviceLockState(MeeGo::QmLocks::Lock, MeeGo::QmLocks::State)));
    if (qmLocks.getState(MeeGo::QmLocks::Device) != MeeGo::QmLocks::Locked) {
        deviceLocked = false;
    } else {
        deviceLocked = true;
    }
#endif

    // Set the X window type, so that the window does not appear in the switcher and
    // home screen can provide the correct UI flow
    setAttribute(Qt::WA_X11NetWmWindowTypeMenu);

    resetMenuWidget();
}

StatusIndicatorMenuWindow::~StatusIndicatorMenuWindow()
{
}

void StatusIndicatorMenuWindow::resetMenuWidget()
{
    if (menuWidget) {
        sceneManager()->dismissSceneWindowNow(menuWidget);
    }

    menuWidget = new StatusIndicatorMenu();
    // The scene takes ownership of menuWidget
    sceneManager()->appearSceneWindowNow(menuWidget, MSceneWindow::DestroyWhenDismissed);

    connect(menuWidget, SIGNAL(showRequested()), this, SLOT(makeVisible()));
    connect(menuWidget, SIGNAL(hideRequested()), this, SLOT(hide()));
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
    }

    // Raise it on top
    raise();
}

#ifdef HAVE_QMSYSTEM
void StatusIndicatorMenuWindow::setWindowStateAccordingToDeviceLockState(MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how)
{
    if (what == MeeGo::QmLocks::Device) {
        if (how == MeeGo::QmLocks::Unlocked) {
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

bool StatusIndicatorMenuWindow::event(QEvent *event)
{
    bool windowIsHandlingEvent = MWindow::event(event);
    // Recreate status menu widget when language changes and window is handling the language change
    if (event->type() == QEvent::LanguageChange && windowIsHandlingEvent) {
        QString newLanguage = MLocale().language();
        // Check that language actually changed to avoid unnecessary resets
        if (currentLanguage != newLanguage) {
            currentLanguage = newLanguage;
            resetMenuWidget();
        }
    }
    return windowIsHandlingEvent;
}
