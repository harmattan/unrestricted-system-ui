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
#include "sysuid.h"

#include <QGraphicsLinearLayout>

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

#define GCONF_BG_LANDSCAPE \
    "/desktop/meego/background/landscape/picture_filename"
#define GCONF_BG_PORTRAIT \
    "/desktop/meego/background/portrait/picture_filename"

LockScreenUI::LockScreenUI () :
        MApplicationPage (),
        m_notificationArea (0),
        m_LockLiftArea (0),
        m_LockLandArea (0),
        m_initialized (false),
        m_emails (0),
        m_messages (0),
        m_calls (0),
        m_im (0)
{
    SYS_DEBUG ("*** landscape_key = %s", GCONF_BG_LANDSCAPE);
    SYS_DEBUG ("*** portrait_key = %s", GCONF_BG_PORTRAIT);

    m_confBgLandscape = new MGConfItem (GCONF_BG_LANDSCAPE, this);
    m_confBgPortrait  = new MGConfItem (GCONF_BG_PORTRAIT, this);

    connect (m_confBgLandscape, SIGNAL(valueChanged()),
            this, SLOT(reloadLandscapeBackground()));
    connect (m_confBgPortrait, SIGNAL(valueChanged()),
            this, SLOT(reloadPortraitBackground()));


    setPannable (false);

    // let's hide home button
    setComponentsDisplayMode (MApplicationPage::AllComponents,
                              MApplicationPageModel::Hide);

    setContentsMargins (0., 0., 0., 0.);
}

LockScreenUI::~LockScreenUI ()
{
    // Free the resources here...
}

void
LockScreenUI::createContent ()
{
    SYS_DEBUG ("");

    if (m_initialized == true)
    {
        SYS_WARNING (" this function called more then once!");
        return;
    }
    m_initialized = true;

    MApplicationPage::createContent ();

    MLayout               *layout;

    /*
     * The main layout and its policy
     */
    layout = new MLayout;
    m_policy = new MLinearLayoutPolicy (layout, Qt::Vertical);

    m_notificationArea = new UnlockNotifications;
    m_notificationArea->setObjectName ("lockscreenNotifications");
    m_LockLiftArea = new UnlockHeader;
    m_LockLiftArea->setObjectName ("lockscreenHeaderContainer");
    m_LockLandArea = new UnlockArea;
    m_LockLandArea->setObjectName ("lockscreenUnlockArea");

    m_notificationArea->setVisible (false);
    m_notificationArea->setSizePolicy (QSizePolicy::Preferred,
                                       QSizePolicy::Minimum);
    // updateMissedEventAmounts will add this ^ to policy

    m_LockLiftArea->setSizePolicy (QSizePolicy::Preferred,
                                   QSizePolicy::Minimum);
    m_policy->addItem (m_LockLiftArea);
    m_LockLandArea->setSizePolicy (QSizePolicy::Preferred,
                                   QSizePolicy::Expanding);
    m_policy->addItem (m_LockLandArea);

    // Set the main layout
    setLayout (layout);

    connect (m_LockLiftArea, SIGNAL (activateArea (bool)),
             m_LockLandArea, SLOT (setEnabled (bool)));

    connect (m_LockLandArea, SIGNAL (unlocked ()),
             this, SLOT (sliderUnlocked ()));

    // Load the backgrounds if any...
    reloadLandscapeBackground ();
    reloadPortraitBackground ();

    // I'm calling this for updating the m_notificationArea
    updateMissedEventAmounts (m_emails, m_messages, m_calls, m_im);
}

void
LockScreenUI::sliderUnlocked ()
{
    SYS_DEBUG ("");

    disappear ();
    emit unlocked ();
}

void
LockScreenUI::updateDateTime ()
{
    if (isContentCreated () == false)
        return;

    static_cast<UnlockHeader *> (m_LockLiftArea)->updateDateTime ();
}

void
LockScreenUI::reloadLandscapeBackground ()
{
    QString filename = m_confBgLandscape->value().toString();

    // TODO: drop this hard-coded default one
    if (filename.isEmpty())
        filename = 
            "/usr/share/themes/base/meegotouch/sysuid/images/bg_landscape.png";

    SYS_DEBUG ("landscape value = %s", SYS_STR(filename));
    m_bgLandscape.load (filename);
}

void
LockScreenUI::reloadPortraitBackground ()
{
    QString filename = m_confBgPortrait->value().toString();

    // TODO: drop this hard-coded default one
    if (filename.isEmpty())
        filename = 
            "/usr/share/themes/base/meegotouch/sysuid/images/bg_portrait.png";

    SYS_DEBUG ("portrait  value = %s", SYS_STR(filename));
    m_bgPortrait.load (filename);
}

void
LockScreenUI::paint (QPainter *painter,
                     const QStyleOptionGraphicsItem *option,
                     QWidget *widget)
{
    Q_UNUSED (option);
    Q_UNUSED (widget);
    bool portrait = geometry().height() > geometry().width ();

    if (isVisible () == false)
        return;

    if (portrait)
    {
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
#if 0
    else
    {
        // No pixmap loaded :-O
        // So calling the Page (the parent class)
        // paint method to painting the default background:
        MApplicationPage::paint (painter, option, widget);
    }
#endif
}

void
LockScreenUI::updateMissedEventAmounts (int emails,
                                        int messages,
                                        int calls,
                                        int im)
{
    SYS_DEBUG ("");

    m_emails = emails;
    m_messages = messages;
    m_calls = calls;
    m_im = im;

    if (m_notificationArea != 0)
    {
        static_cast<UnlockNotifications *> (m_notificationArea)-> 
                    updateMissedEvents (emails, messages, calls, im);

        // Hide the whole missed events notification area when
        // there is no any missed events...
        if (m_notificationArea->isVisible () &&
            ((emails + messages + calls + im) == 0))
        {
            m_notificationArea->setVisible (false);
            m_policy->removeItem (m_notificationArea);
        }

        // Add notification area to policy when previously was
        // hidden, but there are some missed events...
        if ((m_notificationArea->isVisible () == false) &&
            (emails + messages + calls + im) > 0)
        {
            m_notificationArea->setVisible (true);
            m_policy->insertItem (0, m_notificationArea);
        }
    }
}

EventEaterUI::EventEaterUI ()
{
    setPannable (false);
    // let's hide home button
    setComponentsDisplayMode (MApplicationPage::AllComponents,
                              MApplicationPageModel::Hide);
    setContentsMargins (0., 0., 0., 0.);
    setOpacity (0.0);
}

void
EventEaterUI::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED (event);
    emit OneInput ();
}

