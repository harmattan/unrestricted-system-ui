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


#ifndef NOTIFICATIONWIDGETPARAMETERFACTORY_H_
#define NOTIFICATIONWIDGETPARAMETERFACTORY_H_

#include "notificationparameter.h"

/*!
 * Creates NotificationParameters related to notification widgets.
 */
class NotificationWidgetParameterFactory
{
public:

    /*!
     * Returns the keyname of the icon id parameter
     */
    static QString iconIdKey() {
        return QString("iconId");
    }

    /*!
     * Returns the keyname of the preview icon id parameter
     */
    static QString previewIconIdKey() {
        return QString("previewIconId");
    }


    /*!
     * Returns the keyname of the summary parameter
     */
    static QString summaryKey() {
        return QString("summary");
    }

    /*!
     * Returns the keyname of the body parameter
     */
    static QString bodyKey() {
        return QString("body");
    }

    /*!
     * Returns the keyname of the image id parameter
     */
    static QString imageIdKey() {
        return QString("imageId");
    }

    /*!
     * Returns the keyname of the content id parameter
     */
    static QString actionKey() {
        return QString("action");
    }

    /*!
     * Returns the keyname of the user removable parameter
     */
    static QString userRemovableKey() {
        return QString("userRemovable");
    }

    /*!
     * Returns the keyname of the generic text id key
     */
    static QString genericTextIdKey() {
        return QString("genericTextId");
    }

    /*!
     * Returns the keyname of the translation catalogue of the generic text id
     */
    static QString genericTextCatalogueKey() {
        return QString("genericTextCatalogue");
    }

    /*!
     * Creates a NotificationParameter with the given icon ID.
     *
     * \param iconId the icon ID for the notification
     * \return the related NotificationParameter
     */
    static NotificationParameter createIconIdParameter(const QString &iconId) {
        return NotificationParameter(iconIdKey(), QVariant(iconId));
    }

    /*!
     * Creates a NotificationParameter with the given preview icon ID.
     *
     * \param previewIconId the preview icon ID for the notification
     * \return the related NotificationParameter
     */
    static NotificationParameter createPreviewIconIdParameter(const QString &previewIconId) {
        return NotificationParameter(iconIdKey(), QVariant(previewIconId));
    }

    /*!
     * Creates a NotificationParameter with the given body text.
     *
     * \param body the body text for the notification
     * \return the related NotificationParameter
     */
    static NotificationParameter createBodyParameter(const QString &body) {
        return NotificationParameter(bodyKey(), QVariant(body));
    }

    /*!
     * Creates a NotificationParameter with the given summary text.
     *
     * \param summary the summary text for the notification
     * \return the related NotificationParameter
     */
    static NotificationParameter createSummaryParameter(const QString &summary) {
        return NotificationParameter(summaryKey(), QVariant(summary));
    }

    /*!
     * Creates a NotificationParameter with the given image ID.
     *
     * \param imageId the image ID for the notification
     * \return the related NotificationParameter
     */
    static NotificationParameter createImageIdParameter(const QString &imageId) {
        return NotificationParameter(imageIdKey(), QVariant(imageId));
    }

    /*!
     * Creates a NotificationParameter with the given content ID.
     *
     * \param action the action for the notification
     * \return the related NotificationParameter
     */
    static NotificationParameter createActionParameter(const QString &action) {
        return NotificationParameter(actionKey(), QVariant(action));
    }

    /*!
     * Creates a NotificationParameter with the given user removability value.
     *
     * \param removable whether the notification is user removable
     * \return the related NotificationParameter
     */
    static NotificationParameter createUserRemovableParameter(bool userRemovable) {
        return NotificationParameter(userRemovableKey(), QVariant(userRemovable));
    }

    /*!
     * Creates a NotificationParameter with the given generic text value.
     *
     * \param genericTextID ID for the generic text for the notification
     * \return the related NotificationParameter
     */
    static NotificationParameter createGenericTextIdParameter(const QString &genericTextId) {
        return NotificationParameter(genericTextIdKey(), QVariant(genericTextId));
    }

    /*!
     * Creates a NotificationParameter with the given generic text catalogue value.
     *
     * \param genericTextID catalogue for the generic text for the notification
     * \return the related NotificationParameter
     */
    static NotificationParameter createGenericTextCatalogueParameter(const QString &genericTextCatalogue) {
        return NotificationParameter(genericTextCatalogueKey(), QVariant(genericTextCatalogue));
    }
};

#endif /* NOTIFICATIONWIDGETPARAMETERFACTORY_H_ */
