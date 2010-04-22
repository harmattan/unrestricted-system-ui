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
    lockUI (new LockScreenUI)
{
    SYS_DEBUG ("");

    connect (lockUI, SIGNAL(unlocked()),
            this, SLOT(unlockScreen()));
    connect (lockUI, SIGNAL(unlocked()),
            this, SIGNAL(unlockConfirmed()));

    connect (&timer, SIGNAL (timeout ()),
             lockUI, SLOT (updateDateTime ()));

#if defined (DEBUG) && defined (i386)
    // XXX: Remove this... only for debugging/devoloping purposes

     lockUI->updateMissedEventAmounts (3, 10, 0, 4);

     toggleScreenLockUI (true);
#endif
}

LockScreenBusinessLogic::~LockScreenBusinessLogic()
{
    delete lockUI;
}

/*!
 * This function is called when the lock slider is moved by the user so the
 * screen should be unlocked.
 */
void
LockScreenBusinessLogic::unlockScreen ()
{
    SYS_DEBUG ("");
    toggleScreenLockUI (false);
}

void
LockScreenBusinessLogic::toggleScreenLockUI (
        bool toggle)
{
    SYS_DEBUG ("*** toggle = %s", toggle ? "true" : "false");

    if (toggle) {
        lockUI->setOpacity (1.0);
        Sysuid::sysuid ()->applicationWindow ().sceneManager ()->
            appearSceneWindowNow (lockUI);
        lockUI->setActive (true);

        Sysuid::sysuid()->applicationWindow().show ();
        Sysuid::sysuid()->applicationWindow().showFullScreen();
        Sysuid::sysuid()->applicationWindow().raise();

        mayStartTimer ();
    } else {
        hidefromTaskBar ();
        Sysuid::sysuid()->applicationWindow().hide();
        stopTimer ();
    }
}

void
LockScreenBusinessLogic::toggleEventEater (
        bool toggle)
{
    SYS_DEBUG ("*** toggle = %s", toggle ? "true" : "false");

    if (toggle) {
        // Create lockUI content on first dimming...
        if (! lockUI->isContentCreated ())
            lockUI->createContent ();

        // Show the event-eater window...
        Sysuid::sysuid()->applicationWindow().show ();
        Sysuid::sysuid()->applicationWindow().showFullScreen ();
        Sysuid::sysuid()->applicationWindow().raise ();
    } else {
        // Hide the event eater
        Sysuid::sysuid()->applicationWindow().hide ();
    }
}

void
LockScreenBusinessLogic::mayStartTimer ()
{
    SYS_DEBUG ("Starting timer");
    /*if (knownLock == QmLocks::Locked && knownDisplay != QmDisplayState::Off)*/
    {
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

