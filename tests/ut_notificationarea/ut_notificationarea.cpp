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
/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of system ui.
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
#include <MInfoBanner>
#include "ut_notificationarea.h"
#include "notificationarea.h"
#include "notificationareaview.h"
#include "notificationmanager.h"
#include "notificationareasink.h"
#include "notificationsink_stub.h"
#include "notificationareasink_stub.h"
#include "widgetnotificationsink_stub.h"
#include "sysuid_stub.h"

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
    m_subject = new NotificationArea();
    m_subject->setView(new NotificationAreaView(m_subject));

    connect(this, SIGNAL(addNotification(MInfoBanner &)), m_subject, SLOT(addNotification(MInfoBanner &)));
    connect(this, SIGNAL(removeNotification(MInfoBanner &)), m_subject, SLOT(removeNotification(MInfoBanner &)));
    connect(this, SIGNAL(notificationUpdated(MInfoBanner &)), m_subject, SLOT(moveNotificationToTop(MInfoBanner &)));
}

void Ut_NotificationArea::cleanup()
{
    delete m_subject;
}

void Ut_NotificationArea::testAddNotification()
{
    QSignalSpy notificationCountSpy(m_subject, SIGNAL(notificationCountChanged(int)));
    MInfoBanner notification(MInfoBanner::Information);
    emit addNotification(notification);
    QVERIFY(notification.parentItem() != NULL);
    QVERIFY(m_subject->model()->banners().contains(&notification));
    QCOMPARE(notificationCountSpy.count(), 1);
    QCOMPARE(notificationCountSpy.takeAt(0).at(0).toInt(), 1);
}

void Ut_NotificationArea::testRemoveNotification()
{
    QSignalSpy notificationCountSpy(m_subject, SIGNAL(notificationCountChanged(int)));
    MInfoBanner notification(MInfoBanner::Information);
    emit removeNotification(notification);
    QVERIFY(notification.parentItem() == NULL);
    QVERIFY(! m_subject->model()->banners().contains(&notification));
    QCOMPARE(notificationCountSpy.count(), 1);
    QCOMPARE(notificationCountSpy.takeAt(0).at(0).toInt(), 0);
}

void Ut_NotificationArea::testAddNotificationLatestComesFirst()
{
    MInfoBanner notification1(MInfoBanner::Information);
    emit addNotification(notification1);
    MInfoBanner notification2(MInfoBanner::Information);
    emit addNotification(notification2);
    QCOMPARE(m_subject->model()->banners().at(0), &notification2);
    QCOMPARE(m_subject->model()->banners().at(1), &notification1);
}

void Ut_NotificationArea::testUpdatedNotificationComesFirst()
{
    // Add three notifications
    MInfoBanner notification1(MInfoBanner::Information);
    emit addNotification(notification1);
    MInfoBanner notification2(MInfoBanner::Information);
    emit addNotification(notification2);
    MInfoBanner notification3(MInfoBanner::Information);
    emit addNotification(notification3);
    emit notificationUpdated(notification2);
    QCOMPARE(m_subject->model()->banners().at(0), &notification2);
}

QTEST_APPLESS_MAIN(Ut_NotificationArea)
