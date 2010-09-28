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

#include "statusareawithnotifierview.h"
#include "statusarea.h"
#include "contextframeworkcontext.h"
#include "notificationstatusindicator.h"
#include <QGraphicsAnchorLayout>

StatusAreaWithNotifierView::StatusAreaWithNotifierView(StatusArea *controller) :
    StatusAreaView(controller),
    landscapeNotificationIndicator(new NotificationStatusIndicator(&notifierNotificationSink, controller)),
    portraitNotificationIndicator(new NotificationStatusIndicator(&notifierNotificationSink, controller))
{
    // Connect controller's signal for notifying status menu window's visibility change
    connect(controller, SIGNAL(statusIndicatorMenuVisibilityChanged(bool)), this, SLOT(controlSinkBasedOnVisibility(bool)));

    // Getting layout from controller that has been initialized in base class
    if (QGraphicsAnchorLayout *compositeLayout = dynamic_cast<QGraphicsAnchorLayout *> (controller->layout())) {
        // Add notification indicator and anchor it to middle of landscape and portrait widgets
        compositeLayout->addAnchor(landscapeNotificationIndicator, Qt::AnchorVerticalCenter, landscapeWidget, Qt::AnchorVerticalCenter);
        compositeLayout->addAnchor(landscapeNotificationIndicator, Qt::AnchorHorizontalCenter, landscapeWidget, Qt::AnchorHorizontalCenter);

        compositeLayout->addAnchor(portraitNotificationIndicator, Qt::AnchorVerticalCenter, portraitWidget, Qt::AnchorVerticalCenter);
        compositeLayout->addAnchor(portraitNotificationIndicator, Qt::AnchorHorizontalCenter, portraitWidget, Qt::AnchorHorizontalCenter);
    }

    // Set up the class for functional testing
    landscapeNotificationIndicator->setParent(landscapeWidget);
    portraitNotificationIndicator->setParent(portraitWidget);
}

StatusAreaWithNotifierView::~StatusAreaWithNotifierView()
{
}

void StatusAreaWithNotifierView::controlSinkBasedOnVisibility(bool menuVisible)
{
    // When status menu is opened remove all notifications from the sink
    if (menuVisible) {
        notifierNotificationSink.clearSink();
    }

    // We need to enable/disable notifications from being added to notifier sink when status menu is invisible/visible
    notifierNotificationSink.disableNotificationAdditions(menuVisible);
}

M_REGISTER_VIEW_NEW(StatusAreaWithNotifierView, StatusArea)
