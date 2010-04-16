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

#ifndef WIDGETNOTIFICATIONSINK_H
#define WIDGETNOTIFICATIONSINK_H

#include "notificationsink.h"
#include <MInfoBanner>

/*!
 * WidgetNotificationSink is a common base class for all notification sinks that trigger
 * visual feedback using a graphics widget.
 *
 * WidgetNotificationSink creates MInfoBanner widgets for notifications. When MInfoBanner
 * is clicked the action bound to the notification is triggered and notification is removed
 * from the notification system signalling notificationRemovalRequested().
 */
class WidgetNotificationSink : public NotificationSink
{
    Q_OBJECT

signals:
    /*!
     * Requests removal of a notification from the notification system.
     * This can be connected to the associated NotificationManagerInterface::removeNotification().
     * \param notificationId Id of the notification to be removed from the system.
     */
    void notificationRemovalRequested(uint notificationId);

    /*!
     * Requests removal of notifications in a group.
     * \param groupId Id of the group to be cleared.
     */
    void notificationGroupClearingRequested(uint groupId);

protected:
    /*!
     * Determines icon id of a notification based on the given notification parameters.
     * \param parameters Notification parameters to determine the icon id from.
     * \return Logical icon id as a string
     */
    static QString determineIconId(const NotificationParameters &parameters);

    /*!
     * Determines user removability of a notification based on the given notification parameters.
     * \param parameters Notification parameters to determine removability from.
     * \return \c true if the notification should be user removable, \c false otherwise
     */
    static bool determineUserRemovability(const NotificationParameters &parameters);

    /*!
     * Creates a MInfoBanner widget to represent a notification object.
     * Ownership of the constructed object is passed to the caller.
     * \param notification The notification object to represent with the MInfoBanner.
     * \return Constructed MInfoBanner that represents the notification.
     */
    MInfoBanner *createInfoBanner(const Notification &notification);

    /*!
     * Creates a MInfoBanner widget from the given notification parameters.
     * Ownership of the constructed object is passed to the caller.
     * \param type Type of the info banner to be constructed.
     * \param groupId The group ID to be associated with the info banner.
     * \param params NotificationParameters according to which configure the MInfoBanner.
     */
    MInfoBanner *createInfoBanner(MInfoBanner::BannerType type, uint groupId, const NotificationParameters &parameters);

    /*!
     * Removes old actions from the given info banner and adds the action
     * specified in the notification (if any)
     *
     * \param infoBanner the MInfoBanner to update
     * \param parameters the NotificationParameters to get the action from
     */
    void updateActions(MInfoBanner *infoBanner, const NotificationParameters &parameters);

    //! MInfoBanner property to store the user removability into
    static const char *USER_REMOVABLE_PROPERTY;

private:
    /*!
     * Determines icon id of a notification based on the given event type.
     * \param eventType Notification event type.
     * \return Logical icon id as a string or an empty string if icon has not been defined for the
     *         given event type.
     */
    static QString determineIconIdFromEventType(const QString &eventType);

    /*!
     * Determines user removability of a notification based on the given event type.
     * \param eventType Notification event type.
     * \return \c true if the notification should be user removable, \c false otherwise
     */
    static bool determineUserRemovabilityFromEventType(const QString &eventType);

    //! MInfoBanner property to store the notification ID into
    static const char *NOTIFICATION_ID_PROPERTY;
    //! MInfoBanner property to store the group ID into
    static const char *GROUP_ID_PROPERTY;

private slots:
    /*!
     * A slot for catching info banner clicks
     */
    void infoBannerClicked();
};

#endif // WIDGETNOTIFICATIONSINK_H
