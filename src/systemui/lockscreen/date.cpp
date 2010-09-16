/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouchhome-nokia.
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

#include "date.h"
#include <MLabel>
#include <MLocale>
#include <QGraphicsLinearLayout>
#include <QDateTime>

Date::Date(QGraphicsItem *parent) : MWidget(parent),
    locale(MLocale::createSystemMLocale())
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);

    // Create the labels
    dayOfWeekLabel = new MLabel;
    dayOfWeekLabel->setObjectName("EventsDayLabel");
    dateLabel = new MLabel;
    dateLabel->setObjectName("EventsDateLabel");
    layout->addItem(dayOfWeekLabel);
    layout->addItem(dateLabel);

#ifdef HAVE_QMSYSTEM
    // Listen for changes in time settings
    connect(&qmTime, SIGNAL(timeOrSettingsChanged(Maemo::QmTimeWhatChanged)),
            this, SLOT(updateSettings(Maemo::QmTimeWhatChanged)));
#endif

    // Configure the timer
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateDate()));
    updateDate();
}

Date::~Date()
{
}

void Date::setWeekDayFormat(const QString &weekDayFormat)
{
    this->weekDayFormat = weekDayFormat;
    updateDate();
}

void Date::setDateFormat(const QString &dateFormat)
{
    this->dateFormat = dateFormat;
    updateDate();
}

void Date::updateDate()
{
    // Set current time in the labels
    QDateTime currentTime = QDateTime::currentDateTime();
    dayOfWeekLabel->setText(locale->formatDateTime(currentTime, weekDayFormat));
    dateLabel->setText(locale->formatDateTime(currentTime, dateFormat));

    // Calculate the time for the next update
    QDateTime nextUpdateTime = currentTime.addDays(1);
    nextUpdateTime.setTime(QTime());

    // Set the time of the next update and start the timer. The extra one second is used for rounding up to make sure that we always hit the next day, not the current one.
    timer.start((currentTime.secsTo(nextUpdateTime) + 1) * 1000);
}

#ifdef HAVE_QMSYSTEM
void Date::updateSettings(Maemo::QmTimeWhatChanged whatChanged)
{
    if(whatChanged == Maemo::QmTimeTimeChanged) {
        updateDate();
    }
}
#endif
