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

#include "clock.h"
#include <MLocale>

Clock::Clock(QGraphicsItem *parent) :
    MWidgetController(new ClockModel, parent),
    locale(new MLocale())
{
    // Set the initial 24 hour mode
    updateLocaleSettings();

#ifdef HAVE_QMSYSTEM
    // Be interested in changes to system time
    connect(&qmTime, SIGNAL(timeOrSettingsChanged(Maemo::QmTimeWhatChanged)),
            this, SLOT(updateSettings(Maemo::QmTimeWhatChanged)));
#endif

    // Be interested in changes to 24h mode
    connect(locale.data(), SIGNAL(settingsChanged()),
            this, SLOT(updateLocaleSettings()));
    locale->connectSettings();

    // Configure the timer
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateModelAndSetupTimer()));
    updateModelAndSetupTimer();
}


Clock::~Clock()
{
}

void Clock::updateModelAndSetupTimer()
{
    // Set the current time to the model
    QDateTime currentTime = QDateTime::currentDateTime();
    model()->setTime(currentTime);

    // Calculate the time for the next update
    QDateTime nextUpdateTime = currentTime.addSecs(60);
    QTime time = nextUpdateTime.time();
    time.setHMS(time.hour(), time.minute(), 0);
    nextUpdateTime.setTime(time);

    // Set the time of the next update and start the timer. The extra one second is used for rounding up to make sure that we always hit the next minute, not the current one.
    timer.start((currentTime.secsTo(nextUpdateTime) + 1) * 1000);
}

#ifdef HAVE_QMSYSTEM
void Clock::updateSettings(Maemo::QmTimeWhatChanged whatChanged)
{
    if (whatChanged == Maemo::QmTimeTimeChanged) {
        // Set the time when it was changed (set by the user)
        updateModelAndSetupTimer();
    }
}
#endif

void Clock::updateLocaleSettings()
{
    MLocale::TimeFormat24h currentFormat = locale->timeFormat24h();
    if (currentFormat == MLocale::LocaleDefaultTimeFormat24h) {
        currentFormat = locale->defaultTimeFormat24h();
    }
    model()->setTimeFormat24h(
        MLocale::TwentyFourHourTimeFormat24h == currentFormat);
}

void Clock::setShortDisplay(bool isShort) {
    // Set the short display model field
    model()->setShortDisplay(isShort);
}

void Clock::enterDisplayEvent()
{
    timer.stop();
    updateModelAndSetupTimer();
}

void Clock::exitDisplayEvent()
{
    timer.stop();
}
