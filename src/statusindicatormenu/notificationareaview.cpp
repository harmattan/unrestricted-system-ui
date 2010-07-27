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
#include <MBanner>
#include <MContainer>
#include <QGraphicsLinearLayout>

NotificationAreaView::NotificationAreaView(NotificationArea *controller) :
    MWidgetView(controller)
{
    // Set layout
    layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    controller->setLayout(layout);
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
            while (layout->count() > 0) {
                layout->removeAt(0);
            }

            // Add banners from the model to the layout
            foreach(MBanner * banner, model()->banners()) {
                layout->addItem(banner);
            }
        }
    }
}

M_REGISTER_VIEW_NEW(NotificationAreaView, NotificationArea)
