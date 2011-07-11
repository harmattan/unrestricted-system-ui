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
#include "lowbatterynotifier.h"
#include <QTimer>

static const int DEVICE_ACTIVE_NOTIFICATION_INTERVAL = 5 * 60 * 1000; // 5 mins
static const int DEVICE_INACTIVE_NOTIFICATION_INTERVAL = 30 * 60 * 1000; // 30 mins

LowBatteryNotifier::LowBatteryNotifier(QObject *parent) :
    QObject(parent),
#ifdef HAVE_QMSYSTEM
    displayState(new MeeGo::QmDisplayState(this)),
#endif
    notificationTimer(new QTimer(this)),
    previousNotificationTime(QTime::currentTime()),
    deviceInactive(false),
    touchScreenLockActive(false)
{
#ifdef HAVE_QMSYSTEM
    connect(displayState, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)), this, SLOT(setNotificationInterval()));
#endif
    connect(notificationTimer, SIGNAL(timeout()), this, SLOT(sendLowBatteryAlert()));
    setNotificationInterval();
}

LowBatteryNotifier::~LowBatteryNotifier()
{
}

void LowBatteryNotifier::sendLowBatteryAlert()
{
    emit lowBatteryAlert();

    previousNotificationTime.start();
    notificationTimer->start(deviceInactive ? DEVICE_INACTIVE_NOTIFICATION_INTERVAL : DEVICE_ACTIVE_NOTIFICATION_INTERVAL);
}

void LowBatteryNotifier::setNotificationInterval()
{
    bool deviceCurrentlyInactive = touchScreenLockActive;

#ifdef HAVE_QMSYSTEM
    // Device can be considered inactive only if the touch screen lock is active AND the display is off
    deviceCurrentlyInactive &= displayState->get() == MeeGo::QmDisplayState::Off;
#endif

    if (deviceCurrentlyInactive != deviceInactive) {
        deviceInactive = deviceCurrentlyInactive;

        if (deviceInactive) {
            // Device inactive interval is longer than the active interval, so even if the timer is already running the interval can be increased
            notificationTimer->setInterval(DEVICE_INACTIVE_NOTIFICATION_INTERVAL - previousNotificationTime.elapsed());
        } else {
            // Device active interval is shorter than the inactive interval, so the elapsed time must be checked
            if (previousNotificationTime.elapsed() < DEVICE_ACTIVE_NOTIFICATION_INTERVAL) {
                // Elapsed time has not reached the active interval so just decrease the interval
                notificationTimer->setInterval(DEVICE_ACTIVE_NOTIFICATION_INTERVAL - previousNotificationTime.elapsed());
            } else {
                // Elapsed time has reached the active interval, so send the notification immediately
                sendLowBatteryAlert();
            }
        }
    }
}

void LowBatteryNotifier::setTouchScreenLockActive(bool active)
{
    touchScreenLockActive = active;
    setNotificationInterval();
}
