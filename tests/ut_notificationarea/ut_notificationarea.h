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

#ifndef UT_NOTIFICATIONAREA_H
#define UT_NOTIFICATIONAREA_H

#include <QtTest/QtTest>
#include <QObject>
#include "notificationareamodel.h"

class MApplication;
class NotificationArea;
class MBanner;
class QGraphicsLayoutItem;

class Ut_NotificationArea : public QObject
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
    void testAddNotification();
    void testRemoveNotification();
    void testAddNotificationLatestComesFirst();
    void testUpdatedNotificationComesFirst();
    void testRemoveAllRemovableBanners();
    void testHonorPrivacySetting();
    void testWhenNotificationAreaIsCreatedNotificationAreaSinkHasClickablePropertySet();
    void testNotificationSinkUpdatedWhenManagerIsSet();

signals:
    void addNotification(MBanner &notification);
    void removeNotification(MBanner &notification);
    void notificationUpdated(MBanner &notification);

public:
    static QGraphicsLayoutItem *addedItem;
    static const QGraphicsLayoutItem *removedItem;
    static int itemCount;

private:
    MApplication *app;

    // The object being tested
    NotificationArea *m_subject;
};

#endif
