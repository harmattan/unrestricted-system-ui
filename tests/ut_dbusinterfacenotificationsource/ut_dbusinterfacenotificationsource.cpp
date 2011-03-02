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

#include "ut_dbusinterfacenotificationsource.h"
#include "dbusinterfacenotificationsource.h"
#include "dbusinterfacenotificationsourceadaptor.h"
#include "genericnotificationparameterfactory.h"
#include "notificationwidgetparameterfactory.h"
#include "notificationmanager_stub.h"

// DBusInterfaceNotificationSourceAdaptor stubs (used by NotificationManager)
DBusInterfaceNotificationSourceAdaptor::DBusInterfaceNotificationSourceAdaptor(DBusInterfaceNotificationSource *parent) : QDBusAbstractAdaptor(parent)
{
}

DBusInterfaceNotificationSourceAdaptor::~DBusInterfaceNotificationSourceAdaptor()
{
}

bool DBusInterfaceNotificationSourceAdaptor::removeNotification(uint, uint)
{
    return true;
}

uint DBusInterfaceNotificationSourceAdaptor::addNotification(uint, uint, const QString &)
{
    return 0;
}

uint DBusInterfaceNotificationSourceAdaptor::addNotification(uint, uint, const QString &, const QString &, const QString &, const QString &, const QString &, uint)
{
    return 0;
}

uint DBusInterfaceNotificationSourceAdaptor::addNotification(uint, uint, const QString &, const QString &, const QString &, const QString &, const QString &, uint, const QString &)
{
    return 0;
}

bool DBusInterfaceNotificationSourceAdaptor::updateNotification(uint, uint, const QString &)
{
    return true;
}

bool DBusInterfaceNotificationSourceAdaptor::updateNotification(uint, uint, const QString &, const QString &, const QString &, const QString &, const QString &, uint)
{
    return true;
}

bool DBusInterfaceNotificationSourceAdaptor::updateNotification(uint, uint, const QString &, const QString &, const QString &, const QString &, const QString &, uint, const QString &)
{
    return true;
}

uint DBusInterfaceNotificationSourceAdaptor::addGroup(uint, const QString &)
{
    return 0;
}

uint DBusInterfaceNotificationSourceAdaptor::addGroup(uint, const QString &, const QString &, const QString &, const QString &, const QString &, uint)
{
    return 0;
}

uint DBusInterfaceNotificationSourceAdaptor::addGroup(uint, const QString &, const QString &, const QString &, const QString &, const QString &, uint, const QString &)
{
    return 0;
}

bool DBusInterfaceNotificationSourceAdaptor::removeGroup(uint, uint)
{
    return true;
}

bool DBusInterfaceNotificationSourceAdaptor::updateGroup(uint, uint, const QString &)
{
    return true;
}

bool DBusInterfaceNotificationSourceAdaptor::updateGroup(uint, uint, const QString &, const QString &, const QString &, const QString &, const QString &, uint)
{
    return true;
}

bool DBusInterfaceNotificationSourceAdaptor::updateGroup(uint, uint, const QString &, const QString &, const QString &, const QString &, const QString &, uint, const QString &)
{
    return true;
}

uint DBusInterfaceNotificationSourceAdaptor::notificationUserId()
{
    return 1;
}

uint DBusInterfaceNotificationSourceAdaptor::notificationCountInGroup(uint, uint)
{
    return 1;
}

QList<uint> DBusInterfaceNotificationSourceAdaptor::notificationIdList(uint notificationUserId)
{
    QList<uint> tmp;
    Q_UNUSED(notificationUserId);
    return tmp;
}

QList < MNotificationProxy >  DBusInterfaceNotificationSourceAdaptor::notificationList(uint)
{
    QList<MNotificationProxy> l;
    return l;
}

QList < MNotificationWithIdentifierProxy>  DBusInterfaceNotificationSourceAdaptor::notificationListWithIdentifiers(uint)
{
    QList<MNotificationWithIdentifierProxy> l;
    return l;
}

QList<MNotificationGroupProxy> DBusInterfaceNotificationSourceAdaptor::notificationGroupList(uint)
{
    QList<MNotificationGroupProxy> l;
    return l;
}

QList<MNotificationGroupWithIdentifierProxy> DBusInterfaceNotificationSourceAdaptor::notificationGroupListWithIdentifiers(uint)
{
    QList<MNotificationGroupWithIdentifierProxy> l;
    return l;
}

void Ut_DBusInterfaceNotificationSource::initTestCase()
{
}

void Ut_DBusInterfaceNotificationSource::cleanupTestCase()
{
}

void Ut_DBusInterfaceNotificationSource::init()
{
    gDefaultNotificationManagerStub.stubReset();

    manager = new NotificationManager();
    source = new DBusInterfaceNotificationSource(*manager);
}

void Ut_DBusInterfaceNotificationSource::cleanup()
{
    delete source;
    delete manager;
}

const uint USER_ID = 1;
const uint NOTIFICATION_ID1 = 3;
const uint NOTIFICATION_ID2 = 4;
const uint NOTIFICATION_GROUP_ID1 = 0;
const uint NOTIFICATION_GROUP_ID2 = 1;
const QString IDENTIFIER1("identifier1");
const QString IDENTIFIER2("identifier2");

const QString EVENT = "event";
const QString SUMMARY = "summary";
const QString BODY = "body";
const QString ACTION = "action";
const QString IMAGE = "image";
const uint COUNT = 1;
const QString IDENTIFIER = "identifier";
const bool CALL_TO_NOTIFICATION_MANAGER_SUCCEEDS = true;
const bool CALL_TO_NOTIFICATION_MANAGER_DOES_NOT_SUCCEED = false;
const uint NEW_NOTIFICATION_ID = 9;
const uint NEW_GROUP_ID = 11;

void Ut_DBusInterfaceNotificationSource::testAddNotification()
{
    source->addNotification(1, 2, "event");
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("addNotification").parameter<uint>(0), (uint)1);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("addNotification").parameter<uint>(2), (uint)2);
    NotificationParameters params = gDefaultNotificationManagerStub.stubLastCallTo("addNotification").parameter<NotificationParameters>(1);
    QCOMPARE(params.value(GenericNotificationParameterFactory::eventTypeKey()), QVariant("event"));

    source->addNotification(3, 4, "event", "summary", "body", "action", "imageURI", 1);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("addNotification").parameter<uint>(0), (uint)3);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("addNotification").parameter<uint>(2), (uint)4);
    params = gDefaultNotificationManagerStub.stubLastCallTo("addNotification").parameter<NotificationParameters>(1);
    QCOMPARE(params.value(GenericNotificationParameterFactory::eventTypeKey()), QVariant("event"));
    QCOMPARE(params.value(GenericNotificationParameterFactory::countKey()), QVariant("1"));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::summaryKey()), QVariant("summary"));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::bodyKey()), QVariant("body"));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::imageIdKey()), QVariant("imageURI"));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::actionKey()), QVariant("action"));

    source->addNotification(5, 6, "event", "summary", "body", "action", "imageURI", 42);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("addNotification").parameter<uint>(0), (uint)5);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("addNotification").parameter<uint>(2), (uint)6);
    params = gDefaultNotificationManagerStub.stubLastCallTo("addNotification").parameter<NotificationParameters>(1);
    QCOMPARE(params.value(GenericNotificationParameterFactory::countKey()), QVariant("42"));
}

void Ut_DBusInterfaceNotificationSource::testAddNotificationWithIdentifier()
{
    gDefaultNotificationManagerStub.stubSetReturnValue("addNotification", NEW_NOTIFICATION_ID);

    QCOMPARE(source->addNotification(USER_ID, NOTIFICATION_GROUP_ID1, EVENT, SUMMARY, BODY, ACTION, IMAGE, COUNT, IDENTIFIER), NEW_NOTIFICATION_ID);

    NotificationParameters params = gDefaultNotificationManagerStub.stubLastCallTo("addNotification").parameter<NotificationParameters>(1);
    QCOMPARE(params.value(GenericNotificationParameterFactory::identifierKey()), QVariant(IDENTIFIER));
}

void Ut_DBusInterfaceNotificationSource::testUpdateNotification()
{
    source->updateNotification(1, 2, "event");
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("updateNotification").parameter<uint>(0), (uint)1);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("updateNotification").parameter<uint>(1), (uint)2);
    NotificationParameters params = gDefaultNotificationManagerStub.stubLastCallTo("updateNotification").parameter<NotificationParameters>(2);
    QCOMPARE(params.value(GenericNotificationParameterFactory::eventTypeKey()), QVariant("event"));

    source->updateNotification(3, 4, "event", "summary", "body", "action", "imageURI", 42);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("updateNotification").parameter<uint>(0), (uint)3);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("updateNotification").parameter<uint>(1), (uint)4);

    params = gDefaultNotificationManagerStub.stubLastCallTo("updateNotification").parameter<NotificationParameters>(2);
    QCOMPARE(params.value(GenericNotificationParameterFactory::eventTypeKey()), QVariant("event"));
    QCOMPARE(params.value(GenericNotificationParameterFactory::countKey()), QVariant("42"));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::summaryKey()), QVariant("summary"));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::bodyKey()), QVariant("body"));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::imageIdKey()), QVariant("imageURI"));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::actionKey()), QVariant("action"));
}

void Ut_DBusInterfaceNotificationSource::testUpdateNotificationWithIdentifier()
{
    gDefaultNotificationManagerStub.stubSetReturnValue("addNotification", NEW_NOTIFICATION_ID);
    gDefaultNotificationManagerStub.stubSetReturnValue("updateNotification", CALL_TO_NOTIFICATION_MANAGER_SUCCEEDS);

    uint id = source->addNotification(USER_ID, NOTIFICATION_GROUP_ID1, EVENT, SUMMARY, BODY, ACTION, IMAGE, COUNT);
    QCOMPARE(source->updateNotification(USER_ID, id, EVENT, SUMMARY, BODY, ACTION, IMAGE, COUNT, IDENTIFIER), CALL_TO_NOTIFICATION_MANAGER_SUCCEEDS);

    NotificationParameters params = gDefaultNotificationManagerStub.stubLastCallTo("updateNotification").parameter<NotificationParameters>(2);
    QCOMPARE(params.value(GenericNotificationParameterFactory::identifierKey()), QVariant(IDENTIFIER));
}

void Ut_DBusInterfaceNotificationSource::testWhenUpdatingNotificationInManagerFailsThenUpdateNotificationReturnsFalse()
{
    gDefaultNotificationManagerStub.stubSetReturnValue("updateNotification", CALL_TO_NOTIFICATION_MANAGER_DOES_NOT_SUCCEED);
    QCOMPARE(source->updateNotification(USER_ID, NEW_NOTIFICATION_ID, EVENT, SUMMARY, BODY, ACTION, IMAGE, COUNT, IDENTIFIER), CALL_TO_NOTIFICATION_MANAGER_DOES_NOT_SUCCEED);
}

void Ut_DBusInterfaceNotificationSource::testRemoveNotification()
{
    source->removeNotification(1, 2);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("removeNotification").parameter<uint>(0), (uint)1);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("removeNotification").parameter<uint>(1), (uint)2);
}

void Ut_DBusInterfaceNotificationSource::testAddGroup()
{
    source->addGroup(1, "event");
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("addGroup").parameter<uint>(0), (uint)1);
    NotificationParameters params = gDefaultNotificationManagerStub.stubLastCallTo("addGroup").parameter<NotificationParameters>(1);
    QCOMPARE(params.value(GenericNotificationParameterFactory::eventTypeKey()), QVariant("event"));

    source->addGroup(2, "event", "summary", "body", "action", "imageURI", 1);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("addGroup").parameter<uint>(0), (uint)2);

    params = gDefaultNotificationManagerStub.stubLastCallTo("addGroup").parameter<NotificationParameters>(1);
    QCOMPARE(params.value(GenericNotificationParameterFactory::eventTypeKey()), QVariant("event"));
    QCOMPARE(params.value(GenericNotificationParameterFactory::countKey()), QVariant("1"));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::summaryKey()), QVariant("summary"));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::bodyKey()), QVariant("body"));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::imageIdKey()), QVariant("imageURI"));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::actionKey()), QVariant("action"));
}

void Ut_DBusInterfaceNotificationSource::testAddGroupWithIdentifier()
{
    gDefaultNotificationManagerStub.stubSetReturnValue("addGroup", NEW_GROUP_ID);

    QCOMPARE(source->addGroup(USER_ID, EVENT, SUMMARY, BODY, ACTION, IMAGE, COUNT, IDENTIFIER), NEW_GROUP_ID);

    NotificationParameters params = gDefaultNotificationManagerStub.stubLastCallTo("addGroup").parameter<NotificationParameters>(1);
    QCOMPARE(params.value(GenericNotificationParameterFactory::identifierKey()), QVariant(IDENTIFIER));
}

void Ut_DBusInterfaceNotificationSource::testUpdateGroup()
{
    source->updateGroup(1, 2, "event");
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("updateGroup").parameter<uint>(0), (uint)1);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("updateGroup").parameter<uint>(1), (uint)2);
    NotificationParameters params = gDefaultNotificationManagerStub.stubLastCallTo("updateGroup").parameter<NotificationParameters>(2);
    QCOMPARE(params.value(GenericNotificationParameterFactory::eventTypeKey()), QVariant("event"));

    source->updateGroup(3, 4, "event", "summary", "body", "action", "imageURI", 42);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("updateGroup").parameter<uint>(0), (uint)3);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("updateGroup").parameter<uint>(1), (uint)4);
    params = gDefaultNotificationManagerStub.stubLastCallTo("updateGroup").parameter<NotificationParameters>(2);
    QCOMPARE(params.value(GenericNotificationParameterFactory::eventTypeKey()), QVariant("event"));
    QCOMPARE(params.value(GenericNotificationParameterFactory::countKey()), QVariant("42"));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::summaryKey()), QVariant("summary"));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::bodyKey()), QVariant("body"));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::imageIdKey()), QVariant("imageURI"));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::actionKey()), QVariant("action"));
}

void Ut_DBusInterfaceNotificationSource::testUpdateGroupWithIdentifier()
{
    gDefaultNotificationManagerStub.stubSetReturnValue("addGroup", NEW_GROUP_ID);
    gDefaultNotificationManagerStub.stubSetReturnValue("updateGroup", CALL_TO_NOTIFICATION_MANAGER_SUCCEEDS);

    uint id = source->addGroup(USER_ID, EVENT, SUMMARY, BODY, ACTION, IMAGE, COUNT);
    QCOMPARE(source->updateGroup(USER_ID, id, EVENT, SUMMARY, BODY, ACTION, IMAGE, COUNT, IDENTIFIER), CALL_TO_NOTIFICATION_MANAGER_SUCCEEDS);

    NotificationParameters params = gDefaultNotificationManagerStub.stubLastCallTo("updateGroup").parameter<NotificationParameters>(2);
    QCOMPARE(params.value(GenericNotificationParameterFactory::identifierKey()), QVariant(IDENTIFIER));
}

void Ut_DBusInterfaceNotificationSource::testWhenUpdatingGroupInManagerFailsThenUpdateGroupReturnsFalse()
{
    gDefaultNotificationManagerStub.stubSetReturnValue("updateGroup", CALL_TO_NOTIFICATION_MANAGER_DOES_NOT_SUCCEED);
    QCOMPARE(source->updateGroup(USER_ID, NEW_GROUP_ID, EVENT, SUMMARY, BODY, ACTION, IMAGE, COUNT, IDENTIFIER), CALL_TO_NOTIFICATION_MANAGER_DOES_NOT_SUCCEED);
}

void Ut_DBusInterfaceNotificationSource::testRemoveGroup()
{
    source->removeGroup(1, 2);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("removeGroup").parameter<uint>(0), (uint)1);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("removeGroup").parameter<uint>(1), (uint)2);
}

void Ut_DBusInterfaceNotificationSource::testNotificationIdList()
{
    QList<uint> tmp;
    tmp << 1 << 2 << 3 << 4 << 5;
    gDefaultNotificationManagerStub.stubSetReturnValue("notificationIdList", tmp);

    QList<uint> result = source->notificationIdList(10);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("notificationIdList").parameter<uint>(0), (uint)10);

    QCOMPARE(result, tmp);
}

void Ut_DBusInterfaceNotificationSource::testNotificationList()
{
    QList<Notification> expectedResults;
    NotificationParameters params;
    expectedResults.append(Notification(NOTIFICATION_ID1, NOTIFICATION_GROUP_ID1, USER_ID, params, Notification::ApplicationEvent, 0));
    expectedResults.append(Notification(NOTIFICATION_ID2, NOTIFICATION_GROUP_ID2, USER_ID, params, Notification::ApplicationEvent, 0));
    gNotificationManagerStub->stubSetReturnValue("notificationList", expectedResults);

    QList<MNotificationProxy> receivedResults = source->notificationList(USER_ID);
    QCOMPARE(gDefaultNotificationManagerStub.stubCallCount("notificationList"), 1);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("notificationList").parameter<uint>(0), USER_ID);

    QCOMPARE(receivedResults.count(), 2);
    const MNotificationProxy &res0(receivedResults.at(0));
    QCOMPARE(res0.notificationId, NOTIFICATION_ID1);
    QCOMPARE(res0.groupId, NOTIFICATION_GROUP_ID1);

    const MNotificationProxy &res1(receivedResults.at(1));
    QCOMPARE(res1.notificationId, NOTIFICATION_ID2);
    QCOMPARE(res1.groupId, NOTIFICATION_GROUP_ID2);
}

void Ut_DBusInterfaceNotificationSource::testNotificationListWithIdentifiers()
{
    QList<Notification> expectedResults;
    NotificationParameters params;
    params.add(IDENTIFIER, IDENTIFIER1);
    expectedResults.append(Notification(NOTIFICATION_ID1, NOTIFICATION_GROUP_ID1, USER_ID, params, Notification::ApplicationEvent, 0));
    params.add(IDENTIFIER, IDENTIFIER2);
    expectedResults.append(Notification(NOTIFICATION_ID2, NOTIFICATION_GROUP_ID2, USER_ID, params, Notification::ApplicationEvent, 0));
    gNotificationManagerStub->stubSetReturnValue("notificationListWithIdentifiers", expectedResults);

    QList<MNotificationWithIdentifierProxy> receivedResults = source->notificationListWithIdentifiers(USER_ID);

    QCOMPARE(gDefaultNotificationManagerStub.stubCallCount("notificationListWithIdentifiers"), 1);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("notificationListWithIdentifiers").parameter<uint>(0), USER_ID);

    QCOMPARE(receivedResults.count(), 2);
    const MNotificationWithIdentifierProxy &res0(receivedResults.at(0));
    QCOMPARE(res0.notificationId, NOTIFICATION_ID1);
    QCOMPARE(res0.groupId, NOTIFICATION_GROUP_ID1);
    QCOMPARE(res0.identifier, IDENTIFIER1);

    const MNotificationWithIdentifierProxy &res1(receivedResults.at(1));
    QCOMPARE(res1.notificationId, NOTIFICATION_ID2);
    QCOMPARE(res1.groupId, NOTIFICATION_GROUP_ID2);
    QCOMPARE(res1.identifier, IDENTIFIER2);
}

void Ut_DBusInterfaceNotificationSource::testNotificationGroupList()
{
    QList<NotificationGroup> expectedResults;
    NotificationParameters params;
    expectedResults.append(NotificationGroup(NOTIFICATION_GROUP_ID1, USER_ID, params));
    expectedResults.append(NotificationGroup(NOTIFICATION_GROUP_ID2, USER_ID, params));
    gNotificationManagerStub->stubSetReturnValue("notificationGroupList", expectedResults);

    QList<MNotificationGroupProxy> receivedResults = source->notificationGroupList(USER_ID);
    QCOMPARE(gDefaultNotificationManagerStub.stubCallCount("notificationGroupList"), 1);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("notificationGroupList").parameter<uint>(0), USER_ID);

    QCOMPARE(receivedResults.count(), 2);
    const MNotificationGroupProxy &res0(receivedResults.at(0));
    QCOMPARE(res0.groupId, NOTIFICATION_GROUP_ID1);

    const MNotificationGroupProxy &res1(receivedResults.at(1));
    QCOMPARE(res1.groupId, NOTIFICATION_GROUP_ID2);
}

void Ut_DBusInterfaceNotificationSource::testNotificationGroupListWithIdentifiers()
{
    QList<NotificationGroup> expectedResults;
    NotificationParameters params;
    params.add(IDENTIFIER, IDENTIFIER1);
    expectedResults.append(NotificationGroup(NOTIFICATION_GROUP_ID1, USER_ID, params));
    params.add(IDENTIFIER, IDENTIFIER2);
    expectedResults.append(NotificationGroup(NOTIFICATION_GROUP_ID2, USER_ID, params));
    gNotificationManagerStub->stubSetReturnValue("notificationGroupListWithIdentifiers", expectedResults);

    QList<MNotificationGroupWithIdentifierProxy> receivedResults = source->notificationGroupListWithIdentifiers(USER_ID);
    QCOMPARE(gDefaultNotificationManagerStub.stubCallCount("notificationGroupListWithIdentifiers"), 1);
    QCOMPARE(gDefaultNotificationManagerStub.stubLastCallTo("notificationGroupListWithIdentifiers").parameter<uint>(0), USER_ID);

    QCOMPARE(receivedResults.count(), 2);
    const MNotificationGroupWithIdentifierProxy &res0(receivedResults.at(0));
    QCOMPARE(res0.groupId, NOTIFICATION_GROUP_ID1);
    QCOMPARE(res0.groupIdentifier, IDENTIFIER1);

    const MNotificationGroupWithIdentifierProxy &res1(receivedResults.at(1));
    QCOMPARE(res1.groupId, NOTIFICATION_GROUP_ID2);
    QCOMPARE(res1.groupIdentifier, IDENTIFIER2);
}

void Ut_DBusInterfaceNotificationSource::testUpdateGroupWithEmptyStrings()
{
    source->updateGroup(3, 4, "event", "summary", "body", "action", "imageURI", 42);
    NotificationParameters params = gDefaultNotificationManagerStub.stubLastCallTo("updateGroup").parameter<NotificationParameters>(2);
    QCOMPARE(params.value(NotificationWidgetParameterFactory::summaryKey()), QVariant("summary"));

    source->updateGroup(3, 4, "event", "", "", "", "", 42);
    params = gDefaultNotificationManagerStub.stubLastCallTo("updateGroup").parameter<NotificationParameters>(2);
    QCOMPARE(params.value(GenericNotificationParameterFactory::eventTypeKey()), QVariant("event"));
    QCOMPARE(params.value(GenericNotificationParameterFactory::countKey()), QVariant("42"));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::summaryKey()), QVariant(""));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::bodyKey()), QVariant(""));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::imageIdKey()), QVariant(""));
    QCOMPARE(params.value(NotificationWidgetParameterFactory::actionKey()), QVariant(""));
}

void Ut_DBusInterfaceNotificationSource::testNotificationCountInGroup()
{
    source->notificationCountInGroup(1, 1);
    QCOMPARE(gDefaultNotificationManagerStub.stubCallCount("notificationCountInGroup"), 1);
}

QTEST_APPLESS_MAIN(Ut_DBusInterfaceNotificationSource)
