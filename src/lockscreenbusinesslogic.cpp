/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include <DuiApplicationWindow>
#include <DuiSceneManager>
#include <QDBusInterface>
#include <QDebug>
#include <QTime>

#include "lockscreenui.h"
#include "lockscreenbusinesslogic.h"
#include "sysuid.h"

#include <X11/Xlib.h>
// TODO: this include can be removed when duicompositor
// sets the _NET_WM_STATE attribute according to the message.
#include <X11/Xatom.h>

#undef DEBUG
#include "debug.h"

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
    displayStateChanged (display->get ());

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
            // Only appear lockUI when display isn't off,
            // because it can trigger SGX hardware recovery errors
            if (knownLock == QmLocks::Locked)
            {
                SYS_DEBUG ("Show the lock UI");
                lockUI->setOpacity (1.0);
                Sysuid::sysuid()->applicationWindow().sceneManager()->showWindowNow(lockUI);
                lockUI->setActive (true);
            }
            else
            {
                // Hide the event eater if screen isn't locked
                if (DuiApplication::activeApplicationWindow ()->isVisible ())
                    DuiApplication::activeApplicationWindow ()->hide ();
            }
            mayStartTimer ();
            break;

        default:
            SYS_DEBUG ("Dimmed");

            // Create lockUI content on first dimming...
            if (! lockUI->isContentCreated ())
                lockUI->createContent ();

            // Show the event-eater window...
            DuiApplication::activeApplicationWindow ()->show ();
            DuiApplication::activeApplicationWindow ()->raise ();
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
    SYS_DEBUG ("%d, %d, %d, %d", a, b, c ,d);

    lockUI->updateMissedEventAmounts (a, b, c, d);
}

void 
LockScreenBusinessLogic::toggleScreenLockUI (
        bool toggle)
{
    SYS_DEBUG ("*** toggle = %s", toggle ? "true" : "false");

    if (toggle) {
        Sysuid::sysuid()->applicationWindow().show();
        Sysuid::sysuid()->applicationWindow().raise();
    } else {
        hidefromTaskBar ();
        Sysuid::sysuid()->applicationWindow().hide();
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

void
LockScreenBusinessLogic::hidefromTaskBar ()
{
    SYS_DEBUG ("");
    XEvent e;
    e.xclient.type = ClientMessage;
    Display *display = QX11Info::display ();
    Atom netWmStateAtom = XInternAtom (display,
                                       "_NET_WM_STATE",
                                       False);
    Atom skipTaskbarAtom = XInternAtom (QX11Info::display (),
                                        "_NET_WM_STATE_SKIP_TASKBAR",
                                       False);

    e.xclient.message_type = netWmStateAtom;
    e.xclient.display = display;
    e.xclient.window = DuiApplication::activeApplicationWindow ()->internalWinId();
    e.xclient.format = 32;
    e.xclient.data.l[0] = 1;
    e.xclient.data.l[1] = skipTaskbarAtom; 
    e.xclient.data.l[2] = 0;
    e.xclient.data.l[3] = 0;
    e.xclient.data.l[4] = 0;
    XSendEvent (display,
                RootWindow (display, DuiApplication::activeApplicationWindow ()->x11Info ().screen ()),
                FALSE,
                (SubstructureNotifyMask | SubstructureRedirectMask),
                &e);

    // TODO: setting this property by hand can be removed when duicompositor
    // sets the _NET_WM_STATE attribute according to the message.
    QVector<Atom> atoms;
    atoms.append(skipTaskbarAtom);
    XChangeProperty (QX11Info::display (),
                     DuiApplication::activeApplicationWindow ()->internalWinId(),
                     netWmStateAtom,
                     XA_ATOM,
                     32,
                     PropModeReplace,
                     (unsigned char *) atoms.data (),
                     atoms.count ());

    XFlush (display);
}

