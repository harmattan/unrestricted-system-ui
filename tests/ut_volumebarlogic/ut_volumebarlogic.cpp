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
#include <QtTest/QtTest>
#include <QDBusConnection>
#include <MApplication>
#include <MApplicationService>
#include <MWindow>
#include <MGConfItem>
#include <mcomponentdata.h>
#include <mfeedbackplayer.h>
#include "ut_volumebarlogic.h"
#include "volumebarlogic.h"
#include "volumebarwindow_stub.h"
#include "closeeventeater_stub.h"
extern "C"
{
#include <dbus/dbus.h>
#include <dbus/dbus-glib-lowlevel.h>
}

#if (HAVE_LIBRESOURCEQT && HAVE_QMSYSTEM)
#include <policy/resource-set.h>
/*********************************************************************************
 * Stub for ResourcePolicy
 */

namespace ResourcePolicy
{
    ResourceSet::ResourceSet(const QString&, QObject*)
    {
    }

    bool ResourceSet::setAlwaysReply ()
    {
        return true;
    }

    ScaleButtonResource::ScaleButtonResource()
    {
    }

    void ResourceSet::addResourceObject (ResourcePolicy::Resource* rsc)
    {
        Q_UNUSED(rsc);
        return;
    }

    bool ResourceSet::acquire ()
    {
        return true;
    }

    void ResourceSet::deleteResource (ResourcePolicy::ResourceType)
    {
        return;
    }
}

#include <qmkeys.h>
/*********************************************************************************
 * Stub for MeeGo::QmKeys
 */
namespace MeeGo
{
    QmKeys::QmKeys(QObject *parent)
    {
        Q_UNUSED(parent);
    }

    QmKeys::~QmKeys()
    {
    }
}
#endif

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

    DBusMessage * dbus_message_new_method_call (
        const char *bus_name,
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


    DBusConnection * dbus_connection_open (const char *address, DBusError *error)
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

    dbus_bool_t dbus_message_has_member(DBusMessage *message, const char *member)
    {
        Q_UNUSED(message);
        Q_UNUSED(member);
        return 1;
    }

    dbus_bool_t dbus_message_append_args(DBusMessage *message, int first_arg_type, ...)
    {
        Q_UNUSED(message);
        Q_UNUSED(first_arg_type);
        Ut_VolumeBarLogic::dbus_message_append_args = true;
        return 1;
    }

    DBusMessage * dbus_connection_send_with_reply_and_block (
        DBusConnection *connection,
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

    void dbus_message_iter_recurse (
        DBusMessageIter *iter,
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

    void dbus_message_iter_get_basic (DBusMessageIter *iter, void *value)
    {
        Q_UNUSED(iter);
        Q_UNUSED(value);
        QTest::qWarn("dbus_message_iter_get_basic");
        Ut_VolumeBarLogic::dbus_message_iter_get_basic = true;
        return;
    }

    dbus_bool_t dbus_message_iter_next (DBusMessageIter *iter)
    {
        Q_UNUSED(iter);
        Ut_VolumeBarLogic::dbus_message_iter_next = true;
        return 1;
    }

    dbus_bool_t dbus_connection_get_is_connected (DBusConnection *connection)
    {
        Q_UNUSED(connection);
        Ut_VolumeBarLogic::dbus_connection_get_is_connected = true;
        return 0;
    }

    DBusDispatchStatus dbus_connection_get_dispatch_status (DBusConnection *)
    {
        return DBUS_DISPATCH_COMPLETE;
    }
}

bool QDBusConnection::isConnected() const
{
    return true;
}

void MApplicationService::handleServiceRegistrationFailure()
{
}

MWindow::MWindow(QWidget *) : d_ptr(NULL)
{
}

MWindow::~MWindow()
{
}

void MWindow::setVisible(bool)
{
}

MGConfItem::MGConfItem(const QString &, QObject *parent) : QObject(parent)
{
}

MGConfItem::~MGConfItem()
{
}

QVariant MGConfItem::value(const QVariant &def) const
{
    return def;
}

QVariant MGConfItem::value() const
{
    return QVariant();
}

QString MGConfItem::key() const
{
    return QString();
}

void Ut_VolumeBarLogic::initTestCase ()
{
    static int argc = 2;
    static char *argv[] = { (char *)"./ut_volumebarwindow", (char *)"-disable-m-input-context", 0 };
    app = new MApplication(argc, argv);
}

void Ut_VolumeBarLogic::cleanupTestCase ()
{
}

void Ut_VolumeBarLogic::init ()
{
    volumeBarLogic = new VolumeBarLogic;
    volumeBarLogic->stepsUpdated (30, 100);
    resetStubs();
}

void Ut_VolumeBarLogic::cleanup ()
{
    delete volumeBarLogic;
}

void Ut_VolumeBarLogic::testInitValues()
{
    volumeBarLogic->initValues ();
    QVERIFY(dbus_message_append_args);
    QVERIFY(dbus_connection_send_with_reply_and_block);
    QVERIFY(dbus_message_iter_recurse);
    QVERIFY(dbus_message_iter_get_arg_type);
    QVERIFY(dbus_message_iter_get_basic);
    QVERIFY(dbus_message_iter_next);
    resetStubs();
}


// Check if setting / getting works correctly
void Ut_VolumeBarLogic::testVolumeSetGet ()
{
    quint32 val = 5;

    volumeBarLogic->setVolume (val);

    QCOMPARE (volumeBarLogic->volume (), val);
}

void Ut_VolumeBarLogic::testVolumeChangeByPa ()
{
    quint32 currentstep = 10;
    quint32 stepcount   = 20;

    // A D-bus message calls this slot [inside the logic],
    // now we call this outside :
    volumeBarLogic->stepsUpdated (currentstep, stepcount);

    // Check the current ...
    QCOMPARE (volumeBarLogic->volume (), currentstep);
    // .. and the maximal values
    QCOMPARE (volumeBarLogic->maxVolume (), stepcount);
}

void Ut_VolumeBarLogic::testWhenPulseAudioSetsVolumeNewVolumeIsSet()
{
    // Do what PulseAudio do [of course Pa doing this indirectly...]
    volumeBarLogic->stepsUpdated (10, 100);
    QCOMPARE(volumeBarLogic->currentvolume, (uint)10);
    QCOMPARE(volumeBarLogic->currentmax, (uint)100);
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

#if (HAVE_LIBRESOURCEQT && HAVE_QMSYSTEM)
void Ut_VolumeBarLogic::testHwKeyEvent()
{
    // when current_volume >= max_volume
    volumeBarLogic->setVolume(120);
    volumeBarLogic->hwKeyEvent(MeeGo::QmKeys::VolumeUp, MeeGo::QmKeys::KeyDown);
    QCOMPARE(volumeBarLogic->volume(), 99u);

    // Turn up the volume
    volumeBarLogic->setVolume(20);
    volumeBarLogic->hwKeyEvent(MeeGo::QmKeys::VolumeUp, MeeGo::QmKeys::KeyDown);
    QCOMPARE(volumeBarLogic->volume(), 21u);

    // Turn down the volume
    volumeBarLogic->setVolume(20);
    volumeBarLogic->hwKeyEvent(MeeGo::QmKeys::VolumeDown, MeeGo::QmKeys::KeyDown);
    QCOMPARE(volumeBarLogic->volume(), 19u);

    volumeBarLogic->setVolume(20);
    volumeBarLogic->hwKeyEvent(MeeGo::QmKeys::VolumeUp, MeeGo::QmKeys::KeyUp);
    QCOMPARE(volumeBarLogic->volume(), 20u);

    // push some other hw key
    volumeBarLogic->setVolume(20);
    volumeBarLogic->hwKeyEvent(MeeGo::QmKeys::Camera, MeeGo::QmKeys::KeyDown);
    QCOMPARE(volumeBarLogic->volume(), 20u);
}
#endif


QTEST_APPLESS_MAIN(Ut_VolumeBarLogic)
