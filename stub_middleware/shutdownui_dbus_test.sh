#!/bin/sh
# David Kedves <dkedves@blumsoft.eu>
# Copyright 2010 Nokia
# This script just for testing the sysuid ShutdownUi inside scratchbox 1

source /tmp/session_bus_address.user
sysuid -software &

SYSUIDPID=$!
sleep 3; # wait for sysuid startup

METHOD="com.nokia.systemui.shutdownui.showScreen"

echo "Test the show dialog [with 5000 msec timeout....]"
dbus-send --session --type=method_call --dest=com.nokia.systemui / $METHOD string:"Hello world!" string:"Waiting for 5 seconds..." int32:5000
sleep 8;

kill ${SYSUIDPID}

