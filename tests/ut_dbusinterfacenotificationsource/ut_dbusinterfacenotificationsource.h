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

#ifndef UT_DBUSINTERFACENOTIFICATIONSOURCE_H
#define UT_DBUSINTERFACENOTIFICATIONSOURCE_H

#include <QtTest/QtTest>
#include <QObject>

class DBusInterfaceNotificationSource;
class MNotificationProxy;
class MNotificationWithIdentifierProxy;
class MNotificationGroupProxy;
class NotificationManager;

class Ut_DBusInterfaceNotificationSource : public QObject
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

    // Test notification adding
    void testAddNotification();
    void testAddNotificationWithIdentifier();
    // Test notification updating
    void testUpdateNotification();
    void testUpdateNotificationWithIdentifier();
    void testWhenUpdatingNotificationInManagerFailsThenUpdateNotificationReturnsFalse();
    // Test notification removal
    void testRemoveNotification();
    // Test notification group adding
    void testAddGroup();
    void testAddGroupWithIdentifier();
    // Test notification group updating
    void testUpdateGroup();
    void testUpdateGroupWithIdentifier();
    void testWhenUpdatingGroupInManagerFailsThenUpdateGroupReturnsFalse();
    // Test notification group removal
    void testRemoveGroup();
    //Test the query of notifications
    void testNotificationIdList();
    void testNotificationList();
    void testNotificationListWithIdentifiers();
    //Test the query of groups
    void testNotificationGroupList();
    void testNotificationGroupListWithIdentifiers();
    //Test updating group with empty strings
    void testUpdateGroupWithEmptyStrings();
    void testNotificationCountInGroup();

private:
    // Notification manager interface used by the test subject
    NotificationManager *manager;
    // The object being tested
    DBusInterfaceNotificationSource *source;
};

#endif
