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

#ifndef PROXYMNOTIFICATION_H_
#define PROXYMNOTIFICATION_H_

class Notification;
class NotificationGroup;
class DBusArgument;

/*!
 * \brief A proxy class for serializing Notification as MNotification
 *
 * MNotificationProxy is a simple data class that serializes to DBus
 * argument the same way as MNotification and is used to return Notification
 * data to libmeegotouch notifications subsystem. MNotificationProxy objects
 * are initialized from Notificattion and will contain a copy of the
 * notification data relevant to the MNotification class referring to the
 * same notification object.
 */
class MNotificationProxy
{
public:
    /*!
     * Empty constructor. Initializes the values to defaults.
     */
    MNotificationProxy();
    /*!
     * Constructor.
     *
     * \param notification A Notification object to initialize the data from
     */
    MNotificationProxy(const Notification &notification);

    //! The id of the notification presented
    uint notificationId;
    //! The id of the group the notification belongs to
    uint groupId;
    //! The event type of this notification
    QString eventType;
    //! The notification summary (short text)
    QString summary;
    //! The notification body
    QString body;
    //! The notification image
    QString imageId;
    //! The notification action
    QString action;
    //! The item count
    uint count;
};

QDBusArgument &operator<<(QDBusArgument &, const MNotificationProxy &);
const QDBusArgument &operator>>(const QDBusArgument &, MNotificationProxy &);

/*!
 * \brief A proxy class for serializing Notification with identifer key as MNotification
 *
 * MNotificationWithIdentifierProxy is a simple data class that serializes to DBus
 * argument the same way as MNotification and is used to return Notification
 * data to libmeegotouch notifications subsystem. MNotificationWithIdentifierProxy objects
 * are initialized from Notificattion and will contain a copy of the
 * notification data along with identifier key relevant to the MNotification class referring to the
 * same notification object.
 */
class MNotificationWithIdentifierProxy : public MNotificationProxy
{
public:
    /*!
     * Empty constructor. Initializes the values to defaults.
     */
    MNotificationWithIdentifierProxy();
    /*!
     * Constructor.
     *
     * \param notification A Notification object to initialize the data from
     */
    MNotificationWithIdentifierProxy(const Notification &notification);

   //! Notification identifier
    QString identifier;
};

QDBusArgument &operator<<(QDBusArgument &, const MNotificationWithIdentifierProxy &);
const QDBusArgument &operator>>(const QDBusArgument &, MNotificationWithIdentifierProxy &);



/*!
 * \brief A proxy class for serializing NotificationGroup as MNotificationGroup
 *
 * MNotificationGroupProxy is a simple data class that serializes to DBus
 * argument the same way as MNotificationGroup and is used to return
 * NotificationGroup data to libmeegotouch notifications subsystem.
 * MNotificationProxy objects are initialized from NotificattionGroup and
 * will contain a copy of the notification group data relevant to the
 * MNotificationGroup class referring to the same notification group object.
 */
class MNotificationGroupProxy
{
public:
    /*!
     * Empty constructor. Initializes the values to defaults.
     */
    MNotificationGroupProxy();
    /*!
     * Constructor.
     *
     * \param notification A NotificationGroup object to initialize the data from
     */
    MNotificationGroupProxy(const NotificationGroup &group);

    //! The id of this notification group
    uint groupId;
    //! The event type of this notification group
    QString eventType;
    //! The notification group summary (short text)
    QString summary;
    //! The notification group body
    QString body;
    //! The notification image
    QString imageId;
    //! The notification action
    QString action;
    //! The item count
    uint count;
};

QDBusArgument &operator<<(QDBusArgument &, const MNotificationGroupProxy &);
const QDBusArgument &operator>>(const QDBusArgument &, MNotificationGroupProxy &);

/*!
 * \brief A proxy class for serializing NotificationGroup as MNotificationGroup
 *
 * MNotificationGroupWithIdentifierProxy is a simple data class that serializes to DBus
 * argument the same way as MNotificationGroup and is used to return
 * NotificationGroup data to libmeegotouch notifications subsystem.
 * MNotificationProxy objects are initialized from NotificattionGroup and
 * will contain a copy of the notification group data with identifier key relevant to the
 * MNotificationGroup class referring to the same notification group object.
 */
class MNotificationGroupWithIdentifierProxy : public MNotificationGroupProxy
{
public:
    /*!
     * Empty constructor. Initializes the values to defaults.
     */
    MNotificationGroupWithIdentifierProxy();
    /*!
     * Constructor.
     *
     * \param notification A NotificationGroup object to initialize the data from
     */
    MNotificationGroupWithIdentifierProxy(const NotificationGroup &group);

    //! Notification group identifier
    QString groupIdentifier;
};

QDBusArgument &operator<<(QDBusArgument &, const MNotificationGroupWithIdentifierProxy &);
const QDBusArgument &operator>>(const QDBusArgument &, MNotificationGroupWithIdentifierProxy &);

#endif
