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
#include "unlockwidgets.h"
#include "unlocknotifications.h"
#include "unlocknotificationsink.h"
#include "sysuid.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include <QVariant>
#include <QTimer>
#include <QPointF>

#include <MApplication>
#include <MWindow>
#include <MGConfItem>
#include <MWidget>
#include <MLayout>
#include <MSceneManager>
#include <MLinearLayoutPolicy>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(LockScreenUI)

#define DEBUG
#define WARNING
#include "debug.h"

// For WM_SET_NAME:
#include <X11/Xlib.h>
#include <X11/Xatom.h>

static const QString defaultLandscapeImageFile = 
"/usr/share/themes/base/meegotouch/duihome/images/HomeWallpaperLandscape.png";
static const QString defaultPortraitImageFile = 
"/usr/share/themes/base/meegotouch/duihome/images/HomeWallpaperPortrait.png";

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
    m_LockLandArea (lockland)
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

    /*
     * This is hard-coded into duihome, so we have to hardcode too.
     */
    if (filename.isEmpty())
        filename = defaultLandscapeImageFile;

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

    /*
     * This is hard-coded into duihome, so we have to hardcode too.
     */
    if (filename.isEmpty())
        filename = defaultPortraitImageFile; 

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
    if (m_LockLiftArea == 0)
        return;

    // We should go to STATE_MOVING state if user tappend on
    // the top-right corner of the window...
    if (((m_LockLiftArea->pos ().y () + m_LockLiftArea->preferredHeight ()) >
          event->pos ().y ()) &&
        (event->pos ().x () > m_LockLiftArea->preferredWidth () - 160))
    {
        m_DnDstate = STATE_MOVING;

        // Move the icon to the start position:
        QSizeF size = m_DnDoverlay.preferredSize ();
        QPointF pos (event->pos ());

        pos.rx () -= (size.width  () / 2);
        pos.ry () -= (size.height () / 2);
        m_DnDoverlay.setPos (pos);

        updateDnDicon ();
        static_cast<UnlockHeader*>(m_LockLiftArea)->setActive (false);
        static_cast<UnlockArea*> (m_LockLandArea)->setEnabled (true);
    }
    else
        m_DnDstate = STATE_NONE;
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

    pos.rx () -= (size.width  () / 2);
    pos.ry () -= (size.height () / 2);
    m_DnDoverlay.setPos (pos);

    // And then check which icon we need to show:
    int newState = STATE_MOVING;

    // Check whether the DnD icon is inside the lock-land area...
    if (event->pos ().y () > m_LockLandArea->pos ().y ())
        newState = STATE_MOVING_ACTIVE;

    // To avoid unnecessary screen updates...
    if (newState != m_DnDstate)
    {
        m_DnDstate = newState;
        switch (newState)
        {
            case STATE_MOVING_ACTIVE:
                static_cast<UnlockArea*>(m_LockLandArea)->setActive (true);
                break;
            case STATE_MOVING:
            default:
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

    if (m_DnDstate == STATE_NONE)
        return;

    // Restore the default state ...
    static_cast<UnlockHeader*>(m_LockLiftArea)->setActive (true);
    static_cast<UnlockArea*> (m_LockLandArea)->setEnabled (false);

    if (m_DnDstate == STATE_MOVING_ACTIVE)
    {
        emit unlocked ();
    }
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
    MLayout               *layout;
    SYS_DEBUG ("");

    if (m_Realized)
        return;

    setContentsMargins (0., 0., 0., 0.);

    /*
     * The main layout and its policy
     */
    layout = new MLayout;
    m_policy = new MLinearLayoutPolicy (layout, Qt::Vertical);
    m_policy->setContentsMargins (0., 0., 0., 0.);
    m_policy->setSpacing (0.);

    /*
     * The topmost part of the lock-screen ui
     * (this one is optional, based on missed events)
     */
    m_notificationArea = new UnlockNotifications;
    m_notificationArea->setVisible (false);

    connect (m_notificationArea, SIGNAL (needToShow (bool)),
             this, SLOT (showHideNotifications (bool)));

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
    m_SceneWindow->setLayout (layout);
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

/******************************************************************************
 * The EventEaterUI implementation.
 */
EventEaterUI::EventEaterUI ()
{
    SYS_DEBUG ("");
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_X11NetWmWindowTypeDialog);
    setObjectName ("EventEaterUI");
    setProperty ("NoMStyle", true);
}

/*!
 * We got the mouse events, so we can close the event eater window even if MCE
 * does not ask for the closing.
 */
void
EventEaterUI::mousePressEvent (
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
EventEaterUI::mouseReleaseEvent (
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
EventEaterUI::showEvent (
        QShowEvent *event)
{
    Q_UNUSED (event);

    Window      windowID;
    Display    *display;
    Atom        nameAtom;
    Atom        utf8StringAtom;
    Atom        stackingLayerAtom;
    const char *windowName = "EventEaterUI";
    long        layer = 1;

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
