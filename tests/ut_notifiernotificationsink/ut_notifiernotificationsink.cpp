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

#include "ut_notifiernotificationsink.h"
#include "notifiernotificationsink.h"
#include "notification.h"
#include "sysuid_stub.h"
#include "notificationmanager_stub.h"
#include "eventtypestore_stub.h"
#include "notificationgroup_stub.h"
#include "genericnotificationparameterfactory.h"

void Ut_NotifierNotificationSink::initTestCase()
{
}

void Ut_NotifierNotificationSink::cleanupTestCase()
{
}

void Ut_NotifierNotificationSink::init()
{
    m_subject = new NotifierNotificationSink();
    connect(this, SIGNAL(addNotification(const Notification &)), m_subject, SLOT(addNotification(const Notification &)));
    connect(this, SIGNAL(removeNotification(uint)), m_subject, SLOT(removeNotification(uint)));
    connect(this, SIGNAL(removeGroup(uint)), m_subject, SLOT(removeGroup(uint)));

    gNotificationManagerStub->stubReset();
}

void Ut_NotifierNotificationSink::cleanup()
{
    delete m_subject;
}

void Ut_NotifierNotificationSink::testAddNotification()
{
    QSignalSpy spy(m_subject, SIGNAL(notifierSinkActive(bool)));

    // Adding a notification should cause a notification count change
    NotificationParameters notification1Parameters;
    Notification notification1(1, 0, 2, notification1Parameters, Notification::ApplicationEvent, 0);
    emit addNotification(notification1);
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.at(0);
    QCOMPARE(arguments.at(0).toBool(), true);

    // Adding a new notification should not send further signals
    Notification notification2(2, 0, 2, notification1Parameters, Notification::ApplicationEvent, 0);
    emit addNotification(notification2);
    QCOMPARE(spy.count(), 1);
}

void Ut_NotifierNotificationSink::testAddSystemNotification()
{
    QSignalSpy spy(m_subject, SIGNAL(notifierSinkActive(bool)));

    // Adding a system notification should cause no notification count change
    NotificationParameters notification1Parameters;
    notification1Parameters.add("count", QVariant((uint)5));
    Notification notification1(1, 0, 2, notification1Parameters, Notification::SystemEvent, 0);
    emit addNotification(notification1);
    QCOMPARE(spy.count(), 0);
}

void Ut_NotifierNotificationSink::testRemoveNotification()
{
    QSignalSpy spy(m_subject, SIGNAL(notifierSinkActive(bool)));

    // Adding a notification should cause a notification count change
    NotificationParameters notification1Parameters;
    notification1Parameters.add("count", QVariant((uint)5));
    Notification notification1(1, 0, 2, notification1Parameters, Notification::ApplicationEvent, 0);
    Notification notification2(2, 0, 2, notification1Parameters, Notification::ApplicationEvent, 0);
    emit addNotification(notification1);
    emit addNotification(notification2);
    QCOMPARE(m_subject->notificationCount, uint(2));

    // Removing one notification does not do anything just changes the notification count
    emit removeNotification(1);
    QCOMPARE(spy.count(), 1); // Only add sent a signal
    QCOMPARE(m_subject->notificationCount, uint(1));

    emit removeNotification(2);
    QCOMPARE(spy.count(), 2); // Now a signal was sent as all notifications are removed
    QCOMPARE(m_subject->notificationCount, uint(0));
    QList<QVariant> arguments = spy.at(1);
    QCOMPARE(arguments.at(0).toBool(), false);
}

void Ut_NotifierNotificationSink::testClearSink()
{
    QSignalSpy spy(m_subject, SIGNAL(notifierSinkActive(bool)));

    // Add two notifications
    NotificationParameters notification1Parameters;
    notification1Parameters.add("count", QVariant((uint)5));
    Notification notification1(1, 0, 2, notification1Parameters, Notification::ApplicationEvent, 0);
    NotificationParameters notification2Parameters;
    notification2Parameters.add("count", QVariant((uint)8));
    Notification notification2(2, 0, 2, notification2Parameters, Notification::ApplicationEvent, 0);
    emit addNotification(notification1);
    emit addNotification(notification2);
    QCOMPARE(m_subject->notificationCount, uint(2));

    m_subject->clearSink();

    QCOMPARE(spy.count(), 2); // One for addition and one for clear
    QList<QVariant> arguments = spy.at(1);
    QCOMPARE(arguments.at(0).toBool(), false); // check that the last call was for setting notifier inactive
    QCOMPARE(m_subject->notificationCount, uint(0));
}

void Ut_NotifierNotificationSink::testDisablingNotificationAdditions()
{
    QSignalSpy spy(m_subject, SIGNAL(notifierSinkActive(bool)));

    m_subject->disableNotificationAdditions(true);

    // Try to add a notification while disabled
    NotificationParameters notification1Parameters;
    notification1Parameters.add("count", QVariant((uint)5));
    Notification notification1(1, 0, 2, notification1Parameters, Notification::ApplicationEvent, 0);
    emit addNotification(notification1);

    QCOMPARE(spy.count(), 0);

    m_subject->disableNotificationAdditions(false);

    // Add a notification when enabled
    NotificationParameters notification2Parameters;
    notification2Parameters.add("count", QVariant((uint)8));
    Notification notification2(2, 0, 2, notification2Parameters, Notification::ApplicationEvent, 0);
    emit addNotification(notification2);

    QCOMPARE(spy.count(), 1);
}

void Ut_NotifierNotificationSink::testSeenNotificationAddedThenNotifierNotUpdated()
{
    QSignalSpy spy(m_subject, SIGNAL(notifierSinkActive(bool)));

    // Adding a seen notification should not cause a notification count change
    NotificationParameters params;
    params.add(GenericNotificationParameterFactory::unseenKey(), false);
    Notification notification1(1, 0, 2, params, Notification::ApplicationEvent, 0);
    emit addNotification(notification1);
    QCOMPARE(spy.count(), 0);
}

QTEST_APPLESS_MAIN(Ut_NotifierNotificationSink)
