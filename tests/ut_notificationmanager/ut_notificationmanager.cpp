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

#include "ut_notificationmanager.h"

#include <QtTest/QtTest>
#include <QMetaType>
#include <mnotification.h>
#include <mfiledatastore.h>
#include "notificationmanager.h"
#include "notification.h"
#include "notificationgroup.h"
#include "dbusinterfacenotificationsource.h"
#include "dbusinterfacenotificationsink.h"
#include "dbusinterfacenotificationsinkproxy.h"
#include "eventtypestore.h"
#include "genericnotificationparameterfactory.h"
#include "notificationwidgetparameterfactory.h"
#include "notificationsink_stub.h"

bool Ut_NotificationManager::catchTimerTimeouts;
QList<int> Ut_NotificationManager::timerTimeouts;
QBuffer gStateBuffer;
QBuffer gNotificationBuffer;
QList<NotificationGroup> gGroupList;
QList<NotificationGroup> gGroupListWithIdentifiers;
QList<Notification> gNotificationList;
QList<Notification> gNotificationListWithIdentifiers;
quint32 gLastUserId;
bool gTestingPersistent;

#define EVENT_TYPE GenericNotificationParameterFactory::eventTypeKey()
#define COUNT      GenericNotificationParameterFactory::countKey()
#define CLASS      GenericNotificationParameterFactory::classKey()
#define PERSISTENT GenericNotificationParameterFactory::persistentKey()
#define UNSEEN     GenericNotificationParameterFactory::unseenKey()
#define IDENTIFIER GenericNotificationParameterFactory::identifierKey()

#define SUMMARY    NotificationWidgetParameterFactory::summaryKey()
#define BODY       NotificationWidgetParameterFactory::bodyKey()
#define IMAGE      NotificationWidgetParameterFactory::imageIdKey()
#define ICON       NotificationWidgetParameterFactory::iconIdKey()
#define ACTION     NotificationWidgetParameterFactory::actionKey()

// Test Notification Manager
class TestNotificationManager : public NotificationManager
{
public:
    TestNotificationManager(int notificationRelayInterval, uint maxWaitQueueSize = 100);
    void relayNextNotification();
};

TestNotificationManager::TestNotificationManager(int notificationRelayInterval, uint maxWaitQueueSize)
    : NotificationManager(notificationRelayInterval, maxWaitQueueSize)
{
}

void TestNotificationManager::relayNextNotification()
{
    NotificationManager::relayNextNotification();
}

// DBusInterfaceNotificationSource stubs
DBusInterfaceNotificationSource::DBusInterfaceNotificationSource(NotificationManagerInterface &manager) : NotificationSource(manager)
{
}

// DBusInterfaceNotificationSink stubs
DBusInterfaceNotificationSink::DBusInterfaceNotificationSink(NotificationManagerInterface *)
{
}

DBusInterfaceNotificationSink::~DBusInterfaceNotificationSink()
{
}

void DBusInterfaceNotificationSink::addNotification(const Notification &)
{
}

void DBusInterfaceNotificationSink::sendNotificationsToProxy(const QList<Notification> &, const DBusInterface &) const
{
}

void DBusInterfaceNotificationSink::removeNotification(uint)
{
}

void DBusInterfaceNotificationSink::sendGroupsToProxy(const QList<NotificationGroup> &, const DBusInterface &) const
{
}

void DBusInterfaceNotificationSink::addGroup(uint, const NotificationParameters &)
{
}

void DBusInterfaceNotificationSink::removeGroup(uint)
{
}

void DBusInterfaceNotificationSink::sendCurrentNotifications(const DBusInterface&) const
{
}
// QDateTime stub
static uint qDateTimeToTime_t = 0;
uint QDateTime::toTime_t () const
{
    return qDateTimeToTime_t;
}

QString gFileName;

// QFile & QIODevice stubs for file handling
QFile::QFile(const QString & name) {
    gFileName = name;
}

bool QFile::remove(const QString & name) {
    Q_UNUSED(name)
    return true;
}

bool QFile::open(OpenMode mode) {
    if (gFileName.contains("state.data")) {
        gStateBuffer.open(mode);
    } else if (gFileName.contains("notifications.data")) {
        gNotificationBuffer.open(mode);
    } else {
        Q_ASSERT(0);
    }
    return true;
}

void QFile::close() {
    if (gFileName.contains("state.data")) {
        gStateBuffer.close();
    } else if (gFileName.contains("notifications.data")) {
        gNotificationBuffer.close();
    } else {
        Q_ASSERT(0);
    }
}

void QDataStream::setDevice(QIODevice *d)
{
    Q_UNUSED(d)
    if (gFileName.contains("state.data")) {
        dev = &gStateBuffer;
    } else if (gFileName.contains("notifications.data")) {
        dev = &gNotificationBuffer;
    } else {
        Q_ASSERT(0);
    }
}

// EventTypeStore stubs
QHash<QString, QHash<QString, QString> > gEventTypeSettings;
EventTypeStore::EventTypeStore(const QString &eventTypesPath, uint maxStoredEventTypes) :
    eventTypesPath(eventTypesPath),
    maxStoredEventTypes(maxStoredEventTypes)
{
}

QList<QString> EventTypeStore::allKeys(const QString &eventType) const
{
    return gEventTypeSettings.value(eventType).keys();
}

bool EventTypeStore::contains(const QString &eventType, const QString &key) const
{
    return gEventTypeSettings.contains(eventType) && gEventTypeSettings.value(eventType).contains(key);
}

QString EventTypeStore::value(const QString &eventType, const QString &key) const
{
    return gEventTypeSettings.value(eventType).value(key);
}

void EventTypeStore::updateEventTypeFileList()
{
}

void EventTypeStore::updateEventTypeFile(const QString &)
{
}

void EventTypeStore::loadSettings(const QString &)
{
}

// Helper function for loading the last given user id and group information from gStateBuffer
// and repopulating gGroupList accordingly
void loadStateData()
{
    gStateBuffer.open(QIODevice::ReadOnly);
    QDataStream gds(&gStateBuffer);
    gGroupList.clear();
    gStateBuffer.seek(0);

    gds >> gLastUserId;

    NotificationGroup ng;

    while (!gds.atEnd()) {
        gds >> ng;
        gGroupList.append(ng);
    }
}

// Helper function for loading notifications from gNotificationBuffer
// and repopulating gNotificationList
void loadNotifications()
{
    gNotificationBuffer.open(QIODevice::ReadOnly);
    QDataStream nds;
    nds.setDevice(&gNotificationBuffer);

    gNotificationList.clear();
    gNotificationListWithIdentifiers.clear();

    gNotificationBuffer.seek(0);

    Notification notification;

    while (!nds.atEnd()) {
        nds >> notification;
        gNotificationList.append(notification);
        gNotificationListWithIdentifiers.append(notification);
    }
}

// QTimer stubs (used by NotificationManager)
void QTimer::start(int msec)
{
    if (Ut_NotificationManager::catchTimerTimeouts) {
        Ut_NotificationManager::timerTimeouts.append(msec);
    }
}

// QDir stubs (used by NotificationManager)
bool QDir::exists(const QString &) const
{
    return true;
}

bool QDir::mkpath(const QString &) const
{
    return true;
}

// Tests
void Ut_NotificationManager::initTestCase()
{
    qRegisterMetaType<Notification>();
    qRegisterMetaType<NotificationParameters>();
}

void Ut_NotificationManager::cleanupTestCase()
{
}

void Ut_NotificationManager::init()
{
    // Create test notification manager as a pass-through manager (no notifications will be queued).
    manager = new TestNotificationManager(0);
    timerTimeouts.clear();
    catchTimerTimeouts = false;

    gStateBuffer.open(QIODevice::ReadWrite | QIODevice::Truncate);
    gNotificationBuffer.open(QIODevice::ReadWrite| QIODevice::Truncate);
    gEventTypeSettings.clear();
    qDateTimeToTime_t = 0;
}

void Ut_NotificationManager::cleanup()
{
    delete manager;
}

void Ut_NotificationManager::testNotificationUserId()
{
    uint id1 = manager->notificationUserId();
    uint id2 = manager->notificationUserId();

    QVERIFY(id1 != 0);
    QVERIFY(id2 != 0);
    QVERIFY(id1 != id2);

    delete manager;

    gTestingPersistent = true;
    manager = new TestNotificationManager(0);

    uint id3 = manager->notificationUserId();
    QVERIFY(id3 != id1);
    QVERIFY(id3 != id2);

    loadStateData();

    QVERIFY(id3 == gLastUserId);

    gTestingPersistent = false;

    gNotificationBuffer.buffer().clear();
    gStateBuffer.buffer().clear();

    delete manager;
    // new instance for cleanup delete..
    manager = new TestNotificationManager(0);
}

void Ut_NotificationManager::testAddNotification()
{
    QSignalSpy spy(manager, SIGNAL(notificationUpdated(Notification)));

    catchTimerTimeouts = true;

    // Create three notifications - two with a content link and one without
    NotificationParameters parameters0;
    parameters0.add(IMAGE, "icon0");
    qDateTimeToTime_t = 123;
    uint id0 = manager->addNotification(0, parameters0);

    NotificationParameters parameters1;
    parameters1.add(BODY, "body1");
    parameters1.add(CLASS, "system");

    uint id1 = manager->addNotification(0, parameters1, 0);
    NotificationParameters parameters2;

    parameters2.add(ICON, "buttonicon2");
    qDateTimeToTime_t = 12345;
    uint id2 = manager->addNotification(0, parameters2);

    // Verify that timer was not started
    QCOMPARE(timerTimeouts.count(), 0);

    // Test that we didn't get any invalid ids
    QVERIFY(id0 != 0);
    QVERIFY(id1 != 0);
    QVERIFY(id2 != 0);

    // Check that three notifications were created with the given parameters
    QCOMPARE(spy.count(), 3);

    QList<QVariant> arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id0);
    QCOMPARE(n.groupId(), (uint)0);
    QCOMPARE(n.parameters().value(IMAGE).toString(), QString("icon0"));
    QCOMPARE(n.type(), Notification::ApplicationEvent);
    QCOMPARE(n.timeout(), 0);
    QCOMPARE(n.parameters().value("timestamp").toUInt(), (uint)123);

    arguments = spy.takeFirst();
    n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id1);
    QCOMPARE(n.groupId(), (uint)0);
    QCOMPARE(n.parameters().value(BODY).toString(), QString("body1"));
    QCOMPARE(n.type(), Notification::SystemEvent);
    QCOMPARE(n.timeout(), 0);

    arguments = spy.takeFirst();
    n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id2);
    QCOMPARE(n.groupId(), (uint)0);
    QCOMPARE(n.parameters().value(ICON).toString(), QString("buttonicon2"));
    QCOMPARE(n.type(), Notification::ApplicationEvent);
    QCOMPARE(n.parameters().value("timestamp").toUInt(), (uint)12345);
    QCOMPARE(n.timeout(), 0);

    // Verify that the IDs are unique
    QVERIFY(id0 != id1);
    QVERIFY(id0 != id2);
    QVERIFY(id1 != id2);
}

void Ut_NotificationManager::testWhenNotificationIsAddedThenTheNotificationIsFilledWithEventTypeData()
{
    QSignalSpy spy(manager, SIGNAL(notificationUpdated(Notification)));

    gEventTypeSettings["testType"][PERSISTENT] = "true";

    NotificationParameters parameters;
    parameters.add(EVENT_TYPE, "testType");
    manager->addNotification(0, parameters);

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Notification notification = qvariant_cast<Notification>(arguments.at(0));

    QCOMPARE(notification.parameters().value(EVENT_TYPE).toString(), QString("testType"));
    QCOMPARE(notification.parameters().value(PERSISTENT).toBool(), true);
}

void Ut_NotificationManager::testNotificationIsUpdatedWhenEventTypeIsUpdated()
{
    connect(this, SIGNAL(eventTypeModified(QString)),
            manager, SLOT(updateNotificationsWithEventType(QString)));
    QSignalSpy spy(manager, SIGNAL(notificationUpdated(Notification)));

    gEventTypeSettings["testType"][IMAGE] = "iconId";

    NotificationParameters parameters;
    parameters.add(EVENT_TYPE, "testType");
    manager->addNotification(0, parameters);

    gEventTypeSettings["testType"][IMAGE] = "modified-iconId";
    emit eventTypeModified("testType");

    QCOMPARE(spy.count(), 2);
    QList<QVariant> arguments = spy.takeFirst();
    arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.parameters().value(IMAGE).toString(), QString("modified-iconId"));
}

void Ut_NotificationManager::testUpdateNotification()
{
    QSignalSpy spy(manager, SIGNAL(notificationUpdated(Notification)));

    // Create one notification
    NotificationParameters parameters0;
    parameters0.add(IMAGE, "icon0");

    qDateTimeToTime_t = 123;
    uint id0 = manager->addNotification(0, parameters0);

    // Update the notification
    NotificationParameters parameters1;
    parameters1.add(BODY, "body1");
    qDateTimeToTime_t = 12345;
    manager->updateNotification(0, id0, parameters1);

    // Test that the relevant signals are sent
    QCOMPARE(spy.count(), 2);
    QList<QVariant> arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.parameters().value("timestamp").toUInt(), (uint)123);

    arguments = spy.takeFirst();
    n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id0);
    QCOMPARE(n.parameters().value(IMAGE).toString(), QString("icon0"));
    QCOMPARE(n.parameters().value(BODY).toString(), QString("body1"));
    QCOMPARE(n.type(), Notification::ApplicationEvent);
    QCOMPARE(n.timeout(), 0);
    QCOMPARE(n.parameters().value("timestamp").toUInt(), (uint)12345);

}

void Ut_NotificationManager::testRemovingNotificationSignalSent()
{
    QSignalSpy queuedSpy(manager, SIGNAL(queuedNotificationRemove(uint)));

    // Create one notifications
    NotificationParameters parameters1;
    uint id1 = addNotification(&parameters1, "1", 2);

    // Cancel non existing
    manager->removeNotification(0, -1);

    // Test that the signal is NOT are sent
    QCOMPARE(queuedSpy.count(), 0);

    // Cancel the notification
    manager->removeNotification(0, id1);

    // Test that the relevant signals are sent
    QCOMPARE(queuedSpy.count(), 1);
}

void Ut_NotificationManager::testRemovingGroupSignalSent()
{
    QSignalSpy queuedSpy(manager, SIGNAL(queuedGroupRemove(uint)));

    // Create one notifications
    NotificationParameters parameters1;
    uint id1 = addGroup(&parameters1, "1", 2);

    // Cancel non existing
    manager->removeGroup(0, -1);

    // Test that the relevant signal is NOT are sent
    QCOMPARE(queuedSpy.count(), 0);

    // Cancel the notification
    manager->removeGroup(0, id1);

    // Test that the relevant signals are sent
    QCOMPARE(queuedSpy.count(), 1);
}

void Ut_NotificationManager::testRemoveNotification()
{
    QSignalSpy removeSpy(manager, SIGNAL(notificationRemoved(uint)));

    // Create three notifications
    NotificationParameters parameters0;
    addNotification(&parameters0, "0", 1);

    NotificationParameters parameters1;
    uint id1 = addNotification(&parameters1, "1", 2);

    NotificationParameters parameters2;
    addNotification(&parameters2, "2", 3);

    //Remove middle one
    manager->removeNotification(id1);

    //Check that it is removed
    QCOMPARE(removeSpy.count(), 1);
    QList<QVariant> arguments = removeSpy.takeFirst();
    QCOMPARE(arguments.at(0).toUInt(), id1);
}

void Ut_NotificationManager::testAddGroup()
{
    QSignalSpy addGroupSpy(manager, SIGNAL(groupUpdated(uint, const NotificationParameters &)));

    NotificationParameters inParameters1;
    inParameters1.add(IMAGE, "icon1");
    qDateTimeToTime_t = 123;
    uint id1 = manager->addGroup(0, inParameters1);

    NotificationParameters inParameters2;
    inParameters2.add(BODY, "body2");
    qDateTimeToTime_t = 12345;
    uint id2 = manager->addGroup(0, inParameters2);

    // Check that we didn't get invalid numbers
    QVERIFY(id1 != 0);
    QVERIFY(id2 != 0);
    QVERIFY(id1 != id2);

    // Verify that timer was not started
    QCOMPARE(timerTimeouts.count(), 0);

    QCOMPARE(addGroupSpy.count(), 2);

    QList<QVariant> arguments = addGroupSpy.takeFirst();
    QCOMPARE(arguments.at(0).toUInt(), id1);
    NotificationParameters outParameters1 = arguments.at(1).value<NotificationParameters>();
    QCOMPARE(outParameters1.value(IMAGE).toString(), QString("icon1"));
    QCOMPARE(outParameters1.value("timestamp").toUInt(), (uint)123);

    arguments = addGroupSpy.takeFirst();
    QCOMPARE(arguments.at(0).toUInt(), id2);
    NotificationParameters outParameters2 = arguments.at(1).value<NotificationParameters>();
    QCOMPARE(outParameters2.value(BODY).toString(), QString("body2"));
    QCOMPARE(outParameters2.value("timestamp").toUInt(), (uint)12345);
}

void Ut_NotificationManager::testWhenNotificationGroupIsAddedThenTheNotificationGroupIsFilledWithEventTypeData()
{
    QSignalSpy addGroupSpy(manager, SIGNAL(groupUpdated(uint, NotificationParameters)));

    gEventTypeSettings["testType"][PERSISTENT] = "true";

    NotificationParameters parameters;
    parameters.add(EVENT_TYPE, "testType");
    manager->addGroup(0, parameters);

    QCOMPARE(addGroupSpy.count(), 1);
    QList<QVariant> arguments = addGroupSpy.takeFirst();
    NotificationParameters notificationParameters = qvariant_cast<NotificationParameters>(arguments.at(1));

    QCOMPARE(notificationParameters.value(EVENT_TYPE).toString(), QString("testType"));
    QCOMPARE(notificationParameters.value(PERSISTENT).toBool(), true);
}

void Ut_NotificationManager::testUpdateGroup()
{
    QSignalSpy updateGroupSpy(manager, SIGNAL(groupUpdated(uint, const NotificationParameters &)));

    NotificationParameters inParameters1;
    inParameters1.add(IMAGE, "icon1");
    qDateTimeToTime_t = 123;
    uint id1 = manager->addGroup(0, inParameters1);


    NotificationParameters inParameters2;
    inParameters2.add(BODY, "body2");
    qDateTimeToTime_t = 12345;
    manager->updateGroup(0, id1, inParameters2);

    QCOMPARE(updateGroupSpy.count(), 2);

    QList<QVariant> arguments = updateGroupSpy.takeFirst();
    QCOMPARE(arguments.at(0).toUInt(), id1);
    NotificationParameters outParameters = arguments.at(1).value<NotificationParameters>();
    QCOMPARE(outParameters.value("timestamp").toUInt(), (uint)123);

    arguments = updateGroupSpy.takeFirst();
    QCOMPARE(arguments.at(0).toUInt(), id1);
    outParameters = arguments.at(1).value<NotificationParameters>();
    QCOMPARE(outParameters.value(BODY).toString(), QString("body2"));
    QCOMPARE(outParameters.value(IMAGE).toString(), QString("icon1"));
    QVERIFY(outParameters.value("timestamp").toUInt() > 0);
    QCOMPARE(outParameters.value("timestamp").toUInt(), (uint)12345);
}

void Ut_NotificationManager::testUpdateNonexistingGroup()
{
    QSignalSpy updateGroupSpy(manager, SIGNAL(groupUpdated(uint, const NotificationParameters &)));

    // Try to update a group that doesn't exist
    NotificationParameters parameters1;
    parameters1.add(IMAGE, "icon1");
    manager->updateGroup(0, 1, parameters1);
    QCOMPARE(updateGroupSpy.count(), 0);

    // Add a group...
    uint id1 = manager->addGroup(0, parameters1);
    // Need to clear the spy because addGroup sends the same signal
    updateGroupSpy.clear();
    // ...but try to update some other group
    NotificationParameters parameters2;
    parameters2.add(BODY, "body2");
    manager->updateGroup(0, id1 + 1, parameters2);
    QCOMPARE(updateGroupSpy.count(), 0);
}



void Ut_NotificationManager::testRemoveGroup()
{
    QSignalSpy removeGroupSpy(manager, SIGNAL(groupRemoved(uint)));
    NotificationParameters parameters1;
    uint id1 = addGroup(&parameters1, "0" , 1);
    manager->doRemoveGroup(id1);
    QCOMPARE(removeGroupSpy.count(), 1);
    QList<QVariant> arguments = removeGroupSpy.takeFirst();
    QCOMPARE(arguments.at(0).toUInt(), id1);
}

void Ut_NotificationManager::testRemoveGroupWithNotifications()
{
    QSignalSpy addSpy(manager, SIGNAL(notificationUpdated(Notification)));
    QSignalSpy removeGroupSpy(manager, SIGNAL(groupRemoved(uint)));
    QSignalSpy removeNotificationSpy(manager, SIGNAL(notificationRemoved(uint)));

    NotificationParameters gparameters1;
    uint groupId = addGroup(&gparameters1, "1", 1);
    NotificationParameters parameters0;
    uint id0 = addNotification(&parameters0, "0", 1, groupId);
    NotificationParameters parameters1;
    uint id1 = addNotification(&parameters1, "1", 2, groupId);

    QCOMPARE(addSpy.count(), 2);

    manager->doRemoveGroup(groupId);

    QCOMPARE(removeGroupSpy.count(), 1);
    QCOMPARE(removeNotificationSpy.count(), 2);
    QList<QVariant> arguments = removeNotificationSpy.takeFirst();
    QCOMPARE(arguments.at(0).toUInt(), id0);
    arguments = removeNotificationSpy.takeFirst();
    QCOMPARE(arguments.at(0).toUInt(), id1);
}

void Ut_NotificationManager::testRemovingAllNotificationsFromAGroup()
{
    NotificationParameters gparameters1;
    gparameters1.add(IMAGE, "gicon1");
    uint groupId = manager->addGroup(0, gparameters1);

    QSignalSpy removeGroupSpy(manager, SIGNAL(groupRemoved(uint)));
    NotificationParameters parameters0;
    parameters0.add(IMAGE, "icon0");
    uint id0 = manager->addNotification(0, parameters0, groupId);
    manager->removeNotification(0, id0);

    // Check that group is not removed even if the all notifications from it are removed
    QCOMPARE(removeGroupSpy.count(), 0);

    // Check that the group could be reused for showing notifications
    QSignalSpy addSpy(manager, SIGNAL(notificationUpdated(Notification)));
    manager->addNotification(0, parameters0, groupId);
    QCOMPARE(addSpy.count(), 1);
}

void Ut_NotificationManager::testRemoveNonexistingGroup()
{
    QSignalSpy removeGroupSpy(manager, SIGNAL(groupRemoved(uint)));
    manager->removeGroup(0, 1);
    QCOMPARE(removeGroupSpy.count(), 0);

    // Add a group and then try to remove it twice
    NotificationParameters gparameters1;
    gparameters1.add(IMAGE, "gicon1");
    uint id1 = manager->addGroup(0, gparameters1);
    manager->removeGroup(0, id1);
    removeGroupSpy.clear();
    manager->removeGroup(0, id1);
    QCOMPARE(removeGroupSpy.count(), 0);
}

void Ut_NotificationManager::testAddNotificationInGroup()
{
    QSignalSpy groupSpy(manager, SIGNAL(groupUpdated(uint, const NotificationParameters &)));

    NotificationParameters gparameters1;
    gparameters1.add(IMAGE, "gicon1");
    qDateTimeToTime_t = 123;
    uint groupId = manager->addGroup(0, gparameters1);

    QCOMPARE(groupSpy.count(), 1);
    QList<QVariant> arguments = groupSpy.takeFirst();
    NotificationParameters outParameters = arguments.at(1).value<NotificationParameters>();
    QCOMPARE(outParameters.value("timestamp").toUInt(), (uint)123);

    QSignalSpy addSpy(manager, SIGNAL(notificationUpdated(Notification)));

    NotificationParameters parameters0;
    parameters0.add(IMAGE, "icon0");
    qDateTimeToTime_t = 12345;
    uint id0 = manager->addNotification(0, parameters0, groupId);

    QCOMPARE(addSpy.count(), 1);
    arguments = addSpy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id0);
    QCOMPARE(n.groupId(), groupId);
    QCOMPARE(n.parameters().value(IMAGE).toString(), QString("icon0"));
    QCOMPARE(n.parameters().value("timestamp").toUInt(), (uint)12345);
}

void Ut_NotificationManager::testAddNotificationInNonexistingGroup()
{
    QSignalSpy addSpy(manager, SIGNAL(notificationUpdated(Notification)));

    // Add a notification to a group that doesn't exist
    NotificationParameters parameters0;
    parameters0.add(IMAGE, "icon0");
    manager->addNotification(0, parameters0, 1);
    QCOMPARE(addSpy.count(), 0);
}

void Ut_NotificationManager::testSecondSimultaneousNotificationIsLeftInQueue()
{
    // Create a new notification manager with manual notification relay.
    delete manager;
    manager = new TestNotificationManager(-1);

    // Create a signal spy to fake a notification sink
    QSignalSpy spy(manager, SIGNAL(notificationUpdated(Notification)));

    // Create two notifications
    NotificationParameters parameters0;
    parameters0.add(IMAGE, "icon0");
    uint id0 = manager->addNotification(0, parameters0);
    NotificationParameters parameters1;
    parameters1.add(BODY, "body1");
    uint id1 = manager->addNotification(0, parameters1);

    // Verify that timer was not started
    QCOMPARE(timerTimeouts.count(), 0);

    // Check that notification sink was signaled with one notification.
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id0);
    QCOMPARE(n.groupId(), (uint)0);
    QCOMPARE(n.parameters().value(IMAGE).toString(), QString("icon0"));
    QCOMPARE(n.type(), Notification::ApplicationEvent);
    QCOMPARE(n.timeout(), -1);

    // Relay next notification
    manager->relayNextNotification();

    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id1);
    QCOMPARE(n.groupId(), (uint)0);
    QCOMPARE(n.parameters().value(BODY).toString(), QString("body1"));
    QCOMPARE(n.type(), Notification::ApplicationEvent);
    QCOMPARE(n.timeout(), -1);

    // Create third notification. This should be left in the queue since the second notification is being shown.
    NotificationParameters parameters2;
    parameters0.add(ICON, "buttonicon2");
    manager->addNotification(0, parameters2);
    QCOMPARE(spy.count(), 0);
}

void Ut_NotificationManager::testCancellingNotificationInQueue()
{
    // Create a new notification manager with manual notification relay.
    delete manager;
    manager = new TestNotificationManager(-1);

    // Create signal spies to fake a notification sink
    QSignalSpy spy(manager, SIGNAL(notificationUpdated(Notification)));
    QSignalSpy removeSpy(manager, SIGNAL(notificationRemoved(uint)));

    // Create three notifications
    NotificationParameters parameters0;
    parameters0.add(IMAGE, "icon0");
    uint id0 = manager->addNotification(0, parameters0);
    NotificationParameters parameters1;
    parameters1.add(BODY, "body1");
    uint id1 = manager->addNotification(0, parameters1);
    NotificationParameters parameters2;
    parameters2.add(ICON, "buttonicon2");
    uint id2 = manager->addNotification(0, parameters2);

    // Check that notification sink was signaled with first notification.
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id0);

    // Cancel the middle notification
    manager->removeNotification(id1);

    // Check that notification sink was not signaled with cancel.
    QCOMPARE(removeSpy.count(), 0);

    // Relay next notification.
    manager->relayNextNotification();

    // Check that notification sink was signaled with third notification.
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id2);
}

void Ut_NotificationManager::testUpdatingNotificationInQueue()
{
    // Create a new notification manager with manual notification relay.
    delete manager;
    manager = new TestNotificationManager(-1);

    // Create signal spy to fake a notification sink
    QSignalSpy spy(manager, SIGNAL(notificationUpdated(Notification)));

    // Create two notifications
    NotificationParameters parameters0;
    parameters0.add(IMAGE, "icon0");
    uint id0 = manager->addNotification(0, parameters0);
    NotificationParameters parameters1;
    parameters1.add(BODY, "body1");
    uint id1 = manager->addNotification(0, parameters1);

    // Check that notification sink was signaled with first notification.
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id0);

    // Update the second notification
    NotificationParameters parameters2;
    parameters2.add(ICON, "newicon");
    manager->updateNotification(0, id1, parameters2);

    // Check that notification sink was not signaled with update.
    QCOMPARE(spy.count(), 0);

    // Relay next notification.
    manager->relayNextNotification();

    // Check that notification sink was signaled with updated notification data.
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id1);
    QCOMPARE(n.parameters().value(IMAGE).isNull(), true);
    QCOMPARE(n.parameters().value(BODY).toString(), QString("body1"));
    QCOMPARE(n.parameters().value(ICON).toString(), QString("newicon"));
    QCOMPARE(n.parameters().value("contentId").isNull(), true);
    QCOMPARE(n.type(), Notification::ApplicationEvent);
}

// Test that nothing happens if wait queue is empty and relay notification is called
void Ut_NotificationManager::testRelayInEmptyQueue()
{
    delete manager;
    manager = new TestNotificationManager(-1);

    // Create signal spy to fake a notification sink
    QSignalSpy spy(manager, SIGNAL(notificationUpdated(Notification)));

    NotificationParameters parameters0;
    parameters0.add(IMAGE, "icon0");
    uint id0 = manager->addNotification(0, parameters0);

    // Check that notification sink was signaled with first notification.
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id0);

    // Try to relay notification even though it does not exist
    manager->relayNextNotification();
    QCOMPARE(spy.count(), 0);
}

void Ut_NotificationManager::testDroppingNotificationsIfQueueIsFull()
{
    // Create notification manager with wait queue of 5 notifications
    delete manager;
    manager = new TestNotificationManager(-1, 5);

    // Create signal spy to fake a notification sink
    QSignalSpy spy(manager, SIGNAL(notificationUpdated(Notification)));

    // Add 7 notifications to the notification manager.
    // First notification will be relayed to the sink.
    // Next five notifications will fill the wait queue.
    // Last notification will be dropped.
    NotificationParameters parameters0;
    parameters0.add(IMAGE, "icon0");
    uint id0 = manager->addNotification(0, parameters0);
    NotificationParameters parameters15;
    parameters15.add(BODY, "queuefillerbody");
    for (int i = 0; i < 5; ++i) {
        manager->addNotification(0, parameters15);
    }
    NotificationParameters parameters6;
    parameters0.add("icondId", "droppedbuttonicon");
    manager->addNotification(0, parameters6);

    // Check that notification sink was signaled with first notification.
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id0);

    // Relay the next 5 notifications
    for (int i = 0; i < 5; ++i) {
        manager->relayNextNotification();
        QCOMPARE(spy.count(), 1);
        QList<QVariant> arguments = spy.takeFirst();
        n = qvariant_cast<Notification>(arguments.at(0));
        QCOMPARE(n.parameters().value(BODY).toString(), QString("queuefillerbody"));
    }

    // The queue should be empty. Try relaying.
    manager->relayNextNotification();
    QCOMPARE(spy.count(), 0);
}

void Ut_NotificationManager::testWaitQueueTimer()
{
    // Create notification manager with relay interval
    delete manager;
    manager = new TestNotificationManager(3000);

    // Create signal spy to fake a notification sink
    QSignalSpy spy(manager, SIGNAL(notificationUpdated(Notification)));

    catchTimerTimeouts = true;

    // Add two notifications to the notification manager.
    // First notification should pass through.
    // Second is left in wait queue
    NotificationParameters parameters0;
    parameters0.add(IMAGE, "icon0");
    uint id0 = manager->addNotification(0, parameters0);

    NotificationParameters parameters1;
    parameters1.add(BODY, "body1");
    uint id1 = manager->addNotification(0, parameters1);

    // Check that notification sink was signaled with first notification.
    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id0);

    // Check that timer was started.
    QCOMPARE(timerTimeouts.count(), 1);
    QCOMPARE(timerTimeouts.at(0), 3000);

    // Expire timer
    manager->relayNextNotification();

    // Check that notification sink was signaled with second notification.
    QCOMPARE(spy.count(), 1);
    arguments = spy.takeFirst();
    n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), id1);

    // Check that timer was re-started.
    QCOMPARE(timerTimeouts.count(), 2);
    QCOMPARE(timerTimeouts.at(1), 3000);

    // Expire timer
    manager->relayNextNotification();

    // Check that notification sink was not signaled anymore.
    QCOMPARE(spy.count(), 0);

    // Check that timer was not re-started.
    QCOMPARE(timerTimeouts.count(), 2);
}

void Ut_NotificationManager::testRemoveNotificationsInGroup()
{
    QSignalSpy removeSpy(manager, SIGNAL(notificationRemoved(uint)));

    NotificationParameters gparameters1;
    gparameters1.add(IMAGE, "gicon1");
    uint groupId = manager->addGroup(0, gparameters1);

    NotificationParameters parameters0;
    parameters0.add(IMAGE, "icon0");
    uint id0 = manager->addNotification(0, parameters0, groupId);

    NotificationParameters parameters1;
    parameters0.add(IMAGE, "icon1");
    uint id1 = manager->addNotification(0, parameters0, groupId);

    manager->removeNotificationsInGroup(groupId);

    // Test that the relevant signals are sent
    QCOMPARE(removeSpy.count(), 2);
    QCOMPARE(removeSpy.at(0).at(0).toUInt(), id0);
    QCOMPARE(removeSpy.at(1).at(0).toUInt(), id1);
}

void Ut_NotificationManager::testNotificationIdList()
{
    NotificationParameters parameters;
    manager->addNotification(1, parameters);
    manager->addNotification(1, parameters);
    manager->addNotification(1, parameters);
    manager->addNotification(3, parameters);
    manager->addNotification(2, parameters);
    manager->addNotification(1, parameters);

    uint notificationUserId = 1;
    QList<uint> list;
    list = manager->notificationIdList(notificationUserId);
    QCOMPARE(list.size(), 4);

    QCOMPARE(list.at(0), (uint)1);
    QCOMPARE(list.at(1), (uint)2);
    QCOMPARE(list.at(2), (uint)3);
    QCOMPARE(list.at(3), (uint)6);

}

void Ut_NotificationManager::testNotificationIdListZeroNotificationUserId()
{
    NotificationParameters parameters;
    manager->addNotification(1, parameters);
    manager->addNotification(1, parameters);
    manager->addNotification(1, parameters);
    manager->addNotification(3, parameters);
    manager->addNotification(2, parameters);
    manager->addNotification(1, parameters);

    uint notificationUserId = 0;
    QList<uint> list;
    list = manager->notificationIdList(notificationUserId);
    QCOMPARE(list.size(), 0);
}

void Ut_NotificationManager::testNotificationIdListNotificationListEmpty()
{
    uint notificationUserId = 0;
    QList<uint> list;
    list = manager->notificationIdList(notificationUserId);
    QCOMPARE(list.size(), 0);
}

void Ut_NotificationManager::testNotificationList()
{
    // normal notification
    NotificationParameters parameters0;
    parameters0.add(EVENT_TYPE, "type0");
    parameters0.add(SUMMARY, "summary0");
    parameters0.add(BODY, "body0");
    parameters0.add(IMAGE, "image0");
    parameters0.add(ACTION, "action0");
    parameters0.add(COUNT, 0);
    manager->addNotification(1, parameters0);

    // another notification, in a group
    NotificationParameters parameters1;
    NotificationParameters gparameters;
    parameters1.add(EVENT_TYPE, "type1");
    parameters1.add(SUMMARY, "summary1");
    parameters1.add(BODY, "body1");
    parameters1.add(IMAGE, "image1");
    parameters1.add(ACTION, "action1");
    parameters1.add(COUNT, 1);
    uint gid = manager->addGroup(1, gparameters);
    manager->addNotification(1, parameters1, gid);

    // third notification, a different user
    NotificationParameters parameters2;
    parameters2.add(EVENT_TYPE, "type2");
    parameters2.add(SUMMARY, "summary2");
    parameters2.add(BODY, "body2");
    parameters2.add(IMAGE, "image2");
    parameters2.add(ACTION, "action2");
    parameters2.add(COUNT, 2);
    manager->addNotification(2, parameters2);

    QList<Notification> list = manager->notificationList(1);
    QCOMPARE(list.size(), 2);

    const NotificationParameters &resultParameters0(list.at(0).parameters());
    QCOMPARE(resultParameters0.value(EVENT_TYPE).toString(), parameters0.value(EVENT_TYPE).toString());
    QCOMPARE(resultParameters0.value(SUMMARY).toString(), parameters0.value(SUMMARY).toString());
    QCOMPARE(resultParameters0.value(BODY).toString(), parameters0.value(BODY).toString());
    QCOMPARE(resultParameters0.value(IMAGE).toString(), parameters0.value(IMAGE).toString());
    QCOMPARE(resultParameters0.value(COUNT).toUInt(), parameters0.value(COUNT).toUInt());

    const NotificationParameters &resultParameters1(list.at(1).parameters());
    QCOMPARE(resultParameters1.value(EVENT_TYPE).toString(), parameters1.value(EVENT_TYPE).toString());
    QCOMPARE(resultParameters1.value(SUMMARY).toString(), parameters1.value(SUMMARY).toString());
    QCOMPARE(resultParameters1.value(BODY).toString(), parameters1.value(BODY).toString());
    QCOMPARE(resultParameters1.value(IMAGE).toString(), parameters1.value(IMAGE).toString());
    QCOMPARE(resultParameters1.value(COUNT).toUInt(), parameters1.value(COUNT).toUInt());

    list = manager->notificationList(0);
    QCOMPARE(list.size(), 0);
}

void Ut_NotificationManager::testNotificationListWithIdentifiers()
{
    // normal notification
    NotificationParameters parameters0;
    addNotification(&parameters0, "0", 1, 0, true);

    // another notification, in a group
    NotificationParameters gparameters;
    uint gid = manager->addGroup(1, gparameters);
    NotificationParameters parameters1;
    addNotification(&parameters1, "1", 1 , gid, true);

    // third notification, a different user
    NotificationParameters parameters2;
    addNotification(&parameters2, "2", 2, 0, true);

    QList<Notification> list = manager->notificationListWithIdentifiers(1);
    QCOMPARE(list.size(), 2);

    const NotificationParameters &resultParameters0(list.at(0).parameters());
    QCOMPARE(resultParameters0.value(EVENT_TYPE).toString(), parameters0.value(EVENT_TYPE).toString());
    QCOMPARE(resultParameters0.value(SUMMARY).toString(), parameters0.value(SUMMARY).toString());
    QCOMPARE(resultParameters0.value(BODY).toString(), parameters0.value(BODY).toString());
    QCOMPARE(resultParameters0.value(IMAGE).toString(), parameters0.value(IMAGE).toString());
    QCOMPARE(resultParameters0.value(COUNT).toUInt(), parameters0.value(COUNT).toUInt());
    QCOMPARE(resultParameters0.value(IDENTIFIER).toString(), parameters0.value(IDENTIFIER).toString());

    const NotificationParameters &resultParameters1(list.at(1).parameters());
    QCOMPARE(resultParameters1.value(EVENT_TYPE).toString(), parameters1.value(EVENT_TYPE).toString());
    QCOMPARE(resultParameters1.value(SUMMARY).toString(), parameters1.value(SUMMARY).toString());
    QCOMPARE(resultParameters1.value(BODY).toString(), parameters1.value(BODY).toString());
    QCOMPARE(resultParameters1.value(IMAGE).toString(), parameters1.value(IMAGE).toString());
    QCOMPARE(resultParameters1.value(COUNT).toUInt(), parameters1.value(COUNT).toUInt());
    QCOMPARE(resultParameters1.value(IDENTIFIER).toString(), parameters1.value(IDENTIFIER).toString());

    list = manager->notificationListWithIdentifiers(0);
    QCOMPARE(list.size(), 0);
}


uint Ut_NotificationManager::addGroup(NotificationParameters *parameters, QString index, int groupid, bool addIdentifier)
{
    parameters->add(EVENT_TYPE, "type" + index);
    parameters->add(SUMMARY, "summary" + index);
    parameters->add(BODY, "body" + index);
    parameters->add(IMAGE, "image" + index);
    parameters->add(ACTION, "action" + index);
    parameters->add(COUNT, index.toInt());
    if (addIdentifier) {
        parameters->add(IDENTIFIER, "identifier" + index);
    }
    return manager->addGroup(groupid, *parameters);
}

uint Ut_NotificationManager::addNotification(NotificationParameters *parameters, QString index,
                                              int notificationId, int groupId, bool addIdentifier)
{
    parameters->add(EVENT_TYPE, "type" + index);
    parameters->add(SUMMARY, "summary" + index);
    parameters->add(BODY, "body" + index);
    parameters->add(IMAGE, "image" + index);
    parameters->add(ACTION, "action" + index);
    parameters->add(COUNT, index.toInt());
    if (addIdentifier) {
        parameters->add(IDENTIFIER, "identifier" + index);
    }
    return manager->addNotification(notificationId, *parameters, groupId);
}

void Ut_NotificationManager::testNotificationGroupList()
{
    // normal group
    NotificationParameters parameters0;
    addGroup(&parameters0, "0", 1);

    // another group
    NotificationParameters parameters1;
    addGroup(&parameters1, "1", 1);

    // third group, a different user
    NotificationParameters parameters2;
    addGroup(&parameters2, "2", 2);

    QList<NotificationGroup> list = manager->notificationGroupList(1);
    QCOMPARE(list.size(), 2);

    const NotificationParameters &resultParameters0(list.at(0).parameters());
    QCOMPARE(resultParameters0.value(EVENT_TYPE).toString(), parameters0.value(EVENT_TYPE).toString());
    QCOMPARE(resultParameters0.value(SUMMARY).toString(), parameters0.value(SUMMARY).toString());
    QCOMPARE(resultParameters0.value(BODY).toString(), parameters0.value(BODY).toString());
    QCOMPARE(resultParameters0.value(IMAGE).toString(), parameters0.value(IMAGE).toString());
    QCOMPARE(resultParameters0.value(COUNT).toUInt(), parameters0.value(COUNT).toUInt());

    const NotificationParameters &resultParameters1(list.at(1).parameters());
    QCOMPARE(resultParameters1.value(EVENT_TYPE).toString(), parameters1.value(EVENT_TYPE).toString());
    QCOMPARE(resultParameters1.value(SUMMARY).toString(), parameters1.value(SUMMARY).toString());
    QCOMPARE(resultParameters1.value(BODY).toString(), parameters1.value(BODY).toString());
    QCOMPARE(resultParameters1.value(IMAGE).toString(), parameters1.value(IMAGE).toString());
    QCOMPARE(resultParameters1.value(COUNT).toUInt(), parameters1.value(COUNT).toUInt());

    list = manager->notificationGroupList(0);
    QCOMPARE(list.size(), 0);
}

void Ut_NotificationManager::testNotificationGroupListWithIdentifiers()
{
    // normal group
    NotificationParameters parameters0;
    addGroup(&parameters0, 0, 1, true);

    // another group
    NotificationParameters parameters1;
    addGroup(&parameters1, "1", 1, true);

    // third group, a different user
    NotificationParameters parameters2;
    addGroup(&parameters2, "2", 2, true);

    QList<NotificationGroup> list = manager->notificationGroupListWithIdentifiers(1);
    QCOMPARE(list.size(), 2);

    const NotificationParameters &resultParameters0(list.at(0).parameters());
    QCOMPARE(resultParameters0.value(EVENT_TYPE).toString(), parameters0.value(EVENT_TYPE).toString());
    QCOMPARE(resultParameters0.value(SUMMARY).toString(), parameters0.value(SUMMARY).toString());
    QCOMPARE(resultParameters0.value(BODY).toString(), parameters0.value(BODY).toString());
    QCOMPARE(resultParameters0.value(IMAGE).toString(), parameters0.value(IMAGE).toString());
    QCOMPARE(resultParameters0.value(COUNT).toUInt(), parameters0.value(COUNT).toUInt());
    QCOMPARE(resultParameters0.value(IDENTIFIER).toString(), parameters0.value(IDENTIFIER).toString());

    const NotificationParameters &resultParameters1(list.at(1).parameters());
    QCOMPARE(resultParameters1.value(EVENT_TYPE).toString(), parameters1.value(EVENT_TYPE).toString());
    QCOMPARE(resultParameters1.value(SUMMARY).toString(), parameters1.value(SUMMARY).toString());
    QCOMPARE(resultParameters1.value(BODY).toString(), parameters1.value(BODY).toString());
    QCOMPARE(resultParameters1.value(IMAGE).toString(), parameters1.value(IMAGE).toString());
    QCOMPARE(resultParameters1.value(COUNT).toUInt(), parameters1.value(COUNT).toUInt());
    QCOMPARE(resultParameters1.value(IDENTIFIER).toString(), parameters1.value(IDENTIFIER).toString());

    list = manager->notificationGroupListWithIdentifiers(0);
    QCOMPARE(list.size(), 0);
}

void Ut_NotificationManager::testGroupInfoStorage()
{
    gTestingPersistent = true;

    gNotificationBuffer.buffer().clear();
    gStateBuffer.buffer().clear();

    gEventTypeSettings["persistent"][PERSISTENT] = "true";

    delete manager;
    manager = new TestNotificationManager(3000);

    // Add two groups to the notification manager
    NotificationParameters parameters0;
    uint id0 = addGroup(&parameters0, "0", 1);
    NotificationParameters parameters1;
    uint id1 = addGroup(&parameters1, "1", 2);
    loadStateData();
    QCOMPARE((uint)gGroupList.count(), (uint)2);
    QCOMPARE((uint)gGroupList.at(0).groupId(), id0);
    QCOMPARE((uint)gGroupList.at(1).groupId(), id1);
    NotificationGroup ng = gGroupList.at(0);
    QCOMPARE(ng.parameters().value(IMAGE).toString(), QString("image0"));
    ng = gGroupList.at(1);
    QCOMPARE(ng.parameters().value(BODY).toString(), QString("body1"));

    //Remove group and check that it is removed
    manager->doRemoveGroup(id0);
    loadStateData();
    QCOMPARE((uint)gGroupList.count(), (uint)1);
    QCOMPARE((uint)gGroupList.at(0).groupId(), id1);
    ng = gGroupList.at(0);
    QCOMPARE(ng.parameters().value(BODY).toString(), QString("body1"));

    //Update group and verify that it is updated
    manager->updateGroup(0, id1, parameters0);
    loadStateData();
    QCOMPARE((uint)gGroupList.count(), (uint)1);
    QCOMPARE((uint)gGroupList.at(0).groupId(), id1);
    ng = gGroupList.at(0);
    QCOMPARE(ng.parameters().value(IMAGE).toString(), QString("image0"));
}

void Ut_NotificationManager::tesNotificationStorage()
{
    gEventTypeSettings["persistent"][PERSISTENT] = "true";

    delete manager;
    manager = new TestNotificationManager(3000);

    // Add two groups to the notification manager
    NotificationParameters gparameters0;
    gparameters0.add(IMAGE, "icon0");
    uint gid0 = manager->addGroup(0, gparameters0);
    NotificationParameters gparameters1;
    gparameters1.add(BODY, "body1");
    gparameters1.add(EVENT_TYPE, "persistent");
    uint gid1 = manager->addGroup(0, gparameters1);

    // Create three notifications
    NotificationParameters parameters0;
    parameters0.add(IMAGE, "icon0");
    uint id0 =manager->addNotification(0, parameters0);
    NotificationParameters parameters1;
    parameters1.add(BODY, "body1");
    parameters1.add(EVENT_TYPE, "persistent");
    uint id1 = manager->addNotification(0, parameters1, gid0);
    NotificationParameters parameters2;
    parameters2.add(ICON, "buttonicon2");
    manager->addNotification(0, parameters2, gid1);

    //Load notifications and check that they are restored
    loadNotifications();
    QCOMPARE((uint)gNotificationList.count(), (uint)3);
    Notification notification = gNotificationList.at(0);
    QCOMPARE(notification.parameters().value(IMAGE).toString(), QString("icon0"));
    notification = gNotificationList.at(1);
    QCOMPARE(notification.parameters().value(BODY).toString(), QString("body1"));
    notification = gNotificationList.at(2);
    QCOMPARE(notification.parameters().value(ICON).toString(), QString("buttonicon2"));

    //Remove one notification and check that it is removed from persistent storage
    manager->removeNotification(id0);
    loadNotifications();
    QCOMPARE((uint)gNotificationList.count(), (uint)2);
    notification = gNotificationList.at(0);
    QCOMPARE(notification.parameters().value(BODY).toString(), QString("body1"));
    notification = gNotificationList.at(1);
    QCOMPARE(notification.parameters().value(ICON).toString(), QString("buttonicon2"));

    //Verify that persistent storage notifications can be updated
    manager->updateNotification(0, id1, parameters0);
    loadNotifications();
    QCOMPARE((uint)gNotificationList.count(), (uint)2);
    notification = gNotificationList.at(0);
    QCOMPARE(notification.parameters().value(IMAGE).toString(), QString("icon0"));
}

void Ut_NotificationManager::testNotificationRestoration()
{
    delete manager;

    gTestingPersistent = true;

    gNotificationBuffer.buffer().clear();
    gNotificationBuffer.open(QIODevice::WriteOnly);
    QDataStream stream(&gNotificationBuffer);

    // Create three notifications
    NotificationParameters parameters0;
    parameters0.add(IMAGE, "icon0");
    stream << Notification(0, 3, 0, parameters0, Notification::ApplicationEvent, 0);

    NotificationParameters parameters1;
    parameters1.add(BODY, "body1");
    stream << Notification(1, 4, 0, parameters1, Notification::SystemEvent, 1000);

    NotificationParameters parameters2;
    parameters2.add(ICON, "buttonicon2");
    stream << Notification(2, 5, 0, parameters2, Notification::ApplicationEvent, 2000);

    gNotificationBuffer.close();

    manager = new TestNotificationManager(0);
    QSignalSpy spy(manager, SIGNAL(notificationRestored(Notification)));
    manager->restoreData();

    // Verify that timer was not started
    QCOMPARE(timerTimeouts.count(), 0);

    // Check that three notifications were created with the given parameters
    QCOMPARE(spy.count(), 3);

    QList<QVariant> arguments = spy.takeFirst();
    Notification n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), (uint)0);
    QCOMPARE(n.groupId(), (uint)3);
    QCOMPARE(n.parameters().value(IMAGE).toString(), QString("icon0"));
    QCOMPARE(n.type(), Notification::ApplicationEvent);
    QCOMPARE(n.timeout(), 0);

    arguments = spy.takeFirst();
    n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), (uint)1);
    QCOMPARE(n.groupId(), (uint)4);
    QCOMPARE(n.parameters().value(BODY).toString(), QString("body1"));
    QCOMPARE(n.type(), Notification::SystemEvent);
    QCOMPARE(n.timeout(), 1000);

    arguments = spy.takeFirst();
    n = qvariant_cast<Notification>(arguments.at(0));
    QCOMPARE(n.notificationId(), (uint)2);
    QCOMPARE(n.groupId(), (uint)5);
    QCOMPARE(n.parameters().value(ICON).toString(), QString("buttonicon2"));
    QCOMPARE(n.type(), Notification::ApplicationEvent);
    QCOMPARE(n.timeout(), 2000);

    gTestingPersistent = false;
}

void Ut_NotificationManager::testRemovingNotificationsWithEventType()
{
    QSignalSpy notificationRemovedSpy(manager, SIGNAL(notificationRemoved(uint)));

    // add a notification without event type
    manager->addNotification(0);

    // add a notification with sms event type
    NotificationParameters parameters0;
    parameters0.add(EVENT_TYPE, "sms");
    uint id0 = manager->addNotification(0, parameters0);

    // remove notifications with sms eventtype
    manager->removeNotificationsAndGroupsWithEventType("sms");
    QCOMPARE(notificationRemovedSpy.count(), 1);
    QCOMPARE(notificationRemovedSpy.takeFirst()[0].toUInt(), id0);
}

void Ut_NotificationManager::testRemovingGroupsWithEventType()
{
    QSignalSpy groupRemovedSpy(manager, SIGNAL(groupRemoved(uint)));

    // add a group without event type
    manager->addGroup(0);

    // add a group with sms event type
    NotificationParameters parameters0;
    parameters0.add(EVENT_TYPE, "sms");
    uint id1 = manager->addGroup(0, parameters0);

    // remove groups with sms eventtype
    manager->removeNotificationsAndGroupsWithEventType("sms");
    QCOMPARE(groupRemovedSpy.count(), 1);
    QCOMPARE(groupRemovedSpy.takeFirst()[0].toUInt(), id1);
}

void Ut_NotificationManager::testRemovalOfUnseenFlags()
{
    // Create a notification with "unseen" parameter set
    NotificationParameters param;
    param.add(UNSEEN, true);
    uint id = manager->addNotification(0, param);

    connect(this, SIGNAL(notifierSinkActive(bool)), manager, SLOT(removeUnseenFlags(bool)));
    emit notifierSinkActive(false);

    QCOMPARE(manager->notificationContainer.value(id).parameters().value(UNSEEN).toBool(), false);
}

void Ut_NotificationManager::testDBusNotificationSinkConnections()
{
    QVERIFY(disconnect(manager, SIGNAL(groupUpdated(uint, const NotificationParameters &)), manager->dBusSink, SLOT(addGroup(uint, const NotificationParameters &))));
    QVERIFY(disconnect(manager, SIGNAL(groupRemoved(uint)), manager->dBusSink, SLOT(removeGroup(uint))));
    QVERIFY(disconnect(manager, SIGNAL(notificationRemoved(uint)), manager->dBusSink, SLOT(removeNotification(uint))));
    QVERIFY(disconnect(manager, SIGNAL(notificationRestored(const Notification &)), manager->dBusSink, SLOT(addNotification(const Notification &))));
    QVERIFY(disconnect(manager, SIGNAL(notificationUpdated(const Notification &)), manager->dBusSink, SLOT(addNotification(const Notification &))));
    QVERIFY(disconnect(manager->dBusSink, SIGNAL(notificationRemovalRequested(uint)), manager, SLOT(removeNotification(uint))));
    QVERIFY(disconnect(manager->dBusSink, SIGNAL(notificationGroupClearingRequested(uint)), manager, SLOT(removeNotificationsInGroup(uint))));
    QVERIFY(disconnect(manager, SIGNAL(queuedGroupRemove(uint)), manager, SLOT(doRemoveGroup(uint))));
    QVERIFY(disconnect(manager, SIGNAL(queuedNotificationRemove(uint)), manager, SLOT(removeNotification(uint))));
}

void Ut_NotificationManager::testGetNotificationGroups()
{
    NotificationParameters params;
    addGroup(&params, "0", 1);
    addGroup(&params, "1", 2);
    QList<NotificationGroup> groups = manager->groups();
    QCOMPARE(groups.count(), 2);
    QCOMPARE(groups.at(0).groupId(), (uint)1);
    QCOMPARE(groups.at(1).groupId(), (uint)2);
}

void Ut_NotificationManager::testGetNotifications()
{
    NotificationParameters parameters0;
    uint id0 = addNotification(&parameters0, "0" ,1);

    NotificationParameters parameters1;
    uint id1 = addNotification(&parameters1, "1" ,2);

    QList<Notification> notifications = manager->notifications();
    QCOMPARE(notifications.count(), 2);
    QCOMPARE(notifications.at(0).notificationId(), id0);
    QCOMPARE(notifications.at(1).notificationId(), id1);
}

void Ut_NotificationManager::testNotificationCountInGroup()
{
    uint groupId1 = manager->addGroup(0, NotificationParameters());
    uint groupId2 = manager->addGroup(0, NotificationParameters());

    // Add two notifications to 1st group
    manager->addNotification(0, NotificationParameters(), groupId1);
    manager->addNotification(0, NotificationParameters(), groupId1);

    // Add a notification to 2nd group
    manager->addNotification(0, NotificationParameters(), groupId2);

    QCOMPARE(manager->notificationCountInGroup(0, groupId1), (uint)2);
}

QTEST_MAIN(Ut_NotificationManager)
