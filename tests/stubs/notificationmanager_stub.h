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
#ifndef NOTIFICATIONMANAGER_STUB
#define NOTIFICATIONMANAGER_STUB

#include "notificationmanager.h"
#include <stubbase.h>

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotificationManagerStub : public StubBase {
  public:
  virtual void NotificationManagerConstructor(int relayInterval, uint maxWaitQueueSize);
  virtual void NotificationManagerDestructor();
  virtual void restoreData();
  virtual uint addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId);
  virtual bool updateNotification(uint notificationUserId, uint notificationId, const NotificationParameters &parameters);
  virtual bool removeNotification(uint notificationUserId, uint notificationId);
  virtual uint addGroup(uint notificationUserId, const NotificationParameters &parameters);
  virtual bool updateGroup(uint notificationUserId, uint groupId, const NotificationParameters &parameters);
  virtual bool removeGroup(uint notificationUserId, uint groupId);
  virtual uint notificationUserId();
  virtual QList<uint> notificationIdList(uint notificationUserId);
  virtual QList<Notification> notificationList(uint notificationUserId);
  virtual QList<Notification> notificationListWithIdentifiers(uint notificationUserId);
  virtual QList<NotificationGroup> notificationGroupList(uint notificationUserId);
  virtual QList<NotificationGroup> notificationGroupListWithIdentifiers(uint notificationUserId);
  virtual bool removeNotification(uint notificationId);
  virtual bool removeNotificationsInGroup(uint groupId);
  virtual void removeNotificationsAndGroupsWithEventType(const QString &eventType);
  virtual void updateNotificationsWithEventType(const QString &eventType);
  virtual void relayNextNotification();
  virtual Notification::NotificationType determineType(const NotificationParameters &parameters);
  virtual void submitNotification(const Notification &notification);
  virtual int findNotificationFromWaitQueue(uint notificationId);
  virtual uint nextAvailableNotificationID();
  virtual uint nextAvailableGroupID();
  virtual void initializeNotificationUserIdDataStore();
  virtual void initializeEventTypeStore();
  virtual bool ensurePersistentDataPath();
  virtual void saveStateData();
  virtual void saveNotifications();
  virtual void removeUnseenFlags(bool ignore);
  virtual QList<Notification> notifications();
  virtual QList<NotificationGroup> groups();
  virtual void doRemoveGroup(uint groupId);
  virtual uint notificationCountInGroup(uint notificationUserId, uint groupId);
};

// 2. IMPLEMENT STUB
void NotificationManagerStub::NotificationManagerConstructor(int relayInterval, uint maxWaitQueueSize) {
  Q_UNUSED(relayInterval);
  Q_UNUSED(maxWaitQueueSize);

}
void NotificationManagerStub::NotificationManagerDestructor() {

}
void NotificationManagerStub::restoreData() {
  stubMethodEntered("restorePersistentData");
}

uint NotificationManagerStub::addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId) {
  QList<ParameterBase*> params;
  params.append(new Parameter<uint>(notificationUserId));
  params.append(new Parameter<NotificationParameters>(parameters));
  params.append(new Parameter<uint>(groupId));
  stubMethodEntered("addNotification", params);
  return stubReturnValue<uint>("addNotification");
}

bool NotificationManagerStub::updateNotification(uint notificationUserId, uint notificationId, const NotificationParameters &parameters) {
  QList<ParameterBase*> params;
  params.append(new Parameter<uint>(notificationUserId));
  params.append(new Parameter<uint>(notificationId));
  params.append(new Parameter<NotificationParameters>(parameters));
  stubMethodEntered("updateNotification", params);
  return stubReturnValue<bool>("updateNotification");
}

bool NotificationManagerStub::removeNotification(uint notificationUserId, uint notificationId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(notificationUserId));
  params.append( new Parameter<uint >(notificationId));
  stubMethodEntered("removeNotification",params);
  return stubReturnValue<bool>("removeNotification");
}

uint NotificationManagerStub::addGroup(uint notificationUserId, const NotificationParameters &parameters) {
  QList<ParameterBase*> params;
  params.append(new Parameter<uint>(notificationUserId));
  params.append(new Parameter<NotificationParameters>(parameters));
  stubMethodEntered("addGroup", params);
  return stubReturnValue<uint>("addGroup");
}

bool NotificationManagerStub::updateGroup(uint notificationUserId, uint groupId, const NotificationParameters &parameters) {
  QList<ParameterBase*> params;
  params.append(new Parameter<uint>(notificationUserId));
  params.append(new Parameter<uint>(groupId));
  params.append(new Parameter<NotificationParameters>(parameters));
  stubMethodEntered("updateGroup", params);
  return stubReturnValue<bool>("updateGroup");
}

bool NotificationManagerStub::removeGroup(uint notificationUserId, uint groupId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(notificationUserId));
  params.append( new Parameter<uint >(groupId));
  stubMethodEntered("removeGroup",params);
  return stubReturnValue<bool>("removeGroup");
}

uint NotificationManagerStub::notificationUserId() {
  stubMethodEntered("notificationUserId");
  return stubReturnValue<uint>("notificationUserId");
}

QList<uint> NotificationManagerStub::notificationIdList(uint notificationUserId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(notificationUserId));
  stubMethodEntered("notificationIdList",params);
  return stubReturnValue<QList<uint> >("notificationIdList");
}

QList<Notification> NotificationManagerStub::notificationList(uint notificationUserId)
{
    QList<ParameterBase*> params;
    params.append( new Parameter<uint >(notificationUserId));
    stubMethodEntered("notificationList",params);
    return stubReturnValue<QList<Notification> >("notificationList");
}

QList<Notification> NotificationManagerStub::notificationListWithIdentifiers(uint notificationUserId)
{
    QList<ParameterBase*> params;
    params.append( new Parameter<uint >(notificationUserId));
    stubMethodEntered("notificationListWithIdentifiers",params);
    return stubReturnValue<QList<Notification> >("notificationListWithIdentifiers");
}

QList<NotificationGroup> NotificationManagerStub::notificationGroupList(uint notificationUserId)
{
    QList<ParameterBase*> params;
    params.append( new Parameter<uint >(notificationUserId));
    stubMethodEntered("notificationGroupList",params);
    return stubReturnValue<QList<NotificationGroup> >("notificationGroupList");
}

QList<NotificationGroup> NotificationManagerStub::notificationGroupListWithIdentifiers(uint notificationUserId)
{
    QList<ParameterBase*> params;
    params.append( new Parameter<uint >(notificationUserId));
    stubMethodEntered("notificationGroupListWithIdentifiers",params);
    return stubReturnValue<QList<NotificationGroup> >("notificationGroupListWithIdentifiers");
}

bool NotificationManagerStub::removeNotification(uint notificationId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(notificationId));
  stubMethodEntered("removeNotification",params);
  return stubReturnValue<bool>("removeNotification");
}

bool NotificationManagerStub::removeNotificationsInGroup(uint groupId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(groupId));
  stubMethodEntered("removeNotificationsInGroup",params);
  return stubReturnValue<bool>("removeNotificationsInGroup");
}

void NotificationManagerStub::removeNotificationsAndGroupsWithEventType(const QString &eventType) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(eventType));
  stubMethodEntered("removeNotificationsAndGroupsWithEventType",params);
}

void NotificationManagerStub::updateNotificationsWithEventType(const QString &eventType) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(eventType));
  stubMethodEntered("updateNotificationsWithEventType",params);
}

void NotificationManagerStub::relayNextNotification() {
  stubMethodEntered("relayNextNotification");
}

Notification::NotificationType NotificationManagerStub::determineType(const NotificationParameters &parameters) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const NotificationParameters & >(parameters));
  stubMethodEntered("determineType",params);
  return stubReturnValue<Notification::NotificationType>("determineType");
}

void NotificationManagerStub::submitNotification(const Notification &notification) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const Notification & >(notification));
  stubMethodEntered("submitNotification",params);
}

int NotificationManagerStub::findNotificationFromWaitQueue(uint notificationId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(notificationId));
  stubMethodEntered("findNotificationFromWaitQueue",params);
  return stubReturnValue<int>("findNotificationFromWaitQueue");
}

uint NotificationManagerStub::nextAvailableNotificationID() {
  stubMethodEntered("nextAvailableNotificationID");
  return stubReturnValue<uint>("nextAvailableNotificationID");
}

uint NotificationManagerStub::nextAvailableGroupID() {
  stubMethodEntered("nextAvailableGroupID");
  return stubReturnValue<uint>("nextAvailableGroupID");
}

void NotificationManagerStub::initializeNotificationUserIdDataStore() {
  stubMethodEntered("initializeNotificationUserIdDataStore");
}

void NotificationManagerStub::initializeEventTypeStore() {
  stubMethodEntered("initializeEventTypeStore");
}

bool NotificationManagerStub::ensurePersistentDataPath() {
  stubMethodEntered("ensurePersistentDataPath");
  return stubReturnValue<bool>("ensurePersistentDataPath");
}

void NotificationManagerStub::saveStateData() {
  stubMethodEntered("saveStateData");
}

void NotificationManagerStub::saveNotifications() {
  stubMethodEntered("saveNotifications");
}

void NotificationManagerStub::removeUnseenFlags(bool ignore)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<bool>(ignore));
    return stubMethodEntered("removeUnseenFlags", params);
}

QList<Notification> NotificationManagerStub::notifications()
{
    stubMethodEntered("notifications");
    return stubReturnValue<QList<Notification> >("notifications");
}

QList<NotificationGroup> NotificationManagerStub::groups()
{
    stubMethodEntered("groups");
    return stubReturnValue<QList<NotificationGroup> >("groups");
}

void NotificationManagerStub::doRemoveGroup(uint groupId)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<uint>(groupId));
    stubMethodEntered("doRemoveGroup");
}

uint NotificationManagerStub::notificationCountInGroup(uint notificationUserId, uint groupId)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<uint>(notificationUserId));
    params.append(new Parameter<uint>(groupId));
    stubMethodEntered("notificationCountInGroup");
    return stubReturnValue<uint>("notificationCountInGroup");
}

// 3. CREATE A STUB INSTANCE
NotificationManagerStub gDefaultNotificationManagerStub;
NotificationManagerStub* gNotificationManagerStub = &gDefaultNotificationManagerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
NotificationManager::NotificationManager(int relayInterval, uint maxWaitQueueSize) : maxWaitQueueSize(uint()) {
  gNotificationManagerStub->NotificationManagerConstructor(relayInterval, maxWaitQueueSize);
}

NotificationManager::~NotificationManager() {
  gNotificationManagerStub->NotificationManagerDestructor();
}

void NotificationManager::restoreData() {
  gNotificationManagerStub->restoreData();
}

uint NotificationManager::addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId) {
  return gNotificationManagerStub->addNotification(notificationUserId, parameters, groupId);
}

bool NotificationManager::updateNotification(uint notificationUserId, uint notificationId, const NotificationParameters &parameters) {
  return gNotificationManagerStub->updateNotification(notificationUserId, notificationId, parameters);
}

bool NotificationManager::removeNotification(uint notificationUserId, uint notificationId) {
  return gNotificationManagerStub->removeNotification(notificationUserId, notificationId);
}

uint NotificationManager::addGroup(uint notificationUserId, const NotificationParameters &parameters) {
  return gNotificationManagerStub->addGroup(notificationUserId, parameters);
}

bool NotificationManager::updateGroup(uint notificationUserId, uint groupId, const NotificationParameters &parameters) {
  return gNotificationManagerStub->updateGroup(notificationUserId, groupId, parameters);
}

bool NotificationManager::removeGroup(uint notificationUserId, uint groupId) {
  return gNotificationManagerStub->removeGroup(notificationUserId, groupId);
}

uint NotificationManager::notificationUserId() {
  return gNotificationManagerStub->notificationUserId();
}

QList<uint> NotificationManager::notificationIdList(uint notificationUserId) {
  return gNotificationManagerStub->notificationIdList(notificationUserId);
}

QList<Notification> NotificationManager::notificationList(uint notificationUserId) {
    return gNotificationManagerStub->notificationList(notificationUserId);
}

QList<Notification> NotificationManager::notificationListWithIdentifiers(uint notificationUserId) {
    return gNotificationManagerStub->notificationListWithIdentifiers(notificationUserId);
}

QList<NotificationGroup> NotificationManager::notificationGroupList(uint notificationUserId) {
    return gNotificationManagerStub->notificationGroupList(notificationUserId);
}

QList<NotificationGroup> NotificationManager::notificationGroupListWithIdentifiers(uint notificationUserId) {
    return gNotificationManagerStub->notificationGroupListWithIdentifiers(notificationUserId);
}

bool NotificationManager::removeNotification(uint notificationId) {
  return gNotificationManagerStub->removeNotification(notificationId);
}

bool NotificationManager::removeNotificationsInGroup(uint groupId) {
  return gNotificationManagerStub->removeNotificationsInGroup(groupId);
}

void NotificationManager::removeNotificationsAndGroupsWithEventType(const QString &eventType) {
  gNotificationManagerStub->removeNotificationsAndGroupsWithEventType(eventType);
}

void NotificationManager::updateNotificationsWithEventType(const QString &eventType) {
  gNotificationManagerStub->updateNotificationsWithEventType(eventType);
}

void NotificationManager::relayNextNotification() {
  gNotificationManagerStub->relayNextNotification();
}

Notification::NotificationType NotificationManager::determineType(const NotificationParameters &parameters) {
  return gNotificationManagerStub->determineType(parameters);
}

void NotificationManager::submitNotification(const Notification &notification) {
  gNotificationManagerStub->submitNotification(notification);
}

int NotificationManager::findNotificationFromWaitQueue(uint notificationId) {
  return gNotificationManagerStub->findNotificationFromWaitQueue(notificationId);
}

uint NotificationManager::nextAvailableNotificationID() {
  return gNotificationManagerStub->nextAvailableNotificationID();
}

uint NotificationManager::nextAvailableGroupID() {
  return gNotificationManagerStub->nextAvailableGroupID();
}

void NotificationManager::initializeNotificationUserIdDataStore() {
  gNotificationManagerStub->initializeNotificationUserIdDataStore();
}

void NotificationManager::initializeEventTypeStore() {
  gNotificationManagerStub->initializeEventTypeStore();
}

bool NotificationManager::ensurePersistentDataPath() {
  return gNotificationManagerStub->ensurePersistentDataPath();
}

void NotificationManager::saveStateData() {
  gNotificationManagerStub->saveStateData();
}

void NotificationManager::removeUnseenFlags(bool ignore)
{
    gNotificationManagerStub->removeUnseenFlags(ignore);
}

QList<Notification> NotificationManager::notifications() const
{
    return gNotificationManagerStub->notifications();
}

QList<NotificationGroup> NotificationManager::groups() const
{
    return gNotificationManagerStub->groups();
}

QObject *NotificationManager::qObject()
{
    return this;
}

void NotificationManager::doRemoveGroup(uint groupId)
{
    gNotificationManagerStub->doRemoveGroup(groupId);
}

uint NotificationManager::notificationCountInGroup(uint notificationUserId, uint groupId)
{
    return gNotificationManagerStub->notificationCountInGroup(notificationUserId, groupId);
}

#endif
