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

#include "call.h"
#include "callplugin.h"
#include <QGraphicsLinearLayout>
#include <MBasicListItem>
#include <MImageWidget>
#include <QTimer>

Call::Call(CallPlugin *callPlugin, QGraphicsItem *parent) :
    MWidget(parent),
    plugin(callPlugin),
    item(new MBasicListItem(MBasicListItem::IconWithTitleAndSubtitle))
{
    // Create a layout for the plugin
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Vertical);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    // Set up an item for the call
    item->setTitle("Ongoing call");
    item->setSubtitle("00:52");
    item->imageWidget()->setImage("icon-m-status-menu-settings");
    item->setStyleName("CommonBasicListItemInverted");
    connect(item, SIGNAL(clicked()), this, SLOT(hideItem()));
    mainLayout->addItem(item);

    hideItem();
}

Call::~Call()
{
}

QSizeF Call::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return item->isVisible() ? MWidget::sizeHint(which, constraint) : QSizeF(0, 0);
}

void Call::hideItem()
{
    item->hide();
    updateGeometry();
    QTimer::singleShot(10000, this, SLOT(showItem()));
}

void Call::showItem()
{
    item->show();
    updateGeometry();
}
