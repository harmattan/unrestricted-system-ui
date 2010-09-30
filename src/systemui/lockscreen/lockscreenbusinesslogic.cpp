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
#include "lockscreenbusinesslogic.h"
#include "lockscreenwindow.h"
#include "eventeater.h"

LockScreenBusinessLogic::LockScreenBusinessLogic(QObject* parent) :
    QObject(parent),
    lockScreenWindow(new LockScreenWindow),
    eventEaterWindow(new EventEater)
{
    connect(lockScreenWindow, SIGNAL(unlocked()), this, SLOT(unlockScreen()));
    connect(lockScreenWindow, SIGNAL(unlocked()), this, SIGNAL(unlockConfirmed()));

    // Hide the event eater when it is clicked
    connect(eventEaterWindow, SIGNAL(inputEventReceived()), this, SLOT(hideEventEater()));

#ifdef HAVE_QMSYSTEM
    connect(&displayState, SIGNAL(displayStateChanged(Maemo::QmDisplayState::DisplayState)), this, SLOT(displayStateChanged(Maemo::QmDisplayState::DisplayState)));
    connect(&locks, SIGNAL(stateChanged(Maemo::QmLocks::Lock, Maemo::QmLocks::State)), this, SLOT(locksChanged(Maemo::QmLocks::Lock, Maemo::QmLocks::State)));
#endif

}

LockScreenBusinessLogic::~LockScreenBusinessLogic()
{
    delete lockScreenWindow;
    delete eventEaterWindow;
}

void LockScreenBusinessLogic::unlockScreen()
{
    toggleScreenLockUI(false);
}

#ifdef HAVE_QMSYSTEM
void LockScreenBusinessLogic::displayStateChanged(Maemo::QmDisplayState::DisplayState state)
{
    // When the screen is unlocked, the lockscreen is visible the lock screen needs to be reset
    if (state == Maemo::QmDisplayState::On && lockScreenWindow->isVisible()) {
        lockScreenWindow->reset();
        lockScreenWindow->setFocus();
    }
}
#endif

void LockScreenBusinessLogic::hideEventEater()
{
    toggleEventEater(false);
}

void LockScreenBusinessLogic::toggleScreenLockUI(bool toggle)
{
    if (toggle) {
        // Whenever we're showing the lock screen we need to reset its state
        lockScreenWindow->reset();

        if (!lockScreenWindow->isVisible()) {
            lockScreenWindow->show();
        }

        lockScreenWindow->raise();
    } else {
        if (lockScreenWindow->isVisible()) {
            lockScreenWindow->hide();
        }
    }
}

void LockScreenBusinessLogic::toggleEventEater(bool toggle)
{
    if (toggle) {
        if (!eventEaterWindow->isVisible()) {
            eventEaterWindow->show();
            eventEaterWindow->showFullScreen();
        }
    } else {
        if (eventEaterWindow->isVisible()) {
            eventEaterWindow->hide();
        }
    }
}

bool LockScreenBusinessLogic::displayIsOn()
{
#if !defined(__i386__) && defined(HAVE_QMSYSTEM)
    return displayState.get() == Maemo::QmDisplayState::On;
#else
    return true;
#endif
}

#if HAVE_QMSYSTEM
void LockScreenBusinessLogic::locksChanged(Maemo::QmLocks::Lock what, Maemo::QmLocks::State how)
{
    if (what == Maemo::QmLocks::TouchAndKeyboard) {
        switch (how) {
        case Maemo::QmLocks::Locked:
            emit screenIsLocked(true);
            break;
        default:
            emit screenIsLocked(false);
            break;
        }
    }
}
#endif
