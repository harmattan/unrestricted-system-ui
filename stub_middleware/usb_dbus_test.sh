#!/bin/sh
# David Kedves <dkedves@blumsoft.eu>
# Copyright 2010 Nokia
# This script just for testing the sysuid UsbUi in sbox2,
# you should run this in runtime (maemo-sdk enter runtime)

source /tmp/session_bus_address.${USER}
sysuid -software &

SYSUIDPID=$!
sleep 3; # wait for sysuid startup

# send a disconnect event:
dbus-send --session --type=method_call --dest=com.nokia.systemui / com.nokia.systemui.usb.testUsbConnection boolean:false
sleep 3;
echo "Usb connect with Ovi Suite setting"
gconftool-2 -t string -s /Dui/System/UsbMode ovi_suite
dbus-send --session --type=method_call --dest=com.nokia.systemui / com.nokia.systemui.usb.testUsbConnection boolean:true
dbus-send --print-reply --session --dest=com.nokia.systemui / com.nokia.systemui.usb.UsbMode
sleep 3;
echo "Usb disconnect event"
dbus-send --session --type=method_call --dest=com.nokia.systemui / com.nokia.systemui.usb.testUsbConnection boolean:false
sleep 3;

echo "Usb connect with Mass Storage setting"
gconftool-2 -t string -s /Dui/System/UsbMode mass_storage
dbus-send --session --type=method_call --dest=com.nokia.systemui / com.nokia.systemui.usb.testUsbConnection boolean:true
dbus-send --print-reply --session --dest=com.nokia.systemui / com.nokia.systemui.usb.UsbMode
sleep 3;
echo "Usb disconnect event"
dbus-send --session --type=method_call --dest=com.nokia.systemui / com.nokia.systemui.usb.testUsbConnection boolean:false
sleep 3;

echo "Usb connect with do nothing setting"
gconftool-2 -t string -s /Dui/System/UsbMode do_nothing
dbus-send --session --type=method_call --dest=com.nokia.systemui / com.nokia.systemui.usb.testUsbConnection boolean:true
dbus-send --print-reply --session --dest=com.nokia.systemui / com.nokia.systemui.usb.UsbMode
sleep 3;
echo "Usb disconnect event"
dbus-send --session --type=method_call --dest=com.nokia.systemui / com.nokia.systemui.usb.testUsbConnection boolean:false
sleep 3;

echo Usb connect with "Ask on connection" setting
gconftool-2 -t string -s /Dui/System/UsbMode auto
dbus-send --session --type=method_call --dest=com.nokia.systemui / com.nokia.systemui.usb.testUsbConnection boolean:true
dbus-send --print-reply --session --dest=com.nokia.systemui / com.nokia.systemui.usb.UsbMode
sleep 15;
echo "Usb disconnect event"
dbus-send --session --type=method_call --dest=com.nokia.systemui / com.nokia.systemui.usb.testUsbConnection boolean:false

echo "DONE"
kill ${SYSUIDPID}
