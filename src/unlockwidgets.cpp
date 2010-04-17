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

#include <QPixmap>
#include <MTheme>
#include <MImageWidget>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsLinearLayout>

#define DEBUG
#define WARNING
#include "debug.h"

#define DND_MIME_TYPE "application/x-dnditemdata"

UnlockHeader::UnlockHeader () : MWidget (),
    m_dnd_icon (0)
{
    QGraphicsLinearLayout   *layout;

    m_icon = new MImageWidget ("locket");
    m_icon->setObjectName ("lockscreenIconLocked");

    m_dnd_icon = new QPixmap (* (MTheme::pixmap ("big_lock")));

    layout = new QGraphicsLinearLayout;

    layout->addItem (m_icon);

    setLayout (layout);
}

UnlockHeader::~UnlockHeader ()
{
    if (m_dnd_icon != 0)
    {
        delete m_dnd_icon;
        m_dnd_icon = 0;
    }
}

void
UnlockHeader::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
    QDrag *drag = new QDrag (event->widget ());

    QMimeData *mimeData = new QMimeData;
    mimeData->setData (DND_MIME_TYPE, 0);

    drag->setMimeData (mimeData);

    if (m_dnd_icon != 0)
    {
        drag->setPixmap (* m_dnd_icon);
        drag->setHotSpot (QPoint (m_dnd_icon->width () / 2,
                                  m_dnd_icon->height () / 2));
    }
    else
        SYS_WARNING ("ERROR, pixmap is empty!");


    drag->exec (Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
}



UnlockArea::UnlockArea () : MWidget ()
{
    QGraphicsLinearLayout   *layout =
        new QGraphicsLinearLayout;

    m_unlock_icon = new MImageWidget ("unlocked");
    m_unlock_icon->setObjectName ("lockscreenIconUnlock");

    // Add the unlock icon centered
    layout->addItem (m_unlock_icon);
    layout->setAlignment (m_unlock_icon, Qt::AlignCenter);

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
//        SYS_DEBUG ("")
        event->setDropAction (Qt::MoveAction);
        event->accept ();
    }
    else
        event->ignore ();
}

void
UnlockArea::dragMoveEvent (QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData ()->hasFormat (DND_MIME_TYPE))
    {
//        SYS_DEBUG ("")
        event->setDropAction (Qt::MoveAction);
        event->accept ();
    }
    else
        event->ignore ();
}

void
UnlockArea::dropEvent (QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData ()->hasFormat (DND_MIME_TYPE))
    {
        SYS_DEBUG ("Dropped");

        event->setDropAction (Qt::MoveAction);
        event->accept ();

        // Emit the "unlocked" signal
        emit unlocked ();
    }
    else
        event->ignore ();
}

