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
#include <MApplicationExtensionArea>
#include "screenlockextensioninterface.h"
#include "screenlockbusinesslogic.h"
#include "notifiernotificationsink.h"
#include "screenlockwindow.h"
#include "eventeater.h"
#include "closeeventeater.h"
#include "sysuid.h"

ScreenLockBusinessLogic::ScreenLockBusinessLogic(QObject* parent) :
    QObject(parent),
    screenLockWindow(NULL),
    eventEaterWindow(NULL)
{
#ifdef HAVE_QMSYSTEM
    connect(&displayState, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)), this, SLOT(displayStateChanged(MeeGo::QmDisplayState::DisplayState)));
    connect(&locks, SIGNAL(stateChanged(MeeGo::QmLocks::Lock, MeeGo::QmLocks::State)), this, SLOT(locksChanged(MeeGo::QmLocks::Lock, MeeGo::QmLocks::State)));
#endif

    // Create an extension area for the screen lock
    extensionArea = new MApplicationExtensionArea("com.meego.core.ScreenLockExtensionInterface/1.0");
    extensionArea->setStyleName("ScreenLockExtensionArea");
    extensionArea->setInProcessFilter(QRegExp("/sysuid-screenlock.desktop$"));
    extensionArea->setOutOfProcessFilter(QRegExp("$^"));
    connect(extensionArea, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)), this, SLOT(registerExtension(MApplicationExtensionInterface*)));
    connect(extensionArea, SIGNAL(extensionRemoved(MApplicationExtensionInterface*)), this, SLOT(unregisterExtension(MApplicationExtensionInterface*)));
    extensionArea->init();
}

ScreenLockBusinessLogic::~ScreenLockBusinessLogic()
{
    delete screenLockWindow;
    delete eventEaterWindow;
}

void ScreenLockBusinessLogic::registerExtension(MApplicationExtensionInterface *extension)
{
    ScreenLockExtensionInterface *screenLockExtension = static_cast<ScreenLockExtensionInterface *>(extension);
    screenLockExtensions.append(screenLockExtension);
    screenLockExtension->setNotificationManagerInterface(Sysuid::instance()->notificationManagerInterface());
    connect(screenLockExtension->qObject(), SIGNAL(unlocked()), this, SLOT(unlockScreen()));
    connect(screenLockExtension->qObject(), SIGNAL(unlocked()), this, SIGNAL(unlockConfirmed()));
    connect(&Sysuid::instance()->notifierNotificationSink(), SIGNAL(notifierSinkActive(bool)), screenLockExtension->qObject(), SIGNAL(notifierSinkActive(bool)));
}

void ScreenLockBusinessLogic::unregisterExtension(MApplicationExtensionInterface *extension)
{
    ScreenLockExtensionInterface *screenLockExtension = dynamic_cast<ScreenLockExtensionInterface *>(extension);

    if (screenLockExtension != NULL) {
        screenLockExtensions.removeAll(screenLockExtension);
    }
}

void ScreenLockBusinessLogic::reset()
{
    foreach (ScreenLockExtensionInterface *screenLockExtension, screenLockExtensions) {
        screenLockExtension->reset();
    }
}


void ScreenLockBusinessLogic::unlockScreen()
{
    toggleScreenLockUI(false);
}

#ifdef HAVE_QMSYSTEM
void ScreenLockBusinessLogic::displayStateChanged(MeeGo::QmDisplayState::DisplayState state)
{
    // When the screen is unlocked, the screenlock is visible the lock screen needs to be reset
    if (state == MeeGo::QmDisplayState::On && screenLockWindow != NULL && screenLockWindow->isVisible()) {
        reset();
        screenLockWindow->setFocus();
    }
}
#endif

void ScreenLockBusinessLogic::hideEventEater()
{
    toggleEventEater(false);
}

void ScreenLockBusinessLogic::toggleScreenLockUI(bool toggle)
{
    if (toggle) {
        if (screenLockWindow == NULL) {
            // Create the lock screen window if it doesn't exist yet
            screenLockWindow = new ScreenLockWindow(extensionArea);
            screenLockWindow->installEventFilter(new CloseEventEater(this));
        }

        // Whenever we're showing the lock screen we need to reset its state
        reset();

        if (!screenLockWindow->isVisible()) {
            screenLockWindow->show();
        }

        screenLockWindow->raise();
    } else {
        if (screenLockWindow != NULL && screenLockWindow->isVisible()) {
            screenLockWindow->hide();
        }
    }
}

void ScreenLockBusinessLogic::toggleEventEater(bool toggle)
{
    if (toggle) {
        if (eventEaterWindow == NULL) {
            // Create the event eater window if it doesn't exist yet
            eventEaterWindow = new EventEater;
            eventEaterWindow->installEventFilter(new CloseEventEater(this));
            connect(eventEaterWindow, SIGNAL(inputEventReceived()), this, SLOT(hideEventEater()));
        }

        eventEaterWindow->show();
    } else {
        if (eventEaterWindow != NULL) {
            eventEaterWindow->hide();
        }
    }
}

bool ScreenLockBusinessLogic::displayIsOn()
{
#if !defined(__i386__) && defined(HAVE_QMSYSTEM)
    return displayState.get() == MeeGo::QmDisplayState::On;
#else
    return true;
#endif
}

#ifdef HAVE_QMSYSTEM
void ScreenLockBusinessLogic::locksChanged(MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how)
{
    if (what == MeeGo::QmLocks::TouchAndKeyboard) {
        switch (how) {
        case MeeGo::QmLocks::Locked:
            emit screenIsLocked(true);
            break;
        default:
            emit screenIsLocked(false);
            break;
        }
    }
}
#endif
