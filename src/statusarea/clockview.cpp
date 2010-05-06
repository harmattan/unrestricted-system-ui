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
#include <MViewCreator>
#include <MLabel>

ClockView::ClockView(Clock *controller) : MWidgetView(controller)
{
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(Qt::Horizontal);
    l->setContentsMargins(0, 0, 0, 0);
    controller->setLayout(l);

    label = new MLabel(NULL);
    label->setObjectName("ClockLabel");
    l->addItem(label);
}

void ClockView::styleUpdated()
{
    updateLabel();
}

void ClockView::setupModel()
{
    MWidgetView::setupModel();
    QList<const char *> modifications;
    modifications << ClockModel::Time;
    modifications << ClockModel::TimeFormat24h;
    updateData(modifications);
}

void ClockView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == ClockModel::Time || member == ClockModel::AlarmSet) {
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
    QString timeFormat;
    QString text;

    if (model()->alarmSet()) {
        timeFormat = style()->timeFormatWithAlarm();
    } else {
        timeFormat = style()->timeFormat();
    }

    if (model()->timeFormat24h()) {
        text = model()->time().toString(timeFormat);
    } else {
        /* There is no way to obtain time in 12-hour format from QTime
         * without the am/pm indicator. We will force an am/pm indicator
         * at the beginning of the string so that the string has time in
         * 12-hour format and will remove this forced indicator. The style
         * string is then free to use 'ap' if they want to show the
         * indicator.
         */
        text = model()->time().toString(QString("ap:") + timeFormat);
        text.remove(0, text.indexOf(':') + 1);
    }

    if (text != previousLabel) {
        // Only update the label if the text has changed
        label->setText(text);
        previousLabel = text;
    }
}

M_REGISTER_VIEW_NEW(ClockView, Clock)
