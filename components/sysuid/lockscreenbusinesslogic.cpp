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
    display (NULL),
    lockUI (NULL)
{
    SYS_DEBUG ("");

    locks = new QmLocks(this);
    display = new QmDisplayState(this);
    lockUI = new LockScreenUI();

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

#if 0
    /*
     * It seems that the screen is locked after a very short period of time no
     * matter what I do.
     */
    SYS_DEBUG ("##############################################");
    SYS_DEBUG ("*** locks->getDeviceAutolockTime () = %dsec", 
            locks->getDeviceAutolockTime ());

    locks->setDeviceAutolockTime (30);
#endif
    SYS_DEBUG ("################################################");
    SYS_DEBUG ("*** locks->getDeviceAutolockTime () = %dsec", 
            locks->getDeviceAutolockTime ());
    SYS_DEBUG ("################################################");
}

LockScreenBusinessLogic::~LockScreenBusinessLogic()
{
    delete lockUI;
}


void 
LockScreenBusinessLogic::locksChanged (
        Maemo::QmLocks::Lock lock, 
        Maemo::QmLocks::State state)
{
    if (lock == QmLocks::Device)
        return;

    knownLock = state;

    if (knownLock == QmLocks::Locked) {
        SYS_DEBUG ("locked");
        toggleScreenLockUI (true);
        mayStartTimer();
    } else {
        SYS_DEBUG ("not locked");
        toggleScreenLockUI (false);
        stopTimer();
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
            break;
    }
}

/*!
 * This function is called when the lock slider is moved by the user so the
 * screen should be unlocked.
 * FIXME: The locks->setState() function can not be used to unlock the screen
 * (found this in the documentation of the QmLocks::setState()).
 */
void 
LockScreenBusinessLogic::unlockScreen ()
{
    SYS_DEBUG ("");
    toggleScreenLockUI (false); //turn off the UI
#if 0
    bool retval;
    retval = locks->setState (QmLocks::TouchAndKeyboard, QmLocks::Unlocked);
    if (!retval) {
        SYS_WARNING ("Unlock has been failed.");
    } else {
        SYS_DEBUG ("The screen was unlocked.");
    }
#endif
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
    SYS_DEBUG ("");

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

