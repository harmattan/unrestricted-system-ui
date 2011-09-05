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
#include <QSet>
#include <QTimer>
#include "widgetnotificationsink.h"
#include <X11/X.h>

#ifdef HAVE_QMSYSTEM
#include <qmdisplaystate.h>
#endif

#ifdef HAVE_CONTEXTSUBSCRIBER
#include <contextproperty.h>
#endif

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
    //! Supported preview modes
    enum PreviewMode {
        AllEventsEnabled = 0,
        ApplicationEventsDisabled,
        SystemEventsDisabled,
        AllEventsDisabled
    };

    /*!
     * Constructs a new MCompositorNotificationSink.
     */
    MCompositorNotificationSink();

    /*!
     * Destroys the MCompositorNotificationSink.
     */
    virtual ~MCompositorNotificationSink();

    /*!
     * Disables or enables the sink for application event notifications, when disabled, they are just transferred.
     * \param disabled if true, the sink is disabled, otherwise it's enabled
     */
    void setApplicationEventsDisabled(bool disabled);

signals:
    /*!
     * Transfers a notification to another sink.
     *
     * \param notification the notification to be presented
     */
    void notificationAdded(const Notification &notification);

public slots:
    /*!
     * Sets the touch screen lock active state so notifications can be enabled/disabled based on that.
     *
     * \param active \c true if the touch screen lock is active, \c false otherwise
     */
    void setTouchScreenLockActive(bool active);

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
     * When current banner starts to disappear, this should be called to remove references
     * to the banner.
     */
    void currentBannerDone();

    //! Updates notification windows orientation if window is currently hidden
    void updateWindowOrientationIfWindowHidden();

    //! Updates notification windows orientation according to topmost window orientation
    void updateWindowOrientation();

private:
    /*!
     * Creates the window for the notifications if it does not yet exist.
     */
    void createWindowIfNecessary();

    /*!
     * Updates an existing info banner with the given notification parameters.
     *
     * \param notification the notification to be updated
     */
    void updateNotification(const Notification &notification);

    /*!
     * Updates the image of the info banner with the given notification parameters.
     * If the icon is overridden with an image the image is used. Otherwise the
     * preview icon ID is used.
     *
     * \param infoBanner the MBanner to update
     * \param parameters the NotificationParameters to get the image or icon from
     */
    void updateImage(MBanner *infoBanner, const NotificationParameters &parameters);

    /*!
     * Find the current application window id through a root window property
     * and then check a property on that window to determine whether different
     * kinds of notifications should be shown on that window or not.
     *
     * \return the preview mode for the current application
     */
    PreviewMode currentApplicationPreviewMode();

    //! The set of all notification IDs known by this sink. Needed to know also about those notifications which do not have banners anymore.
    QSet<uint> notificationIds;

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

    //! The banners to be displayed, oldest first
    QList<MBanner *> bannerQueue;

    //! The banner currently being displayed
    MBanner *currentBanner;

    //! Timer for disappearing the current banner
    QTimer bannerTimer;

    //! The atom identifier for _MEEGOTOUCH_CURRENT_APP_WINDOW
    Atom currentAppWindowAtom;

    //! The atom identifier for _MEEGOTOUCH_NOTIFICATION_PREVIEWS_DISABLED
    Atom notificationPreviewsDisabledAtom;

    //! Whether the touch screen lock is active or not
    bool touchScreenLockActive;

#ifdef HAVE_CONTEXTSUBSCRIBER
    ContextProperty *currentWindowAngleProperty;
#endif

#ifdef HAVE_QMSYSTEM
    //! Keep track of device display state
    MeeGo::QmDisplayState displayState;
#endif

#ifdef UNIT_TEST
    friend class Ut_MCompositorNotificationSink;
#endif
};

#endif /* MCOMPOSITORNOTIFICATIONSINK_H_ */
