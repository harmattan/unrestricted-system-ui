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

#include "ut_unlockmissedevents.h"
#include "unlockmissedevents.h"

#include <QtTest/QtTest>

void
Ut_UnlockMissedEvents::initTestCase ()
{
}

void
Ut_UnlockMissedEvents::cleanupTestCase ()
{
}

void
Ut_UnlockMissedEvents::init ()
{
}

void
Ut_UnlockMissedEvents::cleanup ()
{
}

void
Ut_UnlockMissedEvents::testSingleton ()
{
    // Check if it is a valid object...
    QVERIFY (UnlockMissedEvents::getInstance ().inherits ("UnlockMissedEvents"));

    // Check the default last type (should be notify last)
    QCOMPARE (UnlockMissedEvents::getInstance ().getLastType (),
              UnlockMissedEvents::NotifyLast);
}

void
Ut_UnlockMissedEvents::testAddEvents ()
{
    UnlockMissedEvents *ume = &UnlockMissedEvents::getInstance ();
    ume->clearAll ();
    QString subject = "subject1";

    QSignalSpy spy (ume, SIGNAL (updated ()));

    // E-mail notification adding...
    ume->addNotification (UnlockMissedEvents::NotifyEmail, subject);

    QCOMPARE (ume->getLastType (), UnlockMissedEvents::NotifyEmail);
    QCOMPARE (ume->getCount (UnlockMissedEvents::NotifyEmail), 1);
    QVERIFY (ume->getLastSubject (UnlockMissedEvents::NotifyEmail) == subject);
    QTest::qWait (20);
    QCOMPARE (spy.count (), 1);

    // Sms notification adding...
    subject = "Hello world!!! [SMS]";
    ume->addNotification (UnlockMissedEvents::NotifySms, subject);

    QCOMPARE (ume->getLastType (), UnlockMissedEvents::NotifySms);
    QCOMPARE (ume->getCount (UnlockMissedEvents::NotifySms), 1);
    QVERIFY (ume->getLastSubject (UnlockMissedEvents::NotifySms) == subject);
    QTest::qWait (20);
    QCOMPARE (spy.count (), 2);

    // Call notification adding...
    subject = "John Doe";
    ume->addNotification (UnlockMissedEvents::NotifyCall, subject);

    QCOMPARE (ume->getLastType (), UnlockMissedEvents::NotifyCall);
    QCOMPARE (ume->getCount (UnlockMissedEvents::NotifyCall), 1);
    QVERIFY (ume->getLastSubject (UnlockMissedEvents::NotifyCall) == subject);
    QTest::qWait (20);
    QCOMPARE (spy.count (), 3);

    // Message notification adding...
    subject = "Dude: what's up?";
    ume->addNotification (UnlockMissedEvents::NotifyMessage, subject);

    QCOMPARE (ume->getLastType (), UnlockMissedEvents::NotifyMessage);
    QCOMPARE (ume->getCount (UnlockMissedEvents::NotifyMessage), 1);
    QVERIFY (ume->getLastSubject (UnlockMissedEvents::NotifyMessage) == subject);
    QTest::qWait (20);
    QCOMPARE (spy.count (), 4);

    // Message notification adding [again]...
    subject = "Dude2: Linux is awesome!";
    ume->addNotification (UnlockMissedEvents::NotifyMessage, subject);

    QCOMPARE (ume->getLastType (), UnlockMissedEvents::NotifyMessage);
    QCOMPARE (ume->getCount (UnlockMissedEvents::NotifyMessage), 2);
    QVERIFY (ume->getLastSubject (UnlockMissedEvents::NotifyMessage) == subject);
    QTest::qWait (20);
    QCOMPARE (spy.count (), 5);

    // Other event...
    subject = "Low battery!";
    ume->addNotification (UnlockMissedEvents::NotifyOther, subject);

    QCOMPARE (ume->getLastType (), UnlockMissedEvents::NotifyOther);
    QCOMPARE (ume->getCount (UnlockMissedEvents::NotifyOther), 1);
    QVERIFY (ume->getLastSubject (UnlockMissedEvents::NotifyOther) == subject);
    QTest::qWait (20);
    QCOMPARE (spy.count (), 6);
}

void
Ut_UnlockMissedEvents::testClearEvents ()
{
    UnlockMissedEvents *ume = &UnlockMissedEvents::getInstance ();

    // Add some events...
    ume->addNotification (UnlockMissedEvents::NotifyMessage, "Message");
    ume->addNotification (UnlockMissedEvents::NotifyOther, "Other event");

    QSignalSpy spy (ume, SIGNAL (updated ()));

    ume->clearAll ();

    QTest::qWait (20);

    // Check if updated signal arrived
    QCOMPARE (spy.count (), 1);

    // Check the last event type...
    QCOMPARE (ume->getLastType (), UnlockMissedEvents::NotifyLast);

    // Check the event-counts and last subjects...
    for (int i = 0; i < UnlockMissedEvents::NotifyLast; i++)
    {
        UnlockMissedEvents::Types type = (UnlockMissedEvents::Types) i;

        QCOMPARE (ume->getCount (type), 0);
        QVERIFY (ume->getLastSubject (type) == "");
    }
}

QTEST_MAIN(Ut_UnlockMissedEvents)
