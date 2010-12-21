/***************************************************************************
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

#ifndef UT_NOTIFICATIONAREASINK_H
#define UT_NOTIFICATIONAREASINK_H

#include <QObject>
#include <QList>
#include <QString>

#include "notification.h"

class MApplication;
class NotificationParameters;
class NotificationAreaSink;
class MBanner;

class MBannerCatcher : public QObject
{
    Q_OBJECT

public slots:
    //! A slot that catches an MBanner
    void mBannerEmitted(MBanner &banner);

public:
    //! Container for the received MBanners
    QList<MBanner*> banners;
};

class Ut_NotificationAreaSink : public QObject
{
    Q_OBJECT

public:
    static QList<QString> titles;
    static QList<QString> subtitles;
    static QList<QString> buttonIcons;
    static QList<QString> contents;
    static QList<QDateTime> timestamps;
    static QHash<MBanner *, QString> prefixTimeStamps;
    static QList<MBanner *> notifications;
    static QList<MBanner *> destroyedNotifications;

private:
    MApplication *app;
    NotificationAreaSink *sink;

public slots:
    // For faking the addition of a notification to a layout
    void addNotification(MBanner &notification);
    // For faking the removal of a notification from a layout
    void removeNotification(MBanner &notification);

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
    void testAddGroup();
    void testRemoveGroup();
    void testAddNotificationToGroup();
    void testUpdateGroup();
    void testUpdateNotification();
    void testRemoveNotification();
    void testRemovingNotificationsWhenNoNotificationLeftGroupBannerIsRemoved();
    void testAddNewNotificationToGroupUpdatesNotificationArea();
    void testWhenAddingNewNotificationToGroupThatHasBeenPreviouslyClearedThenGroupBannerIsConstructedCorrectly();
    void testApplyPrivacySetting_data();
    void testApplyPrivacySetting();
    void testNotificationsFetchedFromNotificationManager();
    void testSetPrefixForNotificationGroupBannerWhenThereIsMoreThanOneNotificationInAGroup();

signals:
    void addGroup(uint groupId, const NotificationParameters &parameters);
    void removeGroup(uint groupId);
    void addNotification(Notification);
    void removeNotification(uint notificationId);
    void removeBanner(uint groupId);
    void privacySettingChanged(bool privacyEnabled);
};

#endif // UT_NOTIFICATIONAREASINK_H
