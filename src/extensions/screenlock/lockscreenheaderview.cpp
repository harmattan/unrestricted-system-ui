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
#include <MStylableWidget>
#include <QGraphicsLinearLayout>
#include "lockscreenheaderview.h"
#include "lockscreenheader.h"
#include "statusarea.h"
#include "clock.h"
#include "date.h"

LockScreenHeaderView::LockScreenHeaderView(MWidgetController *controller) :
    MWidgetView(controller),
    layout(new QGraphicsLinearLayout(Qt::Vertical))
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
    QGraphicsLinearLayout *dateTimeVerticalLayout = new QGraphicsLinearLayout(Qt::Vertical);
    dateTimeVerticalLayout->setContentsMargins(0, 0, 0, 0);
    dateTimeVerticalLayout->setSpacing(0);
    dateTimeVerticalLayout->addItem(clock);
    dateTimeVerticalLayout->addItem(new Date);
    dateTimeVerticalLayout->addStretch();

    // Create a widget with a horizontal layout to align the time and date to the right and to apply a background image
    MWidgetController *dateTimeWidget = new MStylableWidget;
    dateTimeWidget->setStyleName("LockScreenHeaderDateTime");
    QGraphicsLinearLayout *dateTimeHorizontalLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    dateTimeHorizontalLayout->setContentsMargins(0, 0, 0, 0);
    dateTimeHorizontalLayout->setSpacing(0);
    dateTimeHorizontalLayout->addStretch();
    dateTimeHorizontalLayout->addItem(dateTimeVerticalLayout);
    dateTimeWidget->setLayout(dateTimeHorizontalLayout);

    // Add the date and time to the main layout
    layout->addItem(dateTimeWidget);
    controller->setLayout(layout);
}

LockScreenHeaderView::~LockScreenHeaderView()
{
}

M_REGISTER_VIEW_NEW(LockScreenHeaderView, LockScreenHeader)
