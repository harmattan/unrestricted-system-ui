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
#include "ut_volumebarlogic.h"
#include <volumebarlogic.h>
extern "C"
{
#include <dbus/dbus.h>
#include <dbus/dbus-glib-lowlevel.h>
}

bool Ut_VolumeBarLogic::dbus_message_new_method_call = false;
bool Ut_VolumeBarLogic::dbus_message_append_args = false;
bool Ut_VolumeBarLogic::dbus_connection_send_with_reply_and_block = false;
bool Ut_VolumeBarLogic::dbus_message_iter_recurse = false;
bool Ut_VolumeBarLogic::dbus_message_iter_get_arg_type = false;
bool Ut_VolumeBarLogic::dbus_message_iter_get_basic = false;
bool Ut_VolumeBarLogic::dbus_message_iter_next = false;
bool Ut_VolumeBarLogic::dbus_connection_get_is_connected = false;

// Dbus Stubs
extern "C" {
    typedef double DBusMessageRealIter;

    int counter = 0;

    DBusMessage *
    dbus_message_new_method_call(const char *bus_name,
                                 const char *path,
                                 const char *interface,
                                 const char *method)
    {
        Q_UNUSED(bus_name);
        Q_UNUSED(path);
        Q_UNUSED(interface);
        Q_UNUSED(method);
        Ut_VolumeBarLogic::dbus_message_new_method_call = true;
        DBusMessage *dbm = dbus_message_new (DBUS_MESSAGE_TYPE_METHOD_CALL);
        if(dbm == 0)
            QTest::qWarn("DBusMessage == NULL");
        return dbm;
    }


    DBusConnection *
    dbus_connection_open (const char *address,
                          DBusError  *error)
    {
        Q_UNUSED(address);
        Q_UNUSED(error);
        QString addr;
        dbus_error_init(error);
        /**
          * we need to find out if we are in the scratchbox or on the tablet
          * because the dbus-socket's location is different
          * and we need a dbus connection what is not null
          */
        FILE *lsofFile_p = popen("set | grep SBOX | wc -l", "r");

        if (!lsofFile_p) { return NULL; }

        char buffer[1024];
        char *line_p = fgets(buffer, sizeof(buffer), lsofFile_p);
        pclose(lsofFile_p);
        if(atoi(line_p) > 0)
        {
            // in scratchbox
            addr = "unix:path=/var/run/dbus/system_bus_socket";
        }
        else
        {
            // on tablet
            addr = "unix:path=/var/run/pulse/dbus-socket";
        }
        DBusConnection *dbc = dbus_connection_open_private(addr.toAscii(), NULL);;
        return dbc;
    }

    dbus_bool_t
    dbus_message_has_member(DBusMessage  *message, const char *member)
    {
        Q_UNUSED(message);
        Q_UNUSED(member);
        return 1;
    }

    dbus_bool_t
    dbus_message_append_args(DBusMessage *message,
                             int first_arg_type,
                             ...)
    {
        Q_UNUSED(message);
        Q_UNUSED(first_arg_type);
        Ut_VolumeBarLogic::dbus_message_append_args = true;
        return 1;
    }

    DBusMessage *
    dbus_connection_send_with_reply_and_block (DBusConnection *connection,
                                               DBusMessage    *message,
                                               int             timeout_milliseconds,
                                               DBusError      *error)
    {
        Q_UNUSED(connection);
        Q_UNUSED(message);
        Q_UNUSED(timeout_milliseconds);
        Q_UNUSED(error);
        Ut_VolumeBarLogic::dbus_connection_send_with_reply_and_block = true;
        return dbus_message_new (DBUS_MESSAGE_TYPE_METHOD_RETURN);
    }

    void
    dbus_message_iter_recurse (DBusMessageIter *iter,
                               DBusMessageIter *sub)
    {
        Q_UNUSED(iter);
        Q_UNUSED(sub);
        Ut_VolumeBarLogic::dbus_message_iter_recurse = true;
        return;
    }

    int dbus_message_iter_get_arg_type (DBusMessageIter *iter)
    {
        Q_UNUSED(iter);
        Ut_VolumeBarLogic::dbus_message_iter_get_arg_type = true;
        if(counter < 2)
        {
            counter++;
            return DBUS_TYPE_INT32;
        }
        else
        {
            return DBUS_TYPE_INVALID;
        }
    }

    void
    dbus_message_iter_get_basic (DBusMessageIter *iter,
                                 void            *value)
    {
        Q_UNUSED(iter);
        Q_UNUSED(value);
        QTest::qWarn("dbus_message_iter_get_basic");
        Ut_VolumeBarLogic::dbus_message_iter_get_basic = true;
        return;
    }

    dbus_bool_t
    dbus_message_iter_next (DBusMessageIter *iter)
    {
        Q_UNUSED(iter);
        Ut_VolumeBarLogic::dbus_message_iter_next = true;
        return 1;
    }

    dbus_bool_t
    dbus_connection_get_is_connected (DBusConnection *connection)
    {
        Q_UNUSED(connection);
        Ut_VolumeBarLogic::dbus_connection_get_is_connected = true;
        return 0;
    }
}

void
Ut_VolumeBarLogic::init ()
{
}

void
Ut_VolumeBarLogic::cleanup ()
{
}

void
Ut_VolumeBarLogic::initTestCase ()
{
    m_Api = new VolumeBarLogic;
    m_Api->stepsUpdated (30, 100);
    resetStubs();
}

void
Ut_VolumeBarLogic::testInitValues()
{
    m_Api->initValues ();
    QVERIFY(dbus_message_append_args);
    QVERIFY(dbus_connection_send_with_reply_and_block);
    QVERIFY(dbus_message_iter_recurse);
    QVERIFY(dbus_message_iter_get_arg_type);
    QVERIFY(dbus_message_iter_get_basic);
    QVERIFY(dbus_message_iter_next);
    resetStubs();
}


// Check if setting / getting works correctly
void
Ut_VolumeBarLogic::testVolumeSetGet ()
{
    quint32 val = 5;

    m_Api->setVolume (val);

    QVERIFY (m_Api->getVolume () == val)  ;
}

void
Ut_VolumeBarLogic::testVolumeChangeByPa ()
{
    quint32 currentstep = 10;
    quint32 stepcount   = 20;

    // A D-bus message calls this slot [inside the logic],
    // now we call this outside :
    m_Api->stepsUpdated (currentstep, stepcount);

    // Check the current ...
    QVERIFY (m_Api->getVolume () == currentstep);
    // .. and the maximal values
    QVERIFY (m_Api
             ->getMaxVolume () == stepcount);
}

void
Ut_VolumeBarLogic::testSignaling ()
{
    quint32 currentstep = 0;
    quint32 stepcount   = 13;

    // Whe PulseAudio sends the D-Bus signal about stepcount/currentstep
    // changes logic should emit itself volumeChanged signal
    QSignalSpy spy (m_Api, SIGNAL (volumeChanged (quint32, quint32, bool)));

    // Do what PulseAudio do [of course Pa doing this indirectly...]
    m_Api->stepsUpdated (currentstep, stepcount);

    QTest::qWait (500); // wait for a little time

    QList<QVariant> arguments = spy.takeFirst ();
    // Verify the signal parameters
    QVERIFY (arguments.at (0).toUInt () == currentstep);
    QVERIFY (arguments.at (1).toUInt () == stepcount);
}

void
Ut_VolumeBarLogic::testPing()
{
    QSignalSpy spy (m_Api, SIGNAL (volumeChanged (quint32, quint32, bool)));

    m_Api->ping();

    QList<QVariant> arguments = spy.takeFirst ();

    QVERIFY (arguments.at (0).toUInt () == m_Api->getVolume ());
    QVERIFY (arguments.at (1).toUInt () == m_Api->getMaxVolume ());
}

void
Ut_VolumeBarLogic::cleanupTestCase ()
{
    delete m_Api;
}

void Ut_VolumeBarLogic::resetStubs()
{
    counter = 0;
    dbus_message_new_method_call = false;
    dbus_message_append_args = false;
    dbus_connection_send_with_reply_and_block = false;
    dbus_message_iter_recurse = false;
    dbus_message_iter_get_arg_type = false;
    dbus_message_iter_get_basic = false;
    dbus_message_iter_next = false;
    dbus_connection_get_is_connected = false;
}

QTEST_MAIN(Ut_VolumeBarLogic)
