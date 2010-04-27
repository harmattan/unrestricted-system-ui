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

NGFAdapter::NGFAdapter()
{

}

NGFAdapter::~NGFAdapter()
{

}

void NGFAdapter::play(const QString &id)
{
    Ut_NGFNotificationSink::played.append(id);
}

bool NGFAdapter::isValid()
{
    return true;
}
    
// maemosec stubs
int maemosec::storage::get_file(const char *pathname, unsigned char **to_buf, ssize_t *bytes)
{
    Q_UNUSED(pathname);
    Q_UNUSED(to_buf);
    Q_UNUSED(bytes);
    return 0;
}

int maemosec::storage::put_file(const char *pathname, unsigned char *data, ssize_t bytes)
{
    Q_UNUSED(pathname);
    Q_UNUSED(data);
    Q_UNUSED(bytes);
    return 0;
}

void maemosec::storage::release_buffer(unsigned char *data)
{
    Q_UNUSED(data);
}

bool maemosec::storage::contains_file(const char *pathname)
{
    Q_UNUSED(pathname);
    return true;
}

void maemosec::storage::remove_file(const char *pathname)
{
    Q_UNUSED(pathname);
}

void maemosec::storage::commit()
{
}

maemosec::storage::storage(const char *name, maemosec::storage::visibility_t visibility, maemosec::storage::protection_t protection)
{
    Q_UNUSED(name);
    Q_UNUSED(visibility);
    Q_UNUSED(protection);
}

maemosec::storage::~storage()
{
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

void Ut_NGFNotificationSink::testAddNotification()
{
    // Create a notification
    NotificationParameters parameters;
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback"));
    emit addNotification(Notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000));

    // Check that NGFAdapter::play() was called for the feedback
    QCOMPARE(played.count(), 1);
    QCOMPARE(played[0], QString("feedback"));
}

void Ut_NGFNotificationSink::testNotificationWhileApplicationEventsDisabled()
{
    // Create a notification
    NotificationParameters parameters;
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback"));
    sink->setApplicationEventsEnabled(true);
    emit addNotification(Notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000));
    // Check that NGFAdapter::play() was called for the feedback when application events enabled
    QCOMPARE(played.count(), 1);

    sink->setApplicationEventsEnabled(false);
    emit addNotification(Notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000));
    // Check that NGFAdapter::play() was NOT called for the feedback when application events are NOT enabled
    QCOMPARE(played.count(), 1);
}

void Ut_NGFNotificationSink::testRemoveNotification()
{
    emit removeNotification(1);
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
