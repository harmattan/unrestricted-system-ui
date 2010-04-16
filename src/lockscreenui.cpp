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
#include "unlocksliderwidget/unlockslider.h"
#include "sysuid.h"

#include <QTime>
#include <QDateTime>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLinearLayout>

#include <MLabel>
#include <MWidget>
#include <MImageWidget>
#include <MButton>
#include <MLayout>
#include <MGridLayoutPolicy>
#include <MSceneManager>
#include <MLinearLayoutPolicy>
#include <MApplicationWindow>
#include <MTheme>
#include <MLocale>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(LockScreenUI)

#define DEBUG
#include "debug.h"

LockScreenUI::LockScreenUI () :
        m_TimeLabel (0),
        m_DateLabel (0),
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
    SYS_DEBUG ("");
}

class MyImage : public MWidget
{
public:
    MyImage (const char *img) : MWidget ()
    {
        m_img = new MImageWidget (img);
        QGraphicsLinearLayout *layout = new QGraphicsLinearLayout;
        layout->addItem (m_img);
        setLayout (layout);
    }

    virtual void dragEnterEvent (QDragEnterEvent *event)
    {
        SYS_DEBUG("");

        if (event->mimeData ()->hasFormat ("application/x-dnditemdata"))
        {
            SYS_DEBUG ("enter drag");
            event->setDropAction (Qt::MoveAction);
            event->accept ();
        }
        else
            event->ignore ();
    }

    virtual void dragMoveEvent (QDragMoveEvent *event)
    {
        SYS_DEBUG("");

        if (event->mimeData ()->hasFormat ("application/x-dnditemdata"))
        {
            SYS_DEBUG ("dragmove");
            event->setDropAction (Qt::MoveAction);
            event->accept ();
        }
        else
            event->ignore ();

    }

    virtual void dropEvent (QDropEvent *event)
    {
        SYS_DEBUG("");

        if (event->mimeData ()->hasFormat ("application/x-dnditemdata"))
        {
            SYS_DEBUG ("Dropped");
            // TODO: Do the unlocking here...
            event->setDropAction (Qt::MoveAction);
            event->accept ();
        }
        else
            event->ignore ();
    }

    virtual void mousePressEvent (QGraphicsSceneMouseEvent *event)
    {
        SYS_DEBUG("");

        SYS_DEBUG ("press (%f.%f)", event->pos().x(), event->pos().y ());
    }
private:
    MImageWidget  *m_img;
};

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

    QGraphicsLinearLayout *datetimeBox;
    QGraphicsLinearLayout *lockliftBox;
    MLinearLayoutPolicy   *policy;
    MLayout               *layout;

    /*
     * The main layout and its policy
     */
    layout = new MLayout;
    policy = new MLinearLayoutPolicy (layout, Qt::Vertical);

    /*
     * The label that shows the hour and minute
     */
    m_TimeLabel = new MLabel;
    m_TimeLabel->setObjectName ("lockscreenm_TimeLabel");

    /*
     * The label that shows the date
     */
    m_DateLabel = new MLabel;
    m_DateLabel->setObjectName ("lockscreenm_DateLabel");

    /*
     * The two images, one that we start the unlocking (source) and one that we
     * have to drop the source (target) to unlock the screen.
     *
     * icon-m-common-locked and icon-m-common-unlocked?
     */
    m_ImgSource = new MyImage ("icon-m-common-locked");
    m_ImgTarget = new MyImage ("icon-m-common-unlocked");

    datetimeBox = new QGraphicsLinearLayout (Qt::Vertical);
    datetimeBox->addItem (m_TimeLabel);
    datetimeBox->setAlignment (m_TimeLabel, Qt::AlignLeft);
    datetimeBox->addItem (m_DateLabel);
    datetimeBox->setAlignment (m_DateLabel, Qt::AlignLeft);

    lockliftBox = new QGraphicsLinearLayout (Qt::Horizontal);
    lockliftBox->addItem (datetimeBox);
    lockliftBox->setAlignment (datetimeBox, Qt::AlignLeft | Qt::AlignVCenter);
    lockliftBox->addItem (m_ImgSource);
    lockliftBox->setAlignment (m_ImgSource, Qt::AlignRight | Qt::AlignVCenter);

    /*
     * The m_slider: it is deprecated, but we use it until we have something else
     */
    m_slider = new UnlockSlider;
    m_slider->setSizePolicy (QSizePolicy (QSizePolicy::Expanding,
                                        QSizePolicy::Expanding));
    m_slider->setVisible (true);
    m_slider->setObjectName ("unlockslider");
    m_slider->setMinimumWidth (450);
    m_slider->setMaximumWidth (450);
    m_slider->setMaximumHeight (80);

    policy->addItem (lockliftBox);
    policy->addItem (m_slider, Qt::AlignCenter);
    policy->addItem (m_ImgTarget);

    updateDateTime ();

    centralWidget ()->setLayout (layout);

    connect (m_slider, SIGNAL (unlocked ()),
             this, SLOT (sliderUnlocked ()));
}

void
LockScreenUI::sliderUnlocked ()
{
    SYS_DEBUG ("");

    disappear ();
    m_slider->reset ();
    emit unlocked ();
}

void
LockScreenUI::updateDateTime ()
{
    SYS_DEBUG ("");

    if (isContentCreated () == false)
        return;

    MLocale locale;

    QDateTime now (QDateTime::currentDateTime ());

    m_TimeLabel->setText (locale.formatDateTime (
                now, MLocale::DateNone, MLocale::TimeShort));
    m_DateLabel->setText (locale.formatDateTime (
                now, MLocale::DateFull, MLocale::TimeNone));

    update ();
}

