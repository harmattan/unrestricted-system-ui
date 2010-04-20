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

#include <MWidget>
#include <MLayout>
#include <MSceneManager>
#include <MLinearLayoutPolicy>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(LockScreenUI)

#define DEBUG
#define WARNING
#include "debug.h"

LockScreenUI::LockScreenUI () :
        MApplicationPage (),
        m_LockLiftArea (0),
        m_LockLandArea (0),
        m_initialized (false)
{
    SYS_DEBUG ("");

    setPannable (false);

    // let's hide home button
    setComponentsDisplayMode (MApplicationPage::AllComponents,
                              MApplicationPageModel::Hide);
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

    MLinearLayoutPolicy   *policy;
    MLayout               *layout;

    /*
     * The main layout and its policy
     */
    layout = new MLayout;
    policy = new MLinearLayoutPolicy (layout, Qt::Vertical);

    /*
     * TODO: notification ui isn't implemented yet
     *  m_NotificationArea = new UnlockNotifications;
     *  m_NotificationArea->setObjectName ("lockscreenNotifications");
     */
    m_LockLiftArea = new UnlockHeader;
    m_LockLiftArea->setObjectName ("lockscreenHeaderContainer");
    m_LockLandArea = new UnlockArea;
    m_LockLandArea->setObjectName ("lockscreenUnlockArea");

    /*
     * TODO:
     *  policy-addItem (m_NotificationArea)
     */

    m_LockLiftArea->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Minimum);
    policy->addItem (m_LockLiftArea);
    m_LockLandArea->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Expanding);
    policy->addItem (m_LockLandArea);

    setLayout (layout);

    connect (m_LockLandArea, SIGNAL (unlocked ()),
             this, SLOT (sliderUnlocked ()));
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

