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

#undef DEBUG
#define WARNING
#include "debug.h"

#define GCONF_BG_LANDSCAPE \
    "/desktop/standard/background/portrait/picture_filename"
#define GCONF_BG_PORTRAIT \
    "/desktop/standard/background/landscape/picture_filename"

LockScreenUI::LockScreenUI () :
        MApplicationPage (),
        m_notificationArea (0),
        m_LockLiftArea (0),
        m_LockLandArea (0),
        m_bgLandscape (0),
        m_bgPortrait (0),
        m_initialized (false),
        m_emails (0),
        m_messages (0),
        m_calls (0),
        m_im (0)
{
    SYS_DEBUG ("");

    m_confBgLandscape = new MGConfItem (GCONF_BG_LANDSCAPE, this);
    m_confBgPortrait  = new MGConfItem (GCONF_BG_PORTRAIT, this);

    setPannable (false);

    // let's hide home button
    setComponentsDisplayMode (MApplicationPage::AllComponents,
                              MApplicationPageModel::Hide);

    setContentsMargins (0., 0., 0., 0.);
}

LockScreenUI::~LockScreenUI ()
{
    // Free the resources here...
    if (m_bgLandscape != 0)
    {
        delete m_bgLandscape;
        m_bgLandscape = 0;
    }

    if (m_bgPortrait != 0)
    {
        delete m_bgPortrait;
        m_bgPortrait = 0;
    }
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
    // TODO: drop this hard-coded default one
    const char *defaultbg = "/usr/share/themes/base/meegotouch/sysuid/images/bg_landscape.png";

    QPixmap toCheck (m_confBgLandscape->value (QVariant (defaultbg)).toString ());

    if (toCheck.isNull () == false)
    {
        QSize landscape_size =
            MApplication::activeWindow ()->visibleSceneSize (M::Landscape);

        if (m_bgLandscape != 0)
            delete m_bgLandscape;

        m_bgLandscape = new QPixmap (toCheck.scaled (landscape_size));
    }
}

void
LockScreenUI::reloadPortraitBackground ()
{
    // TODO: drop this hard-coded default one
    const char *defaultbg = "/usr/share/themes/base/meegotouch/sysuid/images/bg_portrait.png";

    QPixmap toCheck (m_confBgPortrait->value (QVariant (defaultbg)).toString ());

    if (toCheck.isNull () == false)
    {
        QSize portrait_size =
            MApplication::activeWindow ()->visibleSceneSize (M::Portrait);

        if (m_bgPortrait != 0)
            delete m_bgPortrait;

        m_bgPortrait = new QPixmap (toCheck.scaled (portrait_size));
    }
}

void
LockScreenUI::paint (QPainter *painter,
                     const QStyleOptionGraphicsItem *option,
                     QWidget *widget)
{
    if ((m_bgPortrait != 0) &&
        (geometry ().height () > geometry ().width ()))
    {
        painter->drawPixmap (geometry ().toRect (), *m_bgPortrait);
    }
    else if ((m_bgLandscape != 0) &&
            (geometry ().height () < geometry ().width ()))
    {
        painter->drawPixmap (geometry ().toRect (), *m_bgLandscape);
    }
    else
    {
        // No pixmap loaded :-O
        // So calling the Page (the parent class)
        // paint method to painting the default background:
        MApplicationPage::paint (painter, option, widget);
    }
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

