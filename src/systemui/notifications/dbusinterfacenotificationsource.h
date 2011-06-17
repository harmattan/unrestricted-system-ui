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

#ifndef DBUSINTERFACENOTIFICATIONSOURCE_H
#define DBUSINTERFACENOTIFICATIONSOURCE_H

#include <QObject>
#include "notification.h"
#include "notificationgroup.h"
#include "mnotificationproxy.h"
#include "notificationsource.h"
#include "notificationmanagerinterface.h"

/*!
 * Publishes a D-Bus interface with which application developers can create and
 * manage notifications.
 *
 * DBusInterfaceNotificationSourceAdaptor defines the D-Bus API which calls
 * this source to trigger the notifications.
 *
 * The new API can be tested using the following commands:
 *
 * dbus-send --print-reply --dest=com.meego.core.MNotificationManager /notificationmanager com.meego.core.MNotificationManager.notificationUserId
 * dbus-send --print-reply --dest=com.meego.core.MNotificationManager /notificationmanager com.meego.core.MNotificationManager.addNotification uint32:<return_val_from_previous_cmd> uint32:0 string:'new-message' string:'Message received' string:'Hello M' string:'link' string:'Icon-close' uint32:1
 */
class DBusInterfaceNotificationSource : public QObject, public NotificationSource
{
    Q_OBJECT

public:
    /*!
     * Creates a new DBusInterfaceNotitificationSource.
     *
     * \param interface the NotificationManagerInterface to post the notifications to
     */
    DBusInterfaceNotificationSource(NotificationManagerInterface &interface);

    /*!
     * Returns a user ID for the notification system. The user ID has to
     * be supplied with every notification system call.
     *
     * \return a user ID for the notification system
     */
    uint notificationUserId();

    /*!
     * Adds a new notification.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param groupId the ID of the notification group to put the notification in
     * \param eventType the event type of the notification
     * \return the ID of the new notification
     *
     * \deprecated This method is deprecated, you should use addNotification(NotificationParameters parameters) instead
     */
    uint Q_DECL_DEPRECATED addNotification(uint notificationUserId, uint groupId, const QString &eventType);

    /*!
     * Adds a new notification.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param groupId the ID of the notification group to put the notification in
     * \param eventType the event type of the notification
     * \param summary the summary text to be used in the notification
     * \param body the body text to be used in the notification
     * \param action the ID of the content to be used in the notification
     * \param imageURI the ID of the icon to be used in the notification
     * \param count the number of items inside this notification
     * \return the ID of the new notification
     *
     * \deprecated This method is deprecated, you should use addNotification(NotificationParameters parameters) instead
     */
    uint Q_DECL_DEPRECATED addNotification(uint notificationUserId, uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count);

    /*!
     * Adds a new notification.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param groupId the ID of the notification group to put the notification in
     * \param eventType the event type of the notification
     * \param summary the summary text to be used in the notification
     * \param body the body text to be used in the notification
     * \param action the ID of the content to be used in the notification
     * \param imageURI the ID of the icon to be used in the notification
     * \param count the number of items inside this notification
     * \param identifier the user supplied identifier
     * \return the ID of the new notification
     *
     * \deprecated This method is deprecated, you should use addNotification(NotificationParameters parameters) instead
     */
    uint Q_DECL_DEPRECATED addNotification(uint notificationUserId, uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, const QString &identifier);

    /*!
     * Updates an existing notification.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param notificationId the ID of the notification to be updated
     * \param eventType the event type of the notification
     * \return true if the update succeeded, false otherwise
     *
     * \deprecated This method is deprecated, you should use updateNotification(NotificationParameters parameters) instead
     */
    bool Q_DECL_DEPRECATED updateNotification(uint notificationUserId, uint notificationId, const QString &eventType);

    /*!
     * Updates an existing notification.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param notificationId the ID of the notification to be updated
     * \param eventType the event type of the notification
     * \param summary the summary text to be used in the notification
     * \param body the body text to be used in the notification
     * \param action the ID of the content to be used in the notification
     * \param imageURI the ID of the icon to be used in the notification
     * \param count the number of items inside this notification
     * \return true if the update succeeded, false otherwise
     *
     * \deprecated This method is deprecated, you should use updateNotification(NotificationParameters parameters) instead
     */
    bool Q_DECL_DEPRECATED updateNotification(uint notificationUserId, uint notificationId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count);

    /*!
     * Updates an existing notification.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param notificationId the ID of the notification to be updated
     * \param eventType the event type of the notification
     * \param summary the summary text to be used in the notification
     * \param body the body text to be used in the notification
     * \param action the ID of the content to be used in the notification
     * \param imageURI the ID of the icon to be used in the notification
     * \param count the number of items inside this notification
     * \param identifier the user supplied identifier
     * \return true if the update succeeded, false otherwise
     *
     * \deprecated This method is deprecated, you should use updateNotification(NotificationParameters parameters) instead
     */
    bool Q_DECL_DEPRECATED updateNotification(uint notificationUserId, uint notificationId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, const QString &identifier);

    /*!
     * Removes a notification.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param notificationId the ID of the notification to be removed
     * \return true if the removal succeeded, false otherwise
     */
    bool removeNotification(uint notificationUserId, uint notificationId);

    /*!
     * Adds a new notification group.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param eventType the event type of the notification
     * \return the ID of the new notification group
     *
     * \deprecated This method is deprecated, you should use addGroup(NotificationParameters parameters) instead
     */
    uint Q_DECL_DEPRECATED addGroup(uint notificationUserId, const QString &eventType);

    /*!
     * Adds a new notification group.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param eventType the event type of the notification
     * \param summary the summary text to be used in the notification
     * \param body the body text to be used in the notification
     * \param action the ID of the content to be used in the notification
     * \param imageURI the ID of the icon to be used in the notification
     * \param count the number of items inside this group
     * \return the ID of the new notification group
     *
     * \deprecated This method is deprecated, you should use addGroup(NotificationParameters parameters) instead
     */
    uint Q_DECL_DEPRECATED addGroup(uint notificationUserId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count);

    /*!
     * Adds a new notification group.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param eventType the event type of the notification
     * \param summary the summary text to be used in the notification
     * \param body the body text to be used in the notification
     * \param action the ID of the content to be used in the notification
     * \param imageURI the ID of the icon to be used in the notification
     * \param count the number of items inside this group
     * \param identifier the user supplied identifier
     * \return the ID of the new notification group
     *
     * \deprecated This method is deprecated, you should use addGroup(NotificationParameters parameters) instead
     */
    uint Q_DECL_DEPRECATED addGroup(uint notificationUserId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, const QString &identifier);

    /*!
     * Updates an existing notification group.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param groupId the ID of the notification group to be updated
     * \param eventType the event type of the notification
     * \return true if the update succeeded, false otherwise
     *
     * \deprecated This method is deprecated, you should use updateGroup(NotificationParameters parameters) instead
     */
    bool Q_DECL_DEPRECATED updateGroup(uint notificationUserId, uint groupId, const QString &eventType);

    /*!
     * Updates an existing notification group.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param groupId the ID of the notification group to be updated
     * \param eventType the event type of the notification
     * \param summary the summary text to be used in the notification
     * \param body the body text to be used in the notification
     * \param action the ID of the content to be used in the notification
     * \param imageURI the ID of the icon to be used in the notification
     * \param count the number of items inside this group
     * \return true if the update succeeded, false otherwise
     *
     * \deprecated This method is deprecated, you should use updateGroup(NotificationParameters parameters) instead
     */
    bool Q_DECL_DEPRECATED updateGroup(uint notificationUserId, uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count);

    /*!
     * Updates an existing notification group.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param groupId the ID of the notification group to be updated
     * \param eventType the event type of the notification
     * \param summary the summary text to be used in the notification
     * \param body the body text to be used in the notification
     * \param action the ID of the content to be used in the notification
     * \param imageURI the ID of the icon to be used in the notification
     * \param count the number of items inside this group
     * \param identifier the user supplied identifier
     * \return true if the update succeeded, false otherwise
     *
     * \deprecated This method is deprecated, you should use updateGroup(NotificationParameters parameters) instead
     */
    bool Q_DECL_DEPRECATED updateGroup(uint notificationUserId, uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, const QString &identifier);

    /*!
     * Removes a notification group and all notifications in the group.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param groupId the ID of the notification group to be removed
     * \return true if the removal succeeded, false otherwise
     */
    bool removeGroup(uint notificationUserId, uint groupId);

    /*!
     * Returns list of notification ids by user id
     *
     * \param notificationUserId the ID of the user of notifications
     * \return list of notification ids that belong to notificationUserId
     */
    QList<uint> notificationIdList(uint notificationUserId);

    /*!
     * Returns list of notifications by user id
     *
     * \param notificationUserId the ID of the user of notifications
     * \return list of notifications that belong to notificationUserId
     *
     * \deprecated This method is deprecated, you should use notificationListWithNotificationParameters(uint notificationUserId) instead
     */
    QList<MNotificationProxy> Q_DECL_DEPRECATED notificationList(uint notificationUserId);

    /*!
     * Returns list of notifications with associated identifiers by user id
     *
     * \param notificationUserId the ID of the user of notifications
     * \return list of notifications with associated identifiers that belong to notificationUserId
     *
     * \deprecated This method is deprecated, you should use notificationListWithNotificationParameters(uint notificationUserId) instead
     */
    QList<MNotificationWithIdentifierProxy> Q_DECL_DEPRECATED notificationListWithIdentifiers(uint notificationUserId);

    /*!
     * Returns list of notification groups by user id
     *
     * \param notificationUserId the ID of the user of notifications
     * \return list of notification groups that belong to notificationUserId
     *
     * \deprecated This method is deprecated, you should use notificationGroupListAsNotificationParameters(uint notificationUserId) instead
     */
    QList<MNotificationGroupProxy> Q_DECL_DEPRECATED notificationGroupList(uint notificationUserId);

    /*!
     * Returns list of notification groups with associated identifiers by user id
     *
     * \param notificationUserId the ID of the user of notifications
     * \return list of notification groups with associated identifiers that belong to notificationUserId
     *
     * \deprecated This method is deprecated, you should use notificationGroupListAsNotificationParameters(uint notificationUserId) instead
     */
    QList<MNotificationGroupWithIdentifierProxy> Q_DECL_DEPRECATED notificationGroupListWithIdentifiers(uint notificationUserId);

    /*!
     * Returns amount of notifications in a given group
     *
     * \param notificationUserId the ID of the user of notifications
     * \param groupId the group ID
     * \return amount of notifications in given group
     */
    uint notificationCountInGroup(uint notificationUserId, uint groupId);

    /*!
     * Adds a new notification.
     *
     * Notification is created with NotificationParametes, a hash of key-value pairs
     * that represents the information the notification holds.
     *
     * Notification can be added to group if groupId key is supplied.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param groupId the group ID
     * \param parameters hash of key-value pairs that's used to create notification
     * \return the ID of the new notification
     */
    uint addNotification(uint notificationUserId, uint groupId, const NotificationParameters &parameters);

    /*!
     * Updates an existing notification.
     *
     * Notification is updated with NotificationParameters.
     * Keys that are specified are replaced and others are not changed.
     * Except of timestamp key that is changed to set to system's current time if not specified.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param notificationId the ID of the notification to be updated
     * \param parameters a hash of key-value pairs that's used to update notification
     * \return true if the update succeeded, false otherwise
     */
    bool updateNotification(uint notificationUserId, uint notificationId, const NotificationParameters &parameters);

    /*!
     * Adds a new notification group.
     *
     * NotificationGroup is created with NotificationParametes, a hash of key-value pairs
     * that represents the information the notification group holds.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param parameters a hash of key-value pairs that's used to create notification group
     * \return the ID of the new notification group
     */
    uint addGroup(uint notificationUserId, const NotificationParameters &parameters);

    /*!
     * Updates an existing notification group.
     *
     * NotificationGorup is updated with NotificationParameters.
     * Keys that are specified are replaced and others are not changed.
     * Except of timestamp key that is changed to set to system's current time if not specified.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param groupId the group ID
     * \param parameters a hash of key-value pairts that's used to update group
     * \return true if the update succeeded, false otherwise
     */
    bool updateGroup(uint notificationUserId, uint groupId, const NotificationParameters &parameters);

    /*!
     * Returns list of notifications by user id
     *
     * \param notificationUserId the ID of the user of notifications
     * \return list of notifications as  that belong to notificationUserId
     */
    QList<MNotificationProxyWithParameters> notificationListWithNotificationParameters(uint notificationUserId);

    /*!
     * Returns list of notification groups by user id
     *
     * \param notificationUserId the ID of the user of notifications
     * \return list of notification groups that belong to notificationUserId
     */
    QList<MNotificationGroupProxyWithParameters> notificationGroupListWithNotificationParameters(uint notificationUserId);
};

#endif // DBUSINTERFACENOTIFICATIONSOURCE_H
