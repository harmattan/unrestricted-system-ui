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

#include <QTime>
#include <QDrag>
#include <QMimeData>
#include <QDateTime>
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
#define WARNING
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
    m_ImgSource = new UnlockIcon;
    m_ImgTarget = new UnlockArea;

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

    policy->addItem (lockliftBox);
    policy->addItem (m_ImgTarget);

    updateDateTime ();

    centralWidget ()->setLayout (layout);

    connect (m_ImgTarget, SIGNAL (unlocked ()),
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
//    SYS_DEBUG ("");

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

