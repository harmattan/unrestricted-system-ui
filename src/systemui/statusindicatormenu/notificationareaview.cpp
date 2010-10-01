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
#include "widgetnotificationsink.h"
#include <MViewCreator>
#include <MBanner>
#include <MButton>
#include <QGraphicsLinearLayout>

NotificationAreaView::NotificationAreaView(NotificationArea *controller) :
    MWidgetView(controller),
    bannerLayout(new QGraphicsLinearLayout(Qt::Vertical)),
    clearButtonLayout(new QGraphicsLinearLayout(Qt::Horizontal)),
    clearButton(NULL),
    andMoreBanner(NULL)
{
    // Set up the main layout
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Vertical);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addItem(bannerLayout);
    mainLayout->addItem(clearButtonLayout);
    controller->setLayout(mainLayout);

    // Set up the banner layout
    bannerLayout->setContentsMargins(0, 0, 0, 0);
    bannerLayout->setSpacing(0);

    // Put a clear button into the clear button layout
    clearButtonLayout->setContentsMargins(0, 0, 0, 0);
    clearButtonLayout->setSpacing(0);
    //% "Clear"
    clearButton = new MButton(qtTrId("qtn_noti_clear"));
    clearButton->setObjectName("NotificationAreaClearButton");
    connect(clearButton, SIGNAL(clicked()), controller, SLOT(removeAllRemovableBanners()));
    clearButtonLayout->addStretch();
    clearButtonLayout->addItem(clearButton);
    clearButtonLayout->addStretch();
}

NotificationAreaView::~NotificationAreaView()
{
    // Clear the banner layout
    while (bannerLayout->count() > 0) {
        bannerLayout->removeAt(0);
    }

    delete clearButtonLayout;
    delete andMoreBanner;
}

void NotificationAreaView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == NotificationAreaModel::Banners) {
            updateLayout();
        }
    }
}

void NotificationAreaView::applyStyle()
{
    MWidgetView::applyStyle();

    // Update the layout since "clear all" button visibility or the maximum amount of banners may have changed
    updateLayout();
}

void NotificationAreaView::updateLayout()
{
    // Remove all banners from the banner layout (do not destroy them)
    while (bannerLayout->count() > 0) {
        bannerLayout->removeAt(0);
    }

    delete andMoreBanner;
    andMoreBanner = NULL;

    // Add up to the maximum amount of banners to the layout
    bool removableBannersExist = false;
    for (int i = 0; (style()->maxBanners() < 0 || i < style()->maxBanners()) && i < model()->banners().count(); i++) {
        MBanner *banner = model()->banners().at(i);
        removableBannersExist |= banner->property(WidgetNotificationSink::USER_REMOVABLE_PROPERTY).toBool();
        bannerLayout->addItem(banner);
    }

    if (style()->maxBanners() >= 0 && model()->banners().count() > style()->maxBanners()) {
        // There are more than maximum number of banners to be added: add a "and more" banner
        andMoreBanner = new MBanner;
        andMoreBanner->setObjectName("EventBanner");
        //% "And more"
        andMoreBanner->setTitle(qtTrId("qtn_noti_more"));
        bannerLayout->addItem(andMoreBanner);
    }

    // If removable banners exist make the clear button visible
    clearButton->setObjectName((removableBannersExist && style()->clearButton()) ? "NotificationAreaClearButtonVisible" : "NotificationAreaClearButton");
}

M_REGISTER_VIEW_NEW(NotificationAreaView, NotificationArea)
