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
#ifndef _UNLOCKWIDGETS_H
#define _UNLOCKWIDGETS_H

#include <QSizeF>
#include <QObject>
#include <MWidget>
#include <MWindow>

class QPixmap;
class MLabel;
class MImageWidget;
class QGraphicsLinearLayout;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneDragDropEvent;

class UnlockHeader : public MWidget
{
    Q_OBJECT

public:
    UnlockHeader ();
    virtual ~UnlockHeader ();

    virtual void mousePressEvent (QGraphicsSceneMouseEvent *event);
    virtual void paint (QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget *widget = 0);

    void updateDateTime ();

signals:
    /* Emitted with
     * - true:  when drag is started
     * - false: when drag and drop is cancelled */
    void activateArea (bool enable);

private slots:
    void dndActionChanged (Qt::DropAction action);
    void dndDone ();

private:
    MImageWidget    *m_icon;
    QPixmap         *m_dnd_icon;
    MLabel          *m_TimeLabel;
    MLabel          *m_DateLabel;
    QPixmap         *m_background;

    Qt::DropAction   m_dndAction;
};

class UnlockArea : public MWidget
{
    Q_OBJECT

public:
    UnlockArea ();
    virtual ~UnlockArea ();
    virtual void dragEnterEvent (QGraphicsSceneDragDropEvent *event);
    virtual void dragLeaveEvent (QGraphicsSceneDragDropEvent *event);
    virtual void dropEvent (QGraphicsSceneDragDropEvent *event);
    virtual void paint (QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget *widget = 0);

public slots:
    void setEnabled (bool enabled);

signals:
    void unlocked ();

private:
    MImageWidget    *m_unlock_icon;
    bool             m_enabled;
    bool             m_active;
};

class UnlockNotifications : public MWidget
{
    Q_OBJECT

    enum {
        NOTIFY_CALLS = 0,
        NOTIFY_SMS,
        NOTIFY_EMAIL,
        NOTIFY_CHAT,
        NOTIFY_LAST
    };

public:
    UnlockNotifications ();
    virtual ~UnlockNotifications ();

    virtual void paint (QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget *widget = 0);
public slots:
    void updateMissedEvents (int emails,
                             int messages,
                             int calls,
                             int im);

private:
    QPixmap                 *m_background;
    MLabel                  *m_labels [NOTIFY_LAST];
    MImageWidget            *m_icons [NOTIFY_LAST];
    QGraphicsLinearLayout   *m_layout;
};

#endif
