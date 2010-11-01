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


#ifndef GENERICNOTIFICATIONPARAMETERFACTORY_H
#define GENERICNOTIFICATIONPARAMETERFACTORY_H

#include "notificationparameter.h"

/*!
 * Creates NotificationParameter objects from generic parameters that are common to various sinks.
 */
class GenericNotificationParameterFactory
{
public:
    /*!
     * Returns the keyname of the event type parameter
     */
    static QString eventTypeKey() {
        return QString("eventType");
    }

    /*!
     * Returns the keyname of the count parameter
     */
    static QString countKey() {
        return QString("count");
    }

    /*!
     * Returns the keyname of the persistent parameter
     */
    static QString persistentKey() {
        return QString("persistent");
    }

    /*!
     * Returns the keyname of the class parameter
     */
    static QString classKey() {
        return QString("class");
    }

    /*!
     * Returns the keyname of the unseen parameter
     */
    static QString unseenKey() {
        return QString("unseen");
    }

    /*!
     * Returns the keyname of the identifier parameter
     */
    static QString identifierKey() {
        return QString("identifier");
    }

    /*!
     * Creates a NotificationParameter with the given event type.
     *
     * \param eventType the event type of the notification
     * \return the related NotificationParameter
     */
    static NotificationParameter createEventTypeParameter(const QString &eventType) {
        return NotificationParameter(eventTypeKey(), QVariant(eventType));
    }

    /*!
     * Creates a NotificationParameter with the given count.
     *
     * \param count the count of the notification
     * \return the related NotificationParameter
     */
    static NotificationParameter createCountParameter(uint count) {
        return NotificationParameter(countKey(), QVariant(count));
    }

    /*!
     * Creates a NotificationParameter with the given persistence.
     *
     * \param persistent \c true if the notification is persistent, \c false otherwise
     * \return the related NotificationParameter
     */
    static NotificationParameter createPersistentParameter(bool persistent) {
        return NotificationParameter(persistentKey(), QVariant(persistent));
    }

    /*!
     * Creates a NotificationParameter with the given class.
     *
     * \param notificationClass the class of the notification
     * \return the related NotificationParameter
     */
    static NotificationParameter createClassParameter(const QString &notificationClass) {
        return NotificationParameter(classKey(), QVariant(notificationClass));
    }

    /*!
     * Creates a NotificationParameter with the given unseen status.
     *
     * \param unseen the unseen status of the notification
     * \return the related NotificationParameter
     */
    static NotificationParameter createUnseenParameter(bool unseen) {
        return NotificationParameter(unseenKey(), QVariant(unseen));
    }

    /*!
     * Creates a NotificationParameter with the given identifier.
     *
     * \param identifier the identifier of the notification
     * \return the related NotificationParameter
     */
    static NotificationParameter createIdentifierParameter(const QString &identifier) {
        return NotificationParameter(identifierKey(), QVariant(identifier));
    }
};

#endif // GENERICNOTIFICATIONPARAMETERFACTORY_H
