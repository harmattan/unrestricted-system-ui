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

private slots:
    //! \reimp
    virtual void addNotification(const Notification &notification);
    virtual void removeNotification(uint notificationId);
    //! \reimp_end

    /*!
     * A slot for timing out the notification windows
     */
    void timeout();

    /*!
     * A slot for disabling sink(No notifications generated, they are just transfered)
     * \param bool disabled true for suppressing notification banner from sink. false if sink should generate notification banners
     */
    void setDisabled(bool disabled);

    /*!
     * Shows or hides the fullscreen window based on whether there are banners to be shown in it.
     */
    void showOrHideWindow();

    /*!
      * Adds the latest banner to a fullscreen window which was created earlier.
      */
    void addLatestBannerToWindow();

    /*!
      * Removes a banner from the current banners when it disappears off the screen after timeout.
      */
    void removeBannerFromCurrentBanners();

    /*!
      * Enables or disables Notification Previews according to provided GConf key
      */
    void changeNotificationPreviewMode();

    /*!
     * Updates the window mask to contain the current notification
     */
    void updateWindowMask();

    /*!
     * Clears the window mask
     */
    void clearWindowMask();

private:
    /*!
     * Updates an existing info banner with the given notification parameters.
     *
     * \param notification the notification to be updated
     */
    void updateNotification(const Notification &notification);

    /*!
     * Destroys the notification objects and optionally marks the notification ID to be still in use.
     *
     * \param notificationId the ID of the notification to be removed
     * \param notificationIdInUse true if the notification ID should be marked to be still in use, false otherwise
     */
    void notificationDone(uint notificationId, bool notificationIdInUse);

    /*!
     * Sets up a timer for the banner disappearing
     *
     * \param banner MBanner whose disappearance it handles. Timer would be parented to this banner.
     * \param notification the notification associated with the banner
     */
    void setupWindowTimer(MBanner *banner, const Notification &notification);

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

#ifdef UNIT_TEST
    friend class Ut_MCompositorNotificationSink;
#endif
};

#endif /* MCOMPOSITORNOTIFICATIONSINK_H_ */
