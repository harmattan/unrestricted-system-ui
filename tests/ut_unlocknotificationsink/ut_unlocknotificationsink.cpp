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
#include <notificationwidgetparameterfactory.h>
#include <unlocknotificationsink.h>

#include "ut_unlocknotificationsink.h"
#include "unlockmissedevents_stub.h"

#include <QtTest/QtTest>
#include <MApplication>
#include <QList>
#include <QVariant>
#include <MLocale>
#include <MGConfItem>

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

QString qtTrId(const char *id, int)
{
    if(QString(id) == "translationid")
        return "translatedstring";
    else
        return "";
}

// MLocale stubs
QString gInstalledTrCatalog;
MLocale *gInstalledCatalogLocale;
void MLocale::installTrCatalog(const QString &name)
{
    gInstalledCatalogLocale = this;
    gInstalledTrCatalog = name;
}

MLocale *gSetDefaultLocale;
void MLocale::setDefault(const MLocale &locale)
{
    gSetDefaultLocale = const_cast<MLocale*>(&locale);
}

// MGConfItem stubs

bool gMGConfPrivateNotificationValue;
QVariant MGConfItem::value() const
{
    if(this->key() == "/desktop/meego/privacy/private_lockscreen_notifications") {
        return QVariant(gMGConfPrivateNotificationValue);
    }

    return QVariant();
}


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

    gUnlockMissedEventsStub->stubReset ();
    gInstalledTrCatalog = "";
    gInstalledCatalogLocale = NULL;
    gSetDefaultLocale = NULL;
    gMGConfPrivateNotificationValue = false;
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

    QCOMPARE (gUnlockMissedEventsStub->stubCallCount ("addNotification"), 1);
    QVERIFY (gUnlockMissedEventsStub->stubLastParameters<int> (0) 
             == (int) UnlockMissedEvents::NotifyEmail);
    gUnlockMissedEventsStub->stubReset ();

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

    QCOMPARE (gUnlockMissedEventsStub->stubCallCount ("addNotification"), 1);
    QVERIFY (gUnlockMissedEventsStub->stubLastParameters<int> (0) 
             == (int) UnlockMissedEvents::NotifySms);
    gUnlockMissedEventsStub->stubReset ();

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

    QCOMPARE (gUnlockMissedEventsStub->stubCallCount ("addNotification"), 1);
    QVERIFY (gUnlockMissedEventsStub->stubLastParameters<int> (0) 
             == (int) UnlockMissedEvents::NotifyCall);
    gUnlockMissedEventsStub->stubReset ();

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

    QCOMPARE (gUnlockMissedEventsStub->stubCallCount ("addNotification"), 1);
    QVERIFY (gUnlockMissedEventsStub->stubLastParameters<int> (0) 
             == (int) UnlockMissedEvents::NotifyMessage);
    gUnlockMissedEventsStub->stubReset ();

    NotificationParameters usb_params;
    usb_params.add (GenericNotificationParameterFactory::eventTypeKey (),
                    EVENT_OTHER);
    Notification usb (104,
                      GID,
                      UID,
                      usb_params,
                      Notification::ApplicationEvent,
                      -1);

    QVERIFY (sink->canAddNotification (usb) == true);
    // Add USB notification
    sink->addNotification (usb);

    QCOMPARE (gUnlockMissedEventsStub->stubCallCount ("addNotification"), 1);
    QVERIFY (gUnlockMissedEventsStub->stubLastParameters<int> (0) 
             == (int) UnlockMissedEvents::NotifyOther);
    gUnlockMissedEventsStub->stubReset ();
}

void Ut_UnlockNotificationSink::testWhenPrivacyModeOffThenSummaryIsFromNotification()
{
    sink->setLockedState(true);

    QString notificationSummary("notificationsummary");

    NotificationParameters params;
    params.add(NotificationWidgetParameterFactory::summaryKey(), notificationSummary);

    Notification notification(100, GID, UID, params, Notification::ApplicationEvent, -1);

    sink->addNotification(notification);

    QCOMPARE(gUnlockMissedEventsStub->stubCallCount ("addNotification"), 1);
    QCOMPARE(gUnlockMissedEventsStub->stubLastCallTo("addNotification").parameter<QString>(1), notificationSummary);
}

void Ut_UnlockNotificationSink::testWhenPrivacyModeOnAndGenericTextIdPresentAndCatalogueNotPresentThenSummaryIsEmpty()
{
    sink->setLockedState(true);
    gMGConfPrivateNotificationValue = true;

    NotificationParameters params;
    params.add(NotificationWidgetParameterFactory::genericTextIdKey(), "translationid");

    Notification notification(100, GID, UID, params, Notification::ApplicationEvent, -1);

    sink->addNotification(notification);

    QCOMPARE(gUnlockMissedEventsStub->stubCallCount ("addNotification"), 1);
    QCOMPARE(gUnlockMissedEventsStub->stubLastCallTo("addNotification").parameter<QString>(1), QString(""));
}

void Ut_UnlockNotificationSink::testWhenPrivacyModeOnAndGenericTextIdPresentAndCataloguePresentThenSummaryIsSet()
{
    sink->setLockedState(true);
    gMGConfPrivateNotificationValue = true;

    QString catalogue("translationcatalogue");

    NotificationParameters params;
    params.add(NotificationWidgetParameterFactory::genericTextIdKey(), "translationid");
    params.add(NotificationWidgetParameterFactory::genericTextCatalogueKey(), catalogue);

    Notification notification(100, GID, UID, params, Notification::ApplicationEvent, -1);

    sink->addNotification(notification);

    QCOMPARE(gInstalledTrCatalog, catalogue);
    QCOMPARE(gSetDefaultLocale, gInstalledCatalogLocale);
    QCOMPARE(gUnlockMissedEventsStub->stubCallCount ("addNotification"), 1);
    QCOMPARE(gUnlockMissedEventsStub->stubLastCallTo("addNotification").parameter<QString>(1), QString("translatedstring"));
}


void
Ut_UnlockNotificationSink::testEnableDisableLocking ()
{
    sink->setLockedState (true);
    gUnlockMissedEventsStub->stubReset ();

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


    // To trigger the notification clearing...
    sink->setLockedState (false);
    QCOMPARE (gUnlockMissedEventsStub->stubCallCount ("clearAll"), 1);

    // Try to add one notification
    sink->addNotification (im1);

    QCOMPARE (gUnlockMissedEventsStub->stubCallCount ("addNotification"), 0);
    gUnlockMissedEventsStub->stubReset ();

    sink->setLockedState (true);

    // Try to add some notifications...
    sink->addNotification (im2);
    sink->addNotification (im3);

    QCOMPARE (gUnlockMissedEventsStub->stubCallCount ("addNotification"), 2);
    QVERIFY (gUnlockMissedEventsStub->stubLastParameters<int> (0) 
             == (int) UnlockMissedEvents::NotifyMessage);
    gUnlockMissedEventsStub->stubReset ();

    sink->setLockedState (false);

    QCOMPARE (gUnlockMissedEventsStub->stubCallCount ("clearAll"), 1);
}

QTEST_APPLESS_MAIN(Ut_UnlockNotificationSink)
