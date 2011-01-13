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

#include <MApplication>
#include <MBanner>
#include <QSignalSpy>
#include "ut_notificationarea.h"
#include "notificationarea.h"
#include "notificationareaview.h"
#include "notificationmanager_stub.h"
#include "notification_stub.h"
#include "notificationgroup_stub.h"
#include "notificationareasink.h"
#include "notificationsink_stub.h"
#include "notificationareasink_stub.h"
#include "widgetnotificationsink_stub.h"

// Tests
void Ut_NotificationArea::initTestCase()
{
    // Create a MAapplication
    static int argc = 1;
    static char *app_name = (char *)"./ut_notificationarea";
    app = new MApplication(argc, &app_name);
}

void Ut_NotificationArea::cleanupTestCase()
{
    // Destroy MApplication
    delete app;
}

void Ut_NotificationArea::init()
{
    gNotificationAreaSinkStub->stubReset();
    m_subject = new NotificationArea();

    connect(this, SIGNAL(addNotification(MBanner &)), m_subject, SLOT(addNotification(MBanner &)));
    connect(this, SIGNAL(removeNotification(MBanner &)), m_subject, SLOT(removeNotification(MBanner &)));
    connect(this, SIGNAL(notificationUpdated(MBanner &)), m_subject, SLOT(moveNotificationToTop(MBanner &)));

    gWidgetNotificationSinkStub->stubReset();
}

void Ut_NotificationArea::cleanup()
{
    delete m_subject;
}

void Ut_NotificationArea::testAddNotification()
{
    MBanner notification;
    emit addNotification(notification);
    QVERIFY(notification.parentItem() == NULL);
    QVERIFY(m_subject->model()->banners().contains(&notification));
}

void Ut_NotificationArea::testRemoveNotification()
{
    MBanner notification;
    emit removeNotification(notification);
    QVERIFY(notification.parentItem() == NULL);
    QVERIFY(! m_subject->model()->banners().contains(&notification));
}

void Ut_NotificationArea::testAddNotificationLatestComesFirst()
{
    MBanner notification1;
    emit addNotification(notification1);
    MBanner notification2;
    emit addNotification(notification2);
    QCOMPARE(m_subject->model()->banners().at(0), &notification2);
    QCOMPARE(m_subject->model()->banners().at(1), &notification1);
}

void Ut_NotificationArea::testUpdatedNotificationComesFirst()
{
    // Add three notifications
    MBanner notification1;
    emit addNotification(notification1);
    MBanner notification2;
    emit addNotification(notification2);
    MBanner notification3;
    emit addNotification(notification3);
    emit notificationUpdated(notification2);
    QCOMPARE(m_subject->model()->banners().at(0), &notification2);
}

void Ut_NotificationArea::testRemoveAllRemovableBanners()
{
    QSignalSpy notificationSpy(m_subject, SIGNAL(notificationRemovalRequested(uint)));
    QSignalSpy notificationGroupSpy(m_subject, SIGNAL(notificationGroupClearingRequested(uint)));
    MBanner removableNotification;
    MBanner removableGroupNotification;
    MBanner nonRemovableNotification;
    MBanner nonRemovableGroupNotification;
    removableNotification.setProperty(WidgetNotificationSink::USER_REMOVABLE_PROPERTY, true);
    removableGroupNotification.setProperty(WidgetNotificationSink::USER_REMOVABLE_PROPERTY, true);
    removableNotification.setProperty(WidgetNotificationSink::NOTIFICATION_ID_PROPERTY, 1);
    removableGroupNotification.setProperty(WidgetNotificationSink::GROUP_ID_PROPERTY, 2);
    nonRemovableNotification.setProperty(WidgetNotificationSink::NOTIFICATION_ID_PROPERTY, 3);
    nonRemovableGroupNotification.setProperty(WidgetNotificationSink::GROUP_ID_PROPERTY, 4);
    emit addNotification(removableNotification);
    emit addNotification(removableGroupNotification);
    emit addNotification(nonRemovableNotification);
    emit addNotification(nonRemovableGroupNotification);
    m_subject->removeAllRemovableBanners();
    QCOMPARE(notificationSpy.count(), 1);
    QCOMPARE(notificationGroupSpy.count(), 1);
    QCOMPARE(notificationSpy.at(0).at(0).toInt(), 1);
    QCOMPARE(notificationGroupSpy.at(0).at(0).toInt(), 2);
}

void Ut_NotificationArea::testHonorPrivacySetting()
{
    m_subject->setHonorPrivacySetting(true);
    QCOMPARE(gWidgetNotificationSinkStub->stubCallCount("setHonorPrivacySetting"), 1);
    QCOMPARE(gWidgetNotificationSinkStub->stubLastCallTo("setHonorPrivacySetting").parameter<bool>(0), true);

    m_subject->setHonorPrivacySetting(false);
    QCOMPARE(gWidgetNotificationSinkStub->stubCallCount("setHonorPrivacySetting"), 2);
    QCOMPARE(gWidgetNotificationSinkStub->stubLastCallTo("setHonorPrivacySetting").parameter<bool>(0), false);
}

void Ut_NotificationArea::testWhenNotificationAreaIsCreatedNotificationAreaSinkHasClickablePropertySet()
{
    delete m_subject;
    m_subject = new NotificationArea(NULL, true);
    gWidgetNotificationSinkStub->stubCallCount("setNotificationsClickable");
    QCOMPARE(gWidgetNotificationSinkStub->stubLastCallTo("setNotificationsClickable").parameter<bool>(0), true);
    delete m_subject;
    m_subject = new NotificationArea(NULL, false);
    gWidgetNotificationSinkStub->stubCallCount("setNotificationsClickable");
    QCOMPARE(gWidgetNotificationSinkStub->stubLastCallTo("setNotificationsClickable").parameter<bool>(0), false);
}

void Ut_NotificationArea::testNotificationSinkUpdatedWhenManagerIsSet()
{
    NotificationManager notificationManager;
    m_subject->setNotificationManagerInterface(notificationManager);
    QCOMPARE(gNotificationAreaSinkStub->stubCallCount("updateCurrentNotifications") , 1);
}

QTEST_APPLESS_MAIN(Ut_NotificationArea)
