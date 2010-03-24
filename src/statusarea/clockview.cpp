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

#include "clockview.h"
#include "clock.h"

#include <QTime>
#include <QGraphicsLinearLayout>
#include <DuiViewCreator>
#include <DuiLabel>

ClockView::ClockView(Clock *controller) : DuiWidgetView(controller)
{
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(Qt::Horizontal);
    l->setContentsMargins(0, 0, 0, 0);
    controller->setLayout(l);

    label = new DuiLabel(NULL);
    label->setObjectName("ClockLabel");
    l->addItem(label);
}

void ClockView::styleUpdated()
{
    updateLabel();
}

void ClockView::setupModel()
{
    DuiWidgetView::setupModel();
    QList<const char *> modifications;
    modifications << ClockModel::Time;
    modifications << ClockModel::TimeFormat24h;
    updateData(modifications);
}

void ClockView::updateData(const QList<const char *>& modifications)
{
    DuiWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == ClockModel::Time) {
            updateLabel();
        } else if (member == ClockModel::TimeFormat24h) {
            if (model()->timeFormat24h()) {
                style().setModeDefault();
            } else {
                style().setModeTwelveHour();
            }
            updateLabel();
        }
    }
}

void ClockView::updateLabel()
{
    QString text = model()->time().toString(style()->timeFormat());

    if (text != previousLabel) {
        // Only update the label if the text has changed
        label->setText(text);
        previousLabel = text;
    }
}

DUI_REGISTER_VIEW_NEW(ClockView, Clock)
