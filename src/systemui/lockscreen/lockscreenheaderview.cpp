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

#include <MWidgetController>
#include <QGraphicsLinearLayout>
#include "lockscreenheaderview.h"
#include "statusarea.h"
#include "clock.h"
#include "date.h"

LockScreenHeaderView::LockScreenHeaderView(MWidgetController *controller) :
    MWidgetView(controller),
    layout(new QGraphicsLinearLayout(Qt::Vertical)),
    date(new Date)
{
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Add a status area to the main layout
    StatusArea *statusArea = new StatusArea;
    statusArea->setViewType("lockScreen");
    layout->addItem(statusArea);

    // Create a layout for the time and date
    Clock *clock = new Clock;
    clock->setObjectName("LockScreenHeaderClock");
    QGraphicsLinearLayout *dateTimeLayout = new QGraphicsLinearLayout(Qt::Vertical);
    dateTimeLayout->setContentsMargins(0, 0, 0, 0);
    dateTimeLayout->setSpacing(0);
    dateTimeLayout->addItem(clock);
    dateTimeLayout->addItem(date);
    dateTimeLayout->addStretch();

    // Create a horizontal layout to align the time and date to the right (without allowing them to grow)
    QGraphicsLinearLayout *horizontalLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    horizontalLayout->setSpacing(0);
    horizontalLayout->addStretch();
    horizontalLayout->addItem(dateTimeLayout);

    // Add the date and time to the main layout
    layout->addItem(horizontalLayout);
    controller->setLayout(layout);
}

LockScreenHeaderView::~LockScreenHeaderView()
{
}

M_REGISTER_VIEW_NEW(LockScreenHeaderView, MWidgetController)
