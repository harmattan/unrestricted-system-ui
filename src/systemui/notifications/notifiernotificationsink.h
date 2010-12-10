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

#ifndef NOTIFIERNOTIFICATIONSINK_H_
#define NOTIFIERNOTIFICATIONSINK_H_

#include "notificationsink.h"
#include <QSet>

#ifdef HAVE_QMSYSTEM
#include <qmdisplaystate.h>
#endif

class NGFAdapter;

/*!
 * A notification sink for the Notifier. Just maintains a count of unseen notifications
 */
class NotifierNotificationSink : public NotificationSink
{
    Q_OBJECT

public:
    /*!
     * Creates a notifier notification sink.
     *
     * \param parent the parent object
     */
    NotifierNotificationSink(QObject *parent = NULL);

    /*!
     * Destroys the NotifierNotificationSink.
     */
    virtual ~NotifierNotificationSink();

public slots:
    /*!
     * Clears all notifications from the sink.
     */
    void clearSink();

    /*!
     * Sets whether to disable/enable notifications from being added to sink
     *
     * \param disable Whether notification additions should be disable or enabled
     */
    void disableNotificationAdditions(bool disable);

signals:
    /*!
     * Signal which specifies the current state of notifiernotification sink
     *
     * \param active \c true when sink is to active(has notifications) \c false otherwise
     */
    void notifierSinkActive(bool active);

private slots:
    //! \reimp
    virtual void addNotification(const Notification &notification);
    virtual void removeNotification(uint notificationId);
    //! \reimp_end

    /*!
     * Updates status of notifier's LED feedback to playing or stopped.
     * Play is set only when notifier is enabled and display is off.
     */
    void updateStatusOfLedFeedback();

private:
    /*!
     * Sets notifier enabled or disabled.
     * Emits notifierSinkActive signal and updates led feedback state.
     */
    void setNotifierEnabled(bool active);

    //! Checks if the notification is still unseen.
    static bool isUnseen(const Notification &notification);

    //! The id for notifier NGF
    static const QString NOTIFIER_NGF_ID;

    //! The NGF Adapter
    NGFAdapter *ngfAdapter;

    //! Whether adding of notifications is disabled
    bool additionsDisabled;

    //! A set of application notifications which have arrived here
    QSet<uint> applicationEventIds;

    //! The event id of the currently playing NGF feedback
    uint ngfEventId;

    //! Represents the state of the notifier
    bool notifierEnabled;

#ifdef HAVE_QMSYSTEM
    //! Display state of the device. Needed for LED feedback.
    MeeGo::QmDisplayState displayState;
#endif

#ifdef UNIT_TEST
    friend class Ut_NotifierNotificationSink;
#endif
};

#endif /* NOTIFIERNOTIFICATIONSINK_H_ */
