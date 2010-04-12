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
/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "lockscreenui.h"
#include "unlocksliderwidget/unlockslider.h"
#include "sysuid.h"

#include <QDebug>

#include <MLabel>
#include <MImageWidget>
#include <MButton>
#include <MLayout>
#include <MGridLayoutPolicy>
#include <MSceneManager>
#include <MApplicationWindow>
#include <MTheme>
#include <MLocale>

#include <QTime>
#include <QDateTime>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(LockScreenUI)

#define DEBUG
#include "debug.h"

LockScreenUI::LockScreenUI () :
        timeLabel (0),
        dateLabel (0)
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

/*!
 * FIXME: This is a public method and there is no protection against multiple
 * calling.
 * FIXME: It is not clear why we need createWidgets() and createContent(), one
 * should be enough.
 */
void
LockScreenUI::createContent ()
{
    SYS_DEBUG ("");

    MApplicationPage::createContent();

    /*
     * I had to modify this function because it caused a leak, and that led us
     * to failed unit test. Now the screen looks a little bit different, but
     * that doesn't matter, because we have to change the UI according to the
     * new UI spec anyway. I left the old code here, so we can see what was the
     * original idea.
     */
    //MLayout* layout = new MLayout;
    MLayout* widgets = createWidgets();

#if 0
    QSize size = Sysuid::sysuid()->applicationWindow().sceneManager()->visibleSceneSize(M::Landscape);

    MGridLayoutPolicy* l_policy = new MGridLayoutPolicy(layout);
    l_policy->setSpacing(10);
    l_policy->setRowFixedHeight(1, size.height());
    l_policy->setColumnFixedWidth(0, size.width());
    l_policy->addItem(widgets, 1, 0, Qt::AlignCenter);

    size = Sysuid::sysuid()->applicationWindow().sceneManager()->visibleSceneSize(M::Portrait);

    MGridLayoutPolicy* p_policy = new MGridLayoutPolicy(layout);
    p_policy->setSpacing(10);
    p_policy->setRowFixedHeight(1, size.height());
    p_policy->setColumnFixedWidth(0, size.width());
    p_policy->addItem(widgets, 1, 0, Qt::AlignCenter);

    layout->setLandscapePolicy(l_policy);
    layout->setPortraitPolicy(p_policy);

#endif
    //centralWidget()->setLayout(layout);
    centralWidget()->setLayout(widgets);

    connect(slider, SIGNAL(unlocked()), this, SLOT(sliderUnlocked()));
}

void 
LockScreenUI::sliderUnlocked ()
{
    SYS_DEBUG ("");

    disappear ();
    slider->reset ();
    emit unlocked ();
}

MLayout *
LockScreenUI::createWidgets ()
{
    SYS_DEBUG ("");
    MLayout* layout = new MLayout();

    MGridLayoutPolicy* policy = new MGridLayoutPolicy(layout);

    policy->setColumnAlignment (0, Qt::AlignCenter);

    QGraphicsWidget* spacert = new QGraphicsWidget;
    spacert->setSizePolicy (
            QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding));

    timeLabel = new MLabel;
    timeLabel->setObjectName ("lockscreenTimeLabel");
    timeLabel->setAlignment (Qt::AlignCenter);

    dateLabel = new MLabel;
    dateLabel->setObjectName ("lockscreenDateLabel");
    dateLabel->setAlignment (Qt::AlignCenter);

    updateDateTime();

    slider = new UnlockSlider;
    slider->setSizePolicy (QSizePolicy (QSizePolicy::Expanding,
                                        QSizePolicy::Expanding));
    slider->setVisible (true);

    slider->setObjectName ("unlockslider");
    slider->setMinimumWidth (450);
    slider->setMaximumWidth (450);
    slider->setMaximumHeight (80);

    policy->addItem (spacert,                 0, 0, 1, 6);
    policy->addItem (timeLabel,               1, 0, 1, 6);
    policy->addItem (dateLabel,               2, 0, 1, 6);
    policy->addItem (slider,                  3, 0, 1, 6);

    return layout;
}

void 
LockScreenUI::updateDateTime ()
{
    SYS_DEBUG ("");

    if (isContentCreated () == false)
        return;

    MLocale locale;

    QDateTime now (QDateTime::currentDateTime());

    timeLabel->setText (locale.formatDateTime (
                now, MLocale::DateNone, MLocale::TimeShort));
    dateLabel->setText (locale.formatDateTime (
                now, MLocale::DateFull, MLocale::TimeNone));
    update();
}


