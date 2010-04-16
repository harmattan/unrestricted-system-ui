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
#include <MApplicationWindow>
#include <MSceneManager>
#include <QDBusInterface>
#include <QDebug>
#include <QTime>
#include <QX11Info>

#include "lockscreenui.h"
#include "lockscreenbusinesslogic.h"
#include "sysuid.h"

#include <X11/Xlib.h>
// TODO: this include can be removed when mcompositor
// sets the _NET_WM_STATE attribute according to the message.
#include <X11/Xatom.h>

#define DEBUG
#include "debug.h"

LockScreenBusinessLogic::LockScreenBusinessLogic (
        QObject* parent) :
    QObject (parent),
    display ( new QmDisplayState (this)),
    locks (new QmLocks (this)),
    lockUI (new LockScreenUI)
{
    SYS_DEBUG ("");

    connect(locks, SIGNAL(stateChanged(Maemo::QmLocks::Lock, Maemo::QmLocks::State)),
            this, SLOT(locksChanged(Maemo::QmLocks::Lock, Maemo::QmLocks::State)));
    connect(display, SIGNAL(displayStateChanged(Maemo::QmDisplayState::DisplayState)),
            this, SLOT(displayStateChanged(Maemo::QmDisplayState::DisplayState)));
    connect (lockUI, SIGNAL (unlocked ()),
             this, SLOT (unlockScreen ()));
    connect (lockUI, SIGNAL (unlocked ()),
             this, SIGNAL (unlockConfirmed ()));

    connect (&timer, SIGNAL (timeout ()),
             lockUI, SLOT (updateDateTime ()));

    /*
     * Just to be sure: maybe the screen is already locked when this daemon
     * started...
     */

    displayStateChanged (display->get ());

    locksChanged (
            QmLocks::TouchAndKeyboard,
            locks->getState (QmLocks::TouchAndKeyboard));

    /*
     * XXX: Remove these lines... it is for myself for simplier developing the
     *      new touch-screen-unlock ui
     */
#if defined (DEBUG) && defined (i386)
    // Show the unlock-screen-ui
    locksChanged (QmLocks::TouchAndKeyboard, QmLocks::Locked);
    displayStateChanged (QmDisplayState::On);
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
    if (lock != QmLocks::TouchAndKeyboard)
        return;

    knownLock = state;

    SYS_DEBUG ("locked: %s", SYS_BOOL (knownLock == QmLocks::Locked));

    toggleScreenLockUI (knownLock == QmLocks::Locked);
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
                Sysuid::sysuid ()->applicationWindow ()
                    .sceneManager ()->appearSceneWindowNow (lockUI);
                lockUI->setActive (true);
            }
            else
            {
                // Hide the event eater if screen isn't locked
                if (Sysuid::sysuid()->applicationWindow().isVisible ())
                    Sysuid::sysuid()->applicationWindow().hide ();
            }
            mayStartTimer ();
            break;

        default:
            SYS_DEBUG ("Dimmed");

            // Create lockUI content on first dimming...
            if (! lockUI->isContentCreated ())
                lockUI->createContent ();

            // Show the event-eater window...
            Sysuid::sysuid()->applicationWindow().show ();
            Sysuid::sysuid()->applicationWindow().raise ();
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
LockScreenBusinessLogic::toggleScreenLockUI (
        bool toggle)
{
    SYS_DEBUG ("*** toggle = %s", toggle ? "true" : "false");

    if (toggle) {
        mayStartTimer ();

        Sysuid::sysuid ()->applicationWindow ().show ();
        Sysuid::sysuid ()->applicationWindow ().raise ();
    } else {
        stopTimer ();

        hidefromTaskBar ();
        Sysuid::sysuid ()->applicationWindow ().hide ();
    }
}

void
LockScreenBusinessLogic::mayStartTimer ()
{
    SYS_DEBUG ("");
    if (knownLock == QmLocks::Locked &&
        knownDisplay != QmDisplayState::Off)
    {
        // It's better to update the time straight away.
        lockUI->updateDateTime ();

        timer.start (1000);
    }
}

void
LockScreenBusinessLogic::stopTimer ()
{
    timer.stop ();
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
    e.xclient.window = Sysuid::sysuid()->applicationWindow().internalWinId();
    e.xclient.format = 32;
    e.xclient.data.l[0] = 1;
    e.xclient.data.l[1] = skipTaskbarAtom;
    e.xclient.data.l[2] = 0;
    e.xclient.data.l[3] = 0;
    e.xclient.data.l[4] = 0;
    XSendEvent (display,
                RootWindow (display, Sysuid::sysuid()->applicationWindow().x11Info ().screen ()),
                FALSE,
                (SubstructureNotifyMask | SubstructureRedirectMask),
                &e);

    // TODO: setting this property by hand can be removed when mcompositor
    // sets the _NET_WM_STATE attribute according to the message.
    QVector<Atom> atoms;
    atoms.append(skipTaskbarAtom);
    XChangeProperty (QX11Info::display (),
                     Sysuid::sysuid()->applicationWindow().internalWinId(),
                     netWmStateAtom,
                     XA_ATOM,
                     32,
                     PropModeReplace,
                     (unsigned char *) atoms.data (),
                     atoms.count ());

    XFlush (display);
}

