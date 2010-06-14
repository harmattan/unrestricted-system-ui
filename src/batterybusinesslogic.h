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

#include <QDebug>

#ifdef UNIT_TEST
#include "ledstub.h"
#include "batterystub.h"
#include "devicemodestub.h"
#include "displaystatestub.h"
#include "gconfstub.h"
#define UT_VIRT virtual
#else
#ifdef HAVE_QMSYSTEM
#include <qmled.h>
#include <qmbattery.h>
#include <qmdevicemode.h>
#include <qmdisplaystate.h>
#endif
#include "systemuigconf.h"
#define UT_VIRT
#endif

#include <QObject>
#include <QStringList>
#include <QTime>

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
    void displayStateChanged (Maemo::QmDisplayState::DisplayState state);
#endif

signals:
    void lowBatteryAlert ();

private:
#ifdef HAVE_QMSYSTEM
    Maemo::QmDisplayState       *m_Display;
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
    BatteryBusinessLogic (SystemUIGConf *systemUIGConf, QObject* parent = 0);
    virtual ~BatteryBusinessLogic ();

    UT_VIRT void setPSMThreshold (const QString &threshold);
    UT_VIRT void togglePSM (bool toggle);
    UT_VIRT void togglePSMAuto (bool toggle);
    int batteryBarValue (int percentage = -1);
    UT_VIRT bool PSMValue ();
    void batteryStatus ();
    QVariant GConfItemValue (SystemUIGConf::GConfKey key);
    QStringList remainingTimeValues ();
    QStringList PSMThresholdValues ();
    UT_VIRT QString PSMThresholdValue ();

    typedef enum {
        NotificationCharging,
	    NotificationChargingComplete,
    	NotificationRemoveCharger,
	    NotificationChargingNotStarted,
    	NotificationRechargeBattery,
	    NotificationEnteringPSM,
    	NotificationExitingPSM,
	    NotificationLowBattery,
    } NotificationID;

signals:
    void batteryCharging (int);
    void batteryNotCharging ();
    void batteryBarValueChanged (int);
    void PSMValueChanged (bool);
    void PSMAutoDisabled ();
    void remainingTimeValuesChanged (QStringList);
    void batteryFullyCharged ();

#ifdef UNIT_TEST
    /*
     * To simplify the test case we use this signal that we can catch. See 
     * NB#171466 for further details.
     */
    void notificationSent (
            QString      text,
            QString      icon);
#endif

public slots:
    void initBattery ();
    void lowBatteryAlert ();

private slots:
    void batteryEnergyLevelChanged (int energyLevel);
#ifdef HAVE_QMSYSTEM
    void batteryStateChanged (Maemo::QmBattery::BatteryState state);
    void chargingStateChanged (Maemo::QmBattery::ChargingState state);
    void batteryChargerEvent (Maemo::QmBattery::ChargerType type);
    void devicePSMStateChanged (Maemo::QmDeviceMode::PSMState PSMState);
#endif
    void utiliseLED (bool activate, const QString &pattern);

private:
    void setPSMState (bool on);
    void initSystemUIGConfKeys ();
#ifdef HAVE_QMSYSTEM
    int animationRate (Maemo::QmBattery::ChargerType type);
#endif
    
    void sendNotification (BatteryBusinessLogic::NotificationID id); 
    void sendNotification (
		    const QString &text,
            const QString &feedback = QString(""),
		    const QString &icon = QString(""));

    SystemUIGConf             *m_SystemUIGConf;
#ifdef HAVE_QMSYSTEM
    Maemo::QmBattery          *m_Battery;
    Maemo::QmDeviceMode       *m_DeviceMode;
    Maemo::QmLED              *m_Led;
#endif
    LowBatteryNotifier        *m_LowBatteryNotifier;
    QStringList                m_PSMThresholds;
    QStringList                m_BarValues;
    MNotification             *m_notification;
#ifdef HAVE_QMSYSTEM
    Maemo::QmBattery::ChargerType m_ChargerType;
#endif

#ifdef UNIT_TEST
    friend class Ut_BatteryBusinessLogic;
    friend class Ft_BatteryBusinessLogic;
#endif

};


#endif

