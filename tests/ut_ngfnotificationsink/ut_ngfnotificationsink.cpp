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
#include "ngfadapter_stub.h"

void Ut_NGFNotificationSink::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_ngfnotificationsink";
    app = new MApplication(argc, &app_name);
}

void Ut_NGFNotificationSink::cleanupTestCase()
{
    delete app;
}

void Ut_NGFNotificationSink::init()
{
    sink = new NGFNotificationSink;
    connect(this, SIGNAL(addNotification(Notification)), sink, SLOT(addNotification(Notification)));
    connect(this, SIGNAL(removeNotification(uint)), sink, SLOT(removeNotification(uint)));

    gNGFAdapterStub->stubReset();
    gNGFAdapterStub->stubSetReturnValue<bool>("isValid", true);
    gNGFAdapterStub->stubSetReturnValue<uint>("play", 1);
}

void Ut_NGFNotificationSink::cleanup()
{
    delete sink;
}

void Ut_NGFNotificationSink::testAddAndRemoveNotification()
{
    // Create a notification
    NotificationParameters parameters;
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback"));
    Notification notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000);
    emit addNotification(notification);

    // Check that NGFAdapter::play() was called for the feedback
    QCOMPARE(gNGFAdapterStub->stubCallCount("play"), 1);
    QCOMPARE(gNGFAdapterStub->stubLastCallTo("play").parameter<QString>(0), QString("feedback"));

    emit removeNotification(notification.notificationId());

    // Check that NGFAdapter::stop() was called for the notification
    QCOMPARE(gNGFAdapterStub->stubCallCount("stop"), 1);

    emit removeNotification(notification.notificationId());

    // Check that NGFAdapter::stop() was not called for an already stopped notification
    QCOMPARE(gNGFAdapterStub->stubCallCount("stop"), 1);
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
    QCOMPARE(gNGFAdapterStub->stubCallCount("play"), 0);
}

void Ut_NGFNotificationSink::testWithEventTypeAndFeedbackId()
{
    NotificationParameters parameters;
    parameters.add("eventType", "message-received");
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback"));
    emit addNotification(Notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000));

    // Check that NGFAdapter::play() was called for the feedback
    QCOMPARE(gNGFAdapterStub->stubCallCount("play"), 1);
    QCOMPARE(gNGFAdapterStub->stubLastCallTo("play").parameter<QString>(0), QString("feedback"));
}

void Ut_NGFNotificationSink::testWithoutEventTypeOrFeedbackId()
{
    NotificationParameters parameters;
    parameters.add("eventType", "");
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter(""));
    emit addNotification(Notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000));

    // Check that NGFAdapter::play() was not called for the feedback
    QCOMPARE(gNGFAdapterStub->stubCallCount("play"), 0);
}

void Ut_NGFNotificationSink::testWithoutEventTypeWithFeedbackId()
{
    NotificationParameters parameters;
    parameters.add("eventType", "");
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback"));
    emit addNotification(Notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000));

    // Check that NGFAdapter::play() was called for the feedback
    QCOMPARE(gNGFAdapterStub->stubCallCount("play"), 1);
    QCOMPARE(gNGFAdapterStub->stubLastCallTo("play").parameter<QString>(0), QString("feedback"));
}

void Ut_NGFNotificationSink::testUpdateNotificationIsNotPossible()
{
    // Create a notification
    NotificationParameters parameters1;
    parameters1.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback1"));
    Notification notification(0, 0, 0, parameters1, Notification::ApplicationEvent, 1000);
    emit addNotification(notification);

    // Update the notification
    NotificationParameters parameters2;
    parameters2.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback2"));
    notification.setParameters(parameters2);
    emit addNotification(notification);

    // Check that NGFAdapter::play() was only called for the first feedback
    QCOMPARE(gNGFAdapterStub->stubCallCount("play"), 1);
    QCOMPARE(gNGFAdapterStub->stubLastCallTo("play").parameter<QString>(0), QString("feedback1"));
}

QTEST_APPLESS_MAIN(Ut_NGFNotificationSink)
