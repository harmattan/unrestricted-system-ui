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
#include "unlocknotifications.h"

#include <QGraphicsLinearLayout>
#include <MLabel>
#include <QPixmap>
#include <MImageWidget>

UnlockNotifications::UnlockNotifications ()
{
    m_layout = new QGraphicsLinearLayout (Qt::Horizontal);
    m_layout->addStretch (25);

    for (int i = 0; i < NOTIFY_LAST; i++)
    {
        m_labels [i] = new MLabel;
        m_labels [i]->setVisible (false);
        m_icons [i] = new MImageWidget;
        m_icons [i]->setVisible (false);
    }

    m_icons [NOTIFY_CALLS]->setImage ("icon-m-content-call", QSize (32, 32));
    m_icons [NOTIFY_SMS]->setImage ("icon-m-content-sms", QSize (32, 32));
    m_icons [NOTIFY_EMAIL]->setImage ("icon-m-content-email", QSize (32, 32));
    m_icons [NOTIFY_CHAT]->setImage ("icon-m-content-chat", QSize (32, 32));

    setLayout (m_layout);
}

UnlockNotifications::~UnlockNotifications ()
{
    for (int i = 0; i < NOTIFY_LAST; i++)
    {
        delete m_labels [i];
        delete m_icons [i];
    }
}

void
UnlockNotifications::updateMissedEvents (int emails,
                                         int messages,
                                         int calls,
                                         int im)
{
    for (int i = 0; i < NOTIFY_LAST; i++)
    {
        m_icons [i]->setVisible (true);
        m_labels [i]->setVisible (false);
    }

    if (calls > 0)
    {
        m_labels[NOTIFY_CALLS]->setText (QString ("%1").arg (calls));
        m_labels[NOTIFY_CALLS]->setVisible (true);
        m_icons[NOTIFY_CALLS]->setVisible (true);
    }

    if (messages > 0)
    {
        m_labels[NOTIFY_SMS]->setText (QString ("%1").arg (messages));
        m_labels[NOTIFY_SMS]->setVisible (true);
        m_icons[NOTIFY_SMS]->setVisible (true);
    }

    if (emails > 0)
    {
        m_labels[NOTIFY_EMAIL]->setText (QString ("%1").arg (emails));
        m_labels[NOTIFY_EMAIL]->setVisible (true);
        m_icons[NOTIFY_EMAIL]->setVisible (true);
    }

    if (im > 0)
    {
        m_labels[NOTIFY_CHAT]->setText (QString ("%1").arg (im));
        m_labels[NOTIFY_CHAT]->setVisible (true);
        m_icons[NOTIFY_CHAT]->setVisible (true);
    }

    // Remove the old items
    for (int c = m_layout->count () - 1; c >= 0; c--)
        m_layout->removeAt (c);

    // Add the new ones
    for (int id = 0; id < NOTIFY_LAST; id++)
    {
        if (m_labels [id]->isVisible () == true)
        {
            m_layout->insertItem (id * 2, m_icons [id]);
            m_layout->setAlignment (m_icons [id], Qt::AlignRight);
            m_layout->insertItem (id * 2 + 1, m_labels [id]);
            m_layout->setAlignment (m_labels [id], Qt::AlignLeft);
        }
    }
}

#define ACTIVE_RGBA     0., .67, .97, .5

void
UnlockNotifications::paint (QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    Q_UNUSED (option);
    Q_UNUSED (widget);

#if 0
    // TODO: draw background here
    SYS_DEBUG ("size: %dx%d",
               (int) geometry ().width (),
               (int) geometry ().height ());

    painter->fillRect (geometry ().toRect (), QColor (ACTIVE_RGBA));
#else
    Q_UNUSED (painter);
#endif
}

