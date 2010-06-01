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
    SYS_DEBUG ("");

    connect (lockUI, SIGNAL (unlocked ()),
             this, SLOT (unlockScreen ()));
    connect (lockUI, SIGNAL (unlocked ()),
             this, SIGNAL (unlockConfirmed ()));
    connect (&timer, SIGNAL (timeout ()),
             lockUI, SLOT (updateDateTime ()));

    lockUI->setWindowFlags (Qt::FramelessWindowHint |
                            Qt::WindowStaysOnTopHint);
    eaterUI->setWindowFlags (Qt::FramelessWindowHint |
                             Qt::WindowStaysOnTopHint);
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

void
LockScreenBusinessLogic::toggleScreenLockUI (
        bool toggle)
{
    if (toggle) {
        lockUI->show();
        startTimer ();
    } else {
        lockUI->hide();
        stopTimer ();
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
LockScreenBusinessLogic::startTimer ()
{
    SYS_DEBUG ("Starting timer");
    // It's better to update the time straight away.
    lockUI->updateDateTime ();

    // TODO: some adjustments of time may be done
    timer.start (1000);
}

void
LockScreenBusinessLogic::stopTimer ()
{
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

