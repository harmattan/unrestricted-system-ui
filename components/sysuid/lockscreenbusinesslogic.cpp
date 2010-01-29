/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include <DuiApplicationWindow>
#include <DuiApplication>
#include <QDBusInterface>
#include <QDebug>
#include <QTime>

#include "lockscreenui.h"
#include "lockscreenbusinesslogic.h"

#define DEBUG
#include "../debug.h"

LockScreenBusinessLogic::LockScreenBusinessLogic (
        QObject* parent) :
    QObject (parent), 
    display ( new QmDisplayState(this)),
    locks (new QmLocks(this)),
    lockUI (new LockScreenUI)
{
    SYS_DEBUG ("");

    connect(locks, SIGNAL(stateChanged(Maemo::QmLocks::Lock, Maemo::QmLocks::State)),
            this, SLOT(locksChanged(Maemo::QmLocks::Lock, Maemo::QmLocks::State)));
    connect(display, SIGNAL(displayStateChanged(Maemo::QmDisplayState::DisplayState)),
            this, SLOT(displayStateChanged(Maemo::QmDisplayState::DisplayState)));
    connect (lockUI, SIGNAL(unlocked()), 
            this, SLOT(unlockScreen()));
    connect (lockUI, SIGNAL(unlocked()), 
            this, SIGNAL(unlockConfirmed()));

    connect (&timer, SIGNAL(timeout()), 
            lockUI, SLOT(updateDateTime()));

    /*
     * Just to be sure: maybe the screen is already locked when this daemon
     * started...
     * FIXME: getState() segfaults under scratchbox. 
     */
#ifndef __i386__
    locksChanged (
            QmLocks::TouchAndKeyboard,
            locks->getState (QmLocks::TouchAndKeyboard));
#endif
}

LockScreenBusinessLogic::~LockScreenBusinessLogic()
{
    delete lockUI;
}


void 
LockScreenBusinessLogic::locksChanged (
        Maemo::QmLocks::Lock  lock, 
        Maemo::QmLocks::State state)
{
    if (lock == QmLocks::Device)
        return;

    knownLock = state;

    if (knownLock == QmLocks::Locked) {
        SYS_DEBUG ("Locked");
        toggleScreenLockUI (true);
        mayStartTimer ();
    } else {
        SYS_DEBUG ("Unlocked");
        toggleScreenLockUI (false);
        stopTimer ();
    }
}


void 
LockScreenBusinessLogic::displayStateChanged (
        Maemo::QmDisplayState::DisplayState state)
{
    knownDisplay = state;

    switch (state) {
        case Maemo::QmDisplayState::Off:
            SYS_DEBUG ("Screen off");
            stopTimer ();
            break;

        case Maemo::QmDisplayState::On:
            SYS_DEBUG ("Screen on");
            mayStartTimer ();
            break;

        default:
            /*
             * Might be a dimmed state.
             */
            break;
    }
}

/*!
 * This function is called when the lock slider is moved by the user so the
 * screen should be unlocked.
 */
void 
LockScreenBusinessLogic::unlockScreen ()
{
    SYS_DEBUG ("");
    toggleScreenLockUI (false); //turn off the UI
}

void 
LockScreenBusinessLogic::updateMissedEventAmounts (
        int a, 
        int b, 
        int c, 
        int d)
{
    SYS_DEBUG ("");
    qDebug() << "LockScreenBusinessLogic::updateMissedEventAmounts("
    << a << ", " << b << ", " << c << ", " << d << ")";
    lockUI->updateMissedEventAmounts(a, b, c, d);
}

void 
LockScreenBusinessLogic::toggleScreenLockUI (
        bool toggle)
{
    SYS_DEBUG ("*** toggle = %s", toggle ? "true" : "false");

    if (toggle) {
        DuiApplication::activeApplicationWindow()->show();
        lockUI->appear();
    } else {
        DuiApplication::activeApplicationWindow()->hide();
    }
}

void 
LockScreenBusinessLogic::mayStartTimer ()
{
    SYS_DEBUG ("");
    if (knownLock == QmLocks::Locked && knownDisplay != QmDisplayState::Off) {
        // It's better to update the time straight away.
        lockUI->updateDateTime();

        QTime t(QTime::currentTime());
        // TODO: some adjustments of time may be done
        timer.start (1000);
    }
}

void 
LockScreenBusinessLogic::stopTimer ()
{
    timer.stop();
}

