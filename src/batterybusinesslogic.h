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
/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino=" (0,W2s,i2s,t0,l1,:0" */
#ifndef BATTERYBUSINESSLOGIC_H
#define BATTERYBUSINESSLOGIC_H

#include <QDebug>

#ifdef UNIT_TEST
#include "ledstub.h"
#include "batterystub.h"
#include "devicemodestub.h"
#include "displaystatestub.h"
#include "gconfstub.h"
#else
#include <qmsystem/qmled.h>
#include <qmsystem/qmbattery.h>
#include <qmsystem/qmdevicemode.h>
#include <qmsystem/qmdisplaystate.h>
#include "systemuigconf.h"
#endif

#include <QObject>
#include <QStringList>
#include <QTime>

class QTimer;
class MNotification;

class LowBatteryNotifier : public QObject
{
    Q_OBJECT

public:
    LowBatteryNotifier (QObject* parent = 0);
    virtual ~LowBatteryNotifier ();

public slots:
    void showLowBatteryNotification ();

private slots:
    void displayStateChanged (Maemo::QmDisplayState::DisplayState state);

#ifdef UNIT_TEST
signals:
    void showNotification ();
#endif

private:
    Maemo::QmDisplayState       *m_Display;
    QTimer                      *m_Timer;
    QTime                        m_Time;
    bool                         m_Sleep;
    int                          m_ActiveInterval;
    int                          m_InactiveInterval;
    MNotification               *m_notification;

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

    void setPSMThreshold (const QString &threshold);
    void togglePSM (bool toggle);
    void togglePSMAuto (bool toggle);
    int batteryBarValue (int percentage = -1);
    bool PSMValue ();
    void batteryStatus ();
    QVariant GConfItemValue (SystemUIGConf::GConfKey key);
    QStringList remainingTimeValues ();
    QStringList PSMThresholdValues ();
    QString PSMThresholdValue ();

signals:
    void batteryCharging (int);
    void batteryNotCharging ();
    void batteryBarValueChanged (int);
    void PSMValueChanged (bool);
    void PSMAutoDisabled ();
    void remainingTimeValuesChanged (QStringList);
    void batteryFullyCharged ();

#ifdef UNIT_TEST
    void showNotification ();
#endif


public slots:
    void initBattery ();

private:
    SystemUIGConf             *m_SystemUIGConf;
    Maemo::QmBattery          *m_Battery;
    Maemo::QmDeviceMode       *m_DeviceMode;
    Maemo::QmLED              *m_Led;
    LowBatteryNotifier        *m_LowBatteryNotifier;
    QStringList                m_PSMThresholds;
    MNotification             *m_notification;

    void setPSMState (bool on);
    void initSystemUIGConfKeys ();
    void checkPSMThreshold ();
    int animationRate (Maemo::QmBattery::ChargerType type);

private slots:
    void batteryLevelChanged (Maemo::QmBattery::Level level);
    void batteryEnergyLevelChanged (int energyLevel);
    void batteryStatusChanged (Maemo::QmBattery::State state);
    void batteryChargerEvent (Maemo::QmBattery::ChargerType type);
    void devicePSMStateChanged (Maemo::QmDeviceMode::PSMState PSMState);
    void utiliseLED (bool activate, const QString &pattern);

#ifdef UNIT_TEST
    friend class Ut_BatteryBusinessLogic;
#endif

};


#endif

