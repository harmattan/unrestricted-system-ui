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
    connect(locale.data(), SIGNAL(settingsChanged()), SLOT(applyCurrentLocale()));
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

    applyCurrentLocale();

    QList<const char *> modifications;
    modifications << ClockModel::Time;
    updateData(modifications);
}

void ClockView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (strcmp(member, ClockModel::Time) == 0) {
            updateLabel();
        }
    }
}

void ClockView::applyCurrentLocale()
{
    MLocale::TimeFormat24h currentFormat = locale->timeFormat24h();
    if (currentFormat == MLocale::LocaleDefaultTimeFormat24h) {
        currentFormat = locale->defaultTimeFormat24h();
    }
    if (currentFormat == MLocale::TwentyFourHourTimeFormat24h) {
        style().setModeDefault();
    } else {
        style().setModeTwelveHour();
    }
    updateLabel();
}

void ClockView::updateLabel()
{
    QDateTime time = model()->time();
    QString text = locale->formatDateTime(time, style()->timeFormat());

    if (text != previousLabel) {
        // Only update the label if the text has changed
        label->setText(text);
        previousLabel = text;
    }
}

M_REGISTER_VIEW_NEW(ClockView, Clock)
