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

#include <MApplication>
#include "statusindicator.h"
#include "statusindicatormodel.h"
#include "inputmethodstatusindicatoradaptor.h"
#include "applicationcontext.h"

StatusIndicator::StatusIndicator(MWidget *parent) :
    MWidgetController(new StatusIndicatorModel, parent),
    animateIfPossible(false),
    modelUpdatesEnabled(true),
    currentValue(QVariant())
{
}

StatusIndicator::~StatusIndicator()
{
}

void StatusIndicator::updateGeometry()
{
    MWidgetController::updateGeometry();
}

void StatusIndicator::setValue(QVariant v)
{
    currentValue = v;
    if (modelUpdatesEnabled) {
        model()->setValue(v);
    }
}

QVariant StatusIndicator::value() const
{
    return currentValue;
}

void StatusIndicator::enterDisplayEvent()
{
    setModelUpdatesEnabled(true);
}

void StatusIndicator::exitDisplayEvent()
{
    setModelUpdatesEnabled(false);
}

void StatusIndicator::setModelUpdatesEnabled(bool modelUpdatesEnabled)
{
    this->modelUpdatesEnabled = modelUpdatesEnabled;
    if (modelUpdatesEnabled) {
        model()->setValue(currentValue);
    }
    updateAnimationStatus();
}

void StatusIndicator::updateAnimationStatus()
{
    if (modelUpdatesEnabled) {
        model()->setAnimate(animateIfPossible);
    } else {
        model()->setAnimate(false);
    }
}

PhoneNetworkSignalStrengthStatusIndicator::PhoneNetworkSignalStrengthStatusIndicator(ApplicationContext &context, MWidget *parent) :
    StatusIndicator(parent)
{
    setObjectName(metaObject()->className());

    signalStrength = context.createContextItem("Cellular.SignalStrength");
    connect(signalStrength, SIGNAL(contentsChanged()), this, SLOT(signalStrengthChanged()));

    setValue(0.0);
    setDisplay(false);
}

PhoneNetworkSignalStrengthStatusIndicator::~PhoneNetworkSignalStrengthStatusIndicator()
{
    delete signalStrength;
}

void PhoneNetworkSignalStrengthStatusIndicator::signalStrengthChanged()
{
    setValue(signalStrength->value().toDouble() * 0.01f);
}

void PhoneNetworkSignalStrengthStatusIndicator::setDisplay(bool display)
{
    if(display) {
        setObjectName(metaObject()->className());
    } else {
        setObjectName("");
    }
}


PhoneNetworkTypeStatusIndicator::PhoneNetworkTypeStatusIndicator(ApplicationContext &context, MWidget *parent) :
        StatusIndicator(parent), networkAvailable(false)
{
    cellularTechnology = context.createContextItem("Cellular.Technology");
    connect(cellularTechnology, SIGNAL(contentsChanged()), this, SLOT(setNetworkType()));

    cellularDataTechnology = context.createContextItem("Cellular.DataTechnology");
    connect(cellularDataTechnology, SIGNAL(contentsChanged()), this, SLOT(setNetworkType()));

    cellularRegistrationStatus = context.createContextItem("Cellular.RegistrationStatus");
    connect(cellularRegistrationStatus, SIGNAL(contentsChanged()), this, SLOT(setNetworkType()));

    setNetworkType();
}

PhoneNetworkTypeStatusIndicator::~PhoneNetworkTypeStatusIndicator()
{
    delete cellularTechnology;
    delete cellularDataTechnology;
    delete cellularRegistrationStatus;
}

void PhoneNetworkTypeStatusIndicator::setNetworkType()
{
    QString postFix = "NoNetwork";

    QString technology     = cellularTechnology->value().toString();         // gsm umts
    QString dataTechnology = cellularDataTechnology->value().toString();     // gprs egprs umts hspa
    QString status         = cellularRegistrationStatus->value().toString(); // home roam no-sim offline forbidden

    setValue(0);

    if(status == "no-sim") {
        postFix = "NoSIM";
    } else if(status == "" || status == "offline" || status == "forbidden") {
        postFix = "Offline";
    } else {
        if(technology == "gsm") {
            if(dataTechnology == "") {
                postFix = "2G";
            } else if(dataTechnology == "gprs" || dataTechnology == "egprs") {
                postFix = "25G";
            }
        } else if(technology == "umts") {
            if(dataTechnology == "umts") {
                postFix = "3G";
            } else if(dataTechnology == "hspa") {
                postFix = "35G";
            }
        }
    }

    bool n = !(postFix == "NoNetwork" || postFix == "Offline" || postFix == "NoSIM");
    if(n != networkAvailable) {
        networkAvailable = n;
        emit networkAvailabilityChanged(n);
    }

    setObjectName(metaObject()->className() + postFix);
}

BatteryStatusIndicator::BatteryStatusIndicator(ApplicationContext &context, MWidget *parent) :
    StatusIndicator(parent)
{
    setObjectName(QString(metaObject()->className()) + "Level");

    batteryLevel = context.createContextItem("Battery.ChargePercentage");
    connect(batteryLevel, SIGNAL(contentsChanged()), this, SLOT(batteryLevelChanged()));

    batteryCharging = context.createContextItem("Battery.IsCharging");
    connect(batteryCharging, SIGNAL(contentsChanged()), this, SLOT(batteryChargingChanged()));
}

BatteryStatusIndicator::~BatteryStatusIndicator()
{
    delete batteryLevel;
    delete batteryCharging;
}

void BatteryStatusIndicator::batteryLevelChanged()
{
    if (!batteryCharging->value().toBool()) {
        setValue(batteryLevel->value().toDouble() * 0.01f);
    }
}

void BatteryStatusIndicator::batteryChargingChanged()
{
    if (batteryCharging->value().toBool()) {
        setObjectName(QString(metaObject()->className()) + "Charging");
        animateIfPossible = true;
    } else {
        setObjectName(QString(metaObject()->className()) + "Level");
        animateIfPossible = false;
    }
    updateAnimationStatus();
}

ClockAlarmStatusIndicator::ClockAlarmStatusIndicator(ApplicationContext &context, MWidget *parent) :
    StatusIndicator(parent)
{
    setObjectName(metaObject()->className());

    clockAlarm = context.createContextItem("UserAlarm.Present");
    connect(clockAlarm, SIGNAL(contentsChanged()), this, SLOT(clockAlarmChanged()));
}

ClockAlarmStatusIndicator::~ClockAlarmStatusIndicator()
{
    delete clockAlarm;
}

void ClockAlarmStatusIndicator::clockAlarmChanged()
{
    setValue(clockAlarm->value().toBool() ? 1 : 0);
}

BluetoothStatusIndicator::BluetoothStatusIndicator(ApplicationContext &context, MWidget *parent) :
    StatusIndicator(parent)
{
    setObjectName(metaObject()->className());

    bluetooth = context.createContextItem("Bluetooth.Enabled");
    connect(bluetooth, SIGNAL(contentsChanged()), this, SLOT(bluetoothChanged()));
}

BluetoothStatusIndicator::~BluetoothStatusIndicator()
{
    delete bluetooth;
}

void BluetoothStatusIndicator::bluetoothChanged()
{
    setValue(bluetooth->value().toBool() ? 1 : 0);
}

PresenceStatusIndicator::PresenceStatusIndicator(ApplicationContext &context, MWidget *parent) :
    StatusIndicator(parent)
{
    setObjectName(metaObject()->className());

    presence = context.createContextItem("Presence.State");
    connect(presence, SIGNAL(contentsChanged()), this, SLOT(presenceChanged()));
    presenceChanged();
}

PresenceStatusIndicator::~PresenceStatusIndicator()
{
    delete presence;
}

void PresenceStatusIndicator::presenceChanged()
{
    QString status = presence->value().toString();
    QString suffix;

    if (status == "busy" || status == "available" || status == "away") {
        // Capitalize the status
        status.replace(0, 1, status[0].toUpper());
        setObjectName(QString(metaObject()->className()) + status);
    } else if (status == "offline" || status == "") {
        // No presence information is treated as "offline"
        setObjectName(QString(metaObject()->className()));
    }
}

InternetConnectionStatusIndicator::InternetConnectionStatusIndicator(ApplicationContext &context, MWidget *parent) :
    StatusIndicator(parent)
{
    setObjectName(metaObject()->className());

    internetConnection = context.createContextItem("Internet.NetworkType");
    connect(internetConnection, SIGNAL(contentsChanged()), this, SLOT(internetConnectionChanged()));

    wifiSignalStrength = context.createContextItem("Internet.SignalStrength");
    connect(wifiSignalStrength, SIGNAL(contentsChanged()), this, SLOT(wifiSignalStrengthChanged()));
}

InternetConnectionStatusIndicator::~InternetConnectionStatusIndicator()
{
    delete internetConnection;
    delete wifiSignalStrength;
}

void InternetConnectionStatusIndicator::wifiSignalStrengthChanged()
{
    setValue(wifiSignalStrength->value().toDouble() * 0.01f);
}

void InternetConnectionStatusIndicator::internetConnectionChanged()
{
    // TODO: change to 3G or WiFi signal strength based on connection type
}

PhoneNetworkStatusIndicator::PhoneNetworkStatusIndicator(ApplicationContext &context, MWidget *parent) :
    StatusIndicator(parent)
{
    setObjectName(metaObject()->className());

    networkName = QSharedPointer<ContextItem>(context.createContextItem("Cellular.NetworkName"));
    connect(networkName.data(), SIGNAL(contentsChanged()), this, SLOT(phoneNetworkChanged()));
    phoneNetworkChanged();
}

PhoneNetworkStatusIndicator::~PhoneNetworkStatusIndicator()
{
}

void PhoneNetworkStatusIndicator::phoneNetworkChanged()
{
    setValue(networkName.data()->value().toString().left(13));
}

InputMethodStatusIndicator::InputMethodStatusIndicator(MWidget *parent) :
    StatusIndicator(parent)
{
    setObjectName(metaObject()->className());
    new InputMethodStatusIndicatorAdaptor(this);
}

InputMethodStatusIndicator::~InputMethodStatusIndicator()
{
}

void InputMethodStatusIndicator::setIconID(const QString &iconID)
{
    setValue(iconID);
}

CallStatusIndicator::CallStatusIndicator(ApplicationContext &context, MWidget *parent) :
    StatusIndicator(parent)
{
    setObjectName(metaObject()->className());

    call = context.createContextItem("Phone.Call");
    connect(call, SIGNAL(contentsChanged()), this, SLOT(callOrMutedChanged()));

    muted = context.createContextItem("Phone.Muted");
    connect(muted, SIGNAL(contentsChanged()), this, SLOT(callOrMutedChanged()));
}

CallStatusIndicator::~CallStatusIndicator()
{
    delete call;
    delete muted;
}

void CallStatusIndicator::callOrMutedChanged()
{
    QString callType = call->value().toString();
    if (callType == "ringing" || callType == "knocking") {
        setObjectName(QString(metaObject()->className()) + "Ringing");
        setValue(0);
        animateIfPossible = true;
    } else if (callType == "active") {
        setObjectName(QString(metaObject()->className()) + "Ongoing");
        setValue(muted->value().toBool() ? 1 : 0);
        animateIfPossible = false;
    } else {
        setObjectName(metaObject()->className());
        setValue(0);
        animateIfPossible = false;
    }

    updateAnimationStatus();
}

ProfileStatusIndicator::ProfileStatusIndicator(ApplicationContext &context, MWidget *parent) :
    StatusIndicator(parent)
{
    setObjectName(metaObject()->className());

    profile = context.createContextItem("Profile.Name");
    connect(profile, SIGNAL(contentsChanged()), this, SLOT(profileChanged()));
}

ProfileStatusIndicator::~ProfileStatusIndicator()
{
    delete profile;
}

void ProfileStatusIndicator::profileChanged()
{
    if (profile->value().toString() == "silent") {
        setObjectName(QString(metaObject()->className()) + "Silent");
    } else {
        setObjectName(metaObject()->className());
    }
}

GPSStatusIndicator::GPSStatusIndicator(ApplicationContext &context, MWidget *parent) :
    StatusIndicator(parent)
{
    setObjectName(metaObject()->className());

    gpsState = QSharedPointer<ContextItem>(context.createContextItem("Location.SatPositioningState"));
    connect(gpsState.data(), SIGNAL(contentsChanged()), this, SLOT(gpsStateChanged()));
}

GPSStatusIndicator::~GPSStatusIndicator()
{
}

void GPSStatusIndicator::gpsStateChanged()
{
    if (gpsState.data()->value().toString() == "on") {
        setObjectName(QString(metaObject()->className()) + "On");
        animateIfPossible = false;
    }
    else if (gpsState.data()->value().toString() == "search") {
        setObjectName(QString(metaObject()->className()) + "Search");
        animateIfPossible = true;
    }
    else {
        setObjectName(QString(metaObject()->className()));
        animateIfPossible = false;
    }
    updateAnimationStatus();
}
