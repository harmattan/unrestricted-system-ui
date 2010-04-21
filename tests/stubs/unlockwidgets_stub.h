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
#include <QSizeF>
#include <unlockwidgets.h>

UnlockHeader::UnlockHeader() {

}

UnlockHeader::~UnlockHeader() {

}

void
UnlockHeader::updateDateTime() {

}

void
UnlockHeader::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void
UnlockHeader::dndActionChanged (Qt::DropAction action)
{
    Q_UNUSED(action);
}

void
UnlockHeader::dndDone ()
{

}

void
UnlockHeader::paint (QPainter *painter,
                     const QStyleOptionGraphicsItem *option,
                     QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

UnlockArea::UnlockArea() {

}

UnlockArea::~UnlockArea() {

}

void
UnlockArea::dragEnterEvent (QGraphicsSceneDragDropEvent *event)
{
    Q_UNUSED(event);
}

void
UnlockArea::dragLeaveEvent (QGraphicsSceneDragDropEvent *event)
{
    Q_UNUSED(event);
}

void
UnlockArea::dropEvent (QGraphicsSceneDragDropEvent *event)
{
    Q_UNUSED(event);
}

void
UnlockArea::setEnabled (bool enabled)
{
    Q_UNUSED(enabled);
}

void
UnlockArea::paint (QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

UnlockNotifications::UnlockNotifications ()
{

}

UnlockNotifications::~UnlockNotifications ()
{

}

void
UnlockNotifications::updateMissedEvents (int emails,
                                         int messages,
                                         int calls,
                                         int im)
{
    Q_UNUSED(emails);
    Q_UNUSED(messages);
    Q_UNUSED(calls);
    Q_UNUSED(im);
}

void
UnlockNotifications::paint (QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

