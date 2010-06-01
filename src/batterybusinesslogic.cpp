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
#include "systemuigconf.h"

#include <MLocale>
#include <QTimer>
#include <QTime>

#include <MNotification>
#include <MFeedback>
#include <MGConfItem>

using namespace Maemo;

/* TODO List

   1) What are correct animation rates when charging with USB / Wall?
   2) If USB 100 mA is used, do we show animation at all? In Fremantle not.
   3) Connect sounds with notifications

*/

namespace {
    //% "Charging"
    const char *ChargingText = QT_TRID_NOOP ("qtn_ener_charging");
    const int   LowBatteryActiveInterval = 30 * 60 * 1000; //30 mins
    const int   LowBatteryInactiveInterval = 2 * 60 * 60 * 1000; //2 hours
    const int   ChargingAnimationRateUSB = 800; // 800 ms
    const int   ChargingAnimationRateWall = 400; // 400 ms
}

#define DEBUG
#include "debug.h"


/******************************************************************************
 * Methods for the LowBatteryNotifier class.
 */
LowBatteryNotifier::LowBatteryNotifier (
        QObject *parent) :
        QObject (parent),
        m_Display (new QmDisplayState ()),
        m_Timer (new QTimer (this)),
        m_notification (0)
{
    SYS_DEBUG ("----------------------------------------------");

    m_Sleep = m_Display->get () == QmDisplayState::Off;
    m_ActiveInterval = LowBatteryActiveInterval;
    m_InactiveInterval = LowBatteryInactiveInterval;
    m_Time.start ();

    connect (m_Display,
            SIGNAL (displayStateChanged (Maemo::QmDisplayState::DisplayState)),
            this,
            SLOT (displayStateChanged (Maemo::QmDisplayState::DisplayState)));

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

    switch (m_Display->get ()) {
        case QmDisplayState::On:
        case QmDisplayState::Dimmed:
            m_Sleep = false;
            m_Timer->start (m_ActiveInterval);
            break;

        case QmDisplayState::Off:
            m_Sleep = true;
            m_Timer->start (m_InactiveInterval);
            break;

        default:
            // FIXME: what about the other states [Unknown]?
            SYS_WARNING ("Unknown display state");
            break;
    }
}

void
LowBatteryNotifier::displayStateChanged (
        Maemo::QmDisplayState::DisplayState state)
{
    SYS_DEBUG ("");

    switch (state) {
        case QmDisplayState::On:
            if (!m_Sleep)
                break;
            if (m_Time.elapsed () < m_ActiveInterval)
                m_Timer->setInterval (m_ActiveInterval - m_Time.elapsed ());
            else
                showLowBatteryNotification ();
            m_Sleep = false;
            break;

        case QmDisplayState::Dimmed:
            m_Sleep = false;
            break;

        case QmDisplayState::Off:
            m_Timer->setInterval (m_InactiveInterval - m_Time.elapsed ());
            m_Sleep = true;
            break;

        default:
            // FIXME: what about the other states [Unknown]?
            break;
    }
}

/******************************************************************************
 * Methods for the BatteryBusinessLogic class.
 */
BatteryBusinessLogic::BatteryBusinessLogic (
        SystemUIGConf *systemUIGConf,
        QObject       *parent) :
    QObject (parent),
    m_SystemUIGConf (systemUIGConf),
    m_Battery (new QmBattery),
    m_DeviceMode (new QmDeviceMode),
    m_Led (new QmLED),
    m_LowBatteryNotifier (0),
    m_notification (0)
{
    SYS_DEBUG ("----------------- start ----------------------");

    /*
     * Initializing the automatic power save mode possible values list. 
     */
    m_PSMThresholds = m_SystemUIGConf->value (
            SystemUIGConf::BatteryPSMThresholdsKey).toStringList();

    if (m_PSMThresholds.isEmpty()) {
        SYS_DEBUG ("m_PSMThresholds is empty: Setting the wired in values!!");

        m_PSMThresholds << "10" << "20" << "30" << "40" << "50";
        m_SystemUIGConf->setValue (
                SystemUIGConf::BatteryPSMThresholdsKey,
                QVariant(m_PSMThresholds));
    }

    /*
     * Initializing a list for the battery icons/bars.
     */
    m_BarValues <<
	    QString ("5") <<
	    QString ("10") <<
	    QString ("15") <<
	    QString ("25") <<
	    QString ("35") <<
	    QString ("50") <<
	    QString ("60") <<
	    QString ("75") <<
	    QString ("85") <<
	    QString ("100");

    /* check if gconfvalues need to initialized */
    initSystemUIGConfKeys ();

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
             SIGNAL (batteryEnergyLevelChanged (int)),
             this,
             SLOT (batteryEnergyLevelChanged (int)));
    connect (m_Battery,
             SIGNAL (chargerEvent (Maemo::QmBattery::ChargerType)),
             this,
             SLOT (batteryChargerEvent (Maemo::QmBattery::ChargerType)));
    connect (m_DeviceMode,
             SIGNAL (devicePSMStateChanged (Maemo::QmDeviceMode::PSMState)),
             this,
             SLOT (devicePSMStateChanged (Maemo::QmDeviceMode::PSMState)));

    // Init battery values delayed...
    QTimer::singleShot (3000, this, SLOT (initBattery ()));

    SYS_DEBUG ("------------------ end -----------------------");
}


BatteryBusinessLogic::~BatteryBusinessLogic ()
{
    SYS_DEBUG ("");
    delete m_Battery;
    m_Battery = NULL;

    delete m_DeviceMode;
    m_DeviceMode = NULL;

    delete m_Led;
    m_Led = NULL;
}

void
BatteryBusinessLogic::initSystemUIGConfKeys ()
{
    // What?! This is not robust!
    if (m_SystemUIGConf->keyCount (SystemUIGConf::Battery) < 2) {
        /*
         * GConf keys have not yet been set.
         */
        m_SystemUIGConf->setValue (
                SystemUIGConf::BatteryPSMAutoKey,
                QVariant (true));
        m_SystemUIGConf->setValue (
                SystemUIGConf::BatteryPSMThresholdKey,
                QVariant (1));
    }
}


// This method should be called also when the device is returned from sleep
// mode
void
BatteryBusinessLogic::initBattery ()
{
    SYS_DEBUG ("");
    //init the charging status
    chargingStateChanged (m_Battery->getChargingState ());

    //init the battery level
    batteryStateChanged (m_Battery->getBatteryState ());
}

/*!
 * This slot is called by the low battery notifier to send the notifications.
 */
void
BatteryBusinessLogic::lowBatteryAlert ()
{
    sendNotification (NotificationLowBattery);
}

int
BatteryBusinessLogic::batteryBarValue (
        int percentage)
{
    SYS_DEBUG ("percentage = %d", percentage);

    // in case of overflow (eg. in sbox) when we get value that greater than 100
    // percent we use 10 percent intentionaly
    if (percentage < 0)
        percentage = 0;
    else if (percentage > 100)
        percentage = 10;

    int index = 0;
    if (percentage >= 84)
        index = m_BarValues.indexOf ("100");
    else if (percentage < 84 && percentage >= 73)
        index = m_BarValues.indexOf ("85");
    else if (percentage < 73 && percentage >= 62)
        index = m_BarValues.indexOf ("75");
    else if (percentage < 62 && percentage >= 51)
        index = m_BarValues.indexOf ("60");
    else if (percentage < 51 && percentage >= 39)
        index = m_BarValues.indexOf ("50");
    else if (percentage < 39 && percentage >= 28)
        index = m_BarValues.indexOf ("35");
    else if (percentage < 28 && percentage >= 17)
        index = m_BarValues.indexOf ("25");
    else if (percentage < 17 && percentage >= 5)
        index = m_BarValues.indexOf ("15");
    else if (percentage < 5 && percentage > 1)
        index = m_BarValues.indexOf ("10");
    else // if (percentage == 0)
        index = m_BarValues.indexOf ("5");

    return index;
}

void
BatteryBusinessLogic::chargingStateChanged (
        QmBattery::ChargingState state)
{
    SYS_DEBUG ("");

    switch (state) {
        case QmBattery::StateCharging:
            SYS_DEBUG ("Charging");
            emit batteryCharging (animationRate (m_Battery->getChargerType ()));

            /*
             * The low battery notifications should not be sent when the battery
             * is actually charging.
             */
            if (m_LowBatteryNotifier != 0) {
                delete m_LowBatteryNotifier;
                m_LowBatteryNotifier = 0;
            }

            sendNotification (NotificationCharging);
            break;

        case QmBattery::StateNotCharging:
            SYS_DEBUG ("Not charging");
            emit batteryNotCharging ();
            utiliseLED (false, QString ("PatternBatteryCharging"));
            break;

        case QmBattery::StateChargingFailed:
            SYS_DEBUG ("Charging not started");
            emit batteryNotCharging ();
            sendNotification (NotificationChargingNotStarted);
            break;

        default:
            SYS_WARNING ("Unhandled state: %d", state);
    }

    emit remainingTimeValuesChanged (remainingTimeValues ());
}

void
BatteryBusinessLogic::batteryStateChanged (
        QmBattery::BatteryState state)
{
    SYS_DEBUG ("");

    switch (state) {
    case QmBattery::StateFull:
        SYS_DEBUG ("QmBattery::StateFull");
        sendNotification (NotificationChargingComplete);
        break;

    case QmBattery::StateOK:
        SYS_DEBUG ("QmBattery::StateOK");
        /* no-operation here... */
        break;

    case QmBattery::StateLow:
        SYS_DEBUG ("QmBattery::StateLow");
        if (m_Battery->getChargingState () != QmBattery::StateCharging) {
            if (m_LowBatteryNotifier == 0) {
                m_LowBatteryNotifier = new LowBatteryNotifier ();
                connect (m_LowBatteryNotifier, SIGNAL(lowBatteryAlert()),
                        this, SLOT(lowBatteryAlert()));
            }

            m_LowBatteryNotifier->showLowBatteryNotification ();
        }
        break;

    case QmBattery::StateEmpty:
        SYS_DEBUG ("QmBattery::StateEmpty");
        sendNotification (NotificationRechargeBattery);
        break;

    case QmBattery::StateError:
        // TODO: Get information somewhere...
        SYS_WARNING ("battery state : error ? what about this state?");
        break;
    }
}

void
BatteryBusinessLogic::batteryEnergyLevelChanged (
        int percentage)
{
    SYS_DEBUG ("*** percentage = %d", percentage);

    emit batteryBarValueChanged (batteryBarValue (percentage));
    emit remainingTimeValuesChanged (remainingTimeValues ());
}

void
BatteryBusinessLogic::batteryChargerEvent (
        Maemo::QmBattery::ChargerType type)
{
    SYS_DEBUG ("");

    switch (type) {
        case QmBattery::None: 
            SYS_DEBUG ("QmBattery::None");
            // No  charger connected
            if (m_Battery->getBatteryState () == QmBattery::StateFull) {
                sendNotification (NotificationRemoveCharger);
	        }
            break;

        case QmBattery::Wall: 
            // Wall charger
            SYS_DEBUG ("QmBattery::Wall");
            emit batteryCharging (animationRate (type));
            break;

        case QmBattery::USB_500mA: 
            // USB with 500mA output
            SYS_DEBUG ("QmBattery::USB_500mA");
            emit batteryCharging (animationRate (type));
            break;

        case QmBattery::USB_100mA: 
            // USB with 100mA output
            SYS_DEBUG ("QmBattery::USB_100mA");
            emit batteryCharging (animationRate (type));
            break;

        default: 
            //QmBattery::Unknown
            SYS_WARNING ("Unknown charger state");
            break;
    }
}

void
BatteryBusinessLogic::devicePSMStateChanged (
        QmDeviceMode::PSMState PSMState)
{
    if (PSMState == QmDeviceMode::PSMStateOff) {
        SYS_DEBUG ("Emitting DBus signal on PSM off");
        emit PSMValueChanged (false);
        sendNotification (NotificationExitingPSM);
    } else if (PSMState == QmDeviceMode::PSMStateOn) {
        SYS_DEBUG ("Emitting DBus signal on PSM on");
        emit PSMValueChanged (true);
        sendNotification (NotificationEnteringPSM);
    }

    emit remainingTimeValuesChanged (remainingTimeValues ());
}

bool
BatteryBusinessLogic::PSMValue ()
{
    SYS_DEBUG ("");
    return m_DeviceMode->getPSMState () == QmDeviceMode::PSMStateOn;
}

/*!
 * \param toggle true to go to power save mode, false to leave it
 *
 * Enters or leaves the power saving mode. This function will also disable the
 * auto power save mode.
 */
void
BatteryBusinessLogic::togglePSM (
        bool toggle)
{
    SYS_DEBUG ("*** toggle = %s", SYS_BOOL(toggle));

    setPSMState (toggle);
}

void
BatteryBusinessLogic::togglePSMAuto (
        bool toggle)
{
    SYS_DEBUG ("*** toggle = %s", SYS_BOOL (toggle));

    m_SystemUIGConf->setValue (
            SystemUIGConf::BatteryPSMAutoKey,
            QVariant (toggle));
}

QStringList
BatteryBusinessLogic::remainingTimeValues ()
{
    QStringList values;

    if (m_Battery->getChargingState () == QmBattery::StateCharging)
        values << qtTrId (ChargingText) << qtTrId (ChargingText);
    else {
        QmDeviceMode::PSMState state = m_DeviceMode->getPSMState ();
        QmBattery::RemainingTimeMode mode;
        switch (state) {
            case QmDeviceMode::PSMStateOn:
                mode = QmBattery::PowersaveMode;
                break;

            case QmDeviceMode::PSMStateOff:
            default:
                mode = QmBattery::NormalMode;
                break;
                // This was the default...
                //return QStringList () << "N/A" << "N/A";
        }
        values << QString ("%1").arg (m_Battery->getRemainingTalkTime (mode) / 60)
            << QString ("%1").arg (m_Battery->getRemainingIdleTime (mode) / 60);
    }

    #if 0
    int n = 0;
    foreach (QString thisone, values) {
        SYS_DEBUG ("values[%d] = %s", n, SYS_STR (values[n]));
        ++n;
    }
    #endif

    return values;
}

void
BatteryBusinessLogic::utiliseLED (
        bool           activate,
        const QString &pattern)
{
    if (activate)
        m_Led->activate (pattern);
    else
        m_Led->deactivate (pattern);
}

void
BatteryBusinessLogic::setPSMThreshold (
        const QString &threshold)
{
    SYS_DEBUG ("*** threshold = '%s'", SYS_STR (threshold));
    m_SystemUIGConf->setValue (
            SystemUIGConf::BatteryPSMThresholdKey,
            QVariant (m_PSMThresholds.indexOf (threshold)));
}

QVariant
BatteryBusinessLogic::GConfItemValue (
        SystemUIGConf::GConfKey key)
{
    return m_SystemUIGConf->value (key);
}

/*
 * Returns a 5 elements long list of possible PSM threshold value list. 
 */
QStringList
BatteryBusinessLogic::PSMThresholdValues ()
{
    return m_PSMThresholds;
}

/*
 * Returns the string value of the threshold stored in the GConf database. The 
 * threshold value is saved as an index for the possible threshold values that
 * is a list of strings.
 */
QString
BatteryBusinessLogic::PSMThresholdValue ()
{
    int index;

    index = m_SystemUIGConf->value (
            SystemUIGConf::BatteryPSMThresholdKey).toInt ();

    if (index < 0 || index >= m_PSMThresholds.size())
        index = 0;
    if (m_PSMThresholds.size() == 0)
        return "";
    
    return m_PSMThresholds[index];
}

void
BatteryBusinessLogic::batteryStatus ()
{
    QmBattery::ChargingState state;

    SYS_DEBUG ("What is the state now?");

    state = m_Battery->getChargingState ();
    SYS_DEBUG ("*** state = %d", (int) state);

    switch (state) {
        case QmBattery::StateCharging:
            emit batteryCharging (animationRate (m_Battery->getChargerType ()));
            break;

        case QmBattery::StateNotCharging:
        case QmBattery::StateChargingFailed:
        default:
            /*
             * default:
             * When state isn't StateCharging i'm clearly sure
             * battery is not really charging...
             */
            emit batteryNotCharging ();
            break;
    }
}

int
BatteryBusinessLogic::animationRate (
        QmBattery::ChargerType type)
{
    int rate = -1;
    SYS_DEBUG ("");

    switch (type) {
        case QmBattery::USB_500mA:
            rate = ChargingAnimationRateUSB;
            break;

        case QmBattery::Wall:
            rate = ChargingAnimationRateWall;
            break;

        default: // QmBattery::USB_100mA etc.
            break;
    }

    return rate;
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
                    //% "Charging"
                    qtTrId ("qtn_ener_charging"),
                    "",
                    "icon-m-energy-management-charging");
            break;

        case NotificationChargingComplete:
            SYS_DEBUG ("Notifying NotificationChargingComplete");
            utiliseLED (true, QString ("PatternBatteryFull"));
            sendNotification (
                    //% "Charging complete"
                    qtTrId ("qtn_ener_charcomp"),
                    "",
                    "icon-m-energy-management-charging-complete");
            break;

        case NotificationRemoveCharger:
            SYS_DEBUG ("Notifying NotificationRemoveCharger");
            sendNotification (
                    //% "Disconnect charger from power supply to save energy"
                    qtTrId ("qtn_ener_remcha"));
            break;

        case NotificationChargingNotStarted:
            utiliseLED (false, QString ("PatternBatteryCharging"));
            sendNotification (
                    //% "Charging not started. Replace charger."
                    qtTrId ("qtn_ener_repcharger"),
                    "IDF_WRONG_CHARGER",
                    "icon-m-energy-management-replace-charger");
            break;

        case NotificationRechargeBattery:
            sendNotification (
                    //% "Recharge battery"
                    qtTrId ("qtn_ener_rebatt"),
                    "IDF_RECHARGE_BATTERY",
                    "icon-m-energy-management-recharge");
            break;
        
        case NotificationEnteringPSM:
            sendNotification (
                    //% "Entering power save mode"
                    qtTrId ("qtn_ener_ent_psnote"),
                    "IDF_INFORMATION_STRONG");
            break;
        
        case NotificationExitingPSM:
            sendNotification (
                    //% "Exiting power save mode"
                    qtTrId ("qtn_ener_exit_psnote"),
                    "IDF_INFORMATION_SOUND");
            break;
        
        case NotificationLowBattery:
            sendNotification (
                    //% "Low battery"
                    qtTrId ("qtn_ener_lowbatt"),
                    "IDF_BATTERY_LOW",
                    "icon-m-energy-management-low-battery");
            break;
    }
}

void 
BatteryBusinessLogic::sendNotification (
        const QString &text,
        const QString &feedback,
        const QString &icon)
{
    SYS_DEBUG ("*** text     = %s", SYS_STR(text));
    SYS_DEBUG ("*** feedback = %s", SYS_STR(feedback));
    SYS_DEBUG ("*** icon     = %s", SYS_STR(icon));

    if (m_notification != 0) { 
        m_notification->remove (); 
        delete m_notification; 
        m_notification = 0; 
    }

   SYS_DEBUG ("+++ Sending MNotification");
#ifdef UNIT_TEST
   /*
    * We send this signal before the actual notification so it will arrive as
    * soon as possible.
    */
   emit notificationSent (text, icon);
#endif   
   m_notification = new MNotification (MNotification::DeviceEvent, text); 
   m_notification->setImage (icon);
   m_notification->publish ();

   if (!feedback.isEmpty()) {
       MFeedback player (feedback);
       player.play();
   }
}

/*!
 * \param on true for entering PSM mode, false to leave it
 */
void
BatteryBusinessLogic::setPSMState (
        bool     on)
{
    if (on) {
        if (!m_DeviceMode->setPSMState (QmDeviceMode::PSMStateOn)) {
            SYS_WARNING ("Entering power save mode failed.");
        }
    } else {
        if (!m_DeviceMode->setPSMState (QmDeviceMode::PSMStateOff)) {
            SYS_WARNING ("Leaving power save mode failed.");
        }
    }
}

