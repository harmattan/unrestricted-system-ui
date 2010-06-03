/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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

#include <QDBusInterface>
#include <QTime>
#include <QX11Info>

#include "lockscreenui.h"
#include "lockscreenbusinesslogic.h"

#define DEBUG
#include "debug.h"

#include <X11/Xlib.h>
#include <X11/Xatom.h>

LockScreenBusinessLogic::LockScreenBusinessLogic (
        QObject* parent) :
    QObject (parent),
    lockUI (new LockScreenUI),
    eaterUI (new EventEaterUI)
{
    bool connectSuccess;
    SYS_DEBUG ("");

    /*
     * Connecting the lockUI signals.
     */
    connectSuccess = connect (lockUI, SIGNAL (unlocked ()),
             this, SLOT (unlockScreen ()));
    Q_ASSERT (connectSuccess);

    connectSuccess = connect (lockUI, SIGNAL (unlocked ()),
             this, SIGNAL (unlockConfirmed ()));
    Q_ASSERT (connectSuccess);

    /*
     * Hiding the event eater when it is clicked.
     */
    connectSuccess = connect (eaterUI, SIGNAL(OneInput()),
            this, SLOT(oneInputCame()));
    Q_ASSERT (connectSuccess);

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
    
    /*
     * Connecting to timer that refreshes the 
     */
    connectSuccess = connect (&timer, SIGNAL (timeout ()),
             lockUI, SLOT (updateDateTime ()));
    Q_ASSERT (connectSuccess);
}

LockScreenBusinessLogic::~LockScreenBusinessLogic()
{
    delete lockUI;
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
            eaterUI != NULL && eaterUI->isVisible() &&
            !timer.isActive())
        mayStartTimer ();
}

void 
LockScreenBusinessLogic::oneInputCame()
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
        lockUI->show();
        mayStartTimer ();
    } else {
        lockUI->hide();
        mayStopTimer ();
    }
    
    emit screenIsLocked (toggle);
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
    emit screenIsLocked (toggle);
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
    // It's better to update the time straight away.
    lockUI->updateDateTime ();

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

void
LockScreenBusinessLogic::updateMissedEvents (
    int emails,
    int messages,
    int calls,
    int im)
{
    SYS_DEBUG ("*** emails   = %d", emails);
    SYS_DEBUG ("*** messages = %d", messages);
    SYS_DEBUG ("*** calls    = %d", calls);
    SYS_DEBUG ("*** im       = %d", im);

    lockUI->updateMissedEvents (emails, messages, calls, im);
}

bool 
LockScreenBusinessLogic::displayIsOn ()
{
#ifndef __i386__
    bool retval = m_QmDisplay.get () == Maemo::QmDisplayState::On;
#else
    bool retval = true;
#endif

    SYS_DEBUG ("returning %s", SYS_BOOL(retval));
    return retval;
}
