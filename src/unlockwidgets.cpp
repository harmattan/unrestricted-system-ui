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
#include <QTime>
#include <QDateTime>
#include <QMimeData>

#include <QDrag>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsLinearLayout>

#include <MTheme>
#include <MLocale>
#include <MLabel>
#include <MImageWidget>
#include <MApplicationWindow>
#include <MApplication>

#define DEBUG
#define WARNING
#include "debug.h"

#define DND_MIME_TYPE "application/x-dnditemdata"

UnlockHeader::UnlockHeader () : MWidget (),
    m_dnd_icon (0),
    m_TimeLabel (0),
    m_DateLabel (0),
    m_background (0),
    m_dndAction (Qt::IgnoreAction)
{
    QGraphicsLinearLayout   *datetimeBox;
    QGraphicsLinearLayout   *lockliftBox;

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
    m_icon = new MImageWidget ("locket");
    m_icon->setObjectName ("lockscreenIconLocked");

    /*
     * Pre-load the DnD icon
     * and the background
     */
    m_dnd_icon = MTheme::pixmapCopy ("flat_big_lock");
    m_background = MTheme::pixmapCopy ("top_fade");

    /*
     * Construct the Date/Time box
     */
    datetimeBox = new QGraphicsLinearLayout (Qt::Vertical);
    datetimeBox->setSpacing (0.0);
    datetimeBox->addItem (m_TimeLabel);
    datetimeBox->setAlignment (m_TimeLabel, Qt::AlignLeft);
    datetimeBox->addItem (m_DateLabel);
    datetimeBox->setAlignment (m_DateLabel, Qt::AlignLeft);

    /*
     * Create the main container
     */
    lockliftBox = new QGraphicsLinearLayout (Qt::Horizontal);
    lockliftBox->addItem (datetimeBox);
    lockliftBox->setAlignment (datetimeBox, Qt::AlignLeft | Qt::AlignVCenter);
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
    delete m_dnd_icon;
    delete m_background;
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
    if (geometry ().topRight ().x () >
        (event->pos ().x () + 160.0))
        return;

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

    // Emit the 'drag started' signal
    emit activateArea (true);

    connect (drag, SIGNAL (destroyed ()),
             this, SLOT (dndDone ()));
    connect (drag, SIGNAL (actionChanged (Qt::DropAction)),
             this, SLOT (dndActionChanged (Qt::DropAction)));

    drag->exec (Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
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

void
UnlockHeader::paint (QPainter *painter,
                     const QStyleOptionGraphicsItem *option,
                     QWidget *widget)
{
    Q_UNUSED (option);
    Q_UNUSED (widget);

    const int margin = 10; // FIXME

    painter->drawPixmap (-margin,
                         -margin,
                         (int) geometry ().width () + 2 * margin,
                         (int) m_background->height (),
                         * m_background);
}


UnlockArea::UnlockArea () : MWidget (),
    m_enabled (false),
    m_active (false)
{
    QGraphicsLinearLayout   *layout =
        new QGraphicsLinearLayout;

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

        // TODO: set border to blue one
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

        // TODO: set border to gray one
        m_unlock_icon->setImage ("unlocked");

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
        m_enabled = false;
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

    update ();
}

void
UnlockArea::paint (QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    Q_UNUSED (option);
    Q_UNUSED (widget);

    const qreal borderwidth = 20.; // FIXME

    if (m_enabled)
    {
        // TODO: parse it from some theme or css file
        QColor border_color;
        if (m_active)
            border_color.setRgbF (0., .67, .97, .5);
        else
            border_color.setRgbF (1., 1., 1., .5);

        // Top
        painter->fillRect (0.,
                           0.,
                           size ().width (),
                           borderwidth,
                           border_color);

        // Bottom
        painter->fillRect (0.,
                           size ().height () - borderwidth,
                           size ().width (),
                           borderwidth,
                           border_color);

        // Left
        painter->fillRect (0.,
                           borderwidth,
                           borderwidth,
                           size ().height () - 2 * borderwidth,
                           border_color);

        // Right
        painter->fillRect (size ().width () - borderwidth,
                           borderwidth,
                           borderwidth,
                           size ().height () - 2 * borderwidth,
                           border_color);
    }
}


UnlockNotifications::UnlockNotifications ()
{

}

UnlockNotifications::~UnlockNotifications ()
{
    // Free the resources here...
}

void
UnlockNotifications::updateMissedEvents (int emails,
                                         int messages,
                                         int calls,
                                         int im)
{

}

void
UnlockNotifications::paint (QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{

    // TODO
}

