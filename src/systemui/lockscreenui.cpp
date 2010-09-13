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
#include "lockscreenui.h"
#include "lockscreenwindow.h"
#include "unlockwidgets.h"
#include "unlocknotifications.h"
#include "sysuid.h"
#include <QX11Info>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <MWindow>
#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(LockScreenUI)

#undef DEBUG
#define WARNING
#include "debug.h"

// For WM_SET_NAME:
#include <X11/Xlib.h>
#include <X11/Xatom.h>

/******************************************************************************
 * LockScreenUI implementation.
 */
LockScreenUI::LockScreenUI () :
        m_Realized (false),
        m_SceneWindow (0),
        m_notificationArea (0),
        m_LockLiftArea (0),
        m_LockLandArea (0)
{
    SYS_DEBUG ("");

    setBackgroundBrush (QBrush(Qt::black));
    QTimer::singleShot (0, this, SLOT(realize()));
    setObjectName ("LockScreenUI");
}

LockScreenUI::~LockScreenUI ()
{
    // Free the resources here...
    SYS_DEBUG ("");
}

void
LockScreenUI::createContent ()
{
    if (! m_Realized)
        realize ();
}

void
LockScreenUI::realize ()
{
    SYS_DEBUG ("");

    if (m_Realized)
        return;

    setContentsMargins (0., 0., 0., 0.);

    /*
     * The main layout and its policy
     */
    m_policy = new QGraphicsLinearLayout (Qt::Vertical);
    m_policy->setContentsMargins (0., 0., 0., 0.);
    m_policy->setSpacing (0.);

    /*
     * The topmost part of the lock-screen ui
     * (this one is optional, based on missed events)
     */
    m_notificationArea = new UnlockNotifications;
    m_notificationArea->setVisible (false);

    connect (m_notificationArea, SIGNAL (needToShow (bool)),
             this, SLOT (showHideNotifications (bool)),
             Qt::DirectConnection);

    /*
     * The upper part of the lock-screen ui, this shows the
     * big lock icon at right side, and the date/time at left side
     */
    m_LockLiftArea = new UnlockHeader;
    m_policy->addItem (m_LockLiftArea);
    /*
     * And the big drag and drop area
     */
    m_LockLandArea = new UnlockArea;
    m_policy->addItem (m_LockLandArea);

    // Set the main layout
    m_SceneWindow =
        new LockScreenWindow (this, m_LockLiftArea, m_LockLandArea);
    m_SceneWindow->setLayout (m_policy);
    m_SceneWindow->appear (this);

    connect (m_SceneWindow, SIGNAL (unlocked ()),
             this, SLOT (sliderUnlocked ()),
             Qt::DirectConnection);

    m_Realized = true;
}

void
LockScreenUI::sliderUnlocked ()
{
    SYS_DEBUG ("");

    emit unlocked ();
}

void
LockScreenUI::updateDateTime ()
{
    //if (isContentCreated () == false)
    if (!m_Realized)
        return;

    static_cast<UnlockHeader *> (m_LockLiftArea)->updateDateTime ();
}

// Reset the unlock-screen state to default
// [to avoid half dragged lock icon visible on the screen]
void
LockScreenUI::reset ()
{
    if (m_SceneWindow != 0)
        m_SceneWindow->resetState ();
}

void
LockScreenUI::showHideNotifications (bool show)
{
    if (m_notificationArea == 0)
        return;

    SYS_DEBUG ("show = %s", SYS_BOOL (show));

    // Hide the whole missed events notification area when
    // there is no any missed events...
    if (m_notificationArea->isVisible () && (show == false))
    {
        m_notificationArea->setVisible (false);
        m_policy->removeItem (m_notificationArea);
    }

    // Add notification area to policy when previously was
    // hidden, but there are some missed events...
    if ((m_notificationArea->isVisible () == false) && (show == true))
    {
        m_notificationArea->setVisible (true);
        m_policy->insertItem (0, m_notificationArea);
    }
}

/*!
 * This method is called when we have a windowID, so we can set some properties
 * with the low level XLib API. Currently we set the following properties:
 *   o Window name: it is used for debugging purposes.
 *   o Stackinglayer: please check NB#175815 for further details.
 */
void
LockScreenUI::showEvent (
        QShowEvent *event)
{
    Q_UNUSED (event);

    Window      windowID;
    Display    *display;
    Atom        nameAtom;
    Atom        utf8StringAtom;
    Atom        stackingLayerAtom;
    const char *windowName = "LockScreenUI";
    long        layer = 2;

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
        return;
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
