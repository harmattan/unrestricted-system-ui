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

#include <QDBusConnection>
#include <MNotification>
#include "diskspacenotifier.h"

DiskSpaceNotifier::DiskSpaceNotifier(QObject *parent) : QObject(parent),
    notification(NULL)
{
    QDBusConnection::systemBus().connect(QString(), "/com/nokia/diskmonitor/signal", "com.nokia.diskmonitor.signal", "disk_space_change_ind", this, SLOT(handleDiskSpaceChange(QString, int)));

    // Destroy any previous disk space notifications
    foreach (MNotification *notification, MNotification::notifications()) {
        if (notification->eventType() == "x-nokia.system-memusage") {
            notification->remove();
        }
        delete notification;
    }
}

DiskSpaceNotifier::~DiskSpaceNotifier()
{
    if (notification != NULL) {
        delete notification;
    }
}

void DiskSpaceNotifier::handleDiskSpaceChange(const QString &path, int percentage)
{
    bool notificationShouldBeVisible = false;

    if (percentage == 100) {
        // Disk space usage for path is 100%
        if (!notificationsSentForPath[path].second) {
            notificationShouldBeVisible = true;
            notificationsSentForPath[path].second = true;
        }
    } else {
        // Disk space usage for path is above the notification threshold
        if (!notificationsSentForPath[path].first) {
            notificationShouldBeVisible = true;
            notificationsSentForPath[path].first = true;
        }
    }

    if (notificationShouldBeVisible) {
        if (notification != NULL) {
            // Destroy any previous notification
            notification->remove();
            delete notification;
        }

        // Show a notification
        //% "Getting low with storage. Please check."
        notification = new MNotification("x-nokia.system-memusage", "", qtTrId("qtn_memu_memlow_notification_src"));
        notification->setAction(MRemoteAction("com.nokia.DuiControlPanel", "/", "com.nokia.DuiControlPanelIf", "appletPage", QList<QVariant>() << "Mass Storage Usage"));
        notification->publish();
    }
}
