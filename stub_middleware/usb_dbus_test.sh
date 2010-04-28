#!/bin/sh
# David Kedves <dkedves@blumsoft.eu>
# Copyright 2010 Nokia
# This script just for testing the sysuid UsbUi inside scratchbox 1

source /tmp/session_bus_address.user
sysuid -software &

SYSUIDPID=$!
sleep 3; # wait for sysuid startup

METHOD="com.nokia.systemui.usb.simulateUsbModedSignal"

echo "Show the mode selection dialog"
dbus-send --session --type=method_call --dest=com.nokia.systemui / $METHOD string:"mode_requested_show_dialog"
sleep 3;

echo "Disconnect"
dbus-send --session --type=method_call --dest=com.nokia.systemui / $METHOD string:"USB disconnected"
sleep 3;

echo "Mass Storage mode"
dbus-send --session --type=method_call --dest=com.nokia.systemui / $METHOD string:"mass_storage"
sleep 3;

echo "Ovi Suite mode"
dbus-send --session --type=method_call --dest=com.nokia.systemui / $METHOD string:"ovi_suite"
sleep 3;

echo "Charging only"
dbus-send --session --type=method_call --dest=com.nokia.systemui / $METHOD string:"charging_only"
sleep 3;

echo "Ask mode"
dbus-send --session --type=method_call --dest=com.nokia.systemui / $METHOD string:"ask"
sleep 10;

kill ${SYSUIDPID}

