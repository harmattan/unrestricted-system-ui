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
#include <MLocale>

ClockView::ClockView(Clock *controller) :
    MWidgetView(controller),
    locale(new MLocale(this))
{
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(Qt::Horizontal);
    l->setContentsMargins(0, 0, 0, 0);
    controller->setLayout(l);

    label = new MLabel;
    label->setObjectName(controller->objectName() + "Label");
    l->addItem(label);

    // React to system-wide locale changes
    locale->connectSettings();
    connect(locale, SIGNAL(settingsChanged()), this, SLOT(updateLabel()));
}

void ClockView::applyStyle()
{
    MWidgetView::applyStyle();

    label->setAlignment(style()->horizontalAlign());
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
        if (member == ClockModel::Time || member == ClockModel::ShortDisplay) {
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
    QDateTime time = model()->time();
    QString text = locale->formatDateTime(time, style()->timeFormat());

    if (model()->shortDisplay() && style()->shortRemoveAmPmIndicator()) {
        /* Remove the AM/PM indicator in the "short display" mode if the
         * theme dictates so. Also remove possible spaces that were between
         * the time and the AM/PM indicator.
         */
        QString indicator = locale->formatDateTime(time, "%p");
        text.remove(indicator);
        text = text.trimmed();
    }

    if (text != previousLabel) {
        // Only update the label if the text has changed
        label->setText(text);
        previousLabel = text;
    }
}

M_REGISTER_VIEW_NEW(ClockView, Clock)
