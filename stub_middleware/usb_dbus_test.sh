#!/bin/sh
# David Kedves <dkedves@blumsoft.eu>
# Copyright 2010 Nokia
# This script just for testing the sysuid UsbUi inside scratchbox 1

source /tmp/session_bus_address.user
sysuid -software &

SYSUIDPID=$!
sleep 3; # wait for sysuid startup

# send a disconnect event:
dbus-send --session --type=method_call --dest=com.nokia.systemui / com.nokia.systemui.usb.testUsbConnection boolean:false
sleep 3;
echo "Usb connect with Ovi Suite setting"
gconftool-2 -t string -s /Meego/System/UsbMode ovi_suite
dbus-send --session --type=method_call --dest=com.nokia.systemui / com.nokia.systemui.usb.testUsbConnection boolean:true
dbus-send --print-reply --session --dest=com.nokia.systemui / com.nokia.systemui.usb.isActive
sleep 3;
echo "Usb disconnect event"
dbus-send --session --type=method_call --dest=com.nokia.systemui / com.nokia.systemui.usb.testUsbConnection boolean:false
sleep 3;

echo "Usb connect with Mass Storage setting"
gconftool-2 -t string -s /Meego/System/UsbMode mass_storage
dbus-send --session --type=method_call --dest=com.nokia.systemui / com.nokia.systemui.usb.testUsbConnection boolean:true
dbus-send --print-reply --session --dest=com.nokia.systemui / com.nokia.systemui.usb.isActive
sleep 3;
echo "Usb disconnect event"
dbus-send --session --type=method_call --dest=com.nokia.systemui / com.nokia.systemui.usb.testUsbConnection boolean:false
sleep 3;

echo "Usb connect with do nothing setting"
gconftool-2 -t string -s /Meego/System/UsbMode charging_only
dbus-send --session --type=method_call --dest=com.nokia.systemui / com.nokia.systemui.usb.testUsbConnection boolean:true
dbus-send --print-reply --session --dest=com.nokia.systemui / com.nokia.systemui.usb.isActive
sleep 3;
echo "Usb disconnect event"
dbus-send --session --type=method_call --dest=com.nokia.systemui / com.nokia.systemui.usb.testUsbConnection boolean:false
sleep 3;

echo Usb connect with "Ask on connection" setting
gconftool-2 -t string -s /Meego/System/UsbMode ask
dbus-send --session --type=method_call --dest=com.nokia.systemui / com.nokia.systemui.usb.testUsbConnection boolean:true
dbus-send --print-reply --session --dest=com.nokia.systemui / com.nokia.systemui.usb.isActive
sleep 15;
echo "Usb disconnect event"
dbus-send --session --type=method_call --dest=com.nokia.systemui / com.nokia.systemui.usb.testUsbConnection boolean:false

echo "DONE"
kill ${SYSUIDPID}

