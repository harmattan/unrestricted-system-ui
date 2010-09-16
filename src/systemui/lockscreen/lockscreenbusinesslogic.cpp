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
#include <QDBusInterface>
#include <QTime>
#include <QX11Info>
#include <MApplication>

#include "lockscreenwindow.h"
#include "eventeater.h"
#include "lockscreenbusinesslogic.h"

#undef DEBUG
#define WARNING
#include "debug.h"

#include <X11/Xlib.h>
#include <X11/Xatom.h>


LockScreenBusinessLogic::LockScreenBusinessLogic (QObject* parent) :
    QObject (parent),
    lockScreenWindow (new LockScreenWindow),
    eaterUI (new EventEater)
{
    bool connectSuccess;
    SYS_DEBUG ("");

    /*
     * Connecting the lockUI signals.
     */
    connectSuccess = connect (lockScreenWindow, SIGNAL (unlocked ()), this, SLOT (unlockScreen ()));
    Q_ASSERT (connectSuccess);

    connectSuccess = connect (lockScreenWindow, SIGNAL (unlocked ()), this, SIGNAL (unlockConfirmed ()));
    Q_ASSERT (connectSuccess);

    /*
     * Hiding the event eater when it is clicked.
     */
    connectSuccess = connect (eaterUI, SIGNAL(OneInput()),
            this, SLOT(hideEventEater()));
    Q_ASSERT (connectSuccess);

#ifdef HAVE_QMSYSTEM
    /*
     * We need to sense when the screen turned on/off to prevent unnecessary
     * screen updates and wakeups.
     */
    connectSuccess = connect (
        &m_QmDisplay,
        SIGNAL(displayStateChanged (Maemo::QmDisplayState::DisplayState)),
        this,
        SLOT(displayStateChanged (Maemo::QmDisplayState::DisplayState)));
    Q_ASSERT (connectSuccess);

    connectSuccess = connect (
        &m_QmLocks,
        SIGNAL(stateChanged (Maemo::QmLocks::Lock, Maemo::QmLocks::State)),
        this,
        SLOT(locksChanged (Maemo::QmLocks::Lock, Maemo::QmLocks::State)));
    Q_ASSERT (connectSuccess);
#endif

}

LockScreenBusinessLogic::~LockScreenBusinessLogic()
{
    delete lockScreenWindow;
}

/*!
 * This function is called when the user unlocks the screen manually.
 */
void
LockScreenBusinessLogic::unlockScreen ()
{
    SYS_DEBUG ("");
    toggleScreenLockUI (false);
}

#ifdef HAVE_QMSYSTEM
/*!
 * This slot is called when the display state is changed. We need this to
 * prevent the screen updates in the 'Off' state whan no UI is visible. Please
 * note that the unlockscreen might be showed while the screen is off and MCE
 * does this to be able to show up the unlockscreen immediately after the power
 * button press.
 */
void
LockScreenBusinessLogic::displayStateChanged (
        Maemo::QmDisplayState::DisplayState state)
{
    SYS_DEBUG ("");
    /*
     * When the screen is off the timer should be always turned off.
     */
    if (state == Maemo::QmDisplayState::Off && timer.isActive())
        mayStopTimer ();

    /*
     * When the screen is unlocked, the lockscreen is visible and the timer is
     * off we need to start it.
     */
    if (state == Maemo::QmDisplayState::On &&
            lockScreenWindow != NULL && lockScreenWindow->isVisible ())
    {
        if (timer.isActive () == false)
            mayStartTimer ();
        /*
         * Also we should reset the lock-screen-ui dnd icon state:
         */
        lockScreenWindow->reset();
        lockScreenWindow->setFocus ();
    }
}
#endif

void
LockScreenBusinessLogic::hideEventEater()
{
    SYS_DEBUG ("");
    toggleEventEater (false);
}

void
LockScreenBusinessLogic::toggleScreenLockUI (
        bool toggle)
{
    SYS_DEBUG ("*** toggle = %s", SYS_BOOL(toggle));

    if (toggle) {
        /*
         * Whenever we're showing the lock-screen-ui
         * we need to reset its state.
         */
        lockScreenWindow->reset();
        lockScreenWindow->show();
        mayStartTimer ();
    } else {
        lockScreenWindow->hide();
        mayStopTimer ();
    }

//    emit screenIsLocked (toggle);
}

void
LockScreenBusinessLogic::toggleEventEater (
        bool toggle)
{
    if (toggle) {
        eaterUI->show ();
        eaterUI->showFullScreen ();
    } else {
        eaterUI->hide ();
    }

    // Enable the unlock notification sink also for dimmed state:
//    emit screenIsLocked (toggle);
}

void
LockScreenBusinessLogic::mayStartTimer ()
{
    /*
     * We are not starting the timer when the screen is locked. We only use the
     * timer to refresh the screen.
     */
    SYS_DEBUG ("");
    if (!displayIsOn())
        return;

    SYS_DEBUG ("Starting timer");

    // TODO: some adjustments of time may be done
    timer.start (1000);
}

void
LockScreenBusinessLogic::mayStopTimer ()
{
    /*
     * We are not stopping the timer when it is not on.
     */
    if (!timer.isActive())
        return;

    SYS_DEBUG ("Stopping timer");
    timer.stop ();
}

bool
LockScreenBusinessLogic::displayIsOn ()
{
#if !defined(__i386__) && defined(HAVE_QMSYSTEM)
    bool retval = m_QmDisplay.get () == Maemo::QmDisplayState::On;
#else
    bool retval = true;
#endif

    SYS_DEBUG ("returning %s", SYS_BOOL(retval));
    return retval;
}

#if HAVE_QMSYSTEM
void
LockScreenBusinessLogic::locksChanged (
    Maemo::QmLocks::Lock what,
    Maemo::QmLocks::State how)
{
    if (what == Maemo::QmLocks::TouchAndKeyboard)
    {
        switch (how)
        {
            case Maemo::QmLocks::Locked:
                SYS_DEBUG ("Locked");
                emit screenIsLocked (true);
                break;
            default:
                SYS_DEBUG ("Unlocked");
                emit screenIsLocked (false);
                break;
        }
    }
}
#endif

