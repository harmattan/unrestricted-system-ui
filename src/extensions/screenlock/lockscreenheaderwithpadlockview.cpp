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

#include <QGraphicsLinearLayout>
#include <MImageWidget>
#include "lockscreenheaderwithpadlockview.h"
#include "lockscreenheader.h"
#include "clock.h"
#include "date.h"

LockScreenHeaderWithPadlockView::LockScreenHeaderWithPadlockView(MWidgetController *controller) :
    MWidgetView(controller),
    layout(new QGraphicsLinearLayout(Qt::Horizontal)),
    date(new Date)
{
    // Create a layout for the date and time
    QGraphicsLinearLayout *dateTimeLayout = new QGraphicsLinearLayout(Qt::Vertical);
    dateTimeLayout->setContentsMargins(0, 0, 0, 0);
    dateTimeLayout->setSpacing(0);

    Clock *clock = new Clock;
    clock->setObjectName("LockScreenHeaderClock");
    dateTimeLayout->addItem(clock);
    dateTimeLayout->addItem(date);

    // Create a layout for the padlock image widget
    QGraphicsLinearLayout *imageWidgetLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    imageWidgetLayout->setContentsMargins(0, 0, 0, 0);
    imageWidgetLayout->setSpacing(0);
    padlockImageWidget = new MImageWidget;
    padlockImageWidget->setVisible(true);
    padlockImageWidget->setImage("icon-m-common-locked", QSize (32, 32));
    padlockImageWidget->setZoomFactor(1.0);
    padlockImageWidget->setObjectName("lockscreenIconLocked");
    imageWidgetLayout->addStretch();
    imageWidgetLayout->addItem(padlockImageWidget);

    // Set up the main layout
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addItem(dateTimeLayout);
    layout->addStretch();
    layout->addItem(imageWidgetLayout);
    controller->setLayout(layout);
}

LockScreenHeaderWithPadlockView::~LockScreenHeaderWithPadlockView()
{
}

void LockScreenHeaderWithPadlockView::applyStyle()
{
    MWidgetView::applyStyle();

    padlockImageWidget->setVisible(style()->padlockVisible());
}

M_REGISTER_VIEW_NEW(LockScreenHeaderWithPadlockView, LockScreenHeader)
