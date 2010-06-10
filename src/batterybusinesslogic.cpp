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

#undef DEBUG
#include "debug.h"


/******************************************************************************
 * Methods for the LowBatteryNotifier class.
 */
LowBatteryNotifier::LowBatteryNotifier (
        QObject *parent) :
        QObject (parent),
#ifdef HAVE_QMSYSTEM
        m_Display (new Maemo::QmDisplayState ()),
#endif
        m_Timer (new QTimer (this)),
        m_Sleep (false)
{
    SYS_DEBUG ("----------------------------------------------");

#ifdef HAVE_QMSYSTEM
    m_Sleep = m_Display->get () == Maemo::QmDisplayState::Off;
#endif
    m_ActiveInterval = LowBatteryActiveInterval;
    m_InactiveInterval = LowBatteryInactiveInterval;
    m_Time.start ();

#ifdef HAVE_QMSYSTEM
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
        SystemUIGConf *systemUIGConf,
        QObject       *parent) :
    QObject (parent),
    m_SystemUIGConf (systemUIGConf),
#ifdef HAVE_QMSYSTEM
    m_Battery (new Maemo::QmBattery),
    m_DeviceMode (new Maemo::QmDeviceMode),
    m_Led (new Maemo::QmLED),
#endif
    m_LowBatteryNotifier (0),
    m_notification (0)
#ifdef HAVE_QMSYSTEM
    , m_ChargerType (Maemo::QmBattery::Unknown)
#endif
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
#endif

    // Init battery values delayed...
    QTimer::singleShot (3000, this, SLOT (initBattery ()));

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

/*!
 * \param percentage The energy level percentage or -1 to ask the backend.
 *
 * Returns the bar value (the value that used as an icon index representing for
 * the battery energy level) for the given percentage level. If the argument is
 * -1 this method will ask the QmBattery for the current energy level.
 *
 */
int
BatteryBusinessLogic::batteryBarValue (
        int percentage)
{
    SYS_DEBUG ("percentage = %d", percentage);

#ifdef HAVE_QMSYSTEM
    if (percentage == -1) {
        percentage = m_Battery->getBatteryEnergyLevel ();
        SYS_DEBUG ("getBatteryEnergyLevel returned %d", percentage);
    }
#endif

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

#ifdef HAVE_QMSYSTEM
void
BatteryBusinessLogic::chargingStateChanged (
        Maemo::QmBattery::ChargingState state)
{
    SYS_DEBUG ("");

    switch (state) {
        case Maemo::QmBattery::StateCharging:
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

        case Maemo::QmBattery::StateNotCharging:
            SYS_DEBUG ("Not charging");
            emit batteryNotCharging ();
            utiliseLED (false, QString ("PatternBatteryCharging"));
            break;

        case Maemo::QmBattery::StateChargingFailed:
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
        // TODO: Get information somewhere...
        SYS_WARNING ("battery state : error ? what about this state?");
        break;
    }
}
#endif

void
BatteryBusinessLogic::batteryEnergyLevelChanged (
        int percentage)
{
    SYS_DEBUG ("*** percentage = %d", percentage);

    emit batteryBarValueChanged (batteryBarValue (percentage));
    emit remainingTimeValuesChanged (remainingTimeValues ());
}

#ifdef HAVE_QMSYSTEM
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
            emit batteryCharging (animationRate (type));
            break;

        case Maemo::QmBattery::USB_500mA:
            // USB with 500mA output
            SYS_DEBUG ("QmBattery::USB_500mA");
            emit batteryCharging (animationRate (type));
            break;

        case Maemo::QmBattery::USB_100mA:
            // USB with 100mA output
            SYS_DEBUG ("QmBattery::USB_100mA");
            emit batteryCharging (animationRate (type));
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
    if (PSMState == Maemo::QmDeviceMode::PSMStateOff) {
        SYS_DEBUG ("Emitting DBus signal on PSM off");
        emit PSMValueChanged (false);
        sendNotification (NotificationExitingPSM);
    } else if (PSMState == Maemo::QmDeviceMode::PSMStateOn) {
        SYS_DEBUG ("Emitting DBus signal on PSM on");
        emit PSMValueChanged (true);
        sendNotification (NotificationEnteringPSM);
    }

    emit remainingTimeValuesChanged (remainingTimeValues ());
}
#endif

bool
BatteryBusinessLogic::PSMValue ()
{
    SYS_DEBUG ("");
#ifdef HAVE_QMSYSTEM
    return m_DeviceMode->getPSMState () == Maemo::QmDeviceMode::PSMStateOn;
#else
    return false;
#endif
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

#ifdef HAVE_QMSYSTEM
    if (m_Battery->getChargingState () == Maemo::QmBattery::StateCharging)
        values << qtTrId (ChargingText) << qtTrId (ChargingText);
    else {
        Maemo::QmDeviceMode::PSMState state = m_DeviceMode->getPSMState ();
        Maemo::QmBattery::RemainingTimeMode mode;
        switch (state) {
            case Maemo::QmDeviceMode::PSMStateOn:
                mode = Maemo::QmBattery::PowersaveMode;
                break;

            case Maemo::QmDeviceMode::PSMStateOff:
            default:
                mode = Maemo::QmBattery::NormalMode;
                break;
                // This was the default...
                //return QStringList () << "N/A" << "N/A";
        }
        values << QString ("%1").arg (m_Battery->getRemainingTalkTime (mode) / 60)
            << QString ("%1").arg (m_Battery->getRemainingIdleTime (mode) / 60);
    }
#endif

    return values;
}

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
#ifdef HAVE_QMSYSTEM
    Maemo::QmBattery::ChargingState state;

    SYS_DEBUG ("What is the state now?");

    state = m_Battery->getChargingState ();
    SYS_DEBUG ("*** state = %d", (int) state);

    switch (state) {
        case Maemo::QmBattery::StateCharging:
            emit batteryCharging (animationRate (m_Battery->getChargerType ()));
            break;

        case Maemo::QmBattery::StateNotCharging:
        case Maemo::QmBattery::StateChargingFailed:
        default:
            /*
             * default:
             * When state isn't StateCharging i'm clearly sure
             * battery is not really charging...
             */
            emit batteryNotCharging ();
            break;
    }
#endif
}

#ifdef HAVE_QMSYSTEM
int
BatteryBusinessLogic::animationRate (
        Maemo::QmBattery::ChargerType type)
{
    int rate = -1;
    SYS_DEBUG ("");

    switch (type) {
        case Maemo::QmBattery::USB_500mA:
            rate = ChargingAnimationRateUSB;
            break;

        case Maemo::QmBattery::Wall:
            rate = ChargingAnimationRateWall;
            break;

        default: // QmBattery::USB_100mA etc.
            break;
    }

    return rate;
}
#endif

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
#ifdef HAVE_QMSYSTEM
    if (on) {
        if (!m_DeviceMode->setPSMState (Maemo::QmDeviceMode::PSMStateOn)) {
            SYS_WARNING ("Entering power save mode failed.");
        }
    } else {
        if (!m_DeviceMode->setPSMState (Maemo::QmDeviceMode::PSMStateOff)) {
            SYS_WARNING ("Leaving power save mode failed.");
        }
    }
#endif
}

