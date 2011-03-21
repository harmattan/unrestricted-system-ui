/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary (-ies).
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
#ifndef BATTERYBUSINESSLOGIC_H
#define BATTERYBUSINESSLOGIC_H

#include <QObject>
#include <QTime>

#ifdef HAVE_QMSYSTEM
  #include <qmled.h>
  #include <qmbattery.h>
  #include <qmdevicemode.h>
  #include <qmdisplaystate.h>
#endif

class QTimer;
class MNotification;

/*!
 * Please note that this class will emit the lowBatteryAlert() when the user
 * needed to be alerted about the low battery condition, that is this class will
 * not generate new notifications. The BatteryBusinessLogic will connect to this
 * signal and handle the notifications.
 */
class LowBatteryNotifier : public QObject
{
    Q_OBJECT

public:
    LowBatteryNotifier (QObject* parent = 0);
    virtual ~LowBatteryNotifier ();

public slots:
    void showLowBatteryNotification ();

private slots:
#ifdef HAVE_QMSYSTEM
    void displayStateChanged (MeeGo::QmDisplayState::DisplayState state);
#endif

signals:
    void lowBatteryAlert ();

private:
#ifdef HAVE_QMSYSTEM
    MeeGo::QmDisplayState       *m_Display;
#endif
    QTimer                      *m_Timer;
    QTime                        m_Time;
    bool                         m_Sleep;
    int                          m_ActiveInterval;
    int                          m_InactiveInterval;

#ifdef UNIT_TEST
    friend class Ut_LowBatteryNotifier;
#endif
};

/*!
 * Implements the configuration and state for the battery, the power save mode.
 */
class BatteryBusinessLogic : public QObject
{
    Q_OBJECT

public:
    BatteryBusinessLogic (QObject* parent = 0);
    virtual ~BatteryBusinessLogic ();

    typedef enum {
        NotificationCharging,
        NotificationChargingComplete,
        NotificationRemoveCharger,
        NotificationChargingNotStarted,
        NotificationRechargeBattery,
        NotificationEnteringPSM,
        NotificationExitingPSM,
        NotificationLowBattery,
        NotificationNoEnoughPower,
    } NotificationID;

signals:
    /*
     * To simplify the test case we use this signal that we can catch. See 
     * NB#171466 for further details.
     */
    void notificationSent (
            QString      eventType,
            QString      text,
            QString      icon);

public slots:
    void initBattery ();
    void lowBatteryAlert ();

private slots:
#ifdef HAVE_QMSYSTEM
    void batteryStateChanged (MeeGo::QmBattery::BatteryState state);
    void chargingStateChanged (MeeGo::QmBattery::ChargingState state);
    void batteryChargerEvent (MeeGo::QmBattery::ChargerType type);
    void devicePSMStateChanged (MeeGo::QmDeviceMode::PSMState PSMState);
#endif
    void utiliseLED (bool activate, const QString &pattern);

private:
    void sendNotification (BatteryBusinessLogic::NotificationID id); 
    void sendNotification (
            const QString &eventType, 
            const QString &text,
            const QString &icon = QString(""));
    void removeNotification(const QString &eventType);
    QString chargingImageId ();

    LowBatteryNotifier              *m_LowBatteryNotifier;
    MNotification                   *m_notification;
#ifdef HAVE_QMSYSTEM
    MeeGo::QmBattery                *m_Battery;
    MeeGo::QmDeviceMode             *m_DeviceMode;
    MeeGo::QmLED                    *m_Led;
    MeeGo::QmBattery::ChargerType    m_ChargerType;
#endif

#ifdef UNIT_TEST
    friend class Ut_BatteryBusinessLogic;
    friend class Ft_BatteryBusinessLogic;
#endif
};


#endif

