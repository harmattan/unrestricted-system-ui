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
#include <QDBusMetaType>
#include "metatypedeclarations.h"
#include "notificationmanager.h"
#include "notification.h"
#include "mnotificationproxy.h"
#include "dbusinterfacenotificationsource.h"
#include "dbusinterfacenotificationsink.h"
#include "contextframeworkcontext.h"
#include "genericnotificationparameterfactory.h"
#include "notificationwidgetparameterfactory.h"
#include <QDBusConnection>
#include <QDir>
#include <QDateTime>
#include <mfiledatastore.h>
#include <QFile>

//! Directory in which the persistent data files are located
static const QString PERSISTENT_DATA_PATH = QDir::homePath() + QString("/.config/sysuid/notificationmanager/");

//! The number configuration files to load into the event type store.
static const uint MAX_EVENT_TYPE_CONF_FILES = 100;

//! Name of the file where persistent status data is stored
static const QString STATE_DATA_FILE_NAME = PERSISTENT_DATA_PATH + QString("state.data");

//! Name of the file where persistent notifications are stored
static const QString NOTIFICATIONS_FILE_NAME = PERSISTENT_DATA_PATH + QString("notifications.data");

//! System notifications are identified with 'system' string literal
static const QString SYSTEM_EVENT_ID = "system";

NotificationManager::NotificationManager(int relayInterval, uint maxWaitQueueSize) :
    notificationContainer(),
    groupContainer(),
    maxWaitQueueSize(maxWaitQueueSize),
    notificationInProgress(false),
    relayInterval(relayInterval),
    context(new ContextFrameworkContext()),
    lastUsedNotificationUserId(0),
    persistentDataRestored(false)
{
    dBusSource = new DBusInterfaceNotificationSource(*this);
    dBusSink = new DBusInterfaceNotificationSink(this);

    connect(this, SIGNAL(groupUpdated(uint, const NotificationParameters &)), dBusSink, SLOT(addGroup(uint, const NotificationParameters &)));
    connect(this, SIGNAL(groupRemoved(uint)), dBusSink, SLOT(removeGroup(uint)));
    connect(this, SIGNAL(notificationRemoved(uint)), dBusSink, SLOT(removeNotification(uint)));
    connect(this, SIGNAL(notificationRestored(const Notification &)), dBusSink, SLOT(addNotification(const Notification &)));
    connect(this, SIGNAL(notificationUpdated(const Notification &)), dBusSink, SLOT(addNotification(const Notification &)));
    connect(dBusSink, SIGNAL(notificationRemovalRequested(uint)), this, SLOT(removeNotification(uint)));
    connect(dBusSink, SIGNAL(notificationGroupClearingRequested(uint)), this, SLOT(removeNotificationsInGroup(uint)));
    connect(this, SIGNAL(queuedGroupRemove(uint)), this, SLOT(doRemoveGroup(uint)), Qt::QueuedConnection);
    connect(this, SIGNAL(queuedNotificationRemove(uint)), this, SLOT(removeNotification(uint)), Qt::QueuedConnection);

    waitQueueTimer.setSingleShot(true);
    connect(&waitQueueTimer, SIGNAL(timeout()), this, SLOT(relayNextNotification()));

    if (!QDir::root().exists(PERSISTENT_DATA_PATH)) {
        // No data to restore exists yet
        persistentDataRestored = true;
    }

    //Initialize the event type store
    initializeEventTypeStore();

    // Connect to D-Bus and register the DBus source as an object
    QDBusConnection::sessionBus().registerService("com.meego.core.MNotificationManager");
    QDBusConnection::sessionBus().registerObject("/notificationmanager", dBusSource);
    QDBusConnection::sessionBus().registerObject("/notificationsinkmanager", dBusSink);
}

NotificationManager::~NotificationManager()
{
    delete dBusSource;
    delete dBusSink;
    delete context;
}

bool NotificationManager::ensurePersistentDataPath()
{
    // Create the data store path if it does not exist yet
    if (!QDir::root().exists(PERSISTENT_DATA_PATH)) {
        return QDir::root().mkpath(PERSISTENT_DATA_PATH);
    }

    return true;
}

void NotificationManager::saveStateData()
{
    if (ensurePersistentDataPath()) {
        QFile file(STATE_DATA_FILE_NAME);
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QDataStream stream;
            stream.setDevice(&file);

            stream << lastUsedNotificationUserId;

            foreach(const NotificationGroup & group, groupContainer) {
                stream << group;
            }
            file.close();
        }
    }
}


void NotificationManager::saveNotifications()
{
    if (ensurePersistentDataPath()) {
        if (notificationContainer.size()) {
            QFile file(NOTIFICATIONS_FILE_NAME);
            if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                QDataStream stream;
                stream.setDevice(&file);

                foreach(const Notification &notification, notificationContainer) {
                    stream << notification;
                }
                file.close();
            }
        } else {
            QFile::remove(NOTIFICATIONS_FILE_NAME);
        }
    }
}

void NotificationManager::restoreData()
{
    if (!persistentDataRestored && ensurePersistentDataPath()) {
        // If the data is corrupted or tampered with, the previous state
        // is lost and there's nothing we can do.
        persistentDataRestored = true;

        QFile stateFile(STATE_DATA_FILE_NAME);

        if (stateFile.open(QIODevice::ReadOnly)) {
            QDataStream stream;
            stream.setDevice(&stateFile);

            stream >> lastUsedNotificationUserId;

            NotificationGroup group;

            while (!stream.atEnd()) {
                stream >> group;
                groupContainer.insert(group.groupId(), group);
                emit groupUpdated(group.groupId(), group.parameters());
            }
            stateFile.close();
        }

        QFile notificationFile(NOTIFICATIONS_FILE_NAME);

        if (notificationFile.open(QIODevice::ReadOnly)) {
            QDataStream stream;
            stream.setDevice(&notificationFile);

            Notification notification;

            while (!stream.atEnd()) {
                stream >> notification;
                notificationContainer.insert(notification.notificationId(), notification);
                emit notificationRestored(notification);
            }
            notificationFile.close();
        }
    }
}


void NotificationManager::initializeEventTypeStore()
{
    if (notificationEventTypeStore) {
        return;
    }
    notificationEventTypeStore = QSharedPointer<EventTypeStore> (new EventTypeStore(NOTIFICATIONS_EVENT_TYPES, MAX_EVENT_TYPE_CONF_FILES));

    connect(notificationEventTypeStore.data(), SIGNAL(eventTypeUninstalled(QString)),
            this, SLOT(removeNotificationsAndGroupsWithEventType(QString)));
    connect(notificationEventTypeStore.data(), SIGNAL(eventTypeModified(QString)),
            this, SLOT(updateNotificationsWithEventType(QString)));
}

void NotificationManager::removeNotificationsAndGroupsWithEventType(const QString &eventType)
{
    foreach(const Notification &notification, notificationContainer) {
        if(notification.parameters().value(GenericNotificationParameterFactory::eventTypeKey()).
           toString() == eventType) {
            removeNotification(notification.notificationId());
        }
    }

    foreach(const NotificationGroup &group, groupContainer) {
        if(group.parameters().value(GenericNotificationParameterFactory::eventTypeKey()).
           toString() == eventType) {
            doRemoveGroup(group.groupId());
        }
    }
}

void NotificationManager::updateNotificationsWithEventType(const QString &eventType)
{
    foreach(Notification notification, notificationContainer) {
        if(notification.parameters().value(GenericNotificationParameterFactory::eventTypeKey()).
           toString() == eventType) {
            notification.updateParameters(appendEventTypeParameters(notification.parameters()));
            updateNotification(notification.userId(), notification.notificationId(), notification.parameters());
        }
    }
}

uint NotificationManager::addNotification(uint notificationUserId, const NotificationParameters &parameters, uint groupId)
{
    restoreData();

    if (groupId == 0 || groupContainer.contains(groupId)) {
        uint notificationId = nextAvailableNotificationID();

        NotificationParameters fullParameters(appendEventTypeParameters(parameters));
        fullParameters.add("timestamp", QDateTime::currentDateTimeUtc().toTime_t());
        Notification notification(notificationId, groupId, notificationUserId, fullParameters, determineType(fullParameters), relayInterval);

        // Mark the notification used
        notificationContainer.insert(notificationId, notification);

        saveNotifications();

        submitNotification(notification);

        return notificationId;
    }
    return 0;
}

bool NotificationManager::updateNotification(uint notificationUserId, uint notificationId,
                                             const NotificationParameters &parameters)
{
    Q_UNUSED(notificationUserId);

    restoreData();

    QHash<uint, Notification>::iterator ni = notificationContainer.find(notificationId);

    if (ni != notificationContainer.end()) {
        NotificationParameters fullParameters(parameters);
        fullParameters.add("timestamp", QDateTime::currentDateTimeUtc().toTime_t());
        (*ni).updateParameters(fullParameters);

        saveNotifications();

        int waitQueueIndex = findNotificationFromWaitQueue(notificationId);
        if (waitQueueIndex >= 0) {
            waitQueue[waitQueueIndex].updateParameters(fullParameters);
        } else {
            // Inform the sinks about the update
            emit notificationUpdated(notificationContainer.value(notificationId));
        }

        return true;
    } else {
        return false;
    }
}

bool NotificationManager::removeNotification(uint notificationUserId, uint notificationId)
{
    Q_UNUSED(notificationUserId);

    if (notificationContainer.contains(notificationId))
    {
        emit queuedNotificationRemove(notificationId);
        return true;
    }
    return false;
}

bool NotificationManager::removeNotification(uint notificationId)
{
    restoreData();

    if (notificationContainer.contains(notificationId)) {
        // Mark the notification unused
        notificationContainer.take(notificationId);

        saveNotifications();

        int waitQueueIndex = findNotificationFromWaitQueue(notificationId);
        if (waitQueueIndex >= 0) {
            waitQueue.removeAt(waitQueueIndex);
        } else {
            // Inform the sinks about the removal
            emit notificationRemoved(notificationId);
        }

        return true;
    } else {
        return false;
    }
}

bool NotificationManager::removeNotificationsInGroup(uint groupId)
{
    QList<uint> notificationIds;

    foreach(const Notification & notification, notificationContainer.values()) {
        if (notification.groupId() == groupId) {
            notificationIds.append(notification.notificationId());
        }
    }

    bool result = !notificationIds.isEmpty();
    foreach(uint notificationId, notificationIds) {
        result &= removeNotification(notificationId);
    }
    return result;
}

uint NotificationManager::addGroup(uint notificationUserId, const NotificationParameters &parameters)
{
    restoreData();

    NotificationParameters fullParameters(appendEventTypeParameters(parameters));
    fullParameters.add("timestamp", QDateTime::currentDateTimeUtc().toTime_t());

    uint groupID = nextAvailableGroupID();
    NotificationGroup group(groupID, notificationUserId, fullParameters);
    groupContainer.insert(groupID, group);

    saveStateData();

    emit groupUpdated(groupID, fullParameters);

    return groupID;
}

bool NotificationManager::updateGroup(uint notificationUserId, uint groupId, const NotificationParameters &parameters)
{
    Q_UNUSED(notificationUserId);

    restoreData();

    QHash<uint, NotificationGroup>::iterator gi = groupContainer.find(groupId);

    if (gi != groupContainer.end()) {
        NotificationParameters fullParameters(parameters);
        fullParameters.add("timestamp", QDateTime::currentDateTimeUtc().toTime_t());

        gi->updateParameters(fullParameters);

        saveStateData();

        emit groupUpdated(groupId, gi->parameters());

        return true;
    } else {
        return false;
    }
}

bool NotificationManager::removeGroup(uint notificationUserId, uint groupId)
{
    Q_UNUSED(notificationUserId)

    restoreData();

    if (groupContainer.contains(groupId)) {
        emit queuedGroupRemove(groupId);
        return true;
    }
    return false;
}

void NotificationManager::doRemoveGroup(uint groupId)
{
    if (groupContainer.remove(groupId)) {
        foreach(const Notification & notification, notificationContainer) {
            if (notification.groupId() == groupId) {
                removeNotification(notification.notificationId());
            }
        }

        saveStateData();

        emit groupRemoved(groupId);
    }
}

NotificationParameters NotificationManager::appendEventTypeParameters(const NotificationParameters &parameters) const
{
    NotificationParameters fullParameters(parameters);

    QString eventType = parameters.value(GenericNotificationParameterFactory::eventTypeKey()).toString();
    foreach (const QString &key, notificationEventTypeStore->allKeys(eventType)) {
        fullParameters.add(key, notificationEventTypeStore->value(eventType, key));
    }

    return fullParameters;
}

uint NotificationManager::notificationUserId()
{
    restoreData();

    lastUsedNotificationUserId++;
    saveStateData();

    return lastUsedNotificationUserId;
}

QList<uint> NotificationManager::notificationIdList(uint notificationUserId)
{
    QList<uint> listOfNotificationIds;

    foreach(const Notification & notification, notificationContainer) {
        if (notification.userId() == notificationUserId) {
            listOfNotificationIds.append(notification.notificationId());
        }
    }

    return listOfNotificationIds;
}

QList<Notification> NotificationManager::notificationList(uint notificationUserId)
{
    QList<Notification> userNotifications;

    foreach (const Notification &notification, notificationContainer) {
        if (notification.userId() == notificationUserId) {
            userNotifications.append(notification);
        }
    }

    return userNotifications;
}

QList<Notification> NotificationManager::notificationListWithIdentifiers(uint notificationUserId)
{
    QList<Notification> userNotificationsWithIdentifiers;

    foreach (const Notification &notification, notificationContainer) {
        if (notification.userId() == notificationUserId) {
            userNotificationsWithIdentifiers.append(notification);
        }
    }

    return userNotificationsWithIdentifiers;
}

QList<NotificationGroup> NotificationManager::notificationGroupList(uint notificationUserId)
{
    QList<NotificationGroup> userGroups;

    foreach (const NotificationGroup &group, groupContainer) {
        if (group.userId() == notificationUserId) {
            userGroups.append(group);
        }
    }

    return userGroups;
}

QList<NotificationGroup> NotificationManager::notificationGroupListWithIdentifiers(uint notificationUserId)
{
    QList<NotificationGroup> userGroups;

    foreach (const NotificationGroup &group, groupContainer) {
        if (group.userId() == notificationUserId) {
            userGroups.append(group);
        }
    }

    return userGroups;
}

void NotificationManager::relayNextNotification()
{
    notificationInProgress = false;
    if (!waitQueue.isEmpty()) {
        submitNotification(waitQueue.takeFirst());
    }
}

Notification::NotificationType NotificationManager::determineType(const NotificationParameters &parameters)
{
    QString classStr;

    QVariant classVariant = parameters.value(GenericNotificationParameterFactory::classKey());
    if (classVariant.isValid()) {
        classStr = classVariant.toString();
    } else {
        QVariant eventTypeVariant = parameters.value(GenericNotificationParameterFactory::eventTypeKey());
        if (eventTypeVariant.isValid()) {
            QString eventType = eventTypeVariant.toString();
            if (notificationEventTypeStore->contains(eventType, GenericNotificationParameterFactory::classKey())) {
                classStr = notificationEventTypeStore->value(eventType, GenericNotificationParameterFactory::classKey());
            }
        }
    }

    return classStr == SYSTEM_EVENT_ID ? Notification::SystemEvent : Notification::ApplicationEvent;
}

void NotificationManager::submitNotification(const Notification &notification)
{
    if (!notificationInProgress) {
        // Inform about the new notification
        emit notificationUpdated(notification);

        if (relayInterval != 0) {
            notificationInProgress = true;
            if (relayInterval > 0) {
                waitQueueTimer.start(relayInterval);
            }
        }
    } else {
        // Store new notification in the notification wait queue
        if ((uint)waitQueue.size() < maxWaitQueueSize) {
            waitQueue.append(notification);
        }
    }
}

int NotificationManager::findNotificationFromWaitQueue(uint notificationId)
{
    for (int i = 0; i < waitQueue.count(); ++i) {
        if (waitQueue[i].notificationId() == notificationId) {
            return i;
        }
    }
    return -1;
}

uint NotificationManager::nextAvailableNotificationID()
{
    unsigned int i = 1;
    // Try to find an unused ID but only do it up to 2^32-1 times
    while (i != 0 && notificationContainer.contains(i)) {
        ++i;
    }
    return i;
}

uint NotificationManager::nextAvailableGroupID()
{
    unsigned int i = 1;
    // Try to find an unused ID but only do it up to 2^32-1 times
    while (i != 0 && groupContainer.contains(i)) {
        ++i;
    }
    return i;
}

void NotificationManager::removeUnseenFlags(bool ignore)
{
    if (!ignore) {
        QHash<uint, Notification>::iterator it = notificationContainer.begin();
        while (it != notificationContainer.end()) {
            NotificationParameters newParameters = (*it).parameters();
            newParameters.add(GenericNotificationParameterFactory::unseenKey(), QVariant(false));
            (*it).setParameters(newParameters);
            ++it;
        }
        // Change the states in the filestore
        saveNotifications();
    }
}

QList<Notification> NotificationManager::notifications() const
{
     return notificationContainer.values();
}

QList<NotificationGroup> NotificationManager::groups() const
{
    return groupContainer.values();
}

QObject *NotificationManager::qObject()
{
    return this;
}

uint NotificationManager::notificationCountInGroup(uint notificationUserId, uint groupId)
{
    uint amount = 0;
    foreach(const Notification & notification, notificationContainer.values()) {
        if (notification.groupId() == groupId && notification.userId() == notificationUserId) {
            amount++;
        }
    }
    return amount;
}
