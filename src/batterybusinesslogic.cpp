/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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
#include "batterybusinesslogic.h"

#include <MLocale>
#include <QTimer>

#include <MNotification>
#include <MFeedback>

/*
 * TODO List
 * -) Connect sounds with notifications
 */

namespace {
    const int   LowBatteryActiveInterval = 30 * 60 * 1000; //30 mins
    const int   LowBatteryInactiveInterval = 2 * 60 * 60 * 1000; //2 hours
    const int   ChargingAnimationRateUSB = 800; // 800 ms
    const int   ChargingAnimationRateWall = 400; // 400 ms
}

//#define DEBUG
#include "debug.h"


/******************************************************************************
 * Methods for the LowBatteryNotifier class.
 */
LowBatteryNotifier::LowBatteryNotifier (
        QObject *parent) :
        QObject (parent),
        m_Timer (new QTimer (this)),
        m_Sleep (false)
{
    SYS_DEBUG ("----------------------------------------------");

    m_ActiveInterval = LowBatteryActiveInterval;
    m_InactiveInterval = LowBatteryInactiveInterval;
    m_Time.start ();

#ifdef HAVE_QMSYSTEM
    m_Display = new Maemo::QmDisplayState;
    m_Sleep = m_Display->get () == Maemo::QmDisplayState::Off;
    connect (m_Display,
            SIGNAL (displayStateChanged (Maemo::QmDisplayState::DisplayState)),
            this,
            SLOT (displayStateChanged (Maemo::QmDisplayState::DisplayState)));
#endif

    connect (m_Timer, SIGNAL (timeout ()),
            this, SLOT (showLowBatteryNotification ()));
}

LowBatteryNotifier::~LowBatteryNotifier ()
{
}


void
LowBatteryNotifier::showLowBatteryNotification ()
{
    SYS_DEBUG ("");

    emit lowBatteryAlert ();

    m_Time.start (); //restart time

#ifdef HAVE_QMSYSTEM
    switch (m_Display->get ()) {
        case Maemo::QmDisplayState::On:
        case Maemo::QmDisplayState::Dimmed:
            m_Sleep = false;
            m_Timer->start (m_ActiveInterval);
            break;

        case Maemo::QmDisplayState::Off:
            m_Sleep = true;
            m_Timer->start (m_InactiveInterval);
            break;

        default:
            // FIXME: what about the other states [Unknown]?
            SYS_WARNING ("Unknown display state");
            break;
    }
#endif
}

#ifdef HAVE_QMSYSTEM
void
LowBatteryNotifier::displayStateChanged (
        Maemo::QmDisplayState::DisplayState state)
{
    SYS_DEBUG ("");

    switch (state) {
        case Maemo::QmDisplayState::On:
            if (!m_Sleep)
                break;
            if (m_Time.elapsed () < m_ActiveInterval)
                m_Timer->setInterval (m_ActiveInterval - m_Time.elapsed ());
            else
                showLowBatteryNotification ();
            m_Sleep = false;
            break;

        case Maemo::QmDisplayState::Dimmed:
            m_Sleep = false;
            break;

        case Maemo::QmDisplayState::Off:
            m_Timer->setInterval (m_InactiveInterval - m_Time.elapsed ());
            m_Sleep = true;
            break;

        default:
            // FIXME: what about the other states [Unknown]?
            break;
    }
}
#endif

/******************************************************************************
 * Methods for the BatteryBusinessLogic class.
 */
BatteryBusinessLogic::BatteryBusinessLogic (
        QObject       *parent) :
    QObject (parent),
    m_LowBatteryNotifier (0),
    m_notification (0)
#ifdef HAVE_QMSYSTEM
   ,m_Battery (new Maemo::QmBattery),
    m_DeviceMode (new Maemo::QmDeviceMode),
    m_Led (new Maemo::QmLED),
    m_ChargerType (Maemo::QmBattery::Unknown)
#endif
{
    SYS_DEBUG ("----------------- start ----------------------");

#ifdef HAVE_QMSYSTEM
    /* connect to QmSystem signals */
    SYS_DEBUG ("Connecting QmSystem signals");
    connect (m_Battery,
             SIGNAL (batteryStateChanged (Maemo::QmBattery::BatteryState)),
             this,
             SLOT (batteryStateChanged (Maemo::QmBattery::BatteryState)));
    connect (m_Battery,
             SIGNAL (chargingStateChanged (Maemo::QmBattery::ChargingState)),
             this,
             SLOT (chargingStateChanged (Maemo::QmBattery::ChargingState)));
    connect (m_Battery,
             SIGNAL (chargerEvent (Maemo::QmBattery::ChargerType)),
             this,
             SLOT (batteryChargerEvent (Maemo::QmBattery::ChargerType)));

    connect (m_DeviceMode,
             SIGNAL (devicePSMStateChanged (Maemo::QmDeviceMode::PSMState)),
             this,
             SLOT (devicePSMStateChanged (Maemo::QmDeviceMode::PSMState)));
#endif

    // Init battery values delayed...
    initBattery ();

    SYS_DEBUG ("------------------ end -----------------------");
}


BatteryBusinessLogic::~BatteryBusinessLogic ()
{
#ifdef HAVE_QMSYSTEM
    delete m_Battery;
    m_Battery = NULL;

    delete m_DeviceMode;
    m_DeviceMode = NULL;

    delete m_Led;
    m_Led = NULL;
#endif
}

// This method should be called also when the device is returned from sleep
// mode
void
BatteryBusinessLogic::initBattery ()
{
#ifdef HAVE_QMSYSTEM
    //init the charging status
    chargingStateChanged (m_Battery->getChargingState ());

    //init the battery level
    batteryStateChanged (m_Battery->getBatteryState ());
#endif
}

/*!
 * This slot is called by the low battery notifier to send the notifications.
 */
void
BatteryBusinessLogic::lowBatteryAlert ()
{
    sendNotification (NotificationLowBattery);
}

#ifdef HAVE_QMSYSTEM
void
BatteryBusinessLogic::chargingStateChanged (
        Maemo::QmBattery::ChargingState state)
{
    SYS_DEBUG ("");

    switch (state) {
        case Maemo::QmBattery::StateCharging:
            SYS_DEBUG ("Charging");

            if (m_Battery->getChargerType () == Maemo::QmBattery::USB_100mA)
            {
                sendNotification (NotificationNoEnoughPower);
            }
            else
            {
                /*
                 * The low battery notifications should not be sent
                 * when the battery is actually charging.
                 */
                if (m_LowBatteryNotifier != 0)
                {
                    delete m_LowBatteryNotifier;
                    m_LowBatteryNotifier = 0;
                }

                sendNotification (NotificationCharging);
            }
            break;

        case Maemo::QmBattery::StateNotCharging:
            SYS_DEBUG ("Not charging");
            utiliseLED (false, QString ("PatternBatteryCharging"));
            break;

        case Maemo::QmBattery::StateChargingFailed:
            SYS_DEBUG ("Charging not started");
            sendNotification (NotificationChargingNotStarted);
            break;
    }
}

void
BatteryBusinessLogic::batteryStateChanged (
        Maemo::QmBattery::BatteryState state)
{
    SYS_DEBUG ("");

    switch (state) {
    case Maemo::QmBattery::StateFull:
        SYS_DEBUG ("QmBattery::StateFull");
        sendNotification (NotificationChargingComplete);
        break;

    case Maemo::QmBattery::StateOK:
        SYS_DEBUG ("QmBattery::StateOK");
        /* no-operation here... */
        break;

    case Maemo::QmBattery::StateLow:
        SYS_DEBUG ("QmBattery::StateLow");
        if (m_Battery->getChargingState () != Maemo::QmBattery::StateCharging) {
            if (m_LowBatteryNotifier == 0) {
                m_LowBatteryNotifier = new LowBatteryNotifier ();
                connect (m_LowBatteryNotifier, SIGNAL(lowBatteryAlert()),
                        this, SLOT(lowBatteryAlert()));
            }

            m_LowBatteryNotifier->showLowBatteryNotification ();
        }
        break;

    case Maemo::QmBattery::StateEmpty:
        SYS_DEBUG ("QmBattery::StateEmpty");
        sendNotification (NotificationRechargeBattery);
        break;

    case Maemo::QmBattery::StateError:
        break;
    }
}

void
BatteryBusinessLogic::batteryChargerEvent (
        Maemo::QmBattery::ChargerType type)
{
    SYS_DEBUG ("");

    switch (type) {
        case Maemo::QmBattery::None:
            SYS_DEBUG ("QmBattery::None");
            /*
             * After the user plugs out the charger from the device, this system
             * banner is displayed to remind the users to unplug charger from
             * the power supply for conserving energy.  Remove charger
             * notification should not be shown in case if USB cable is used for
             * charging the device.
             */
            if (m_ChargerType == Maemo::QmBattery::Wall)
                sendNotification (NotificationRemoveCharger);
            break;

        case Maemo::QmBattery::Wall:
            // Wall charger
            SYS_DEBUG ("QmBattery::Wall");
            break;

        case Maemo::QmBattery::USB_500mA:
            // USB with 500mA output
            SYS_DEBUG ("QmBattery::USB_500mA");
            break;

        case Maemo::QmBattery::USB_100mA:
            // USB with 100mA output
            SYS_DEBUG ("QmBattery::USB_100mA");
            break;

        default: 
            //QmBattery::Unknown
            SYS_WARNING ("Unknown charger state");
            break;
    }

    m_ChargerType = type;
}

void
BatteryBusinessLogic::devicePSMStateChanged (
        Maemo::QmDeviceMode::PSMState PSMState)
{
    if (PSMState == Maemo::QmDeviceMode::PSMStateOff)
    {
        sendNotification (NotificationExitingPSM);
    }
    else if (PSMState == Maemo::QmDeviceMode::PSMStateOn)
    {
        sendNotification (NotificationEnteringPSM);
    }
}
#endif

void
BatteryBusinessLogic::utiliseLED (
        bool           activate,
        const QString &pattern)
{
#ifdef HAVE_QMSYSTEM
    if (activate)
        m_Led->activate (pattern);
    else
        m_Led->deactivate (pattern);
#endif
}

void 
BatteryBusinessLogic::sendNotification (
        BatteryBusinessLogic::NotificationID id)
{
    switch (id) {
        case NotificationCharging:
            SYS_DEBUG ("Notifying NotificationCharging");
            utiliseLED (true, QString ("PatternBatteryCharging"));
            sendNotification (
                    "x-nokia.battery",
                    //% "Charging"
                    qtTrId ("qtn_ener_charging"),
                    chargingImageId ());
            break;

        case NotificationChargingComplete:
            SYS_DEBUG ("Notifying NotificationChargingComplete");
            utiliseLED (true, QString ("PatternBatteryFull"));
            sendNotification (
                    "x-nokia.battery.chargingcomplete",
                    //% "Charging complete"
                    qtTrId ("qtn_ener_charcomp"));
            break;

        case NotificationRemoveCharger:
            SYS_DEBUG ("Notifying NotificationRemoveCharger");
            sendNotification (
                    "x-nokia.battery",
                    //% "Disconnect charger from power supply to save energy"
                    qtTrId ("qtn_ener_remcha"));
            break;

        case NotificationChargingNotStarted:
            utiliseLED (false, QString ("PatternBatteryCharging"));
            sendNotification (
                    "x-nokia.battery.chargingnotstarted",
                    //% "Charging not started. Replace charger."
                    qtTrId ("qtn_ener_repcharger"));
            break;

        case NotificationRechargeBattery:
            sendNotification (
                    "x-nokia.battery.recharge",
                    //% "Recharge battery"
                    qtTrId ("qtn_ener_rebatt"));
            break;

        case NotificationEnteringPSM:
            sendNotification (
                    "x-nokia.battery.enterpsm",
                    //% "Entering power save mode"
                    qtTrId ("qtn_ener_ent_psnote"));
            break;

        case NotificationExitingPSM:
            sendNotification (
                    "x-nokia.battery.exitpsm",
                    //% "Exiting power save mode"
                    qtTrId ("qtn_ener_exit_psnote"));
            break;

        case NotificationLowBattery:
            sendNotification (
                    "x-nokia.battery.lowbattery",
                    //% "Low battery"
                    qtTrId ("qtn_ener_lowbatt"));
            break;

        case NotificationNoEnoughPower:
            sendNotification (
                    "x-nokia.battery.notenoughpower",
                    //% "Not enough power to charge"
                    qtTrId ("qtn_ener_nopowcharge"),
                    "icon-m-energy-management-insufficient-power");
            break;
    }
}

void 
BatteryBusinessLogic::sendNotification (
        const QString &eventType,
        const QString &text,
        const QString &icon)
{
    SYS_DEBUG ("*** text      = %s", SYS_STR(text));
    SYS_DEBUG ("*** eventType = %s", SYS_STR(eventType));
    SYS_DEBUG ("*** icon      = %s", SYS_STR(icon));

    if (m_notification != 0) { 
        m_notification->remove (); 
        delete m_notification; 
        m_notification = 0; 
    }

    /*
     * We send this signal before the actual notification so it will arrive as
     * soon as possible.
     */
    SYS_DEBUG ("Emitting notificationSent (%s, %s, %s)",
            SYS_STR(eventType), SYS_STR(text), SYS_STR(icon));
    emit notificationSent (eventType, text, icon);

    m_notification = new MNotification (eventType, "", text);
    if (!icon.isEmpty())
        m_notification->setImage (icon);
    m_notification->publish ();
}

QString
BatteryBusinessLogic::chargingImageId ()
{
#ifdef HAVE_QMSYSTEM
    int percentage = m_Battery->getRemainingCapacityPct ();

    if (percentage >= 84)
        return QString ("icon-m-energy-management-charging8");
    else if (percentage >= 73)
        return QString ("icon-m-energy-management-charging7");
    else if (percentage >= 62)
        return QString ("icon-m-energy-management-charging6");
    else if (percentage >= 51)
        return QString ("icon-m-energy-management-charging5");
    else if (percentage >= 39)
        return QString ("icon-m-energy-management-charging4");
    else if (percentage >= 28)
        return QString ("icon-m-energy-management-charging3");
    else if (percentage >= 17)
        return QString ("icon-m-energy-management-charging2");
    else if (percentage >= 5)
        return QString ("icon-m-energy-management-charging1");
#endif
    return QString ("icon-m-energy-management-charging-low");
}

