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
#ifndef UT_VOLUMEBARLOGIC_H
#define UT_VOLUMEBARLOGIC_H

#include <QObject>

class MApplication;
class VolumeBarLogic;

class Ut_VolumeBarLogic : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void testInitValues();
    void testVolumeSetGet();
    void testVolumeChangeByPa();
    void testWhenPulseAudioSetsVolumeNewVolumeIsSet();
#if (HAVE_LIBRESOURCEQT && HAVE_QMSYSTEM)
    void testHwKeyEvent();
#endif
    
private:
    MApplication *app;
    VolumeBarLogic *volumeBarLogic;
    void resetStubs();

public:
    static bool dbus_message_new_method_call;
    static bool dbus_message_append_args;
    static bool dbus_connection_send_with_reply_and_block;
    static bool dbus_message_iter_recurse;
    static bool dbus_message_iter_get_arg_type;
    static bool dbus_message_iter_get_basic;
    static bool dbus_message_iter_next;
    static bool dbus_connection_get_is_connected;
};

#endif
