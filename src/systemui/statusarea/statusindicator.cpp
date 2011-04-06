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
#include <MLocale>
#include "statusindicator.h"
#include "statusindicatormodel.h"
#include "applicationcontext.h"

// keep these in sync with the context framework!
static const QString CONTEXT_CALLSTATE_ALERTING = "alerting";
static const QString CONTEXT_CALLSTATE_KNOCKING = "knocking";
static const QString CONTEXT_CALLSTATE_ACTIVE = "active";

static const QString BATTERY_MODE_NORMAL = "Level";
static const QString BATTERY_MODE_CHARGING = "Charging";
static const QString BATTERY_MODE_POWERSAVE = "PowerSave";
static const QString BATTERY_MODE_POWERSAVE_AND_CHARGING = "PowerSaveCharging";

static const QString NETWORK_NAME_START_DELIMITER = "(";
static const QString NETWORK_NAME_END_DELIMITER = ")";

const QString TransferStatusIndicator::TRANSFER_UI_DBUS_PATH="/com/meego/transferui";
const QString TransferStatusIndicator::TRANSFER_UI_DBUS_INTERFACE="com.meego.transferui";
const QString TransferStatusIndicator::TRANSFER_UI_DBUS_SIGNAL="stateChanged";
const QString TransferStatusIndicator::TRANSFER_UI_STATE_IDLE = "idle";
const QString TransferStatusIndicator::TRANSFER_UI_STATE_LIVE = "live";
const QString TransferStatusIndicator::TRANSFER_UI_STATE_FAIL = "fail";
const QString TransferStatusIndicator::TRANSFER_UI_STATE_PENDING = "pending";
const QString TransferStatusIndicator::TRANSFER_UI_SUFFIX_FAIL = "Fail";
const QString TransferStatusIndicator::TRANSFER_UI_SUFFIX_LIVE = "Live";
const QString TransferStatusIndicator::TRANSFER_UI_SUFFIX_PENDING = "Pending";

StatusIndicator::StatusIndicator(QGraphicsItem *parent) :
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

void StatusIndicator::setStyleNameAndUpdate(const QString &name)
{
    setStyleName(name);
    update();
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
    foreach(ContextItem* item, contextItems) {
        item->subscribe();
    }
    emit subscriptionMade();
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
        model()->setAnimate(animateIfPossible);
    } else {
        model()->setAnimate(false);
    }
}

ContextItem *StatusIndicator::createContextItem(ApplicationContext& context, const QString& key)
{
    ContextItem *item = context.createContextItem(key);
    contextItems.append(item);
    connect(this, SIGNAL(subscriptionMade()), item, SIGNAL(contentsChanged()));
    return item;
}

PhoneNetworkSignalStrengthStatusIndicator::PhoneNetworkSignalStrengthStatusIndicator(ApplicationContext &context, QGraphicsItem *parent) :
    StatusIndicator(parent), networkAvailable(false)
{
    signalStrength = createContextItem(context, "Cellular.SignalBars");
    connect(signalStrength, SIGNAL(contentsChanged()), this, SLOT(signalStrengthChanged()));

    systemOfflineMode = createContextItem(context, "System.OfflineMode");
    connect(systemOfflineMode, SIGNAL(contentsChanged()), this, SLOT(setNetworkStatus()));

    cellularRegistrationStatus = createContextItem(context, "Cellular.RegistrationStatus");
    connect(cellularRegistrationStatus, SIGNAL(contentsChanged()), this, SLOT(setNetworkStatus()));

    setNetworkStatus();
}

PhoneNetworkSignalStrengthStatusIndicator::~PhoneNetworkSignalStrengthStatusIndicator()
{
}

void PhoneNetworkSignalStrengthStatusIndicator::signalStrengthChanged()
{
    setValue(signalStrength->value().toDouble() * 0.2f );
}

void PhoneNetworkSignalStrengthStatusIndicator::setNetworkStatus()
{
    QString postFix = "";

    bool offlineMode = systemOfflineMode->value().toBool();
    QString status = cellularRegistrationStatus->value().toString(); // home roam no-sim offline forbidden

    if (offlineMode) {
        postFix = "Offline";
    } else if (status == "no-sim") {
        postFix = "NoSIM";
    } else if (status == "" || status == "offline" || status == "forbidden") {
        postFix = "NoNetwork";
    }

    bool networkCurrentlyAvailable = postFix.isEmpty();
    if (networkCurrentlyAvailable) {
        signalStrengthChanged();
    } else {
        setValue(0);
    }
    if (networkAvailable != networkCurrentlyAvailable) {
        emit networkAvailabilityChanged(networkCurrentlyAvailable);
        networkAvailable = networkCurrentlyAvailable;
    }
    setStyleNameAndUpdate(metaObject()->className() + postFix);
}

PhoneNetworkTypeStatusIndicator::PhoneNetworkTypeStatusIndicator(ApplicationContext &context, QGraphicsItem *parent) :
        StatusIndicator(parent)
{
    cellularDataTechnology = createContextItem(context, "Cellular.DataTechnology");
    connect(cellularDataTechnology, SIGNAL(contentsChanged()), this, SLOT(setNetworkType()));

    connectionType = createContextItem(context, "Internet.NetworkType");
    connect(connectionType, SIGNAL(contentsChanged()), this, SLOT(setNetworkType()));

    connectionState = createContextItem(context, "Internet.NetworkState");
    connect(connectionState, SIGNAL(contentsChanged()), this, SLOT(setNetworkType()));

    packetData = createContextItem(context, "Cellular.PacketData");
    connect(packetData, SIGNAL(contentsChanged()), this, SLOT(setNetworkType()));

    setNetworkType();
}

PhoneNetworkTypeStatusIndicator::~PhoneNetworkTypeStatusIndicator()
{
}

void PhoneNetworkTypeStatusIndicator::setNetworkAvailability(bool available)
{
    QString state = connectionState->value().toString(); // disconnected connecting connected
    if (!available && (state == "disconnected")) {
        setStyleNameAndUpdate();
    } else {
        setNetworkType();
    }
}

void PhoneNetworkTypeStatusIndicator::setNetworkType()
{
    QString postFix = "";
    QString postFixPacketData = "";
    QString dataTechnology = cellularDataTechnology->value().toString(); // gprs egprs umts hspa
    QString state = connectionState->value().toString(); // disconnected connecting connected
    QString connection = connectionType->value().toString(); // GPRS WLAN
    bool data = packetData->value().toBool();

    setValue(0);

    if (state == "disconnected") {
        setStyleNameAndUpdate(); // hide indicator
        return; // no further actions needed
    }

    if (connection == "WLAN") {
        postFix = "WLAN";
    }
    if (dataTechnology == "gprs") {
        postFixPacketData = "2G";
    } else if (dataTechnology == "egprs") {
        postFixPacketData = "25G";
    } else if (dataTechnology == "umts") {
        postFixPacketData = "3G";
    } else if (dataTechnology == "hspa") {
        postFixPacketData = "35G";
    }

    // if wlan connected and packet data active e.g. when sending mms
    if (data) {
        postFix += postFixPacketData;
        postFix += "Active";
        animateIfPossible = (connection == "WLAN");
    } else {
        if (postFix.isEmpty()) {
            postFix = postFixPacketData;
        }
        if (state == "connecting") {
            postFix += "Connecting";
            animateIfPossible = true;
        } else {
            animateIfPossible = false;
        }
    }

    setStyleNameAndUpdate(metaObject()->className() + postFix);

    updateAnimationStatus();
}

BatteryStatusIndicator::BatteryStatusIndicator(ApplicationContext &context, QGraphicsItem *parent) :
    StatusIndicator(parent)
{
    setStyleName(QString(metaObject()->className()) + BATTERY_MODE_NORMAL);

    batteryLevel = createContextItem(context, "Battery.ChargeBars");
    connect(batteryLevel, SIGNAL(contentsChanged()), this, SLOT(batteryLevelChanged()));

    batteryCharging = createContextItem(context, "Battery.IsCharging");
    connect(batteryCharging, SIGNAL(contentsChanged()), this, SLOT(batteryChargingChanged()));

    batterySaveMode = createContextItem(context, "System.PowerSaveMode");
    connect(batterySaveMode, SIGNAL(contentsChanged()), this, SLOT(batteryChargingChanged()));

    // Set the initial power save mode (in case it has been switched on before reboot, etc)
    if (batterySaveMode->value().toBool()) {
        setStyleName(QString(metaObject()->className()) + BATTERY_MODE_POWERSAVE);
    }

    batteryLevelChanged ();
}

BatteryStatusIndicator::~BatteryStatusIndicator()
{
}

void BatteryStatusIndicator::batteryLevelChanged()
{
    QList<QVariant> chargeBars = batteryLevel->value().toList();
    if (chargeBars.count() == 2) {
        int remainingBars = chargeBars.at(0).toInt();
        int maximumBars = chargeBars.at(1).toInt();

        // Smoke test - check that charge bar values are valid
        if((maximumBars > 0) && (remainingBars >= 0) && (maximumBars >= remainingBars)) {
            if (batteryCharging->value().toBool() && remainingBars == maximumBars) {
                // While charging always animate at least one bar
                remainingBars = maximumBars - 1;
            }

            // imageList contains maximumBars + 2 images
            int images = maximumBars + 2;

            // First icon is for battery empty situation, hence remainingBars + 1
            setValue((remainingBars + 1) / (double)images);
        } else {
            // Error situation
            setValue(0.0);
        }
    }
}

void BatteryStatusIndicator::batteryChargingChanged()
{
    if (batteryCharging->value().toBool()) {
        if (batterySaveMode->value().toBool()) {
            setStyleNameAndUpdate(QString(metaObject()->className()) + BATTERY_MODE_POWERSAVE_AND_CHARGING);
        } else {
            setStyleNameAndUpdate(QString(metaObject()->className()) + BATTERY_MODE_CHARGING);
        }
        animateIfPossible = true;
    } else {
        if (batterySaveMode->value().toBool()) {
            setStyleNameAndUpdate(QString(metaObject()->className()) + BATTERY_MODE_POWERSAVE);
        } else {
            setStyleNameAndUpdate(QString(metaObject()->className()) + BATTERY_MODE_NORMAL);
        }
        animateIfPossible = false;
    }

    updateAnimationStatus();
    batteryLevelChanged ();
}

AlarmStatusIndicator::AlarmStatusIndicator(ApplicationContext &context, QGraphicsItem *parent) :
    StatusIndicator(parent)
{
    setStyleName(metaObject()->className());

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
        setStyleNameAndUpdate(QString(metaObject()->className()) + "Set");
    } else {
        setStyleNameAndUpdate(QString(metaObject()->className()));
    }
}

BluetoothStatusIndicator::BluetoothStatusIndicator(ApplicationContext &context, QGraphicsItem *parent) :
    StatusIndicator(parent)
{
    setStyleName(metaObject()->className());

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
            setStyleNameAndUpdate(QString(metaObject()->className()) + "Active");
        } else {
            setStyleNameAndUpdate(QString(metaObject()->className()) + "On");
        }
    } else {
        setStyleNameAndUpdate(QString(metaObject()->className()));
    }
}

PresenceStatusIndicator::PresenceStatusIndicator(ApplicationContext &context, QGraphicsItem *parent) :
    StatusIndicator(parent)
{
    setStyleName(metaObject()->className());

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

    if (status == "busy" || status == "available") {
        // Capitalize the status
        status.replace(0, 1, status[0].toUpper());
        setStyleNameAndUpdate(QString(metaObject()->className()) + status);
    } else if (status == "offline" || status == "") {
        // No presence information is treated as "offline"
        setStyleNameAndUpdate(QString(metaObject()->className()));
    }
}

PhoneNetworkStatusIndicator::PhoneNetworkStatusIndicator(ApplicationContext &context, QGraphicsItem *parent) :
    StatusIndicator(parent)
{
    setStyleNameAndUpdate(metaObject()->className());

    networkName = createContextItem(context, "Cellular.NetworkName");
    extendedNetworkName = createContextItem(context, "Cellular.ExtendedNetworkName");
    connect(networkName, SIGNAL(contentsChanged()), this, SLOT(phoneNetworkChanged()));
    connect(extendedNetworkName, SIGNAL(contentsChanged()), this, SLOT(phoneNetworkChanged()));
    connect(&networkChangeShowVisitorTimer, SIGNAL(timeout()), this, SLOT(showVisitorNetworkName()));
    networkChangeShowVisitorTimer.setSingleShot(true);
    networkChangeShowVisitorTimer.setInterval(3 * 1000);
    phoneNetworkChanged();
}

QString PhoneNetworkStatusIndicator::localizedNetwork() const
{
    MLocale locale;
    QString name = extendedNetworkName->value().toString();

    if(locale.language() != QLatin1String("zh") || name.isEmpty()) {
       name = networkName->value().toString();
    }

    return name;
}

QString PhoneNetworkStatusIndicator::homeNetwork() const
{
    QStringList netNames(localizedNetwork().split(NETWORK_NAME_START_DELIMITER));

    if (netNames.count() >= 1) {
        return netNames.first().trimmed();
    } else {
        return QString();
    }
}

QString PhoneNetworkStatusIndicator::visitorNetwork() const
{
    QString networkString = localizedNetwork().trimmed();

    if (networkString.contains(NETWORK_NAME_START_DELIMITER) &&
        networkString.endsWith(NETWORK_NAME_END_DELIMITER)) {
        // separates networkString into pieces divided by "("
        QStringList netNames(networkString.split(NETWORK_NAME_START_DELIMITER));
        // removes first string before "(" which is home network
        netNames.removeFirst();
        // returns remaining string as it was, by joining the separated strings
        QString visitor = netNames.join(QString(NETWORK_NAME_START_DELIMITER));
        // removes end delimiter ")"
        visitor.chop(1);
        return visitor.trimmed();
    } else {
        return QString();
    }
}

PhoneNetworkStatusIndicator::~PhoneNetworkStatusIndicator()
{
}

void PhoneNetworkStatusIndicator::phoneNetworkChanged()
{
    if (networkChangeShowVisitorTimer.isActive()) {
        networkChangeShowVisitorTimer.stop();
    }
    QString home(homeNetwork());
    QString visitor(visitorNetwork());
    setValue(home);

    if (!visitor.isEmpty() && !home.isEmpty() && (home != visitor)) {
        networkChangeShowVisitorTimer.start();
    }
}

void PhoneNetworkStatusIndicator::showVisitorNetworkName() {
    setValue(visitorNetwork());
}

InputMethodStatusIndicator::InputMethodStatusIndicator(QGraphicsItem *parent) :
    StatusIndicator(parent)
{
    setStyleName(metaObject()->className());
}

InputMethodStatusIndicator::~InputMethodStatusIndicator()
{
}

void InputMethodStatusIndicator::setIconID(const QString &iconID)
{
    setValue(iconID);
}

CallStatusIndicator::CallStatusIndicator(ApplicationContext &context, QGraphicsItem *parent) :
    StatusIndicator(parent)
{
    setStyleName(metaObject()->className());

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
        setStyleNameAndUpdate(QString(metaObject()->className()) + "Ringing");
        setValue(0);
        animateIfPossible = true;
    } else if (callState == CONTEXT_CALLSTATE_ACTIVE) {
        setStyleNameAndUpdate(QString(metaObject()->className()) + "Ongoing");
        setValue(muted->value().toBool() ? 1 : 0);
        animateIfPossible = false;
    } else {
        setStyleNameAndUpdate(metaObject()->className());
        setValue(0);
        animateIfPossible = false;
    }

    updateAnimationStatus();
}

ProfileStatusIndicator::ProfileStatusIndicator(ApplicationContext &context, QGraphicsItem *parent) :
    StatusIndicator(parent)
{
    setStyleName(metaObject()->className());

    profile = createContextItem(context, "Profile.Name");
    connect(profile, SIGNAL(contentsChanged()), this, SLOT(profileChanged()));
}

ProfileStatusIndicator::~ProfileStatusIndicator()
{
}

void ProfileStatusIndicator::profileChanged()
{
    if (profile->value().toString() == "silent") {
        setStyleNameAndUpdate(QString(metaObject()->className()) + "Silent");
    } else {
        setStyleNameAndUpdate(metaObject()->className());
    }
}

GPSStatusIndicator::GPSStatusIndicator(ApplicationContext &context, QGraphicsItem *parent) :
    StatusIndicator(parent)
{
    setStyleName(metaObject()->className());

    gpsState = createContextItem(context, "Location.SatPositioningState");
    connect(gpsState, SIGNAL(contentsChanged()), this, SLOT(gpsStateChanged()));
}

GPSStatusIndicator::~GPSStatusIndicator()
{
}

void GPSStatusIndicator::gpsStateChanged()
{
    if (gpsState->value().toString() == "on") {
        setStyleNameAndUpdate(QString(metaObject()->className()) + "On");
        animateIfPossible = false;
    }
    else if (gpsState->value().toString() == "search") {
        setStyleNameAndUpdate(QString(metaObject()->className()) + "Search");
        animateIfPossible = true;
    }
    else {
        setStyleNameAndUpdate(QString(metaObject()->className()));
        animateIfPossible = false;
    }
    updateAnimationStatus();
}

TransferStatusIndicator::TransferStatusIndicator(QGraphicsItem *parent) :
    StatusIndicator(parent),
    connectionSessionBus(QDBusConnection::sessionBus())
{
    setStyleName(metaObject()->className());

    connectionSessionBus.connect(QString(),TRANSFER_UI_DBUS_PATH, TRANSFER_UI_DBUS_INTERFACE, TRANSFER_UI_DBUS_SIGNAL,
        this, SLOT(transferStateChanged(const QString&)));

}

TransferStatusIndicator::~TransferStatusIndicator()
{
}

void TransferStatusIndicator::transferStateChanged(const QString &state)
{
    if (state == TRANSFER_UI_STATE_LIVE) {
        setStyleNameAndUpdate(QString(metaObject()->className()) + TRANSFER_UI_SUFFIX_LIVE);
    } else if (state == TRANSFER_UI_STATE_FAIL) {
        setStyleNameAndUpdate(QString(metaObject()->className()) + TRANSFER_UI_SUFFIX_FAIL);
    } else if (state == TRANSFER_UI_STATE_PENDING) {
        setStyleNameAndUpdate(QString(metaObject()->className()) + TRANSFER_UI_SUFFIX_PENDING);
    } else {
        setStyleNameAndUpdate(metaObject()->className());
    }

    updateAnimationStatus();
}

CallForwardingStatusIndicator::CallForwardingStatusIndicator(ApplicationContext &context, QGraphicsItem *parent) :
    StatusIndicator(parent)
{
    setStyleName(metaObject()->className());

    callForwarding = createContextItem(context, "Phone.CallForwarding");
    connect(callForwarding, SIGNAL(contentsChanged()), this, SLOT(callForwardingChanged()));
    callForwardingChanged();
}

CallForwardingStatusIndicator::~CallForwardingStatusIndicator()
{
}

void CallForwardingStatusIndicator::callForwardingChanged()
{
    bool isSet = callForwarding->value().toBool();

    if (isSet) {
        setStyleNameAndUpdate(QString(metaObject()->className()) + "Set");
    } else {
        setStyleNameAndUpdate(QString(metaObject()->className()));
    }
}
