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

#ifndef UT_DBUSINTERFACENOTIFICATIONSINK_H
#define UT_DBUSINTERFACENOTIFICATIONSINK_H

#include <QtTest/QtTest>
#include <QObject>
#include "notification.h"

#include <QDBusAbstractInterface>
#include <QDBusPendingReply>

class NotificationManager;

class DBusInterfaceNotificationSinkProxy: public QDBusAbstractInterface
{
    Q_OBJECT

public:
    DBusInterfaceNotificationSinkProxy(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);
    ~DBusInterfaceNotificationSinkProxy() {}

public Q_SLOTS:
    QDBusPendingReply<> addGroup(uint groupId, NotificationParameters parameters);
    QDBusPendingReply<> addNotification(Notification notification);
    QDBusPendingReply<> removeGroup(uint groupId);
    QDBusPendingReply<> removeNotification(uint notificationId);
};

class DBusInterfaceNotificationSink;

class Ut_DBusInterfaceNotificationSink : public QObject
{
    Q_OBJECT

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test cases
    void testNothingCalledWhenNothingRegistered();
    void testProxyCalledWhenServiceRegistered();
    void testSignalsConnectedWhenServiceRegistered();
    void testRegisteringSameServiceAndPathReplacesPrevious();
    void testRegisteringTwoDifferentServicesCallsBoth();
    void testUnregistering();
    void testNotificationsAndGroupsFetchedWhenProxyRegisters();
    void testSendingGroupsToProxy();
    void testSendingNotificationsToProxy();
    void testManagerNotDefined();

signals:
    void addNotification(Notification n);
    void addGroup(uint groupId, NotificationParameters parameters);
    void removeGroup(uint groupId);
    void removeNotification(uint notificationId);

private:
    // The object being tested
    DBusInterfaceNotificationSink *sink;
    //Stubbed manager
    NotificationManager *manager;
};

#endif
