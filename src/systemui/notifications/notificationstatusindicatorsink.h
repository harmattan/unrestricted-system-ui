/***************************************************************************
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

#ifndef NOTIFICATIONSTATUSINDICATORSINK_H_
#define NOTIFICATIONSTATUSINDICATORSINK_H_

#include <notificationsink.h>

class NotificationManagerInterface;

typedef QPair<QString, int> NotificationData;

/*!
 * The notification status indicator sink picks the notification with the
 * highest priority and signals the icon ID of that notification to the
 * notification status indicator.
 */
class NotificationStatusIndicatorSink : public NotificationSink
{
    Q_OBJECT

public:
    /*!
     * Creates a notification icon area sink.
     *
     * \param parent the parent object
     */
    NotificationStatusIndicatorSink(QObject *parent = NULL);

    /*!
     * Destroys the notification icon area sink.
     */
    ~NotificationStatusIndicatorSink();

    //! \reimp
    virtual void addNotification(const Notification &notification);
    virtual void removeNotification(uint notificationId);
    virtual void addGroup(uint groupId, const NotificationParameters &parameters);
    virtual void removeGroup(uint groupId);
    //! \reimp_end

signals:
    /*!
     * Sent when the icon of the notification to be shown has changed.
     *
     * \param iconId the ID of the icon to be shown
     */
    void iconIdChanged(const QString &iconId);

private:
    //! Adds a standalone notification
    void addStandaloneNotification(int notificationId, const QString &iconId, int priority);

    //! Removes a standalone notification
    void removeStandaloneNotification(int notificationId);

    //! Adds a notification to a group
    void addNotificationToGroup(int notificationId, int groupId);

    //! Removes a notification from a group
    void removeNotificationFromGroup(int notificationId, int groupId);

    //! Updates the data of a notification
    void updateNotificationData(int notificationId, const QString &iconId, int priority);

    //! Updates the data of a group
    void updateGroupData(int groupId, const QString &iconId, int priority);

    //! Checks which notification is most important and emits iconIdChanged() based on that
    void checkMostImportantNotification();

    //! Maps a notification ID to the group ID into which the notification belongs
    QMap<int, int> groupForNotification;

    //! Maps a group ID to the list of IDs of the notifications belonging to the group
    QMap<int, QList<int> > notificationsForGroup;

    //! Maps a notification ID to data
    QMap<int, NotificationData *> dataForNotification;

    //! Maps a group ID to data
    QMap<int, NotificationData *> dataForGroup;

    //! Datas of all notifications to be shown, latest first
    QList<NotificationData *> datas;
};

#endif /* NOTIFICATIONSTATUSINDICATORSINK_H_ */
