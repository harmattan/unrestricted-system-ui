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
#include <MApplication>
#include <MApplicationWindow>
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
    SYS_DEBUG ("");

    connect (lockUI, SIGNAL (unlocked ()),
             this, SLOT (unlockScreen ()));
    connect (lockUI, SIGNAL (unlocked ()),
             this, SIGNAL (unlockConfirmed ()));

    connect (&timer, SIGNAL (timeout ()),
             lockUI, SLOT (updateDateTime ()));

#if 0
    // Hide from taskbar at the beginning
    hidefromTaskBar ();
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
    if (toggle)
        lockUI->show();
    else
        lockUI->hide();
#if 0
    MApplicationWindow& mainwindow =
        Sysuid::sysuid ()->applicationWindow ();
    SYS_DEBUG ("*** toggle     = %s", SYS_BOOL(toggle));

    MApplicationWindow *window;
    SYS_DEBUG ("*** mainwindow = %p", &mainwindow);
    window = MApplication::instance()->activeApplicationWindow();
    SYS_DEBUG ("*** active win = %p", window);
    
    if (eaterUI->isVisible ())
        eaterUI->hide ();

    if (toggle) {
        if (mainwindow.isHidden ()) {
            SYS_DEBUG ("Showing main window");
            mainwindow.show ();
        } else {
            SYS_DEBUG ("The main window already visible");
        }

        lockUI->setOpacity (1.0);
        mainwindow.sceneManager ()->appearSceneWindowNow (lockUI);
        lockUI->show ();

        mainwindow.showFullScreen ();
        mainwindow.raise ();

        mayStartTimer ();
    } else {
        if (mainwindow.isVisible ())
            mainwindow.hide ();

        stopTimer ();
    }
#endif
    emit screenIsLocked (toggle);
}

void
LockScreenBusinessLogic::toggleEventEater (
        bool toggle)
{
    if (toggle)
        eaterUI->show ();
    else
        eaterUI->hide ();
#if 0
    MApplicationWindow& mainwindow =
        Sysuid::sysuid ()->applicationWindow ();

    SYS_DEBUG ("*** toggle = %s", SYS_BOOL(toggle));
    
    MApplicationWindow *window;
    SYS_DEBUG ("*** mainwindow = %p", &mainwindow);
    window = MApplication::instance()->activeApplicationWindow();
    SYS_DEBUG ("*** active win = %p", window);

    // Hide the unlock ui if visible
    if (lockUI->isVisible ())
        lockUI->hide ();

    if (toggle) {
        if (mainwindow.isHidden ())
            mainwindow.show ();

        eaterUI->setOpacity (0.0);
        mainwindow.sceneManager ()->appearSceneWindowNow (eaterUI);
        eaterUI->show ();

        mainwindow.showFullScreen ();
        mainwindow.raise ();
    } else {
        if (mainwindow.isVisible ())
            mainwindow.hide ();

        stopTimer ();
    }
#endif
    // Enable the unlock notification sink also for dimmed state:
    emit screenIsLocked (toggle);
}

void
LockScreenBusinessLogic::mayStartTimer ()
{
    SYS_DEBUG ("Starting timer");
    /*if (knownLock == QmLocks::Locked && knownDisplay != QmDisplayState::Off)*/
    {
        // It's better to update the time straight away.
        lockUI->updateDateTime ();

        QTime t (QTime::currentTime ());
        // TODO: some adjustments of time may be done
        timer.start (1000);
    }
}

void
LockScreenBusinessLogic::stopTimer ()
{
    timer.stop ();
}

#if 0
/*
 * According to MeegoTouch guy there is no need for this,
 * because eg.: we aren't really using a real window for
 * event-eater window...
 */
void
LockScreenBusinessLogic::hidefromTaskBar ()
{
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
#endif

void
LockScreenBusinessLogic::updateMissedEvents (
    int emails,
    int messages,
    int calls,
    int im)
{
    static_cast<LockScreenUI*> (lockUI)->updateMissedEvents (
                                        emails, messages, calls, im);
}

