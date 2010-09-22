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
#include "clock.h"
#include "date.h"

LockScreenHeaderView::LockScreenHeaderView(MWidgetController *controller) :
    MWidgetView(controller),
    layout(new QGraphicsLinearLayout(Qt::Horizontal)),
    date(new Date)
{
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    Clock *clock = new Clock;
    clock->setObjectName("LockScreenHeaderClock");
    QGraphicsLinearLayout* clockArea = new QGraphicsLinearLayout(Qt::Horizontal);
    clockArea->setContentsMargins(0, 0, 0, 0);
    clockArea->setSpacing(0);
    clockArea->addStretch();
    clockArea->addItem(clock);

    QGraphicsLinearLayout* dateArea = new QGraphicsLinearLayout(Qt::Horizontal);
    dateArea->setContentsMargins(0, 0, 0, 0);
    dateArea->setSpacing(0);
    dateArea->addStretch();
    dateArea->addItem(date);

    // Create a layout for the date and time
    QGraphicsLinearLayout *dateTimeLayout = new QGraphicsLinearLayout(Qt::Vertical);
    dateTimeLayout->setContentsMargins(0, 0, 0, 0);
    dateTimeLayout->setSpacing(0);
    dateTimeLayout->addItem(clockArea);
    dateTimeLayout->addItem(dateArea);
    dateTimeLayout->addStretch();

    // Add the date and time to the main layout
    layout->addItem(dateTimeLayout);
    controller->setLayout(layout);
}

LockScreenHeaderView::~LockScreenHeaderView()
{
}

void LockScreenHeaderView::applyStyle()
{
    MWidgetView::applyStyle();

    date->setFormat(style()->dateFormat());
}

M_REGISTER_VIEW_NEW(LockScreenHeaderView, MWidgetController)
