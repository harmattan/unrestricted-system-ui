/****************************************************************************
**
** Copyright (C) 2012 Tomasz Pieniążek.
** All rights reserved.
** Contact: <t.pieniazek@gazeta.pl>
**
** This file is part of systemui.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef LEDNOTIFICATIONSINK_H_
#define LEDNOTIFICATIONSINK_H_

#include "notificationsink.h"

#include <QDBusInterface>
#include <QDBusConnection>
#include <mce/mode-names.h>
#include <mce/dbus-names.h>
#include <MGConfItem>

#ifdef HAVE_QMSYSTEM
#include <qmdisplaystate.h>
#endif
	
#define LED_PATTERN "PatternCommunication"
#define TWITTER_FEEDBACK_PARAMETER "genericTextCatalogue"
#define GCONF_LED_KEY "/desktop/meego/notifications/led_enabled"


/*!
 * LEDNotificationSink implements the NotificationSink interface for
 * presenting notifications through LED.
 */
class LEDNotificationSink : public NotificationSink
{
    Q_OBJECT
public:
    /*!
     * Constructs a new LEDNotificationSink.
     *
     * \param parent the parent object
     */
    LEDNotificationSink(QObject *parent = NULL);

    /*!
     * Destroys the LEDNotificationSink.
     */
    virtual ~LEDNotificationSink();

private slots:
    //! \reimp
    virtual void addNotification(const Notification &notification);
    virtual void removeNotification(uint notificationId);
    //! \reimp_end

    /*!
	* Updates status of notifier's LED feedback to playing or stopped.
	*/
	void updateStatusOfLedFeedback();

private:
    /*!
     * Determines feedback id of a notification based on the given notification parameters.
     * \param parameters Notification parameters to determine the feedback id from.
     * \return Logical feedback id as a string
     */
    QString determineFeedbackId(const NotificationParameters &parameters);

    QString determineGenericTextCatalogue(const NotificationParameters &parameters);

    bool checkIfAcceptable(const NotificationParameters &parameters);

    /*!
	* Sets notifier enabled or disabled and updates led feedback state.
	*/
	void setNotifierEnabled(bool active);

    void enableLed();
	void disableLed();

    //! Represents the state of the notifier
	bool notifierEnabled;

#ifdef HAVE_QMSYSTEM
	//! Display state of the device. Needed for LED feedback.
	MeeGo::QmDisplayState displayState;
#endif

    //! Notification IDs.
    QList<uint> notificationsId;

    QDBusInterface *m_interface;

    MGConfItem *configItem;

};

#endif /* LEDNOTIFICATIONSINK_H_ */
