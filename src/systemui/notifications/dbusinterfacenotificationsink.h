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

#ifndef DBUSINTERFACENOTIFICATIONSINK_H_
#define DBUSINTERFACENOTIFICATIONSINK_H_

#include <QSharedPointer>
#include "notificationsink.h"

class DBusInterfaceNotificationSinkProxy;

/*!
 * A notification sink that handles communication with external sinks.
 */
class DBusInterfaceNotificationSink : public NotificationSink
{
    Q_OBJECT

public:
    /*!
     * Creates a notification sink that handles communication with external sinks.
     */
    DBusInterfaceNotificationSink();

    /*!
     * Destroys the DBusInterfaceNotificationSink.
     */
    virtual ~DBusInterfaceNotificationSink();

    /*!
     * Registers an external sink at the given path.
     *
     * \param service D-Bus service name for the sink
     * \param path D-Bus path name for the sink
     */
    void registerSink(const QString &service, const QString &path);

    /*!
     * Unregisters an external sink at the given path.
     *
     * \param service D-Bus service name for the sink
     * \param path D-Bus path name for the sink
     */
    void unregisterSink(const QString &service, const QString &path);

private slots:
    //! \reimp
    virtual void addGroup(uint groupId, const NotificationParameters &parameters);
    virtual void removeGroup(uint groupId);
    virtual void addNotification(const Notification &notification);
    virtual void removeNotification(uint notificationId);
    //! \reimp_end

private:
    /*!
     * Proxies for the registered external sinks.
     * Maps a D-Bus service and path pair to a sink proxy
     */
    QHash<QPair<QString, QString>, QSharedPointer<DBusInterfaceNotificationSinkProxy> > proxies;
};

#endif /* DBUSINTERFACENOTIFICATIONSINK_H_ */
