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


#include "ut_ngfnotificationsink.h"
#include <MApplication>
#include "ngfnotificationsink.h"
#include "feedbackparameterfactory.h"
#include "eventtypestore_stub.h"
#include "notificationmanager_stub.h"
#include "sysuid_stub.h"
#include "ngfadapter.h"

// xlib.h defines Status, undef it so we can use QSettings::Status
#ifdef Status
#undef Status
#endif

static NotificationManager *manager;

static uint NGFAdaptor_play_id = 0;
static int NGFAdaptor_stop_called = 0;

NGFAdapter::NGFAdapter()
{
#ifdef HAVE_LIBNGF
    client = NULL;
    connection = NULL;
#endif
}

NGFAdapter::~NGFAdapter()
{
}

uint NGFAdapter::play(const QString &id)
{
    Ut_NGFNotificationSink::played.append(id);
    return ++NGFAdaptor_play_id;
}

void NGFAdapter::stop(uint eventId)
{
    Q_UNUSED(eventId);
    NGFAdaptor_stop_called++;
}

bool NGFAdapter::isValid()
{
    return true;
}

QList<QString> Ut_NGFNotificationSink::played;

void Ut_NGFNotificationSink::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_ngfnotificationsink";
    app = new MApplication(argc, &app_name);
    eventTypeSettings = new QSettings;
    gEventTypeStoreStub->stubSetReturnValue("settingsForEventType", eventTypeSettings);
}

void Ut_NGFNotificationSink::cleanupTestCase()
{
    delete app;
}

void Ut_NGFNotificationSink::init()
{
    NGFAdaptor_play_id = 0;
    NGFAdaptor_stop_called = 0;
    played.clear();

    sink = new NGFNotificationSink();
    manager = new NotificationManager();
    connect(this, SIGNAL(addNotification(Notification)), sink, SLOT(addNotification(Notification)));
    connect(this, SIGNAL(removeNotification(uint)), sink, SLOT(removeNotification(uint)));
    eventTypeSettings->clear();
}

void Ut_NGFNotificationSink::cleanup()
{
    delete sink;
    delete manager;
}

void Ut_NGFNotificationSink::testAddAndRemoveNotification()
{
    // Create a notification
    NotificationParameters parameters;
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback"));
    Notification notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000);
    emit addNotification(notification);

    // Check that NGFAdapter::play() was called for the feedback
    QCOMPARE(played.count(), 1);
    QCOMPARE(played[0], QString("feedback"));

    emit removeNotification(notification.notificationId());

    // Check that NGFAdapter::stop() was called for the notification
    QCOMPARE(NGFAdaptor_stop_called, 1);

    emit removeNotification(notification.notificationId());

    // Check that NGFAdapter::stop() was not called for an already stopped
    // notification
    QCOMPARE(NGFAdaptor_stop_called, 1);

}

void Ut_NGFNotificationSink::testNotificationWhileApplicationEventsDisabled()
{
    // Create a notification
    NotificationParameters parameters;
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback"));
    sink->setApplicationEventsEnabled(false);
    Notification notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000);

    emit addNotification(notification);
    // Check that NGFAdapter::play() was NOT called for the feedback when
    // application events are NOT enabled
    QCOMPARE(played.count(), 0);
}

void Ut_NGFNotificationSink::testWithEventTypeAndFeedbackId()
{
    NotificationParameters parameters;
    parameters.add("eventType", "message-received");
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback"));
    emit addNotification(Notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000));

    // Check that NGFAdapter::play() was called for the feedback
    QCOMPARE(played.count(), 1);
    QCOMPARE(played[0], QString("feedback"));
}

void Ut_NGFNotificationSink::testWithEventTypeWithoutFeedbackId()
{
    eventTypeSettings->setValue(FeedbackParameterFactory::feedbackIdKey(), "eventTypeStoreFeedback");

    NotificationParameters parameters;
    parameters.add("eventType", "message-received");
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter(""));
    emit addNotification(Notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000));

    // Check that NGFAdapter::play() was called for the feedback
    QCOMPARE(played.count(), 1);
    QCOMPARE(played[0], QString("eventTypeStoreFeedback"));
}

void Ut_NGFNotificationSink::testWithoutEventTypeOrFeedbackId()
{
    EventTypeStore eventTypeStore("", 0);
    gNotificationManagerStub->stubSetReturnValue<const EventTypeStore&>("eventTypeStore", eventTypeStore);

    NotificationParameters parameters;
    parameters.add("eventType", "");
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter(""));
    emit addNotification(Notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000));

    // Check that NGFAdapter::play() was not called for the feedback
    QCOMPARE(played.count(), 0);
}

void Ut_NGFNotificationSink::testWithoutEventTypeWithFeedbackId()
{
    NotificationParameters parameters;
    parameters.add("eventType", "");
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback"));
    emit addNotification(Notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000));

    // Check that NGFAdapter::play() was called for the feedback
    QCOMPARE(played.count(), 1);
    QCOMPARE(played[0], QString("feedback"));
}

void Ut_NGFNotificationSink::testDetermineFeedBackId()
{
    eventTypeSettings->setValue(FeedbackParameterFactory::feedbackIdKey(), "eventTypeStoreFeedback");

    NotificationParameters parameters;
    parameters.add("eventType", "message-received");
    emit addNotification(Notification(0, 0, 0,  parameters, Notification::ApplicationEvent, 1000));

    // Check that NGFAdapter::play() was called for the feedback
    QCOMPARE(played.count(), 1);
    QCOMPARE(played[0], QString("eventTypeStoreFeedback"));
}

QTEST_APPLESS_MAIN(Ut_NGFNotificationSink)
