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
#include "unlockarea.h"

#include <QTime>
#include <QDateTime>

#include <QGraphicsLinearLayout>

#include <MLabel>
#include <MImageWidget>

UnlockArea::UnlockArea(QGraphicsItem *parent) :
    MStylableWidget(parent),
    m_enabled(false),
    m_active(false)
{
    QGraphicsLinearLayout   *layout =
        new QGraphicsLinearLayout (Qt::Vertical);

    setObjectName ("LockLandArea");

    // Create the unlock icon
    MImageWidget    *unlock_icon;

    unlock_icon = new MImageWidget;
    unlock_icon->setImage ("icon-m-common-unlocked", QSize (32, 32));
    unlock_icon->setZoomFactor (1.0);
    unlock_icon->setObjectName ("LockScreenUnlockIcon");

    m_unlock_icon = new MWidget;
    QGraphicsLinearLayout   *icon_layout =
        new QGraphicsLinearLayout (Qt::Horizontal);
    icon_layout->addStretch ();
    icon_layout->addItem (unlock_icon);
    icon_layout->addStretch ();
    m_unlock_icon->setLayout (icon_layout);

    // Add the unlock icon centered
    layout->addStretch ();
    layout->addItem (m_unlock_icon);
    layout->addStretch ();

    setLayout (layout);

    updateState ();
}

UnlockArea::~UnlockArea ()
{
    // Free the resources here...
}

void
UnlockArea::setActive (bool active)
{
    if (m_enabled == false)
        return; /* no-op when disabled... */

    if (m_active == active)
        return;

    m_active = active;

    updateState ();
}

void
UnlockArea::setEnabled (bool enabled)
{
    if (m_enabled == enabled)
        return;

    m_enabled = enabled;
    // Set active to false when enable val. has been changed...
    m_active = false;

    updateState ();
}

void
UnlockArea::updateState ()
{
    if (m_enabled == false)
    {
        setObjectName ("LockLandArea");
    }
    else if (m_active == true)
    {
        setObjectName ("LockLandAreaActive");
    }
    else
    {
        setObjectName ("LockLandAreaDragged");
    }

    m_unlock_icon->setVisible (m_enabled);
    m_unlock_icon->update ();

    // After object-name change a screen-refresh is needed:
    update ();
}

