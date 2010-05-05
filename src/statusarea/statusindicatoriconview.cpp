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

#include "statusindicatoriconview.h"
#include "statusindicator.h"
#include <MViewCreator>
#include <MTheme>
#include <QGraphicsLinearLayout>
#include <QDebug>

StatusIndicatorIconView::StatusIndicatorIconView(StatusIndicator *controller) :
    MWidgetView(controller),
    controller(controller),
    pixmap(NULL)
{
}

StatusIndicatorIconView::~StatusIndicatorIconView()
{
    if (pixmap != NULL) {
        MTheme::releasePixmap(pixmap);
    }
}

void StatusIndicatorIconView::setupModel()
{
    MWidgetView::setupModel();

    QList<const char *> modifications;
    modifications << StatusIndicatorModel::Value;
    updateData(modifications);
}

void StatusIndicatorIconView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == StatusIndicatorModel::Value) {
            if (pixmap != NULL) {
                // Release the old pixmap
                MTheme::releasePixmap(pixmap);
                pixmap = NULL;
            }

            QString iconID = model()->value().toString();
            if (!iconID.isEmpty()) {
                // Get the pixmap from the theme
                style().setModeIcon();
                pixmap = MTheme::pixmap(iconID, QSize(style()->preferredSize().width(), style()->preferredSize().height()));
            } else {
                style().setModeDefault();
            }

            // Redraw
            controller->updateGeometry();
            controller->update();
        }
    }
}

void StatusIndicatorIconView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *) const
{
    if (pixmap != NULL) {
        painter->drawPixmap(QPointF(0, 0), *pixmap);
    }
}

M_REGISTER_VIEW_NEW(StatusIndicatorIconView, StatusIndicator)
