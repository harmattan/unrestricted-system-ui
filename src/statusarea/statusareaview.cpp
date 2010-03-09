/***************************************************************************
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

#include "statusareaview.h"
#include "statusarea.h"
#include "notifier.h"
#include "clock.h"
#include "contextframeworkcontext.h"
#include <QGraphicsLinearLayout>
#include <QGraphicsAnchorLayout>
#include <DuiSceneWindow>
#include <DuiPannableViewport>
#include <DuiSceneManager>
#include <DuiViewCreator>

StatusAreaView::StatusAreaView(StatusArea *controller) :
    DuiWidgetView(controller),
    controller(controller),
    notifier(new Notifier())
{
    // Layout for clock and search button
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    controller->setLayout(layout);

    // Create a composite widget with a clock and the alarm indicator
    Clock *clock = new Clock;
    QGraphicsAnchorLayout *clockAlarmLayout = new QGraphicsAnchorLayout;
    clockAlarmLayout->setContentsMargins(0, 0, 0, 0);
    clockAlarmLayout->addCornerAnchors(clock, Qt::TopLeftCorner, clockAlarmLayout, Qt::TopLeftCorner);
    clockAlarmLayout->addCornerAnchors(clock, Qt::TopRightCorner, clockAlarmLayout, Qt::TopRightCorner);
    DuiWidget *clockAlarmWidget = new DuiWidget;
    clockAlarmWidget->setLayout(clockAlarmLayout);
    layout->addItem(clockAlarmWidget);

    // Create the notifier
    notifier->setObjectName("Notifier");
    layout->addItem(notifier);

    // Stretch
    layout->addStretch();
}

StatusAreaView::~StatusAreaView()
{
}

DUI_REGISTER_VIEW_NEW(StatusAreaView, StatusArea)
