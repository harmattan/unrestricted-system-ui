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
#include <MLocale>
#include <mstatusbar.h>
#include "statusindicatormenuwindow.h"
#include "statusindicatormenu.h"

StatusIndicatorMenuWindow::StatusIndicatorMenuWindow(QWidget *parent) :
    MWindow(parent),
    statusBar(new MStatusBar),
    menuWidget(NULL)
{
    // Show status bar
    setSceneManager(new MSceneManager);
    sceneManager()->appearSceneWindowNow(statusBar);

    currentLanguage = MLocale().language();

    // Set the X window type, so that the window does not appear in the switcher and
    // home screen can provide the correct UI flow
    setAttribute(Qt::WA_X11NetWmWindowTypeMenu);
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

    resetMenuWidget();
}

StatusIndicatorMenuWindow::~StatusIndicatorMenuWindow()
{
    delete menuWidget;
}

void StatusIndicatorMenuWindow::resetMenuWidget()
{
    delete menuWidget;

    menuWidget = new StatusIndicatorMenu();

    connect(menuWidget, SIGNAL(showRequested()), this, SLOT(makeVisible()));
    connect(menuWidget, SIGNAL(hideRequested()), menuWidget, SLOT(disappear()));
    connect(menuWidget, SIGNAL(disappeared()), this, SLOT(hideWindow()));
}

void StatusIndicatorMenuWindow::hideWindow()
{
    hide();
    emit visibilityChanged(false);
}

void StatusIndicatorMenuWindow::displayActive()
{
    emit visibilityChanged(true);
    sceneManager()->appearSceneWindow(menuWidget);
}

void StatusIndicatorMenuWindow::displayInActive()
{
    if (menuWidget && menuWidget->sceneWindowState() != MSceneWindow::Disappeared) {
        sceneManager()->disappearSceneWindowNow(menuWidget);
    }

    // Hide the window when the it is obscured by another view
    // Note: Dialogs and notifications won't close it anyways,
    // as they are not supposed to be full screen and don't completely
    // obstruct the status menu window fully.
    hideWindow();
}

void StatusIndicatorMenuWindow::makeVisible()
{
#ifdef HAVE_QMSYSTEM
    if (deviceLocked) {
        return;
    }
#endif

    // Show the window if it's not visible (from Qt's point of view). Note that isOnDisplay() should NOT be used here: show() means setVisible(true) and the accessor is isVisible().
    if (!isVisible()) {
        show();
    }

    // Always raise the window. Even if it's visible (from Qt's point of view) it may not be the topmost window.
    raise();

    // If the menu is closed and opened quickly enough, we might never get the displayEnter signal because the display state
    // exits with a delay, so displayActive is not called and we need to make the menu widget appear here
    if(menuWidget->sceneWindowState() == MSceneWindow::Disappeared && isOnDisplay()) {
        sceneManager()->appearSceneWindow(menuWidget);
    }
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

void StatusIndicatorMenuWindow::mousePressEvent(QMouseEvent * event)
{
    mousePressPosition = event->pos();

    // Don't pass the event to the status bar
    if (itemAt(mousePressPosition) != statusBar) {
        MWindow::mousePressEvent(event);
    }
}

void StatusIndicatorMenuWindow::mouseReleaseEvent(QMouseEvent * event)
{
    if (itemAt(mousePressPosition) == statusBar && menuWidget) {
        menuWidget->disappear();
    } else {
        MWindow::mouseReleaseEvent(event);
    }
}
