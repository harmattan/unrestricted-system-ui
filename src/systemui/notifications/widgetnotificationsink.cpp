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
#include "widgetnotificationsink.h"
#include <MRemoteAction>
#include <MLocale>
#include <MGConfItem>
#include <QDir>

const char *WidgetNotificationSink::NOTIFICATION_ID_PROPERTY = "notificationId";
const char *WidgetNotificationSink::GROUP_ID_PROPERTY = "groupId";
const char *WidgetNotificationSink::USER_REMOVABLE_PROPERTY = "userRemovable";
const char *WidgetNotificationSink::TITLE_TEXT_PROPERTY = "titleText";
const char *WidgetNotificationSink::SUBTITLE_TEXT_PROPERTY = "subtitleText";
const char *WidgetNotificationSink::GENERIC_TEXT_PROPERTY = "genericText";

WidgetNotificationSink::WidgetNotificationSink() :
    NotificationSink(),
    privacySetting(NULL),
    clickableNotifications(true)
{
}

bool WidgetNotificationSink::determineUserRemovability(const NotificationParameters &parameters)
{
    QVariant value = parameters.value(NotificationWidgetParameterFactory::userRemovableKey());
    if(value.isValid()) {
        return value.toBool();
    }
    return true;
}

MBanner *WidgetNotificationSink::createInfoBanner(const Notification &notification)
{
    MBanner *infoBanner = createInfoBanner(notification.type(), notification.groupId(), notification.parameters());
    infoBanner->setProperty(NOTIFICATION_ID_PROPERTY, notification.notificationId());

    return infoBanner;
}

MBanner *WidgetNotificationSink::createInfoBanner(Notification::NotificationType type, uint groupId, const NotificationParameters &parameters)
{
    // Create a banner on the basis of notification type
    MBanner *infoBanner = new MBanner;
    infoBanner->setObjectName(type == Notification::ApplicationEvent ? "EventBanner" : "SystemBanner");
    infoBanner->setProperty(TITLE_TEXT_PROPERTY, infoBannerTitleText(parameters));
    infoBanner->setProperty(SUBTITLE_TEXT_PROPERTY, infoBannerSubtitleText(parameters));
    infoBanner->setProperty(GENERIC_TEXT_PROPERTY, infoBannerGenericText(parameters));
    infoBanner->setProperty(GROUP_ID_PROPERTY, groupId);
    infoBanner->setProperty(USER_REMOVABLE_PROPERTY, determineUserRemovability(parameters));
    updateImage(infoBanner, parameters);
    updateTitles(infoBanner);
    updateActions(infoBanner, parameters);

    if(type == Notification::ApplicationEvent) {
        infoBanner->setBannerTimeStamp(QDateTime::fromTime_t(parameters.value("timestamp").toUInt()));
    }

    // Catch clicks from the info banner
    if(clickableNotifications) {
        connect(infoBanner, SIGNAL(clicked()), this, SLOT(infoBannerClicked()), Qt::QueuedConnection);
    }
    return infoBanner;
}

bool WidgetNotificationSink::containsText(const Notification &notification)
{
    return !(infoBannerTitleText(notification.parameters()).isEmpty()
             && infoBannerGenericText(notification.parameters()).isEmpty()
             && infoBannerSubtitleText(notification.parameters()).isEmpty());
}

void WidgetNotificationSink::updateTitles(MBanner *infoBanner)
{
    if (privacySetting != NULL && privacySetting->value().toBool()) {
        // Privacy is honored and privacy mode is enabled: use a generic text in the banner
        infoBanner->setTitle(infoBanner->property(GENERIC_TEXT_PROPERTY).toString());
        infoBanner->setSubtitle(QString());
    } else {
        // Privacy is not honored or privacy mode is disabled: use the given text in the banner
        if (infoBanner->objectName() == "EventBanner") {
            // Event banner: use both title and subtitle text
            infoBanner->setTitle(infoBanner->property(TITLE_TEXT_PROPERTY).toString());
            infoBanner->setSubtitle(infoBanner->property(SUBTITLE_TEXT_PROPERTY).toString());
        } else {
            // System banner: use subtitle text only
            infoBanner->setTitle(infoBanner->property(SUBTITLE_TEXT_PROPERTY).toString());
            infoBanner->setSubtitle(QString());
        }
    }
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

void WidgetNotificationSink::updateImage(MBanner *infoBanner, const NotificationParameters &parameters)
{
    QString imageId(parameters.value(NotificationWidgetParameterFactory::imageIdKey()).toString());
    if (imageId.isEmpty()) {
        imageId = parameters.value(NotificationWidgetParameterFactory::iconIdKey()).toString();
    }

    if (QDir::isAbsolutePath(imageId)) {
        QPixmap pixmap;
        pixmap.load(imageId);
        if (!pixmap.isNull()) {
            infoBanner->setPixmap(pixmap);
        }
    } else {
        infoBanner->setIconID(imageId);
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

QString WidgetNotificationSink::infoBannerGenericText(const NotificationParameters &parameters)
{
    QString genericText;
    QString genericTextId = parameters.value(NotificationWidgetParameterFactory::genericTextIdKey()).toString();

    if(!genericTextId.isEmpty()) {
        QString genericTextCatalogue = parameters.value(NotificationWidgetParameterFactory::genericTextCatalogueKey()).toString();

        if(!genericTextCatalogue.isEmpty()) {
            // Load the catalog from disk if it's not yet loaded
            MLocale locale;
            locale.installTrCatalog(genericTextCatalogue);
            MLocale::setDefault(locale);

            int eventCount = parameters.value(GenericNotificationParameterFactory::countKey()).toInt();
            genericText = qtTrId(genericTextId.toUtf8(), eventCount).arg(eventCount);
        }
    }

    return genericText;
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

void WidgetNotificationSink::setHonorPrivacySetting(bool honor)
{
    if (honor) {
        if (privacySetting == NULL) {
            privacySetting = new MGConfItem("/desktop/meego/privacy/private_lockscreen_notifications", this);
            emitPrivacySettingValue();

            connect(privacySetting, SIGNAL(valueChanged()), this, SLOT(emitPrivacySettingValue()));
        }
    } else {
        if (privacySetting != NULL) {
            delete privacySetting;
            privacySetting = NULL;

            emitPrivacySettingValue();
        }
    }
}

void WidgetNotificationSink::emitPrivacySettingValue()
{
    emit privacySettingChanged(privacySetting != NULL ? privacySetting->value().toBool() : false);
}

void WidgetNotificationSink::setNotificationsClickable(bool clickable)
{
    clickableNotifications = clickable;
}
