/***************************************************************************
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

#include <notification.h>
#include <notificationparameters.h>
#include <genericnotificationparameterfactory.h>

#include "ut_unlocknotificationsink.h"

#include <QtTest/QtTest>
#include <MApplication>

// Users group and user id:
#define GID 29999
#define UID 29999

// Define the used event-types:
#define EVENT_EMAIL "email.arrived"
#define EVENT_MSG   "x-nokia.message.arrived"
#define EVENT_CALL  "x-nokia.call"
#define EVENT_IM    "im.received"
// and an another one for testing:
#define EVENT_OTHER "device.added"

static int argc = 1;
static char *app_name = (char *) "./ut_unlocknotificationsink";

void
Ut_UnlockNotificationSink::initTestCase ()
{
    // Create a MAapplication
    app = new MApplication (argc, &app_name);
}

void
Ut_UnlockNotificationSink::cleanupTestCase ()
{
    // Destroy MApplication
    delete app;
}

void
Ut_UnlockNotificationSink::init ()
{
    sink = new UnlockNotificationSink ();
}

void
Ut_UnlockNotificationSink::cleanup ()
{
    delete sink;
}

void
Ut_UnlockNotificationSink::testAddNotification ()
{
    // Test enabled sink (locked-state)
    sink->setLockedState (true);

    QTest::qWait (100);

    QSignalSpy  spy (sink, SIGNAL (updateNotificationsCount (int, int, int, int)));

    NotificationParameters email_params;
    email_params.add (GenericNotificationParameterFactory::eventTypeKey (),
                      EVENT_EMAIL);
    Notification email (100,
                        GID,
                        UID,
                        email_params,
                        Notification::ApplicationEvent,
                        -1);

    QVERIFY (sink->canAddNotification (email) == true);
    // Add e-mail notification
    sink->addNotification (email);

    NotificationParameters msg_params;
    msg_params.add (GenericNotificationParameterFactory::eventTypeKey (),
                    EVENT_MSG);
    Notification msg (101,
                      GID,
                      UID,
                      msg_params,
                      Notification::ApplicationEvent,
                      -1);

    QVERIFY (sink->canAddNotification (msg) == true);
    // Add SMS notification
    sink->addNotification (msg);

    NotificationParameters call_params;
    call_params.add (GenericNotificationParameterFactory::eventTypeKey (),
                     EVENT_CALL);
    Notification call (102,
                       GID,
                       UID,
                       call_params,
                       Notification::ApplicationEvent,
                       -1);

    QVERIFY (sink->canAddNotification (call) == true);
    // Add call notification
    sink->addNotification (call);

    NotificationParameters im_params;
    im_params.add (GenericNotificationParameterFactory::eventTypeKey (),
                   EVENT_IM);
    Notification im (103,
                     GID,
                     UID,
                     im_params,
                     Notification::ApplicationEvent,
                     -1);

    QVERIFY (sink->canAddNotification (im) == true);
    // Add instant message notification
    sink->addNotification (im);

    NotificationParameters usb_params;
    usb_params.add (GenericNotificationParameterFactory::eventTypeKey (),
                    EVENT_OTHER);
    Notification usb (104,
                      GID,
                      UID,
                      usb_params,
                      Notification::ApplicationEvent,
                      -1);

    QVERIFY (sink->canAddNotification (usb) == false);
    // Add USB notification
    sink->addNotification (usb);

    // Try to remove a notification (no signal should be emitted)
    // TODO: This should follow the implementation changes...
    //       i have to figure out whether unlock-screen should handle
    //       the notification removals...
    sink->removeNotification (101);

    // Wait for some time, so signals can arrive...
    QTest::qWait (150);

    // e-mail, msg (sms), call, im
    QVERIFY (spy.count () == 4);

    // SIGNAL:
    // void updateNotificationsCount (int emails, int messages, int calls, int im);
    QList<QVariant> arguments = spy.takeFirst ();
    // One e-mail arrived...
    QVERIFY (arguments.at (0).toInt () == 1);
    QVERIFY (arguments.at (1).toInt () == 0);
    QVERIFY (arguments.at (2).toInt () == 0);
    QVERIFY (arguments.at (3).toInt () == 0);

    QList<QVariant> arguments = spy.takeFirst ();
    // One message arrived...
    QVERIFY (arguments.at (0).toInt () == 1);
    QVERIFY (arguments.at (1).toInt () == 1);
    QVERIFY (arguments.at (2).toInt () == 0);
    QVERIFY (arguments.at (3).toInt () == 0);

    QList<QVariant> arguments = spy.takeFirst ();
    // One call arrived...
    QVERIFY (arguments.at (0).toInt () == 1);
    QVERIFY (arguments.at (1).toInt () == 1);
    QVERIFY (arguments.at (2).toInt () == 1);
    QVERIFY (arguments.at (3).toInt () == 0);

    QList<QVariant> arguments = spy.takeFirst ();
    // One instant message arrived...
    QVERIFY (arguments.at (0).toInt () == 1);
    QVERIFY (arguments.at (1).toInt () == 1);
    QVERIFY (arguments.at (2).toInt () == 1);
    QVERIFY (arguments.at (3).toInt () == 1);
}

void
Ut_UnlockNotificationSink::testEnableDisableLocking ()
{
    QSignalSpy  spy (sink, SIGNAL (updateNotificationsCount (int, int, int, int)));

    // Test notifications
    NotificationParameters im_params;
    im_params.add (GenericNotificationParameterFactory::eventTypeKey (),
                   EVENT_IM);
    Notification im1 (200, GID, UID, im_params,
                      Notification::ApplicationEvent, -1);
    Notification im2 (200, GID, UID, im_params,
                      Notification::ApplicationEvent, -1);
    Notification im3 (200, GID, UID, im_params,
                      Notification::ApplicationEvent, -1);

    sink->setLockedState (false);

    // Try to add one notification
    sink->addNotification (im1);

    sink->setLockedState (true);

    // Try to add some notifications...
    sink->addNotification (im2);
    sink->addNotification (im3);

    sink->setLockedState (false);

    // 1.) setLockedState (false) emit update counts with zeros
    // 2.) im2
    // 3.) im3
    // 4.) setLockedState (true) emit update counts with zeros
    // 5.) setLockedState (false) emit update counts with zeros...
    QVERIFY (spy.count () == 5);

    QList<QVariant> arguments = spy.takeAt (2);
    // One instant message arrived...
    QVERIFY (arguments.at (0).toInt () == 0);
    QVERIFY (arguments.at (1).toInt () == 0);
    QVERIFY (arguments.at (2).toInt () == 0);
    QVERIFY (arguments.at (3).toInt () == 2);
}

QTEST_APPLESS_MAIN(Ut_UnlockNotificationSink)
