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

#ifndef UT_MCOMPOSITORNOTIFICATIONSINK_H
#define UT_MCOMPOSITORNOTIFICATIONSINK_H

#include <QObject>
#include <QList>
#include <QString>
#include "notificationmanagerinterface.h"
#include <QTimer>
#include <MSceneWindow>

class MApplication;
class MCompositorNotificationSink;

/*!
 * A class used to access the MSceneWindow internals, modeled after 'ut_mscenewindow'
 * in libmeegotouch
 */
class MSceneWindowBridge : public QObject
{
    Q_OBJECT
public:
    MSceneWindowBridge(QObject *parent = 0);
    void setSceneWindowState(MSceneWindow::SceneWindowState newState);
};

class MockNotificationManager : public QObject, public NotificationManagerInterface
{
    Q_OBJECT
public:
    MockNotificationManager();
    uint addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId, int timeout);
    uint addNotification(uint notificationUserId, const NotificationParameters &parameters = NotificationParameters(), uint groupId = 0);
    bool updateNotification(uint notificationUserId, uint notificationId, const NotificationParameters &parameters = NotificationParameters());
    uint addGroup(uint notificationUserId, const NotificationParameters &parameters = NotificationParameters());
    bool updateGroup(uint notificationUserId, uint groupId, const NotificationParameters &parameters = NotificationParameters());
    bool removeGroup(uint notificationUserId, uint groupId);
    uint notificationUserId();
    QList<uint> notificationIdList(uint notificationUserId);
    QList<Notification> notificationList(uint notificationUserId);
    QList<NotificationGroup> notificationGroupList(uint notificationUserId);
    QList<Notification> notificationListWithIdentifiers(uint notificationUserId);
    QList<NotificationGroup> notificationGroupListWithIdentifiers(uint notificationUserId);

    QList<Notification> notifications() const;
    QList<NotificationGroup> groups() const;
    uint notificationCountInGroup(uint notificationUserId, uint groupId);

    uint nextAvailableNotificationID;
    QList<Notification> notificationContainer;

    QObject *qObject();


public slots:
    bool removeNotification(uint notificationUserId, uint notificationId);

signals:
    void notificationRemoved(uint notificationId);
    void notificationUpdated(const Notification &notification);
};

class WindowEventFilter : public QObject
{
    Q_OBJECT

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

class Ut_MCompositorNotificationSink : public QTimer
{
    Q_OBJECT

public:
private:
    void emitDisplayEntered();

    MApplication *app;
    MCompositorNotificationSink *sink;
    MockNotificationManager *notificationManager;
    NotificationParameters setupSinkDisabledTests(bool isSystemEvent = false);

signals:
    void displayEntered();

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
    void testNotificationWindowProperties();
    void testAddNotificationWhenWindowNotOpen();
    void testAddNotificationWhenWindowAlreadyOpen();
    void testWhenNotificationAlreadyOnDisplayNoNewNotificationIsShown();
    void testWhenWindowOnDisplayThenNotificationAddedWithoutSignal();
    void testUpdateNotification();
    void testRemoveNotification();
    void testWhenRemovingCurrentNotificationThenTimerStopsAndBannerDisappears();
    void testTimeout();
    void testNotificationWhileApplicationEventsDisabled();
    void testWhenSinkDisableTrueNoBannerCreated();
    void testWhenSinkIsSetToDisabledSystemNotificationsAreStillGenerated();
    void testNotificationPreviewsDisabled_data();
    void testNotificationPreviewsDisabled();
    void testWindowMasking_data();
    void testWindowMasking();
    void testPreviewIconId();
    void testNotificationPreviewsDisabledForApplication_data();
    void testNotificationPreviewsDisabledForApplication();
    void updateNotificationDoesNotCreateWindowIfBannerNotOnDisplay();
    void testCurrentBannerDoneDoesntRemoveOtherBanners();
    void testSystemNotificationIsRemovedWhenPreviewsAreDisabled();
    void testSystemNotificationIsRemovedWhenBannerHasBeenShown();
    void testOrientationChanges();
    void testOrientationUpdatedWhenBannerShown();
    void testOrientationUpdatedWhenWindowHidden();
    void testOrientationUpdatedWhenOrientationChangesAndWindowHidden();
    void testMaskUpdatedWhenOrientationChanges();

private:
    const QRegion calculateTargetMaskRegion(M::OrientationAngle angle, MSceneWindow* window);
};

#endif // UT_MCOMPOSITORNOTIFICATIONSINK_H
