#!/bin/sh
#
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
#***************************************************************************/
# This script just for testing the sysuid ShutdownUi inside scratchbox 1
#

source /tmp/session_bus_address.user
sysuid -software &

SYSUIDPID=$!
sleep 3; # wait for sysuid startup

METHOD="com.nokia.systemui.shutdownui.showScreen"

echo "Test the show dialog [with 5000 msec timeout....]"
dbus-send --session --type=method_call --dest=com.nokia.systemui / $METHOD string:"Hello world!" string:"Waiting for 5 seconds..." int32:5000
sleep 8;

kill ${SYSUIDPID}

