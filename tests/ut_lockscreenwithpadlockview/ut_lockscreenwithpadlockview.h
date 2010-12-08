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
**
****************************************************************************/
#ifndef UT_LOCKSCREENWITHPADLOCKVIEW_H
#define UT_LOCKSCREENWITHPADLOCKVIEW_H

#include <QtTest/QtTest>
#include <QObject>
#include <notificationmanagerinterface.h>

class MApplication;
class LockScreenWithPadlockView;
class LockScreen;

class MNotificationProxy { };
class MNotificationWithIdentifierProxy { };
class MNotificationGroupProxy { };
class MNotificationGroupWithIdentifierProxy { };

class NotificationManager : public QObject, public NotificationManagerInterface
{
    Q_OBJECT

public:
    NotificationManager();

    //! \reimp
    virtual uint addNotification(uint notificationUserId, const NotificationParameters &parameters = NotificationParameters(), uint groupId = 0);
    virtual bool updateNotification(uint notificationUserId, uint notificationId, const NotificationParameters &parameters = NotificationParameters());
    virtual uint addGroup(uint notificationUserId, const NotificationParameters &parameters = NotificationParameters());
    virtual bool updateGroup(uint notificationUserId, uint groupId, const NotificationParameters &parameters = NotificationParameters());
    virtual bool removeGroup(uint notificationUserId, uint groupId);
    virtual QList<NotificationGroup> groups() const;
    virtual QList<Notification> notifications() const;
    virtual uint notificationUserId();
    virtual QList<uint> notificationIdList(uint notificationUserId);
    virtual QList<MNotificationProxy> notificationList(uint notificationUserId);
    virtual QList<MNotificationWithIdentifierProxy> notificationListWithIdentifiers(uint notificationUserId);
    virtual QList<MNotificationGroupProxy> notificationGroupList(uint notificationUserId);
    virtual QList<MNotificationGroupWithIdentifierProxy> notificationGroupListWithIdentifiers(uint notificationUserId);
    virtual QObject *qObject();
    virtual bool removeNotification(uint notificationUserId, uint notificationId);
    //! \reimp_end

signals:
    void notificationUpdated(const Notification &notification);
    void notificationRemoved(uint notificationId);
};

class Ut_LockScreenWithPadlockView : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void checkOverlayPos(const QPointF &eventPos);

    void testInitialState();
    void testDraggableIconMovement_data();
    void testDraggableIconMovement();
    void testNotificationAreaVisibility();
    void testCreatedItemsAreRemovedFromTheControllerAndTheScene();

private:
    LockScreenWithPadlockView *m_subject;
    MApplication *app;
    NotificationManager notificationManager;
    LockScreen *controller;
};

#endif
