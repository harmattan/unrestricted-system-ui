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

#ifndef NOTIFICATIONMANAGERINTERFACE_H
#define NOTIFICATIONMANAGERINTERFACE_H

#include <QString>
#include "notificationparameters.h"
#include "notification.h"
#include "notificationgroup.h"

/*!
 * Abstract notification manager interface. Declares interface for adding,
 * updating, removing individual notifications as well as adding a
 * notification to the group, updating and removing the group.
 */
class NotificationManagerInterface
{
public:
    /*!
     * Adds a notification. Optionally adds the notification to a
     * notification group. The default group number 0 means that the
     * notification is not added to any group. You can create groups
     * by calling \c addGroup.
     *
     * If an invalid group ID is given the notification is not added.
     * In this case 0 is returned.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param parameters Parameters for the notification
     * \param groupId A notification group where this notification is added.
     * \param type the type of the notification.
     * \return a notification ID.
     * \see addGroup
     */
    virtual uint addNotification(uint notificationUserId, const NotificationParameters &parameters = NotificationParameters(), uint groupId = 0) = 0;

    /*!
     * Updates a notification.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param notificationId The ID of the notification to be updated
     * \param parameters Parameters for the notification
     * \return true if the update succeeded, false otherwise
     */
    virtual bool updateNotification(uint notificationUserId, uint notificationId, const NotificationParameters &parameters = NotificationParameters()) = 0;

    /*!
     * Adds a new notification group. Later on notifications can be added to
     * this group.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param parameters Parameters for the notification group
     *
     * \return the new group id.
     */
    virtual uint addGroup(uint notificationUserId, const NotificationParameters &parameters = NotificationParameters()) = 0;

    /*!
     * Updates the contents of a notification group.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param groupId The ID of the notification group to be updated
     * \param parameters Parameters for the notification group
     * \return true if the update succeeded, false otherwise
     */
    virtual bool updateGroup(uint notificationUserId, uint groupId, const NotificationParameters &parameters = NotificationParameters()) = 0;

    /*!
     * Removes a notification group.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param groupId The ID of the notification group to be removed.
     * \return true if the removal succeeded, false otherwise
     */
    virtual bool removeGroup(uint notificationUserId, uint groupId) = 0;

    //! Returns copy of groups known to manager
    virtual QList<NotificationGroup> groups() const = 0;

    //! Returns copy of notifications know to manager
    virtual QList<Notification> notifications() const = 0;

    /*!
     * Returns a user ID for the notification system. The user ID has to
     * be supplied with every notification system call.
     *
     * \return a user ID for the notification system
     */
    virtual uint notificationUserId() = 0;

    /*!
     * Returns list of notification ids by user id
     *
     * \param notificationUserId the ID of the user of notifications
     * \return list of notification ids that belong to notificationUserId
     */
    virtual QList<uint> notificationIdList(uint notificationUserId) = 0;

    /*!
     * Returns list of notifications by user id
     *
     * \param notificationUserId the ID of the user of notifications
     * \return list of notifications that belong to notificationUserId
     */
    virtual QList<Notification> notificationList(uint notificationUserId) = 0;

    /*!
     * Returns list of notifications with identifiers by user id
     *
     * \param notificationUserId the ID of the user of notifications
     * \return list of notifications with identifiers that belong to notificationUserId
     */
    virtual QList<Notification> notificationListWithIdentifiers(uint notificationUserId) = 0;

    /*!
     * Returns list of notification groups by user id
     *
     * \param notificationUserId the ID of the user of notifications
     * \return list of notification groups that belong to notificationUserId
     */
    virtual QList<NotificationGroup> notificationGroupList(uint notificationUserId) = 0;

    /*!
     * Returns list of notification groups with identifiers by user id
     *
     * \param notificationUserId the ID of the user of notifications
     * \return list of notification groups with identifiers that belong to notificationUserId
     */
    virtual QList<NotificationGroup> notificationGroupListWithIdentifiers(uint notificationUserId) = 0;

    /*!
     * Returns the qObject that implements the manager for signal connections.
     *
     * \return the qObject that implements the manager
     */
    virtual QObject *qObject() = 0;

public slots:
    /*!
     * Removes a notification.
     *
     * \param notificationUserId the ID of the user of notifications
     * \param notificationId The ID of the notification to be removed.
     * \return true if the removal succeeded, false otherwise
     */
    virtual bool removeNotification(uint notificationUserId, uint notificationId) = 0;
};

#endif // NOTIFICATIONMANAGERINTERFACE_H
