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
#include "unlockwidgets.h"

#include <QTime>
#include <QDateTime>
#include <QMimeData>

#include <QDrag>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsLinearLayout>

#include <MLocale>
#include <MLabel>
#include <MImageWidget>

#undef DEBUG
#define WARNING
#include "debug.h"

#define DND_MIME_TYPE "application/x-dnditemdata"

UnlockHeader::UnlockHeader () :
    m_TimeLabel (0),
    m_DateLabel (0),
    m_dndAction (Qt::IgnoreAction)
{
    QGraphicsLinearLayout   *datetimeBox;
    QGraphicsLinearLayout   *lockliftBox;

    setObjectName ("LockLiftArea");

    /*
     * The label that shows the hour and minute
     */
    m_TimeLabel = new MLabel;
    m_TimeLabel->setObjectName ("lockscreenTimeLabel");

    /*
     * The label that shows the date
     */
    m_DateLabel = new MLabel;
    m_DateLabel->setObjectName ("lockscreenDateLabel");

    /*
     * The lock icon @ right side
     */
    m_icon = new MImageWidget;
    m_icon->setImage ("icon-m-common-locked", QSize (32, 32));
    m_icon->setZoomFactor (1.0);
    m_icon->setObjectName ("lockscreenIconLocked");

    /*
     * Construct the Date/Time box
     */
    datetimeBox = new QGraphicsLinearLayout (Qt::Vertical);
    datetimeBox->setContentsMargins (0., 0., 0., 0.);
    datetimeBox->setSpacing (0.0);
    datetimeBox->addItem (m_TimeLabel);
    datetimeBox->setAlignment (m_TimeLabel, Qt::AlignLeft);
    datetimeBox->addItem (m_DateLabel);
    datetimeBox->setAlignment (m_DateLabel, Qt::AlignLeft);

    /*
     * Create the main container
     */
    lockliftBox = new QGraphicsLinearLayout (Qt::Horizontal);
    lockliftBox->setContentsMargins (0., 0., 0., 0.);
    lockliftBox->addItem (datetimeBox);
    lockliftBox->setAlignment (datetimeBox, Qt::AlignLeft | Qt::AlignTop);
    lockliftBox->addItem (m_icon);
    lockliftBox->setAlignment (m_icon, Qt::AlignRight | Qt::AlignVCenter);

    /*
     * And set the layout
     */
    setLayout (lockliftBox);

    /*
     * And initialize the widgets contents
     */
    updateDateTime ();
}

UnlockHeader::~UnlockHeader ()
{
}

void
UnlockHeader::updateDateTime ()
{
    if ((m_TimeLabel == 0) ||
        (m_DateLabel == 0))
        return;

    MLocale locale;

    QDateTime now (QDateTime::currentDateTime ());

    m_TimeLabel->setText (locale.formatDateTime (
                now, MLocale::DateNone, MLocale::TimeShort));
    m_DateLabel->setText (locale.formatDateTime (
                now, MLocale::DateFull, MLocale::TimeNone));

    update ();
}

void
UnlockHeader::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
    QDrag *drag = new QDrag (event->widget ());

    // TBD: i used 160px for active lock lift area
    if (geometry ().topRight ().x () > (event->pos ().x () + 160.0))
        return;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData (DND_MIME_TYPE, 0);

    drag->setMimeData (mimeData);

    // Emit the 'drag started' signal
    emit activateArea (true);

    connect (drag, SIGNAL (destroyed ()),
             this, SLOT (dndDone ()));
    connect (drag, SIGNAL (actionChanged (Qt::DropAction)),
             this, SLOT (dndActionChanged (Qt::DropAction)));

    drag->exec (Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
}

void
UnlockHeader::mouseMoveEvent (QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->scenePos ();
    SYS_DEBUG ("pos = %2.2f, %2.2f", pos.x (), pos.y ());
}

void
UnlockHeader::dndActionChanged (Qt::DropAction action)
{
    // Store the last DnD action
    m_dndAction = action;
}

void
UnlockHeader::dndDone ()
{
    // QDrag destroyed, check the last action,
    // when it is ignore then dropped outside on the area
    //  -> then i should disable it (hide the icon and border)
    if (m_dndAction == Qt::IgnoreAction)
        emit activateArea (false);
}

UnlockArea::UnlockArea () :
    m_enabled (false),
    m_active (false)
{
    QGraphicsLinearLayout   *layout =
        new QGraphicsLinearLayout;

    setObjectName ("LockLandArea");

    m_unlock_icon = new MImageWidget ("unlocked");
    m_unlock_icon->setObjectName ("lockscreenIconUnlock");

    layout->addStretch ();

    // Add the unlock icon centered
    layout->addItem (m_unlock_icon);
    layout->setAlignment (m_unlock_icon, Qt::AlignCenter);

    m_unlock_icon->setVisible (m_enabled);

    layout->addStretch ();

    setLayout (layout);

    setAcceptDrops (true);
}

UnlockArea::~UnlockArea ()
{
    // Free the resources here...
}

void
UnlockArea::dragEnterEvent (QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData ()->hasFormat (DND_MIME_TYPE))
    {
        event->setDropAction (Qt::MoveAction);
        event->accept ();

        setObjectName ("LockLandAreaActive");

        m_unlock_icon->setImage ("unlock_drop");

        m_active = true;

        update ();
    }
    else
        event->ignore ();
}

void
UnlockArea::dragLeaveEvent (QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData ()->hasFormat (DND_MIME_TYPE))
    {
        event->setDropAction (Qt::MoveAction);
        event->accept ();

        setObjectName ("LockLandAreaDragged");

        m_active = false;

        update ();
    }
    else
        event->ignore ();
}

void
UnlockArea::dropEvent (QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData ()->hasFormat (DND_MIME_TYPE))
    {
//        SYS_DEBUG ("Dropped");

        event->setDropAction (Qt::MoveAction);
        event->accept ();

        // Emit the "unlocked" signal
        emit unlocked ();

        // Restore the old image...
        m_unlock_icon->setImage ("unlocked");

        m_active = false;
        setEnabled (false);
    }
    else
        event->ignore ();
}

void
UnlockArea::setEnabled (bool enabled)
{
    if (m_enabled == enabled)
        return;

    
    m_enabled = enabled;

    m_unlock_icon->setVisible (m_enabled);

    if (! m_enabled)
    {
        setObjectName ("LockLandArea");
    }
    else if (! m_active)
    {
        setObjectName ("LockLandAreaDragged");
    }
    else
    {
        setObjectName ("LockLandAreaActive");
    }

    update ();
}

