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

// keep these in sync with the context framework!
static const QString CONTEXT_CALLSTATE_ALERTING = "alerting";
static const QString CONTEXT_CALLSTATE_KNOCKING = "knocking";
static const QString CONTEXT_CALLSTATE_ACTIVE = "active";

static const QString BATTERY_MODE_NORMAL = "Level";
static const QString BATTERY_MODE_CHARGING = "Charging";
static const QString BATTERY_MODE_POWERSAVE = "PowerSave";
static const QString BATTERY_MODE_POWERSAVE_AND_CHARGING = "PowerSaveCharging";

StatusIndicator::StatusIndicator(MWidget *parent) :
    MWidgetController(new StatusIndicatorModel, parent),
    animateIfPossible(false),
    modelUpdatesEnabled(true),
    currentValue(QVariant())
{
}

StatusIndicator::~StatusIndicator()
{
    foreach(ContextItem* item, contextItems) {
        delete item;
    }
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
    foreach(ContextItem* item, contextItems) {
        item->subscribe();
    }
}

void StatusIndicator::exitDisplayEvent()
{
    setModelUpdatesEnabled(false);
    foreach(ContextItem* item, contextItems) {
        item->unsubscribe();
    }
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

ContextItem *StatusIndicator::createContextItem(ApplicationContext& context, const QString& key)
{
    ContextItem *item = context.createContextItem(key);
    contextItems.append(item);
    return item;
}

PhoneNetworkSignalStrengthStatusIndicator::PhoneNetworkSignalStrengthStatusIndicator(ApplicationContext &context, MWidget *parent) :
    StatusIndicator(parent)
{
    setObjectName(metaObject()->className());

    signalStrength = createContextItem(context, "Cellular.SignalBars");
    connect(signalStrength, SIGNAL(contentsChanged()), this, SLOT(signalStrengthChanged()));

    setValue(0);
    setDisplay(false);
}

PhoneNetworkSignalStrengthStatusIndicator::~PhoneNetworkSignalStrengthStatusIndicator()
{
}

void PhoneNetworkSignalStrengthStatusIndicator::signalStrengthChanged()
{
    setValue(signalStrength->value().toDouble() * 0.2f );
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
    cellularDataTechnology = createContextItem(context, "Cellular.DataTechnology");
    connect(cellularDataTechnology, SIGNAL(contentsChanged()), this, SLOT(setNetworkType()));

    cellularRegistrationStatus = createContextItem(context, "Cellular.RegistrationStatus");
    connect(cellularRegistrationStatus, SIGNAL(contentsChanged()), this, SLOT(setNetworkType()));

    setNetworkType();
}

PhoneNetworkTypeStatusIndicator::~PhoneNetworkTypeStatusIndicator()
{
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
    setObjectName(QString(metaObject()->className()) + BATTERY_MODE_NORMAL);

    batterySaveModeEnabled = false;
#ifdef HAVE_QMSYSTEM
    if (qmDeviceMode.getPSMState() == Maemo::QmDeviceMode::PSMStateOn) {
        setObjectName(QString(metaObject()->className()) + BATTERY_MODE_POWERSAVE);
        batterySaveModeEnabled = true;
    }
#endif

    batteryLevel = createContextItem(context, "Battery.ChargePercentage");
    connect(batteryLevel, SIGNAL(contentsChanged()), this, SLOT(batteryLevelChanged()));

    batteryCharging = createContextItem(context, "Battery.IsCharging");
    connect(batteryCharging, SIGNAL(contentsChanged()), this, SLOT(batteryChargingChanged()));

#ifdef HAVE_QMSYSTEM
    connect(&qmDeviceMode, SIGNAL(devicePSMStateChanged(Maemo::QmDeviceMode::PSMState)),
            this, SLOT(batterySaveModeChanged(Maemo::QmDeviceMode::PSMState)));
#endif

    batteryLevelChanged ();
}

BatteryStatusIndicator::~BatteryStatusIndicator()
{
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
        if (batterySaveModeEnabled) {
            setObjectName(QString(metaObject()->className()) + BATTERY_MODE_POWERSAVE_AND_CHARGING);
        } else {
            setObjectName(QString(metaObject()->className()) + BATTERY_MODE_CHARGING);
        }
        animateIfPossible = true;
    } else {
        if (batterySaveModeEnabled) {
            setObjectName(QString(metaObject()->className()) + BATTERY_MODE_POWERSAVE);
        } else {
            setObjectName(QString(metaObject()->className()) + BATTERY_MODE_NORMAL);
        }
        animateIfPossible = false;
    }

    updateAnimationStatus();
    //SYS_DEBUG ("extra batteryLevelChanged() call");
    batteryLevelChanged ();
}

#ifdef HAVE_QMSYSTEM
void BatteryStatusIndicator::batterySaveModeChanged(Maemo::QmDeviceMode::PSMState state)
{
    if (state == Maemo::QmDeviceMode::PSMStateOn) {
        batterySaveModeEnabled = true;
    } else {
        batterySaveModeEnabled = false;
    }
    batteryChargingChanged();
}
#endif

AlarmStatusIndicator::AlarmStatusIndicator(ApplicationContext &context, MWidget *parent) :
    StatusIndicator(parent)
{
    setObjectName(metaObject()->className());

    alarm = createContextItem(context, "Alarm.Present");
    connect(alarm, SIGNAL(contentsChanged()), this, SLOT(alarmChanged()));
    alarmChanged();
}

AlarmStatusIndicator::~AlarmStatusIndicator()
{
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

    bluetoothEnabled = createContextItem(context, "Bluetooth.Enabled");
    connect(bluetoothEnabled, SIGNAL(contentsChanged()), this, SLOT(bluetoothChanged()));
    bluetoothConnected = createContextItem(context, "Bluetooth.Connected");
    connect(bluetoothConnected, SIGNAL(contentsChanged()), this, SLOT(bluetoothChanged()));
}

BluetoothStatusIndicator::~BluetoothStatusIndicator()
{
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

    presence = createContextItem(context, "Presence.State");
    connect(presence, SIGNAL(contentsChanged()), this, SLOT(presenceChanged()));
    presenceChanged();
}

PresenceStatusIndicator::~PresenceStatusIndicator()
{
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
    connectionType = createContextItem(context, "Internet.NetworkType");
    connect(connectionType, SIGNAL(contentsChanged()), this, SLOT(updateStatus()));

    connectionState = createContextItem(context, "Internet.NetworkState");
    connect(connectionState, SIGNAL(contentsChanged()), this, SLOT(updateStatus()));

    trafficIn  = createContextItem(context, "Internet.TrafficIn");
    connect(trafficIn, SIGNAL(contentsChanged()), this, SLOT(updateStatus()));

    trafficOut = createContextItem(context, "Internet.TrafficOut");
    connect(trafficOut, SIGNAL(contentsChanged()), this, SLOT(updateStatus()));

    updateStatus();
}

InternetConnectionStatusIndicator::~InternetConnectionStatusIndicator()
{
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

    networkName = createContextItem(context, "Cellular.NetworkName");
    connect(networkName, SIGNAL(contentsChanged()), this, SLOT(phoneNetworkChanged()));
    phoneNetworkChanged();
}

PhoneNetworkStatusIndicator::~PhoneNetworkStatusIndicator()
{
}

void PhoneNetworkStatusIndicator::phoneNetworkChanged()
{
    setValue(networkName->value().toString().left(13));
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

    call = createContextItem(context, "Phone.Call");
    connect(call, SIGNAL(contentsChanged()), this, SLOT(callOrMutedChanged()));

    muted = createContextItem(context, "Phone.Muted");
    connect(muted, SIGNAL(contentsChanged()), this, SLOT(callOrMutedChanged()));
}

CallStatusIndicator::~CallStatusIndicator()
{
}

void CallStatusIndicator::callOrMutedChanged()
{
    QString callState = call->value().toString();
    if (callState == CONTEXT_CALLSTATE_ALERTING
        || callState == CONTEXT_CALLSTATE_KNOCKING) {
        setObjectName(QString(metaObject()->className()) + "Ringing");
        setValue(0);
        animateIfPossible = true;
    } else if (callState == CONTEXT_CALLSTATE_ACTIVE) {
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

    profile = createContextItem(context, "Profile.Name");
    connect(profile, SIGNAL(contentsChanged()), this, SLOT(profileChanged()));
}

ProfileStatusIndicator::~ProfileStatusIndicator()
{
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

    gpsState = createContextItem(context, "Location.SatPositioningState");
    connect(gpsState, SIGNAL(contentsChanged()), this, SLOT(gpsStateChanged()));
}

GPSStatusIndicator::~GPSStatusIndicator()
{
}

void GPSStatusIndicator::gpsStateChanged()
{
    if (gpsState->value().toString() == "on") {
        setObjectName(QString(metaObject()->className()) + "On");
        animateIfPossible = false;
    }
    else if (gpsState->value().toString() == "search") {
        setObjectName(QString(metaObject()->className()) + "Search");
        animateIfPossible = true;
    }
    else {
        setObjectName(QString(metaObject()->className()));
        animateIfPossible = false;
    }
    updateAnimationStatus();
}
