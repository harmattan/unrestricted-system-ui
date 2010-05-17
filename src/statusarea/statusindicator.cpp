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

//#define DEBUG
#define WARNING
#include "debug.h"

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
    SYS_DEBUG ("*** currentValue = %g", currentValue.toDouble());
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
	SYS_DEBUG ("setAnimate(%s)", SYS_BOOL(animateIfPossible));
        model()->setAnimate(animateIfPossible);
    } else {
	SYS_DEBUG ("setAnimate(false)");
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
    cellularDataTechnology = context.createContextItem("Cellular.DataTechnology");
    connect(cellularDataTechnology, SIGNAL(contentsChanged()), this, SLOT(setNetworkType()));

    cellularRegistrationStatus = context.createContextItem("Cellular.RegistrationStatus");
    connect(cellularRegistrationStatus, SIGNAL(contentsChanged()), this, SLOT(setNetworkType()));

    setNetworkType();
}

PhoneNetworkTypeStatusIndicator::~PhoneNetworkTypeStatusIndicator()
{
    delete cellularDataTechnology;
    delete cellularRegistrationStatus;
}

void PhoneNetworkTypeStatusIndicator::setNetworkType()
{
    QString postFix = "NoNetwork";

    QString dataTechnology = cellularDataTechnology->value().toString();     // gprs egprs umts hspa
    QString status         = cellularRegistrationStatus->value().toString(); // home roam no-sim offline forbidden

    setValue(0);

    if(status == "no-sim") {
        postFix = "NoSIM";
    } else if(status == "" || status == "offline" || status == "forbidden") {
        postFix = "Offline";
    } else {
        if(dataTechnology == "gprs") {
            postFix = "2G";
        } else if(dataTechnology == "egprs") {
            postFix = "25G";
        } else if(dataTechnology == "umts") {
            postFix = "3G";
        } else if(dataTechnology == "hspa") {
            postFix = "35G";
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

    batteryLevelChanged ();
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
    //SYS_DEBUG ("extra batteryLevelChanged() call");
    batteryLevelChanged ();
}

AlarmStatusIndicator::AlarmStatusIndicator(ApplicationContext &context, MWidget *parent) :
    StatusIndicator(parent)
{
    setObjectName(metaObject()->className());

    alarm = context.createContextItem("UserAlarm.Present");
    connect(alarm, SIGNAL(contentsChanged()), this, SLOT(alarmChanged()));
    alarmChanged();
}

AlarmStatusIndicator::~AlarmStatusIndicator()
{
    delete alarm;
}

void AlarmStatusIndicator::alarmChanged()
{
    bool isSet = alarm->value().toBool();

    if (isSet) {
        setObjectName(QString(metaObject()->className()) + "Set");
    } else {
        setObjectName(QString(metaObject()->className()));
    }

    emit alarmSettingChanged(isSet);
}

BluetoothStatusIndicator::BluetoothStatusIndicator(ApplicationContext &context, MWidget *parent) :
    StatusIndicator(parent)
{
    setObjectName(metaObject()->className());

    bluetoothEnabled = context.createContextItem("Bluetooth.Enabled");
    connect(bluetoothEnabled, SIGNAL(contentsChanged()), this, SLOT(bluetoothChanged()));
    bluetoothConnected = context.createContextItem("Bluetooth.Connected");
    connect(bluetoothConnected, SIGNAL(contentsChanged()), this, SLOT(bluetoothChanged()));
}

BluetoothStatusIndicator::~BluetoothStatusIndicator()
{
    delete bluetoothEnabled;
    delete bluetoothConnected;
}

void BluetoothStatusIndicator::bluetoothChanged()
{
    bool enabled = bluetoothEnabled->value().toBool();
    bool connected = bluetoothConnected->value().toBool();

    if (enabled) {
        if (connected) {
            setObjectName(QString(metaObject()->className()) + "Active");
        } else {
            setObjectName(QString(metaObject()->className()) + "On");
        }
    } else {
        setObjectName(QString(metaObject()->className()));
    }
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
    connectionType = context.createContextItem("Internet.NetworkType");
    connect(connectionType, SIGNAL(contentsChanged()), this, SLOT(updateStatus()));

    connectionState = context.createContextItem("Internet.NetworkState");
    connect(connectionState, SIGNAL(contentsChanged()), this, SLOT(updateStatus()));

    trafficIn  = context.createContextItem("Internet.TrafficIn");
    connect(trafficIn, SIGNAL(contentsChanged()), this, SLOT(updateStatus()));

    trafficOut = context.createContextItem("Internet.TrafficOut");
    connect(trafficOut, SIGNAL(contentsChanged()), this, SLOT(updateStatus()));

    updateStatus();
}

InternetConnectionStatusIndicator::~InternetConnectionStatusIndicator()
{
    delete trafficOut;
    delete trafficIn;
    delete connectionState;
    delete connectionType;
}

void InternetConnectionStatusIndicator::updateStatus()
{
    QString postFix = "";

    QString state      = connectionState->value().toString(); // disconnected connecting connected
    QString connection = connectionType->value().toString();  // GPRS WLAN

    uint trafficInPercentage  = trafficIn->value().toInt();
    uint trafficOutPercentage = trafficOut->value().toInt();

    setValue(0);

    if(connection == "WLAN") {
        postFix = "WLAN";
    } else if(connection == "GPRS") {
        postFix = "PacketData";
        if(state == "connected" && (trafficInPercentage > 0 || trafficOutPercentage > 0)) {
            postFix += "Active";
        }
    }

    if(state == "connecting") {
        postFix += "Connecting";
        animateIfPossible = true;
    } else if(state == "connected") {
        animateIfPossible = false;
    } else {
        postFix = "";
        animateIfPossible = false;
    }

    setObjectName(metaObject()->className() + postFix);

    updateAnimationStatus();
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
