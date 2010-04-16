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


#include "ut_mfeedbacknotificationsink.h"
#include <MApplication>
#include <MFeedbackPlayer>
#include "mfeedbacknotificationsink.h"
#include "feedbackparameterfactory.h"
#include "eventtypestore_stub.h"
#include "notificationmanager_stub.h"
#include "sysuid_stub.h"

#if 0
#include "mfeedbackplayer_p.h"
#endif

// xlib.h defines Status, undef it so we can use QSettings::Status
#ifdef Status
#undef Status
#endif

static NotificationManager *manager;

// MFeedbackPlayer stubs (used by MApplication)
void MFeedbackPlayer::play(const QString &feedbackName)
{
    Ut_MFeedbackNotificationSink::played.append(feedbackName);
}

#if 0
// XXX: FIXME
// Seems the libdui returns true like here, but that
// is also initalizing a connection to mfeedbackd
bool MFeedbackPlayerPrivate::init(const QString &)
{
    return true;
}
#endif

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

QList<QString> Ut_MFeedbackNotificationSink::played;

void Ut_MFeedbackNotificationSink::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_mfeedbacknotificationsink";
    app = new MApplication(argc, &app_name);
    eventTypeSettings = new QSettings;
    gEventTypeStoreStub->stubSetReturnValue("settingsForEventType", eventTypeSettings);
}

void Ut_MFeedbackNotificationSink::cleanupTestCase()
{
    delete app;
}

void Ut_MFeedbackNotificationSink::init()
{
    played.clear();
    sink = new MFeedbackNotificationSink();
    manager = new NotificationManager();
    connect(this, SIGNAL(addNotification(Notification)), sink, SLOT(addNotification(Notification)));
    connect(this, SIGNAL(removeNotification(uint)), sink, SLOT(removeNotification(uint)));
    eventTypeSettings->clear();
}

void Ut_MFeedbackNotificationSink::cleanup()
{
    delete sink;
    delete manager;
}

void Ut_MFeedbackNotificationSink::testAddNotification()
{
    // Create a notification
    NotificationParameters parameters;
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback"));
    emit addNotification(Notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000));

    // Check that MFeedbackPlayer::play() was called for the feedback
    QCOMPARE(played.count(), 1);
    QCOMPARE(played[0], QString("feedback"));
}

void Ut_MFeedbackNotificationSink::testNotificationWhileApplicationEventsDisabled()
{
    // Create a notification
    NotificationParameters parameters;
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback"));
    sink->setApplicationEventsEnabled(true);
    emit addNotification(Notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000));
    // Check that MFeedbackPlayer::play() was called for the feedback when application events enabled
    QCOMPARE(played.count(), 1);

    sink->setApplicationEventsEnabled(false);
    emit addNotification(Notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000));
    // Check that MFeedbackPlayer::play() was NOT called for the feedback when application events are NOT enabled
    QCOMPARE(played.count(), 1);
}

void Ut_MFeedbackNotificationSink::testRemoveNotification()
{
    emit removeNotification(1);
}

void Ut_MFeedbackNotificationSink::testWithEventTypeAndFeedbackId()
{
    NotificationParameters parameters;
    parameters.add("eventType", "message-received");
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback"));
    emit addNotification(Notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000));

    // Check that MFeedbackPlayer::play() was called for the feedback
    QCOMPARE(played.count(), 1);
    QCOMPARE(played[0], QString("feedback"));
}

void Ut_MFeedbackNotificationSink::testWithEventTypeWithoutFeedbackId()
{
    eventTypeSettings->setValue(FeedbackParameterFactory::feedbackIdKey(), "eventTypeStoreFeedback");

    NotificationParameters parameters;
    parameters.add("eventType", "message-received");
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter(""));
    emit addNotification(Notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000));

    // Check that MFeedbackPlayer::play() was called for the feedback
    QCOMPARE(played.count(), 1);
    QCOMPARE(played[0], QString("eventTypeStoreFeedback"));
}

void Ut_MFeedbackNotificationSink::testWithoutEventTypeOrFeedbackId()
{
    NotificationParameters parameters;
    parameters.add("eventType", "");
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter(""));
    emit addNotification(Notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000));

    // Check that MFeedbackPlayer::play() was not called for the feedback
    QCOMPARE(played.count(), 0);
}

void Ut_MFeedbackNotificationSink::testWithoutEventTypeWithFeedbackId()
{
    NotificationParameters parameters;
    parameters.add("eventType", "");
    parameters.add(FeedbackParameterFactory::createFeedbackIdParameter("feedback"));
    emit addNotification(Notification(0, 0, 0, parameters, Notification::ApplicationEvent, 1000));

    // Check that MFeedbackPlayer::play() was called for the feedback
    QCOMPARE(played.count(), 1);
    QCOMPARE(played[0], QString("feedback"));
}

void Ut_MFeedbackNotificationSink::testDetermineFeedBackId()
{
    eventTypeSettings->setValue(FeedbackParameterFactory::feedbackIdKey(), "eventTypeStoreFeedback");

    NotificationParameters parameters;
    parameters.add("eventType", "message-received");
    emit addNotification(Notification(0, 0, 0,  parameters, Notification::ApplicationEvent, 1000));

    // Check that MFeedbackPlayer::play() was called for the feedback
    QCOMPARE(played.count(), 1);
    QCOMPARE(played[0], QString("eventTypeStoreFeedback"));
}

QTEST_APPLESS_MAIN(Ut_MFeedbackNotificationSink)
