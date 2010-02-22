#!/bin/sh
# This file is part of ke-recv
#
# Copyright (C) 2008-2010 Nokia Corporation. All rights reserved.
#
# Contact: Kimmo Hämäläinen <kimmo.hamalainen@nokia.com>
# Modified for Harmattan: David Kedves <dkedves@blumsoft.eu>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License 
# version 2 as published by the Free Software Foundation. 
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
# 02110-1301 USA

/sbin/lsmod | grep g_file_storage > /dev/null
if [ $? = 0 ]; then
    logger "$0: removing g_file_storage"
    initctl emit G_FILE_STORAGE_REMOVE
    /sbin/rmmod g_file_storage
fi

/sbin/lsmod | grep g_nokia > /dev/null
if [ $? != 0 ]; then
    /sbin/modprobe g_nokia
    RC=$?
fi

if [ $RC != 0 ]; then
    logger "$0: failed to install g_nokia"
    exit 1
fi

initctl emit --no-wait G_NOKIA_READY

# Wait until daemons are up
INC=1
#while [ "x$(pidof pnatd)x" = "xx" -o "x$(pidof obexd)x" = "xx" \
#       -o "x$(pidof syncd)x" = "xx" ]; do
while [ "x$(pidof obexd)x" = "xx" ]; do
    if [ $INC -gt 20 ]; then
      echo "$0: Error, daemons did not start"
      logger "$0: Error, daemons did not start"
      exit 1
    fi
    sleep 1
    INC=`expr $INC + 1`
done

OBEXD_PID=`pidof obexd`
if [ $? != 0 ]; then
    logger "$0: obexd is not running"
else
    kill -USR1 $OBEXD_PID
    logger "$0: sent SIGUSR1 to obexd"
fi

#SYNCD_PID=`pidof syncd`
#if [ $? != 0 ]; then
#    logger "$0: failed to get syncd's PID"
#    exit 1
#fi

#kill -USR1 $SYNCD_PID
#logger "$0: sent SIGUSR1 to syncd"

exit 0

