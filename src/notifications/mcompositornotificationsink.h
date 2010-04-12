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
#include <X11/Xlib.h>

class QGraphicsView;
class QTimer;
class MInfoBanner;

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
     * \brief Rotates the info banner to a new orientation
     *
     * \param orientation the new orientation
     */
    void rotateInfoBanners(const M::Orientation &orientation);

    /*!
     * A slot for timing out the notification windows
     */
    void timeout();

    /*!
     * A slot for disabling sink(No notifications generated, they are just transfered)
     * \param bool disabled true for suppressing notification banner from sink. false if sink should generate notification banners
     */
    void setDisabled(bool disabled);

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
     * Sets the size and rotation of a view based on the size of an info
     * banner and the current orientation.
     *
     * \param view the view to manipulate
     * \param infoBanner the infoBanner from which to take the size
     */
    static void setViewSizeAndRotation(QGraphicsView &view, const MInfoBanner &infoBanner);

    /*!
     * A private class for storing notification information
     */
    class MCompositorNotificationSinkNotification
    {
    public:
        MCompositorNotificationSinkNotification(QGraphicsView *view, QTimer *timer, MInfoBanner *infoBanner);
        ~MCompositorNotificationSinkNotification();

        //! The view in which the widget resides
        QGraphicsView *view;
        //! A timer for dismissing the notification
        QTimer *timer;
        //! The MInfoBanner
        MInfoBanner *infoBanner;
    };

    //! A mapping between notification IDs and private notification information classes
    QHash<uint, MCompositorNotificationSinkNotification *> idToNotification;

    //! Whether the orientation change signal has been connected
    bool orientationChangeSignalConnected;

    //! Whether the sink is currently showing notifications or just transferring them
    bool sinkDisabled;
};

#endif /* MCOMPOSITORNOTIFICATIONSINK_H_ */
