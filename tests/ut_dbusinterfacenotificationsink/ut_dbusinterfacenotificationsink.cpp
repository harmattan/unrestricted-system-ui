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
#include "ut_dbusinterfacenotificationsink.h"
#include "dbusinterfacenotificationsink.h"
#include "dbusinterfacenotificationsinkadaptor.h"
#include "genericnotificationparameterfactory.h"
#include "notificationwidgetparameterfactory.h"
#include "notification.h"
#include "notificationgroup.h"
#include "notificationparameters.h"
#include "notificationmanager_stub.h"
#include "notificationgroup_stub.h"

NotificationParameters fakeParams;

// DBusInterfaceNotificationSinkAdaptor stubs (used by NotificationManager)
DBusInterfaceNotificationSinkAdaptor::DBusInterfaceNotificationSinkAdaptor(DBusInterfaceNotificationSink *parent) : QDBusAbstractAdaptor(parent)
{
}

DBusInterfaceNotificationSinkAdaptor::~DBusInterfaceNotificationSinkAdaptor()
{
}

void DBusInterfaceNotificationSinkAdaptor::registerSink(QString const&, QString const&)
{
}

void DBusInterfaceNotificationSinkAdaptor::unregisterSink(QString const&, QString const&)
{
}

QList<DBusInterfaceNotificationSinkProxy*> gNewSinkProxies;
QList<DBusInterfaceNotificationSinkProxy*> gAddNotificationProxies;
QList<DBusInterfaceNotificationSinkProxy*> gAddGroupProxies;
QList<DBusInterfaceNotificationSinkProxy*> gRemoveNotificationProxies;
QList<DBusInterfaceNotificationSinkProxy*> gRemoveGroupProxies;


// DBusInterfaceNotificationSinkProxy stubs
DBusInterfaceNotificationSinkProxy::DBusInterfaceNotificationSinkProxy(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent) :
        QDBusAbstractInterface(service, path, "foo", connection, parent)
{
    gNewSinkProxies.append(this);
}

QDBusPendingReply<> DBusInterfaceNotificationSinkProxy::addNotification(Notification /*notification*/)
{
    gAddNotificationProxies.append(this);

    return QDBusPendingReply<>();
}

QDBusPendingReply<> DBusInterfaceNotificationSinkProxy::removeNotification(uint /*notificationId*/)
{
    gRemoveNotificationProxies.append(this);

    return QDBusPendingReply<>();
}

QDBusPendingReply<> DBusInterfaceNotificationSinkProxy::addGroup(uint /*groupId*/, NotificationParameters /*parameters*/)
{
    gAddGroupProxies.append(this);

    return QDBusPendingReply<>();
}

QDBusPendingReply<> DBusInterfaceNotificationSinkProxy::removeGroup(uint /*groupId*/)
{
    gRemoveGroupProxies.append(this);

    return QDBusPendingReply<>();
}

QList<QString> qDBusConnectionConnectService;
QList<QString> qDBusConnectionConnectPath;
QList<QString> qDBusConnectionConnectInterface;
QList<QString> qDBusConnectionConnectName;
QList<QObject *> qDBusConnectionConnectReceiver;
QList<const char *> qDBusConnectionConnectSlot;
bool QDBusConnection::connect(const QString &service, const QString &path, const QString &interface, const QString &name, QObject *receiver, const char *slot)
{
    qDBusConnectionConnectService.append(service);
    qDBusConnectionConnectPath.append(path);
    qDBusConnectionConnectInterface.append(interface);
    qDBusConnectionConnectName.append(name);
    qDBusConnectionConnectReceiver.append(receiver);
    qDBusConnectionConnectSlot.append(slot);
    return true;
}


void Ut_DBusInterfaceNotificationSink::initTestCase()
{
}

void Ut_DBusInterfaceNotificationSink::cleanupTestCase()
{
}

void Ut_DBusInterfaceNotificationSink::init()
{
    qDBusConnectionConnectService.clear();
    qDBusConnectionConnectPath.clear();
    qDBusConnectionConnectInterface.clear();
    qDBusConnectionConnectName.clear();
    qDBusConnectionConnectReceiver.clear();
    qDBusConnectionConnectSlot.clear();

    manager = new NotificationManager;
    sink = new DBusInterfaceNotificationSink(manager);

    connect(this, SIGNAL(addNotification(Notification)), sink, SLOT(addNotification(Notification)));
    connect(this, SIGNAL(addGroup(uint,NotificationParameters)), sink, SLOT(addGroup(uint,NotificationParameters)));
    connect(this, SIGNAL(removeNotification(uint)), sink, SLOT(removeNotification(uint)));
    connect(this, SIGNAL(removeGroup(uint)), sink, SLOT(removeGroup(uint)));
    gNotificationManagerStub->stubReset();

    gNotificationGroupStub->stubReset();
    gNotificationGroupStub->stubSetReturnValue<const NotificationParameters&>("parameters", fakeParams);
}

void Ut_DBusInterfaceNotificationSink::cleanup()
{
    delete sink;
    delete manager;
    gAddNotificationProxies.clear();
    gNewSinkProxies.clear();
    gAddGroupProxies.clear();
}

void Ut_DBusInterfaceNotificationSink::testNothingCalledWhenNothingRegistered()
{
    Notification n;
    NotificationParameters np;

    emit addNotification(n);
    emit addGroup(0, np);
    emit removeNotification(0);
    emit removeGroup(0);

    QCOMPARE(gAddNotificationProxies.count(), 0);
    QCOMPARE(gAddGroupProxies.count(), 0);
    QCOMPARE(gRemoveNotificationProxies.count(), 0);
    QCOMPARE(gRemoveGroupProxies.count(), 0);
}


void Ut_DBusInterfaceNotificationSink::testProxyCalledWhenServiceRegistered()
{
    Notification n;
    NotificationParameters np;

    sink->registerSink("service1", "path");
    QCOMPARE(gNewSinkProxies.count(), 1);

    emit addNotification(n);
    emit addGroup(0, np);
    emit removeNotification(0);
    emit removeGroup(0);

    QCOMPARE(gAddNotificationProxies.count(), 1);
    QCOMPARE(gAddNotificationProxies.at(0), gNewSinkProxies.at(0));
    QCOMPARE(gAddGroupProxies.count(), 1);
    QCOMPARE(gAddGroupProxies.at(0), gNewSinkProxies.at(0));
    QCOMPARE(gRemoveNotificationProxies.count(), 1);
    QCOMPARE(gRemoveNotificationProxies.at(0), gNewSinkProxies.at(0));
    QCOMPARE(gRemoveGroupProxies.count(), 1);
    QCOMPARE(gRemoveGroupProxies.at(0), gNewSinkProxies.at(0));
}

void Ut_DBusInterfaceNotificationSink::testSignalsConnectedWhenServiceRegistered()
{
    sink->registerSink("service1", "path");

    QCOMPARE(qDBusConnectionConnectService.count(), 2);
    QCOMPARE(qDBusConnectionConnectService.at(0), QString("service1"));
    QCOMPARE(qDBusConnectionConnectPath.at(0), QString("path"));
    QCOMPARE(qDBusConnectionConnectInterface.at(0), QString("com.meego.core.MNotificationSink"));
    QCOMPARE(qDBusConnectionConnectName.at(0), QString("notificationRemovalRequested"));
    QCOMPARE(qDBusConnectionConnectReceiver.at(0), sink);
    QCOMPARE(qDBusConnectionConnectSlot.at(0), SIGNAL(notificationRemovalRequested(uint)));
    QCOMPARE(qDBusConnectionConnectService.at(1), QString("service1"));
    QCOMPARE(qDBusConnectionConnectPath.at(1), QString("path"));
    QCOMPARE(qDBusConnectionConnectInterface.at(1), QString("com.meego.core.MNotificationSink"));
    QCOMPARE(qDBusConnectionConnectName.at(1), QString("notificationGroupClearingRequested"));
    QCOMPARE(qDBusConnectionConnectReceiver.at(1), sink);
    QCOMPARE(qDBusConnectionConnectSlot.at(1), SIGNAL(notificationGroupClearingRequested(uint)));
}

void Ut_DBusInterfaceNotificationSink::testRegisteringSameServiceAndPathReplacesPrevious()
{
    Notification n;

    sink->registerSink("service1", "path");
    QCOMPARE(gNewSinkProxies.count(), 1);

    sink->registerSink("service1", "path");
    QCOMPARE(gNewSinkProxies.count(), 2);

    emit addNotification(n);

    QCOMPARE(gAddNotificationProxies.count(), 1);
    QCOMPARE(gAddNotificationProxies.at(0), gNewSinkProxies.at(1));
}

void Ut_DBusInterfaceNotificationSink::testRegisteringTwoDifferentServicesCallsBoth()
{
    Notification n;

    sink->registerSink("service1", "path");
    QCOMPARE(gNewSinkProxies.count(), 1);

    sink->registerSink("service2", "path");
    QCOMPARE(gNewSinkProxies.count(), 2);

    emit addNotification(n);
    QCOMPARE(gAddNotificationProxies.count(), 2);
    QVERIFY(gAddNotificationProxies.contains(gNewSinkProxies.at(0)));
    QVERIFY(gAddNotificationProxies.contains(gNewSinkProxies.at(1)));
}

void Ut_DBusInterfaceNotificationSink::testUnregistering()
{
    Notification n;

    sink->registerSink("service1", "path");
    QCOMPARE(gNewSinkProxies.count(), 1);

    sink->registerSink("service2", "path");
    QCOMPARE(gNewSinkProxies.count(), 2);

    sink->unregisterSink("service1", "path");

    emit addNotification(n);

    QCOMPARE(gAddNotificationProxies.count(), 1);
    QCOMPARE(gAddNotificationProxies.at(0), gNewSinkProxies.at(1));
}

QList<NotificationGroup> createGroups()
{
    NotificationGroup ng;
    QList<NotificationGroup> ngs;
    ngs.append(ng);
    return ngs;
}

QList<Notification> createNotifications()
{
    Notification n1(1, 2, 3, NotificationParameters(), Notification::ApplicationEvent, 4);
    Notification n2(5, 6, 7, NotificationParameters(), Notification::SystemEvent, 8);
    QList<Notification> ns;
    ns.append(n1);
    ns.append(n2);
    return ns;
}

void Ut_DBusInterfaceNotificationSink::testNotificationsAndGroupsFetchedWhenProxyRegisters()
{
    sink->registerSink("service1", "path");
    QCOMPARE(gNotificationManagerStub->stubCallCount("groups"), 1);
    QCOMPARE(gNotificationManagerStub->stubCallCount("notifications"), 1);
}


void Ut_DBusInterfaceNotificationSink::testSendingGroupsToProxy()
{
    QList<Notification> notifications;
    gNotificationManagerStub->stubSetReturnValue("notifications", notifications);

    QList<NotificationGroup> groups(createGroups());
    gNotificationManagerStub->stubSetReturnValue("groups", groups);

    sink->registerSink("service1", "path");

    QCOMPARE(gAddGroupProxies.count(), 1);
}

void Ut_DBusInterfaceNotificationSink::testSendingNotificationsToProxy()
{

    QList<Notification> notifications(createNotifications());
    gNotificationManagerStub->stubSetReturnValue("notifications", notifications);

    QList<NotificationGroup> groups;
    gNotificationManagerStub->stubSetReturnValue("groups", groups);

    sink->registerSink("service1", "path");

    QCOMPARE(gAddNotificationProxies.count(), 1);
}

void Ut_DBusInterfaceNotificationSink::testManagerNotDefined()
{
    delete sink;
    sink = new DBusInterfaceNotificationSink(NULL);
    QList<NotificationGroup> groups;
    gNotificationManagerStub->stubSetReturnValue("groups", groups);

    QList<Notification> notifications(createNotifications());
    gNotificationManagerStub->stubSetReturnValue("notifications", notifications);

    sink->registerSink("service1", "path");

    QCOMPARE(gNotificationManagerStub->stubCallCount("groups"), 0);
    QCOMPARE(gNotificationManagerStub->stubCallCount("notifications"), 0);
}


QTEST_APPLESS_MAIN(Ut_DBusInterfaceNotificationSink)
