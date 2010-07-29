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

#include "widgetnotificationsink.h"
#include "notificationwidgetparameterfactory.h"
#include "genericnotificationparameterfactory.h"
#include <MRemoteAction>
#include "sysuid.h"
#include "eventtypestore.h"
#include <QSettings>
#include <QImageReader>
#include <QFileInfo>

const char *WidgetNotificationSink::NOTIFICATION_ID_PROPERTY = "notificationId";
const char *WidgetNotificationSink::GROUP_ID_PROPERTY = "groupId";
const char *WidgetNotificationSink::USER_REMOVABLE_PROPERTY = "userRemovable";

QString WidgetNotificationSink::determineIconIdFromEventType(const QString &eventType)
{
    QString iconID;
    if (!eventType.isEmpty()) {
        NotificationManager &notificationManager = Sysuid::sysuid()->notificationManager();
        const EventTypeStore &store = notificationManager.eventTypeStore();
        const QSettings *settings = store.settingsForEventType(eventType);
        if (settings != NULL) {
            iconID = settings->value(NotificationWidgetParameterFactory::iconIdKey()).toString();
        }
    }
    return iconID;
}

QString WidgetNotificationSink::determineIconId(const NotificationParameters &parameters)
{
    QString iconID = parameters.value(NotificationWidgetParameterFactory::iconIdKey()).toString();
    if (iconID.isEmpty()) {
        QString eventType = parameters.value(GenericNotificationParameterFactory::eventTypeKey()).toString();
        iconID = determineIconIdFromEventType(eventType);
        if (iconID.isEmpty()) {
            iconID = "default";
        }
    }
    return iconID;
}

bool WidgetNotificationSink::determineUserRemovabilityFromEventType(const QString &eventType)
{
    // Banners are user removable by default
    bool userRemovable = true;
    if (!eventType.isEmpty()) {
        NotificationManager &notificationManager = Sysuid::sysuid()->notificationManager();
        const EventTypeStore &store = notificationManager.eventTypeStore();
        const QSettings *settings = store.settingsForEventType(eventType);
        if (settings != NULL) {
            if (settings->contains(NotificationWidgetParameterFactory::userRemovableKey())) {
                userRemovable = settings->value(NotificationWidgetParameterFactory::userRemovableKey()).toBool();
            }
        }
    }
    return userRemovable;
}

bool WidgetNotificationSink::determineUserRemovability(const NotificationParameters &parameters)
{
    QVariant value = parameters.value(NotificationWidgetParameterFactory::userRemovableKey());
    if (value.isValid() && value.canConvert<bool>()) {
        return value.toBool();
    } else {
        return determineUserRemovabilityFromEventType(parameters.value(GenericNotificationParameterFactory::eventTypeKey()).toString());
    }
}

MBanner *WidgetNotificationSink::createInfoBanner(const Notification &notification)
{
    MBanner *infoBanner = createInfoBanner(notification.type(), notification.groupId(), notification.parameters());
    infoBanner->setProperty(NOTIFICATION_ID_PROPERTY, notification.notificationId());

    return infoBanner;
}

MBanner *WidgetNotificationSink::createInfoBanner(Notification::NotificationType type, uint groupId, const NotificationParameters &parameters)
{
    QString title    = infoBannerTitleText(parameters);
    QString subtitle = infoBannerSubtitleText(parameters);
    QString iconID  = determineIconId(parameters);
    MBanner *infoBanner = new MBanner();

    // Create a banner on the basis of notification type
    if (type == Notification::ApplicationEvent)
    {
        infoBanner->setTitle(title);
    }
    // Add subtitle and iconid for both event and system notifications
    infoBanner->setSubtitle(subtitle);
    infoBanner->setIconID(iconID);

    infoBanner->setProperty(GROUP_ID_PROPERTY, groupId);
    infoBanner->setProperty(USER_REMOVABLE_PROPERTY, determineUserRemovability(parameters));

    updateActions(infoBanner, parameters);

    // Catch clicks from the info banner
    connect(infoBanner, SIGNAL(clicked()), this, SLOT(infoBannerClicked()), Qt::QueuedConnection);

    return infoBanner;
}

void WidgetNotificationSink::updateActions(MBanner *infoBanner, const NotificationParameters &parameters)
{
    // Remove the old actions
    foreach(QAction * qAction, infoBanner->actions()) {
        infoBanner->removeAction(qAction);
        delete qAction;
    }

    // Add the action if it exists
    QString action = parameters.value(NotificationWidgetParameterFactory::actionKey()).toString();
    if (!action.isEmpty()) {
        MRemoteAction *remoteAction = new MRemoteAction(action, infoBanner);
        remoteAction->setVisible(false);
        infoBanner->addAction(remoteAction);
    }
}

QString WidgetNotificationSink::infoBannerTitleText(const NotificationParameters &parameters)
{
    return parameters.value(NotificationWidgetParameterFactory::summaryKey()).toString();
}

QString WidgetNotificationSink::infoBannerSubtitleText(const NotificationParameters &parameters)
{
    return parameters.value(NotificationWidgetParameterFactory::bodyKey()).toString();
}

void WidgetNotificationSink::infoBannerClicked()
{
    MBanner *infoBanner = qobject_cast<MBanner *>(sender());

    if (infoBanner != NULL) {
        // Trigger each remote action associated with the clicked info banner
        foreach(QAction * qAction, infoBanner->actions()) {
            MRemoteAction *remoteAction = dynamic_cast<MRemoteAction *>(qAction);
            if (remoteAction) {
                remoteAction->trigger();
            }
        }

        // Only remove the banner if it is user removable
        if (infoBanner->property(USER_REMOVABLE_PROPERTY).toBool()) {
            // Get the notification ID from the info banner
            bool ok = false;
            uint notificationId = infoBanner->property(NOTIFICATION_ID_PROPERTY).toUInt(&ok);
            if (ok) {
                // Request notification removal
                emit notificationRemovalRequested(notificationId);
            } else {
                uint groupId = infoBanner->property(GROUP_ID_PROPERTY).toUInt(&ok);
                if (ok) {
                    // Request notification group clearing
                    emit notificationGroupClearingRequested(groupId);
                }
            }
        }
    }
}
