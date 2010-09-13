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
#include "lockscreenwindow.h"
#include "lockscreenui.h"
#include "unlockwidgets.h"
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QPointF>
#include <MGConfItem>
#include <MWidget>
#include <MFeedback>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(LockScreenUI)

#undef DEBUG
#define WARNING
#include "debug.h"

// For WM_SET_NAME:
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#define GCONF_BG_LANDSCAPE \
    "/desktop/meego/background/landscape/picture_filename"
#define GCONF_BG_PORTRAIT \
    "/desktop/meego/background/portrait/picture_filename"


/******************************************************************************
 * LockScreenWindow implementation.
 */
LockScreenWindow::LockScreenWindow (MWindow *window, MWidget *locklift, MWidget *lockland) :
    MSceneWindow (0),
    m_Window (window),
    m_DnDicon (0),
    m_DnDstate (STATE_NONE),
    m_LockLiftArea (locklift),
    m_LockLandArea (lockland),
    m_pendingDraw (false)
{
    /*
     * Creating the GConf keys to sense the wallpaper changes.
     */
    SYS_DEBUG ("*** landscape_key = %s", GCONF_BG_LANDSCAPE);
    SYS_DEBUG ("*** portrait_key = %s", GCONF_BG_PORTRAIT);
    m_confBgLandscape = new MGConfItem (GCONF_BG_LANDSCAPE, this);
    m_confBgPortrait  = new MGConfItem (GCONF_BG_PORTRAIT, this);

    connect (m_confBgLandscape, SIGNAL(valueChanged()),
            this, SLOT(reloadLandscapeBackground()));
    connect (m_confBgPortrait, SIGNAL(valueChanged()),
            this, SLOT(reloadPortraitBackground()));

    m_DnDoverlay.setVisible (false);
    m_DnDoverlay.setManagedManually (true);

    // Load the backgrounds if any...
    reloadLandscapeBackground ();
    reloadPortraitBackground ();

    setObjectName ("LockScreenWindow");
}

LockScreenWindow::~LockScreenWindow ()
{
    delete m_confBgLandscape;
    delete m_confBgPortrait;
}

void
LockScreenWindow::reloadLandscapeBackground ()
{
    QString filename = m_confBgLandscape->value().toString();
    bool    success;

    if (filename.isEmpty())
        return;

    SYS_DEBUG ("landscape value = %s", SYS_STR(filename));
    success = m_bgLandscape.load (filename);
    if (!success) {
        SYS_WARNING ("Loading image failed: %s", SYS_STR(filename));
    }
}

void
LockScreenWindow::reloadPortraitBackground ()
{
    QString filename = m_confBgPortrait->value().toString();
    bool    success;

    if (filename.isEmpty())
        return;

    SYS_DEBUG ("portrait  value = %s", SYS_STR(filename));
    success = m_bgPortrait.load (filename);
    if (!success) {
        SYS_WARNING ("Loading image failed: %s", SYS_STR(filename));
    }
}

/*!
 * Paints the background image for the lockscreen. I heard rumors, that there is
 * a simpler way, the window can paint its background, but I'm not sure how,
 * and I'm not sure if it is better that way.
 */
void
LockScreenWindow::paint (
        QPainter                       *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget                        *widget)
{
    Q_UNUSED (option);
    Q_UNUSED (widget);
    bool portrait = geometry().height() > geometry().width ();

    if (portrait) {
        painter->drawPixmap (
                0, 0,
                m_bgPortrait.width(),
                m_bgPortrait.height(),
                m_bgPortrait);
    } else {
        painter->drawPixmap (
                0, 0,
                m_bgLandscape.width(),
                m_bgLandscape.height(),
                m_bgLandscape);
    }
}

void
LockScreenWindow::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
    bool startDnD = false;

    if (m_LockLiftArea == 0)
        return;

    if ((m_LockLiftArea->pos ().y () + m_LockLiftArea->preferredHeight ()) >
         event->pos ().y ())
    {
        if (m_LockLiftArea->layoutDirection () == Qt::RightToLeft)
        {
            // Icon position in RTL layout direction
            startDnD = event->pos ().x () < 160;
        }
        else
        {
            // Icon position in LTR layout direction
            startDnD = event->pos ().x () > m_LockLiftArea->preferredWidth () - 160;
        }
    }

    // We should go to STATE_MOVING state if user tappend on
    // the top-right corner of the window...
    if (startDnD)
    {
        m_DnDstate = STATE_MOVING;

        // Move the icon to the start position:
        QSizeF size = m_DnDoverlay.preferredSize ();

        m_DnDposition.setX (event->pos ().rx () - (size.width () / 1.4));
        m_DnDposition.setY (event->pos ().ry () - (size.height () / 1.4));

        redraw ();

        static_cast<UnlockHeader*>(m_LockLiftArea)->setActive (false);
        static_cast<UnlockArea*> (m_LockLandArea)->setEnabled (true);

        /*
         * Playing the appropriate feedback.
         */
        MFeedback feedback ("start-dragndrop");
        feedback.play ();
    }
    else
        m_DnDstate = STATE_NONE;
}

void
LockScreenWindow::redraw ()
{
    if (m_pendingDraw == true)
        return;

    m_pendingDraw = true;
    QTimer::singleShot (50, this, SLOT (redrawIdle ()));
}

void
LockScreenWindow::redrawIdle ()
{
    m_pendingDraw = false;

    m_DnDoverlay.setPos (m_DnDposition);
    updateDnDicon ();
}

void
LockScreenWindow::mouseMoveEvent (QGraphicsSceneMouseEvent *event)
{
    if (m_LockLandArea == 0)
        return;

    if (m_DnDstate == STATE_NONE)
        return;

    // First of all, icon should follow the mouse positions:
    QSizeF size = m_DnDoverlay.preferredSize ();
    QPointF pos (event->pos ());

    pos.rx () -= (size.width  () / 1.4);
    pos.ry () -= (size.height () / 1.4);
    m_DnDoverlay.setPos (pos);

    // And then check which icon we need to show:
    int newState = STATE_MOVING;

    // Check whether the DnD icon is inside the lock-land area...
    if (event->pos ().y () > m_LockLandArea->pos ().y ())
        newState = STATE_MOVING_ACTIVE;

    // To avoid unnecessary screen updates...
    if (newState != m_DnDstate)
    {
        MFeedback feedback;
        m_DnDstate = newState;
        switch (newState)
        {
            case STATE_MOVING_ACTIVE:
                /*
                 * Entered the active area.
                 */
                feedback.setName ("enter-dragndrop-dropzone");
                feedback.play ();
                static_cast<UnlockArea*>(m_LockLandArea)->setActive (true);
                break;
            case STATE_MOVING:
            default:
                /*
                 * Exited the active area.
                 */
                feedback.setName ("exit-dragndrop-dropzone");
                feedback.play ();
                static_cast<UnlockArea*>(m_LockLandArea)->setActive (false);
                break;
        }
        // And update the DnD icon based on the current state :
        updateDnDicon ();
    }
}

void
LockScreenWindow::mouseReleaseEvent (QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED (event);
    bool unlock = false;
    MFeedback feedback;

    if (m_DnDstate == STATE_NONE) {
        return;
    }

    if (m_DnDstate == STATE_MOVING_ACTIVE)
        unlock = true;

    /*
     * Playing the appropriate feedback.
     */
    if (unlock) {
        feedback.setName ("release-inside-dragndrop-dropzone");
    } else {
        feedback.setName ("release-outside-dragndrop-dropzone");
    }
    feedback.play ();

    // Reset the state to defaults...
    resetState ();

    if (unlock == true)
        emit unlocked ();
}

void
LockScreenWindow::resetState ()
{
    // Restore the default state ...
    if (m_LockLiftArea != 0)
        static_cast<UnlockHeader*>(m_LockLiftArea)->setActive (true);
    if (m_LockLandArea != 0)
        static_cast<UnlockArea*> (m_LockLandArea)->setEnabled (false);

    m_DnDstate = STATE_NONE;
    updateDnDicon ();
}

void
LockScreenWindow::updateDnDicon ()
{
    bool enable = (m_DnDstate != STATE_NONE);

    // Icon not yet created... make it:
    if (m_DnDicon == 0)
    {
        m_DnDicon = new MImageWidget;

        m_DnDicon->setImage ("icon-m-common-locked", QSize (64, 64));
        m_DnDicon->setObjectName ("LockScreenDnDIcon");
        m_DnDicon->setZoomFactor (1.0);

        m_DnDoverlay.setWidget (m_DnDicon);
    }

    // Update the theming...
    if ((m_DnDstate == STATE_MOVING_ACTIVE) &&
        (m_DnDicon->objectName () != "LockScreenDnDIconActive"))
    {
            m_DnDicon->setObjectName ("LockScreenDnDIconActive");
            m_DnDicon->update ();
    }
    else if (m_DnDicon->objectName () != "LockScreenDnDIcon")
    {
        m_DnDicon->setObjectName ("LockScreenDnDIcon");
        m_DnDicon->update ();
    }

    m_DnDoverlay.setVisible (enable);
    if (enable)
    {
        if (m_Window != 0)
            m_DnDoverlay.appear (m_Window);
        else
            m_DnDoverlay.appear ();
    }
    else
        m_DnDoverlay.disappear ();
}
