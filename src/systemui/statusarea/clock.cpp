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
    MWidgetController(new ClockModel, parent)
{
#ifdef HAVE_QMSYSTEM
    // Be interested in changes to system time
    connect(&qmTime, SIGNAL(timeOrSettingsChanged(MeeGo::QmTime::WhatChanged)),
            this, SLOT(updateSettings(MeeGo::QmTime::WhatChanged)));
#endif

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
void Clock::updateSettings(MeeGo::QmTime::WhatChanged whatChanged)
{
    if (whatChanged == MeeGo::QmTime::TimeChanged) {
        // Set the time when it was changed (set by the user)
        updateModelAndSetupTimer();
    }
}
#endif

void Clock::enterDisplayEvent()
{
    timer.stop();
    updateModelAndSetupTimer();
}

void Clock::exitDisplayEvent()
{
    timer.stop();
}
