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

#ifndef MCOMPOSITORNOTIFICATIONSINK_H_
#define MCOMPOSITORNOTIFICATIONSINK_H_

#include <QHash>
#include <QTimer>
#include "widgetnotificationsink.h"

class MBanner;
class MGConfItem;

/*!
 * MCompositorNotificationSink implements the NotificationSink interface for
 * displaying notifications on top of other applications.
 *
 * Notification is displayed for a certain time after which it is hidden.
 */
class MCompositorNotificationSink : public WidgetNotificationSink
{
    Q_OBJECT

public:
    /*!
     * Constructs a new MCompositorNotificationSink.
     */
    MCompositorNotificationSink();

    /*!
     * Destroys the MCompositorNotificationSink.
     */
    virtual ~MCompositorNotificationSink();

signals:
    /*!
     * Transfers a notification to another sink.
     *
     * \param notification the notification to be presented
     */
    void notificationAdded(const Notification &notification);

public:
    /*!
     * Disables or enables the sink (no notifications generated when disabled, they are just transferred).
     * \param disabled if true, the sink is disabled, otherwise it's enabled
     */
    void setDisabled(bool disabled);

private slots:
    //! \reimp
    virtual void addNotification(const Notification &notification);
    virtual void removeNotification(uint notificationId);
    //! \reimp_end

    /*!
     * Makes the currently showing banner disappear
     */
    void disappearCurrentBanner();

    /*!
      * Adds the oldest banner to a fullscreen window which was created earlier.
      */
    void addOldestBannerToWindow();

    /*!
      * Enables or disables Notification Previews according to provided GConf key
      */
    void changeNotificationPreviewMode();

    /*!
     * Updates the window mask based on the currently showing banner.
     */
    void updateWindowMask();

    /*!
     * Updates the window mask to contain the current notification of the given banner
     * \param banner The banner that is used to update the window mask
     */
    void updateWindowMask(MBanner* banner);

    /*!
     * Clears the window mask
     */
    void clearWindowMask();

    /*!
     * When current banner starts to disappear, this should be called to remove references
     * to the banner.
     */
    void currentBannerDone();

private:
    /*!
     * Updates an existing info banner with the given notification parameters.
     *
     * \param notification the notification to be updated
     */
    void updateNotification(const Notification &notification);

    /*!
     * Returns the banner that is currently on the screen, or NULL is no banner is on the screen.
     *
     * \return the current banner
     */
    MBanner *currentBanner();

    /*!
     * Determines preview icon id of a notification based on the given notification parameters.
     * \param parameters Notification parameters to determine the preview icon id from.
     * \return Logical icon id as a string
     */
    static QString determinePreviewIconId(const NotificationParameters &parameters);

    //! A mapping between notification IDs and info banners
    QHash<uint, MBanner *> idToBanner;

    //! Whether the sink is currently showing notifications or just transferring them
    bool sinkDisabled;

    //! Similar than sinkDisabled, but also system notifications are disabled
    bool allPreviewsDisabled;

    //! Full screen window for the notification
    MWindow* window;

    //! GConf item which tracks if notification previews are enabled
    MGConfItem* notificationPreviewMode;

    //! The banners currently being displayed, oldest first
    QList<MBanner *> currentBanners;

    //! Timer for disappearing the current banner
    QTimer bannerTimer;

#ifdef UNIT_TEST
    friend class Ut_MCompositorNotificationSink;
#endif
};

#endif /* MCOMPOSITORNOTIFICATIONSINK_H_ */
