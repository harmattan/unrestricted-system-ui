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

#include "statusindicatorlabelview.h"
#include "statusindicator.h"
#include <QGraphicsLinearLayout>
#include <MTheme>
#include <MLabel>
#include <MViewCreator>

StatusIndicatorLabelView::StatusIndicatorLabelView(StatusIndicator *controller) :
    MWidgetView(controller),
    controller(controller),
    label(new MLabel)
{
    label->setStyleName("StatusIndicatorLabel");
    label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addItem(label);
    controller->setLayout(layout);
}

StatusIndicatorLabelView::~StatusIndicatorLabelView()
{
}

void StatusIndicatorLabelView::setupModel()
{
    MWidgetView::setupModel();

    updateData(QList<const char *>() << StatusIndicatorModel::Value << StatusIndicatorModel::StylePostfix);
}

void StatusIndicatorLabelView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (strcmp(member, StatusIndicatorModel::Value) == 0) {
            label->setText(model()->value().toString());
        } else if (strcmp(member, StatusIndicatorModel::StylePostfix) == 0) {
            QString prefix;
            if (controller->objectName().contains("Landscape")) {
                prefix = "StatusIndicatorLabelLandscape";
            } else if (controller->objectName().contains("Portrait")) {
                prefix = "StatusIndicatorLabelPortrait";
            } else {
                prefix = "StatusIndicatorLabel";
            }
            label->setStyleName(prefix + model()->stylePostfix());
        }
    }
}

void StatusIndicatorLabelView::applyStyle()
{
    MWidgetView::applyStyle();

    label->setTextElide(style()->textElide());
    label->setWordWrap(style()->wordWrap());
    label->setSizePolicy(QSizePolicy::Preferred, style()->wordWrap() ? QSizePolicy::MinimumExpanding : QSizePolicy::Preferred, QSizePolicy::Label);
}

M_REGISTER_VIEW_NEW(StatusIndicatorLabelView, StatusIndicator)
