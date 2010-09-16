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
#include "eventeater.h"
#include "sysuid.h"
#include <QMouseEvent>
#include <QX11Info>


#undef DEBUG
#define WARNING
#include "debug.h"

// For WM_SET_NAME:
#include <X11/Xlib.h>
#include <X11/Xatom.h>


/******************************************************************************
 * The EventEater implementation.
 */
EventEater::EventEater ()
{
    SYS_DEBUG ("");
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_X11NetWmWindowTypeDialog);
    setAttribute(Qt::WA_X11DoNotAcceptFocus);
    setObjectName ("EventEater");
    setProperty ("NoMStyle", true);
}

/*!
 * We got the mouse events, so we can close the event eater window even if MCE
 * does not ask for the closing.
 */
void
EventEater::mousePressEvent (
        QMouseEvent *event)
{
    SYS_DEBUG ("");

    Q_UNUSED (event);
    emit OneInput ();
}

/*!
 * We got the mouse events, so we can close the event eater window even if MCE
 * does not ask for the closing.
 */
void
EventEater::mouseReleaseEvent (
        QMouseEvent *event)
{
    SYS_DEBUG ("");

    Q_UNUSED (event);
    emit OneInput ();
}

/*!
 * This method is called when we have a windowID, so we can set some properties
 * with the low level XLib API. Currently we set the following properties:
 *   o Window name: it is used for debugging purposes.
 *   o Stackinglayer: please check NB#175815 for further details.
 */
void
EventEater::showEvent (
        QShowEvent *event)
{
    Q_UNUSED (event);

    Window      windowID;
    Display    *display;
    Atom        nameAtom;
    Atom        utf8StringAtom;
    Atom        stackingLayerAtom;
    const char *windowName = "EventEater";
    long        layer = 6;

    display = QX11Info::display ();
    if (!display) {
        SYS_WARNING ("QX11Info::display() failed");
        return;
    }

    stackingLayerAtom = XInternAtom (display, "_MEEGO_STACKING_LAYER", False);
    if (stackingLayerAtom == None) {
        SYS_WARNING ("Atom '_MEEGO_STACKING_LAYER' does not exists");
    }

    nameAtom = XInternAtom (display, "_NET_WM_NAME", False);
    if (nameAtom == None) {
        SYS_WARNING ("Atom '_NET_WM_NAME' does not exists");
    }

    utf8StringAtom = XInternAtom (display, "UTF8_STRING", False);
    if (utf8StringAtom == None) {
        SYS_WARNING ("Atom 'UTF8_STRING' does not exists");
    }

    windowID = internalWinId();
    if (windowID == None) {
        SYS_WARNING ("internalWinId() failed");
    }
    SYS_DEBUG ("*** windowID = 0x%lx", windowID);

    /*
     * Setting the stacking layer.
     */
    if (stackingLayerAtom != None)
        XChangeProperty (display, windowID, stackingLayerAtom, XA_CARDINAL,
                32, PropModeReplace, (unsigned char*)&layer, 1);

    /*
     * Setting the name.
     */
    if (nameAtom != None && utf8StringAtom != None)
        XChangeProperty (display, windowID, nameAtom, utf8StringAtom,
                8, PropModeReplace,
                (unsigned char *) windowName, strlen(windowName));
}

