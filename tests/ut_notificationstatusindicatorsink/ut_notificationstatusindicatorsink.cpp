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

#include "ut_notificationstatusindicatorsink.h"
#include "notificationstatusindicatorsink.h"

#include <QtTest/QtTest>
#include <MImageWidget>
#include <MApplication>
#include "notification.h"
#include "notificationgroup_stub.h"
#include "notificationmanager_stub.h"

TestNotificationParameters::TestNotificationParameters(const QString &image, const QVariant &priority)
{
    if (!image.isEmpty()) {
        add("statusAreaIconId", image);
    }

    if (priority.isValid()) {
        add("priority", priority);
    }
}

static char *gAppName = (char *)"./ut_notificationstatusindicatorsink";

// QCoreApplication stubs to avoid crashing in processEvents()
QStringList QCoreApplication::arguments()
{
    return QStringList(gAppName);
}

// Tests
void Ut_NotificationStatusIndicatorSink::initTestCase()
{
    // Create a MAapplication
    static int argc = 1;
    app = new MApplication(argc, &gAppName);
}

void Ut_NotificationStatusIndicatorSink::cleanupTestCase()
{
    // Destroy MApplication
    delete app;
}

void Ut_NotificationStatusIndicatorSink::init()
{
    sink = new NotificationStatusIndicatorSink();
    connect(this, SIGNAL(addNotification(Notification)), sink, SLOT(addNotification(Notification)));
    connect(this, SIGNAL(removeNotification(uint)), sink, SLOT(removeNotification(uint)));
    connect(this, SIGNAL(addGroup(uint, const NotificationParameters &)), sink, SLOT(addGroup(uint, const NotificationParameters &)));
    connect(this, SIGNAL(removeGroup(uint)), sink, SLOT(removeGroup(uint)));
}

void Ut_NotificationStatusIndicatorSink::cleanup()
{
    delete sink;
}
void Ut_NotificationStatusIndicatorSink::testAddNotification()
{
    QSignalSpy spy(sink, SIGNAL(iconIdChanged(QString)));

    // Create four notifications
    TestNotificationParameters parameters0("", 10);
    TestNotificationParameters parameters1("icon1", 11);
    TestNotificationParameters parameters2("icon2", 13);
    TestNotificationParameters parameters3("icon3", 12);
    emit addNotification(Notification(0, 0, 2, parameters0, Notification::ApplicationEvent, 1000));
    emit addNotification(Notification(1, 0, 2, parameters1, Notification::SystemEvent, 1000));
    emit addNotification(Notification(2, 0, 2, parameters2, Notification::ApplicationEvent, 1000));
    emit addNotification(Notification(3, 0, 2, parameters3, Notification::ApplicationEvent, 1000));

    // Check that system notification are not taken into account
    QCOMPARE(spy.count(), 3);

    // Check that no defining no icon ID uses the default icon
    QCOMPARE(spy.at(0).at(0).toString(), QString("icon-s-status-notifier"));

    // Check that higher priority overrides lower priority
    QCOMPARE(spy.at(1).at(0).toString(), QString("icon2"));
    QCOMPARE(spy.at(2).at(0).toString(), QString("icon2"));
}

void Ut_NotificationStatusIndicatorSink::testUpdateNotification()
{
    QSignalSpy spy(sink, SIGNAL(iconIdChanged(QString)));

    // Add two notifications with the same ID
    TestNotificationParameters parameters0("", 10);
    TestNotificationParameters parameters1("icon1", 11);
    emit addNotification(Notification(0, 0, 2, parameters0, Notification::ApplicationEvent, 1000));
    emit addNotification(Notification(0, 0, 2, parameters1, Notification::ApplicationEvent, 1000));

    // The second should update the existing one
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(0).at(0).toString(), QString("icon-s-status-notifier"));
    QCOMPARE(spy.at(1).at(0).toString(), QString("icon1"));
}

void Ut_NotificationStatusIndicatorSink::testRemoveNotification()
{
    QSignalSpy spy(sink, SIGNAL(iconIdChanged(QString)));

    // Create three notifications
    TestNotificationParameters parameters0("icon0");
    TestNotificationParameters parameters1("icon1", 11);
    TestNotificationParameters parameters2("icon2", 10);
    emit addNotification(Notification(0, 0, 2, parameters0, Notification::SystemEvent, 1000));
    emit addNotification(Notification(1, 0, 2, parameters1, Notification::ApplicationEvent, 1000));
    emit addNotification(Notification(2, 0, 2, parameters2, Notification::ApplicationEvent, 1000));

    // Check that expected ID is signaled
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(0).at(0).toString(), QString("icon1"));
    QCOMPARE(spy.at(1).at(0).toString(), QString("icon1"));

    // Remove the second one
    emit removeNotification(1);

    // Check that the lower priority one is now the most important one
    QCOMPARE(spy.count(), 3);
    QCOMPARE(spy.at(2).at(0).toString(), QString("icon2"));

    // Recreate the second notification and create an additional one
    TestNotificationParameters parameters3("icon3");
    emit addNotification(Notification(1, 0, 2, parameters1, Notification::ApplicationEvent, 1000));
    emit addNotification(Notification(3, 0, 2, parameters3, Notification::SystemEvent, 1000));

    // Check that the higher priority one still overrides the lower priority one
    QCOMPARE(spy.count(), 4);
    QCOMPARE(spy.at(3).at(0).toString(), QString("icon1"));
}

void Ut_NotificationStatusIndicatorSink::testAddGroup()
{
    QSignalSpy spy(sink, SIGNAL(iconIdChanged(QString)));

    // Creating an empty group should not send signals
    TestNotificationParameters parameters0("icon0");
    emit addGroup(1, parameters0);
    QCOMPARE(spy.count(), 0);
}

void Ut_NotificationStatusIndicatorSink::testAddNotificationToGroup()
{
    QSignalSpy spy(sink, SIGNAL(iconIdChanged(QString)));
    TestNotificationParameters parameters0("icon0", 10);
    emit addGroup(1, parameters0);

    // Check that the group icon ID is used when the group contains notifications
    TestNotificationParameters parameters1("icon1", 11);
    emit addNotification(Notification(0, 1, 2, parameters1, Notification::ApplicationEvent, 1000));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), QString("icon0"));
}

void Ut_NotificationStatusIndicatorSink::testUpdateGroup()
{
    QSignalSpy spy(sink, SIGNAL(iconIdChanged(QString)));

    TestNotificationParameters parameters0("icon0", 10);
    TestNotificationParameters parameters1("icon1", 11);
    TestNotificationParameters parameters2("icon2", 11);
    emit addGroup(1, parameters0);
    emit addNotification(Notification(0, 1, 2, parameters1, Notification::ApplicationEvent, 1000));

    // After updating the group the new group icon ID should be used
    emit addGroup(1, parameters2);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toString(), QString("icon2"));
}

void Ut_NotificationStatusIndicatorSink::testRemoveGroup()
{
    QSignalSpy spy(sink, SIGNAL(iconIdChanged(QString)));

    // No signals are sent if no notification has been added to the group
    TestNotificationParameters parameters0("icon0");
    emit addGroup(1, parameters0);
    emit removeGroup(1);
    QCOMPARE(spy.count(), 0);

    // Remove a non-empty group
    TestNotificationParameters parameters1("icon1");
    emit addGroup(1, parameters0);
    emit addNotification(Notification(0, 1, 2, parameters1, Notification::ApplicationEvent, 1000));
    emit removeGroup(1);

    // Check that removing a non-empty group removes its icon from the status indicator
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(0).at(0).toString(), QString("icon0"));
    QCOMPARE(spy.at(1).at(0).toString(), QString());
}

void Ut_NotificationStatusIndicatorSink::testRemovingNotificationsWhenNoNotificationLeftGroupIconIsRemoved()
{
    QSignalSpy spy(sink, SIGNAL(iconIdChanged(QString)));

    TestNotificationParameters parameters0("icon0");
    emit addGroup(1, parameters0);

    // Add two notifications to this group
    TestNotificationParameters parameters1("icon1");
    emit addNotification(Notification(0, 1, 2, parameters1, Notification::ApplicationEvent, 1000));
    emit addNotification(Notification(1, 1, 2, parameters1, Notification::ApplicationEvent, 1000));

    // Remove first notification
    emit(removeNotification(0));

    // There is still one notification in the group so icon is not removed.
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toString(), QString("icon0"));

    // Last notification in group removed, so icon will be removed
    emit(removeNotification(1));
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toString(), QString());
}

void Ut_NotificationStatusIndicatorSink::testWhenAddingNewNotificationToGroupThatHasBeenPreviouslyClearedThenGroupIconIsConstructedCorrectly()
{
    const uint GROUP_ID = 1;
    const uint NOTIFICATION_ID = 1;
    const QString GROUP_ICON("groupIcon");
    const QString NOTIFICATION_ICON("notificationIcon");

    // Create a non-empty group, make it empty and add a notification to it again
    QSignalSpy spy(sink, SIGNAL(iconIdChanged(QString)));
    TestNotificationParameters groupParameters(GROUP_ICON);
    TestNotificationParameters notificationParameters(NOTIFICATION_ICON);
    emit addGroup(GROUP_ID, groupParameters);
    emit addNotification(Notification(NOTIFICATION_ID, GROUP_ID, 2, notificationParameters, Notification::ApplicationEvent, 1000));
    emit removeNotification(NOTIFICATION_ID);
    emit addNotification(Notification(NOTIFICATION_ID, GROUP_ID, 2, notificationParameters, Notification::ApplicationEvent, 1000));

    // The icon should be the notification group's icon
    QCOMPARE(spy.count(), 3);
    QCOMPARE(spy.at(2).at(0).toString(), QString(GROUP_ICON));
}

QTEST_APPLESS_MAIN(Ut_NotificationStatusIndicatorSink)
