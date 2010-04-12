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

#include "notificationareaview.h"
#include "notificationarea.h"
#include <MViewCreator>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MInfoBanner>
#include <MContainer>
#include <QGraphicsLinearLayout>

NotificationAreaView::NotificationAreaView(NotificationArea *controller) :
    MWidgetView(controller)
{
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Vertical);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    controller->setLayout(mainLayout);
    MContainer *container = new MContainer("Notifications");
    mainLayout->addItem(container);

    // Set layout
    MLayout *layout = new MLayout;
    layoutPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    layoutPolicy->setObjectName("NotificationAreaLayoutPolicy");
    container->centralWidget()->setLayout(layout);

    // MLayout properties can only be set after the layout has a parent
    layout->setContentsMargins(0, 0, 0, 0);
}

NotificationAreaView::~NotificationAreaView()
{
}

void NotificationAreaView::updateData(const QList<const char *>& modifications)
{
    const char *member;
    foreach(member, modifications) {
        if (member == NotificationAreaModel::Banners) {
            // Remove all banners from the layoutpolicy (do not destroy them)
            while (layoutPolicy->count() > 0) {
                layoutPolicy->removeAt(0);
            }

            // Add banners from the model to the layout
            foreach(MInfoBanner * banner, model()->banners()) {
                layoutPolicy->addItem(banner);
            }
        }
    }
}

M_REGISTER_VIEW_NEW(NotificationAreaView, NotificationArea)
