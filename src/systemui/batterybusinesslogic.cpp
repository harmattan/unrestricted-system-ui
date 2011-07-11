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
#include "lowbatterynotifier.h"
#include <QTimer>
#include <MNotification>

BatteryBusinessLogic::BatteryBusinessLogic(QObject *parent) :
    QObject(parent), m_LowBatteryNotifier(0), m_notification(0), touchScreenLockActive(false)
#ifdef HAVE_QMSYSTEM
    ,m_Battery(new MeeGo::QmBattery),
    m_DeviceMode(new MeeGo::QmDeviceMode),
    m_Led(new MeeGo::QmLED),
    m_ChargerType(MeeGo::QmBattery::Unknown)
#endif
{
#ifdef HAVE_QMSYSTEM
    connect(m_Battery, SIGNAL(batteryStateChanged(MeeGo::QmBattery::BatteryState)), this, SLOT(batteryStateChanged(MeeGo::QmBattery::BatteryState)));
    connect(m_Battery, SIGNAL(chargingStateChanged(MeeGo::QmBattery::ChargingState)), this, SLOT(chargingStateChanged(MeeGo::QmBattery::ChargingState)));
    connect(m_Battery, SIGNAL(chargerEvent(MeeGo::QmBattery::ChargerType)), this, SLOT(batteryChargerEvent(MeeGo::QmBattery::ChargerType)));

    connect(m_DeviceMode, SIGNAL(devicePSMStateChanged(MeeGo::QmDeviceMode::PSMState)), this, SLOT(devicePSMStateChanged(MeeGo::QmDeviceMode::PSMState)));
#endif

    // Init battery values delayed...
    initBattery();

    notificationTimer.setInterval(5000);
    notificationTimer.setSingleShot(true);
}

BatteryBusinessLogic::~BatteryBusinessLogic()
{
#ifdef HAVE_QMSYSTEM
    delete m_Battery;
    delete m_DeviceMode;
    delete m_Led;
#endif
}

// This method should be called also when the device is returned from sleep mode
void BatteryBusinessLogic::initBattery()
{
#ifdef HAVE_QMSYSTEM
    //init the charging status
    chargingStateChanged(m_Battery->getChargingState());

    //init the battery level
    batteryStateChanged(m_Battery->getBatteryState());
#endif
}

/*!
 * This slot is called by the low battery notifier to send the notifications.
 */
void BatteryBusinessLogic::lowBatteryAlert()
{
    sendNotification(NotificationLowBattery);
}

#ifdef HAVE_QMSYSTEM
void BatteryBusinessLogic::chargingStateChanged(MeeGo::QmBattery::ChargingState state)
{
    switch(state) {
    case MeeGo::QmBattery::StateCharging:
        if (m_Battery->getChargerType() == MeeGo::QmBattery::USB_100mA) {
            sendNotification(NotificationNoEnoughPower);
        } else {
            // The low battery notifications should not be sent when the battery is actually charging.
            if (m_LowBatteryNotifier != 0) {
                delete m_LowBatteryNotifier;
                m_LowBatteryNotifier = 0;
            }

            sendNotification(NotificationCharging);
        }
        break;

    case MeeGo::QmBattery::StateNotCharging:
        removeNotification("x-nokia.battery");
        utiliseLED(false, QString("PatternBatteryCharging"));
        break;

    case MeeGo::QmBattery::StateChargingFailed:
        sendNotification(NotificationChargingNotStarted);
        break;
    }
}

void BatteryBusinessLogic::batteryStateChanged(MeeGo::QmBattery::BatteryState state)
{
    switch(state) {
    case MeeGo::QmBattery::StateFull:
        sendNotification(NotificationChargingComplete);
        break;

    case MeeGo::QmBattery::StateOK:
        /* no-operation here... */
        break;

    case MeeGo::QmBattery::StateLow:
        if (m_Battery->getChargingState() != MeeGo::QmBattery::StateCharging) {
            if (m_LowBatteryNotifier == 0) {
                m_LowBatteryNotifier = new LowBatteryNotifier();
                connect(m_LowBatteryNotifier, SIGNAL(lowBatteryAlert()), this, SLOT(lowBatteryAlert()));
                m_LowBatteryNotifier->setTouchScreenLockActive(touchScreenLockActive);
            }

            m_LowBatteryNotifier->sendLowBatteryAlert();
        }
        break;

    case MeeGo::QmBattery::StateEmpty:
        sendNotification(NotificationRechargeBattery);
        break;

    case MeeGo::QmBattery::StateError:
        break;
    }
}

void BatteryBusinessLogic::batteryChargerEvent(MeeGo::QmBattery::ChargerType type)
{
    switch(type) {
    case MeeGo::QmBattery::None:
        /*
         * After the user plugs out the charger from the device, this system
         * banner is displayed to remind the users to unplug charger from
         * the power supply for conserving energy.  Remove charger
         * notification should not be shown in case if USB cable is used for
         * charging the device.
         */
        if (m_ChargerType == MeeGo::QmBattery::Wall) {
            removeNotification("x-nokia.battery");
            sendNotification(NotificationRemoveCharger);
        }
        break;

    case MeeGo::QmBattery::Wall:
        // Wall charger
        break;

    case MeeGo::QmBattery::USB_500mA:
        // USB with 500mA output
        break;

    case MeeGo::QmBattery::USB_100mA:
        // USB with 100mA output
        break;

    default:
        break;
    }

    m_ChargerType = type;
}

void BatteryBusinessLogic::devicePSMStateChanged(MeeGo::QmDeviceMode::PSMState PSMState)
{
    if (PSMState == MeeGo::QmDeviceMode::PSMStateOff) {
        sendNotification(NotificationExitingPSM);
    } else if (PSMState == MeeGo::QmDeviceMode::PSMStateOn) {
        sendNotification(NotificationEnteringPSM);
    }
}
#endif

void BatteryBusinessLogic::utiliseLED(bool activate, const QString &pattern)
{
#ifdef HAVE_QMSYSTEM
    if (activate) {
        m_Led->activate(pattern);
    } else {
        m_Led->deactivate(pattern);
    }
#endif
}

void BatteryBusinessLogic::sendNotification(BatteryBusinessLogic::NotificationID id)
{
    switch(id) {
    case NotificationCharging:
        utiliseLED(true, QString("PatternBatteryCharging"));
        sendNotification("x-nokia.battery",
                //% "Charging"
                qtTrId("qtn_ener_charging"), chargingImageId());
        break;

    case NotificationChargingComplete:
        utiliseLED(true, QString("PatternBatteryFull"));
        sendNotification("x-nokia.battery.chargingcomplete",
                //% "Charging complete"
                qtTrId("qtn_ener_charcomp"));
        break;

    case NotificationRemoveCharger:
        sendNotification("x-nokia.battery.removecharger",
                //% "Disconnect charger from power supply to save energy"
                qtTrId("qtn_ener_remcha"));
        break;

    case NotificationChargingNotStarted:
        utiliseLED(false, QString("PatternBatteryCharging"));
        sendNotification("x-nokia.battery.chargingnotstarted",
                //% "Charging not started. Replace charger."
                qtTrId("qtn_ener_repcharger"));
        break;

    case NotificationRechargeBattery:
        sendNotification("x-nokia.battery.recharge",
                //% "Recharge battery"
                qtTrId("qtn_ener_rebatt"));
        break;

    case NotificationEnteringPSM:
        sendNotification("x-nokia.battery.enterpsm",
                //% "Entering power save mode"
                qtTrId("qtn_ener_ent_psnote"), chargingImageId());
        break;

    case NotificationExitingPSM:
        sendNotification("x-nokia.battery.exitpsm",
                //% "Exiting power save mode"
                qtTrId("qtn_ener_exit_psnote"), chargingImageId());
        break;

    case NotificationLowBattery:
        sendNotification("x-nokia.battery.lowbattery",
                //% "Low battery"
                qtTrId("qtn_ener_lowbatt"));
        break;

    case NotificationNoEnoughPower:
        sendNotification("x-nokia.battery.notenoughpower",
                //% "Not enough power to charge"
                qtTrId("qtn_ener_nopowcharge"), "icon-m-energy-management-insufficient-power");
        break;
    }
}

void BatteryBusinessLogic::sendNotification(const QString &eventType, const QString &text, const QString &icon)
{
    if (m_notification != 0) {
        delete m_notification;
        m_notification = 0;
    }

    /*
     * We send this signal before the actual notification so it will arrive as
     * soon as possible.
     */
    emit notificationSent(eventType, text, icon);

    m_notification = new MNotification(eventType, "", text);
    if (!icon.isEmpty()) {
        m_notification->setImage(icon);
    }
    m_notification->publish();
    notificationTimer.start();
}

void BatteryBusinessLogic::removeNotification(const QString &eventType)
{
    if (m_notification != 0 && m_notification->eventType() == eventType && notificationTimer.isActive()) {
        m_notification->remove();
        delete m_notification;
        m_notification = 0;
        notificationTimer.stop();
    }
}

QString BatteryBusinessLogic::chargingImageId()
{
#ifdef HAVE_QMSYSTEM
    int percentage = m_Battery->getRemainingCapacityPct();

    if (percentage >= 84) {
        return QString("icon-m-energy-management-charging8");
    } else if (percentage >= 73) {
        return QString("icon-m-energy-management-charging7");
    } else if (percentage >= 62) {
        return QString("icon-m-energy-management-charging6");
    } else if (percentage >= 51) {
        return QString("icon-m-energy-management-charging5");
    } else if (percentage >= 39) {
        return QString("icon-m-energy-management-charging4");
    } else if (percentage >= 28) {
        return QString("icon-m-energy-management-charging3");
    } else if (percentage >= 17) {
        return QString("icon-m-energy-management-charging2");
    } else if (percentage >= 5) {
        return QString("icon-m-energy-management-charging1");
    }
#endif
    return QString("icon-m-energy-management-charging-low");
}

void BatteryBusinessLogic::setTouchScreenLockActive(bool active)
{
    touchScreenLockActive = active;
    if (m_LowBatteryNotifier != NULL) {
        m_LowBatteryNotifier->setTouchScreenLockActive(active);
    }
}
