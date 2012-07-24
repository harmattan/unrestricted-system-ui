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

#include "lednotificationsink.h"
#include "feedbackparameterfactory.h"

LEDNotificationSink::LEDNotificationSink(QObject *parent) : NotificationSink(parent),
    notifierEnabled(false)
{
    m_interface = new QDBusInterface(MCE_SERVICE, MCE_REQUEST_PATH, MCE_REQUEST_IF,
	                                 QDBusConnection::systemBus(), this);
	
    configItem = new MGConfItem(GCONF_LED_KEY);

#ifdef HAVE_QMSYSTEM
	connect(&displayState, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)),
	this, SLOT(updateStatusOfLedFeedback()));
#endif
}

LEDNotificationSink::~LEDNotificationSink()
{
    setNotifierEnabled(false);
}

QString LEDNotificationSink::determineFeedbackId(const NotificationParameters &parameters)
{
    QString feedbackId = parameters.value(FeedbackParameterFactory::feedbackIdKey()).toString();
    return feedbackId;
}

QString LEDNotificationSink::determineGenericTextCatalogue(const NotificationParameters &parameters)
{
    QString feedbackId = parameters.value(TWITTER_FEEDBACK_PARAMETER).toString();
    return feedbackId;
}

bool LEDNotificationSink::checkIfAcceptable(const NotificationParameters &parameters)
{
    QString feedbackId = determineFeedbackId(parameters);
    QString genericTextCatalogue = determineGenericTextCatalogue(parameters);
    QStringList allowedNotifications = configItem->value().toStringList();

    // x-nokia.call.missed event
    if (feedbackId.compare("default") == 0 && genericTextCatalogue.compare("telephony") == 0) {
        if (allowedNotifications.contains("call"))
            return true;
    }
    // x-nokia.messaging.im event
    else if (feedbackId.compare("chat") == 0) {
        if (allowedNotifications.contains("chat"))
            return true;    
    }
    // x-nokia.messaging.mms x-nokia.messaging.sms 
    // x-nokia.messaging.voicemail-SMS events
    else if (feedbackId.compare("sms") == 0 && genericTextCatalogue.compare("messaging") == 0) {
        if (allowedNotifications.contains("sms"))
            return true;    
    }
    // x-nokia.messaging.voicemail event
    else if (feedbackId.compare("sms") == 0 && genericTextCatalogue.compare("telephony") == 0) {
        if (allowedNotifications.contains("voicemail"))
            return true;    
    }
    // email.arrived event
    else if (feedbackId.compare("email") == 0) {
        if (allowedNotifications.contains("email"))
            return true;    
    }
    // events.service.facebook friendrequests.service.facebook
    // messages.service.facebook notifications.service.facebook events
    else if (genericTextCatalogue.compare("facebook") == 0) {
        if (allowedNotifications.contains("facebook"))
            return true;    
    }
    // mentions.service.twitter messages.service.twitter events
    else if (genericTextCatalogue.compare("twitter") == 0) {
        if (allowedNotifications.contains("twitter"))
            return true;    
    }
    // x-nokia.calendar.missed event
    else if (feedbackId.compare("organiser") == 0) {
        if (allowedNotifications.contains("organiser"))
            return true;
    }

    return false;
}

void LEDNotificationSink::addNotification(const Notification &notification)
{
    if (canAddNotification(notification) && !notificationsId.contains(notification.notificationId())) {
        if (checkIfAcceptable(notification.parameters())) {
            notificationsId.append(notification.notificationId());

            if (notificationsId.size() == 1)
                setNotifierEnabled(true);
        }
    }    
}

void LEDNotificationSink::removeNotification(uint notificationId)
{
    int index = notificationsId.indexOf(notificationId);
    if (index > -1) {
        notificationsId.removeAt(index);
    }

    if (notificationsId.empty())
        setNotifierEnabled(false);
}

void LEDNotificationSink::setNotifierEnabled(bool enabled)
{
	notifierEnabled = enabled;
	updateStatusOfLedFeedback();
}

void LEDNotificationSink::updateStatusOfLedFeedback()
{
#ifdef HAVE_QMSYSTEM
	if (displayState.get() == MeeGo::QmDisplayState::Off && notifierEnabled) {
	    // Display is off and notifier is enabled, LED should be ON
        enableLed();
    } else {
	    // Display is on or notifier is disabled, LED should be OFF
        disableLed();
	}
#endif
}

void LEDNotificationSink::enableLed()
{
	m_interface->call(MCE_ACTIVATE_LED_PATTERN, LED_PATTERN);
}

void LEDNotificationSink::disableLed()
{
	m_interface->call(MCE_DEACTIVATE_LED_PATTERN, LED_PATTERN);
}

