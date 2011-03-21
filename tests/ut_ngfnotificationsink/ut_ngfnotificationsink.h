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

#ifndef UT_NGFNOTIFICATIONSINK_H
#define UT_NGFNOTIFICATIONSINK_H

#include <QtTest/QtTest>
#include <QObject>
#include <QList>
#include "notification.h"

class MApplication;
class NGFNotificationSink;

class Ut_NGFNotificationSink : public QObject
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

    // Test that feedback is played and stopped
    void testAddAndRemoveNotification();
    // Test that feedback is not played if application events are disabled
    void testNotificationWhileApplicationEventsDisabled();
    void testWithoutEventTypeOrFeedbackId();
    void testWithEventTypeAndFeedbackId();
    void testWithoutEventTypeWithFeedbackId();
    void testUpdateNotificationIsNotPossible();

private:
    // MApplication
    MApplication *app;
    // The object being tested
    NGFNotificationSink *sink;

signals:
    void addNotification(Notification notification);
    void removeNotification(uint notificationId);
};

#endif
