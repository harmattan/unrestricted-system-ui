/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-systemui-nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
****************************************************************************/

#ifndef UT_NOTIFICATIONSTATUSINDICATORSINK_H
#define UT_NOTIFICATIONSTATUSINDICATORSINK_H

#include <QObject>
#include <QList>
#include <QString>

#include "notification.h"
#include "notificationparameters.h"

class MApplication;
class NotificationParameters;
class NotificationStatusIndicatorSink;
class MImageWidget;

class TestNotificationParameters : public NotificationParameters
{
public:
    TestNotificationParameters(const QString &image = "", const QVariant &priority = QVariant());
};

class Ut_NotificationStatusIndicatorSink : public QObject
{
    Q_OBJECT

public:
    static QList<QString> images;
    static QList<MImageWidget *> notifications;
    static QList<MImageWidget *> destroyedNotifications;

private:
    MApplication *app;
    NotificationStatusIndicatorSink *sink;

private slots:
    // Executed once before every test case
    void init();
    // Executed once after every test case
    void cleanup();
    // Executed once before first test case
    void initTestCase();
    // Executed once after last test case
    void cleanupTestCase();

    // Test cases
    void testAddNotification();
    void testUpdateNotification();
    void testRemoveNotification();
    void testAddGroup();
    void testAddNotificationToGroup();
    void testUpdateGroup();
    void testRemoveGroup();
    void testRemovingNotificationsWhenNoNotificationLeftGroupIconIsRemoved();
    void testWhenAddingNewNotificationToGroupThatHasBeenPreviouslyClearedThenGroupIconIsConstructedCorrectly();

signals:
    void addGroup(uint groupId, const NotificationParameters &parameters);
    void removeGroup(uint groupId);
    void addNotification(Notification);
    void removeNotification(uint notificationId);
    void removeIcon(uint groupId);
};

#endif // UT_NOTIFICATIONSTATUSINDICATORSINK_H
