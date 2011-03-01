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

#ifndef NOTIFICATIONAREASINK_H_
#define NOTIFICATIONAREASINK_H_

#include "widgetnotificationsink.h"

class MBanner;
class NotificationManagerInterface;

/*!
 * A notification sink for the NotificationArea.
 */
class NotificationAreaSink : public WidgetNotificationSink
{
    Q_OBJECT

public:
    /*!
     * Creates a notification sink for the NotificationArea.
     */
    NotificationAreaSink();

    /*!
     * Destroys the NotificationAreaSink.
     */
    virtual ~NotificationAreaSink();

    //! Updates the current notifications from manager
    void updateCurrentNotifications(NotificationManagerInterface &notificationManagerInterface);

private slots:
    //! \reimp
    virtual void addGroup(uint groupId, const NotificationParameters &parameters);
    virtual void removeGroup(uint groupId);
    virtual void addNotification(const Notification &notification);
    virtual void removeNotification(uint notificationId);
    //! \reimp_end

    /*!
     * Applies the current privacy setting to all banners.
     *
     * \param privacyEnabled \c true if privacy is enabled, \c false if disabled
     */
    void applyPrivacySetting(bool privacyEnabled);

signals:
    /*!
     * Adds a notification to a notification area.
     *
     * \param notification the MBanner to be added
     */
    void addNotification(MBanner &notification);

    /*!
     * Removes a notification from a notification area.
     *
     * \param notification the MBanner to be removed
     */
    void removeNotification(MBanner &notification);

    /*!
     * Signal that notification was clicked .
     */
    void bannerClicked();

    /*!
     * Signal notification was added to a group
     *
     * \param banner The group banner to which a notification has been added
     */
    void notificationAddedToGroup(MBanner &banner);

private:
    //! Sets up the info banner can connects its signals
    void setupInfoBanner(MBanner *infoBanner, const NotificationParameters &parameters);

    //! Updates a notification widget
    void updateNotification(MBanner *infoBanner, const NotificationParameters &parameters);

    //! A mapping between notification IDs and MBanner widgets
    QHash<uint, MBanner *> notificationIdToMBanner;

    //! Notification group parameters. The key is the group id.
    QHash<uint, NotificationParameters> notificationGroupParameters;

    //! A mapping between group IDs and MBanner widgets
    QHash<uint, MBanner *> groupIdToMBanner;

    //! A mapping between group and count of notifications belonging to this group
    QHash<uint, uint> notificationCountOfGroup;

    //! A mapping between notification id and group id. Many to one relationship may exist here.
    QHash<uint, uint> notificationIdToGroupId;

    //! Removes the banner for this group id but does not remove the group
    void removeGroupBanner(uint groupId);

    //! Increases notification count for the group to which this notification belongs by 1
    void increaseNotificationCountOfGroup(const Notification &notification);

    /*!
     * Creates a banner for a notification group.
     * \param groupId the id of the notification group
     * \param parameters the notification group parameters
     * \return a newly created banner for the notification group
     */
    MBanner* createGroupBanner(uint groupId, const NotificationParameters &parameters);

    //! Adds a notifications to a group specified by the notification's group id
    void addNotificationToGroup(const Notification &notification);
    //! Adds a notification which has no group id or 0 group id
    void addStandAloneNotification(const Notification &notification);
    //! Deletes the group from the hash and clears all notifications ids held
    void deleteGroupFromNotificationCountOfGroup(const uint groupId);
    //! Decreases notification count for the group
    uint decreaseNotificationCountOfGroup(uint groupId);
    //! Updates the latest prefix for notification group timestamp.
    void updatePrefixForNotificationGroupBannerTimestamp(MBanner *infoBanner, uint count);
};

#endif /* NOTIFICATIONAREASINK_H_ */
