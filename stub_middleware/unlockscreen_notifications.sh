#!/bin/sh
#****************************************************************************
#*
#* Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
#* All rights reserved.
#* Contact: Nokia Corporation (directui@nokia.com)
#*
#* This file is part of systemui.
#*
#* If you have questions regarding the use of this file, please contact
#* Nokia at directui@nokia.com.
#*
#* This library is free software; you can redistribute it and/or
#* modify it under the terms of the GNU Lesser General Public
#* License version 2.1 as published by the Free Software Foundation
#* and appearing in the file LICENSE.LGPL included in the packaging
#* of this file.
#*
#* This script just for testing the unlock-screen notifications
#***************************************************************************/

source /tmp/session_bus_address.user

if pgrep sysuid > /dev/null; then
	echo "sysuid running... already..."
else
	sysuid -software &
fi

SLEEPTIME=6

SYSUIDPID=$!
sleep 2; # wait for sysuid startup

echo "******************************************* Open the unlockscreen"
dbus-send --print-reply --system --dest=com.nokia.system_ui /com/nokia/system_ui/request com.nokia.system_ui.request.tklock_open string: string: string: string: uint32:5 boolean:false boolean:false

sleep 1; # wait for unlockscreen...
echo "******************************************* Test e-mail..:"
dbus-send --print-reply --dest=com.meego.core.MNotificationManager /notificationmanager com.meego.core.MNotificationManager.addNotification uint32:1000 uint32:0 string:'email.arrived' string:'Message received' string:'Hello world!' string:'' string:'' uint32:0

sleep $SLEEPTIME; # wait for a while
echo "******************************************* Test call from 'John Doe'"
dbus-send --print-reply --dest=com.meego.core.MNotificationManager /notificationmanager com.meego.core.MNotificationManager.addNotification uint32:1000 uint32:0 string:'x-nokia.call' string:'John Doe' string:'Incoming call' string:'' string:'' uint32:0

sleep $SLEEPTIME; # wait for a while
echo "******************************************* Test SMS from 'Santa Claus'"
dbus-send --print-reply --dest=com.meego.core.MNotificationManager /notificationmanager com.meego.core.MNotificationManager.addNotification uint32:1000 uint32:0 string:'x-nokia.message.arrived' string:'Santa Claus' string:'Happy christmas!' string:'' string:'' uint32:0

sleep $SLEEPTIME; # wait for a while
echo "******************************************* Test instant message from 'Bill Gates'"
dbus-send --print-reply --dest=com.meego.core.MNotificationManager /notificationmanager com.meego.core.MNotificationManager.addNotification uint32:1000 uint32:0 string:'im.received' string:'Bill Gates' string:'Hi dude! Whats up?' string:'' string:'' uint32:0

sleep $SLEEPTIME; # wait for a while
echo "******************************************* Closing the lockscreen"
dbus-send --print-reply --system --dest=com.nokia.system_ui /com/nokia/system_ui/request com.nokia.system_ui.request.tklock_close boolean:false

sleep 1; # wait for a very little...
echo "******************************************* Opening it again (should be empty...)"
dbus-send --print-reply --system --dest=com.nokia.system_ui /com/nokia/system_ui/request com.nokia.system_ui.request.tklock_open string: string: string: string: uint32:5 boolean:false boolean:false

sleep $SLEEPTIME; # wait for a while
echo "******************************************* Testing over... bye-bye!"
dbus-send --print-reply --system --dest=com.nokia.system_ui /com/nokia/system_ui/request com.nokia.system_ui.request.tklock_close boolean:false
kill ${SYSUIDPID}

