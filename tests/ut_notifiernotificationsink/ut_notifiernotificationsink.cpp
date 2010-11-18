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

#include "ut_notifiernotificationsink.h"
#include "notifiernotificationsink.h"
#include "notification.h"
#include "notificationgroup_stub.h"
#include "genericnotificationparameterfactory.h"
#include "ngfadapter_stub.h"

#define UNSEEN GenericNotificationParameterFactory::unseenKey()
#define COUNT  GenericNotificationParameterFactory::countKey()

#ifdef HAVE_QMSYSTEM
// MeeGo::QmDisplayState stub
static MeeGo::QmDisplayState::DisplayState meeGoQmDisplayState;
MeeGo::QmDisplayState::DisplayState MeeGo::QmDisplayState::get() const
{
    return meeGoQmDisplayState;
}
#endif

void Ut_NotifierNotificationSink::initTestCase()
{
}

void Ut_NotifierNotificationSink::cleanupTestCase()
{
}

void Ut_NotifierNotificationSink::init()
{
    m_subject = new NotifierNotificationSink();
    connect(this, SIGNAL(addNotification(const Notification &)), m_subject, SLOT(addNotification(const Notification &)));
    connect(this, SIGNAL(removeNotification(uint)), m_subject, SLOT(removeNotification(uint)));
    connect(this, SIGNAL(removeGroup(uint)), m_subject, SLOT(removeGroup(uint)));

    gNGFAdapterStub->stubReset();
    gNGFAdapterStub->stubSetReturnValue("play", (uint)1);
}

void Ut_NotifierNotificationSink::cleanup()
{
    delete m_subject;
}

void Ut_NotifierNotificationSink::testAddNotification()
{
    QSignalSpy spy(m_subject, SIGNAL(notifierSinkActive(bool)));

    // Adding an unseen notification should cause a notification count change
    NotificationParameters params1;
    params1.add(UNSEEN, true);
    Notification notification1(1, 0, 2, params1, Notification::ApplicationEvent, 0);
    emit addNotification(notification1);

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.at(0);
    QCOMPARE(arguments.at(0).toBool(), true);

    // Adding a new unseen notification (having one unseen notification 
    // already) should not send further signals
    Notification notification2(2, 0, 2, params1, Notification::ApplicationEvent, 0);
    emit addNotification(notification2);

    QCOMPARE(spy.count(), 1);
}

void Ut_NotifierNotificationSink::testAddSystemNotification()
{
    QSignalSpy spy(m_subject, SIGNAL(notifierSinkActive(bool)));

    // Adding an unseen system notification should cause no notification count
    // change
    NotificationParameters params1;
    params1.add(UNSEEN, true);
    params1.add(COUNT, QVariant((uint)5));
    Notification notification1(1, 0, 2, params1, Notification::SystemEvent, 0);
    emit addNotification(notification1);

    QCOMPARE(spy.count(), 0);
}

void Ut_NotifierNotificationSink::testRemoveNotification()
{
    QSignalSpy spy(m_subject, SIGNAL(notifierSinkActive(bool)));

    // Adding an unseen notification should cause a notification count change
    NotificationParameters params1;
    params1.add(UNSEEN, true);
    params1.add(COUNT, QVariant((uint)5));

    Notification notification1(1, 0, 2, params1, Notification::ApplicationEvent, 0);
    Notification notification2(2, 0, 2, params1, Notification::ApplicationEvent, 0);
    emit addNotification(notification1);
    emit addNotification(notification2);
    QCOMPARE(m_subject->applicationEventIds.count(), 2);

    // Removing one notification does not do anything just changes the notification count
    emit removeNotification(1);

    QCOMPARE(spy.count(), 1); // Only add sent a signal
    QCOMPARE(m_subject->applicationEventIds.count(), 1);

    emit removeNotification(2);

    QCOMPARE(spy.count(), 2); // Now a signal was sent as all notifications are removed
    QCOMPARE(m_subject->applicationEventIds.count(), 0);
    QList<QVariant> arguments = spy.at(1);
    QCOMPARE(arguments.at(0).toBool(), false);
}

void Ut_NotifierNotificationSink::testClearSink()
{
    QSignalSpy spy(m_subject, SIGNAL(notifierSinkActive(bool)));

    // Add two notifications
    NotificationParameters params1;
    params1.add(UNSEEN, true);
    params1.add(COUNT, QVariant((uint)5));
    Notification notification1(1, 0, 2, params1, Notification::ApplicationEvent, 0);

    NotificationParameters params2;
    params2.add(COUNT, QVariant((uint)8));
    params2.add(UNSEEN, true);
    Notification notification2(2, 0, 2, params2, Notification::ApplicationEvent, 0);

    emit addNotification(notification1);
    emit addNotification(notification2);

    QCOMPARE(m_subject->applicationEventIds.count(), 2);

    m_subject->clearSink();

    QCOMPARE(spy.count(), 2); // One for addition and one for clear
    QList<QVariant> arguments = spy.at(1);
    QCOMPARE(arguments.at(0).toBool(), false); // check that the last call was for setting notifier inactive
    QCOMPARE(m_subject->applicationEventIds.count(), 0);
}

void Ut_NotifierNotificationSink::testDisablingNotificationAdditions()
{
    QSignalSpy spy(m_subject, SIGNAL(notifierSinkActive(bool)));

    m_subject->disableNotificationAdditions(true);

    // Try to add an unseen notification while disabled
    NotificationParameters params1;
    params1.add(COUNT, QVariant((uint)5));
    params1.add(UNSEEN, true);
    Notification notification1(1, 0, 2, params1, Notification::ApplicationEvent, 0);
    emit addNotification(notification1);

    QCOMPARE(spy.count(), 0);

    m_subject->disableNotificationAdditions(false);

    // Add an unseen notification when enabled
    NotificationParameters params2;
    params2.add(COUNT, QVariant((uint)5));
    params2.add(UNSEEN, true);
    Notification notification2(2, 0, 2, params2, Notification::ApplicationEvent, 0);
    emit addNotification(notification2);

    QCOMPARE(spy.count(), 1);
}

void Ut_NotifierNotificationSink::testSeenNotificationAddedThenNotifierNotUpdated()
{
    QSignalSpy spy(m_subject, SIGNAL(notifierSinkActive(bool)));

    // Adding a seen notification should not cause a notification count change
    NotificationParameters params;
    params.add(UNSEEN, false);
    Notification notification1(1, 0, 2, params, Notification::ApplicationEvent, 0);
    emit addNotification(notification1);
    QCOMPARE(spy.count(), 0);
}

void Ut_NotifierNotificationSink::addSystemNotification()
{
    NotificationParameters params;
    Notification notification1(1, 0, 2, params, Notification::SystemEvent, 0);
    emit addNotification(notification1);
}

void Ut_NotifierNotificationSink::testWhenAddSystemNotificationNotificationIdIsNotStored()
{
    addSystemNotification();
    QCOMPARE(m_subject->applicationEventIds.count(), 0);
}

void Ut_NotifierNotificationSink::testThatNotifierIsActiveOnlyWhenThereIsNotifications()
{
    NotificationParameters params1;
    NotificationParameters params2;
    params1.add(COUNT, QVariant((uint)1));
    params1.add(UNSEEN, true);
    params2.add(COUNT, QVariant((uint)1));
    params2.add(UNSEEN, true);
    Notification notification1(1, 0, 2, params1, Notification::ApplicationEvent, 0);
    Notification notification2(2, 0, 2, params2, Notification::ApplicationEvent, 0);

    emit addNotification(notification1);

    QCOMPARE(m_subject->notifierEnabled, true);

    emit addNotification(notification2);

    QCOMPARE(m_subject->notifierEnabled, true);

    emit removeNotification(1);
    QCOMPARE(m_subject->notifierEnabled, true);
    emit removeNotification(2);
    QCOMPARE(m_subject->notifierEnabled, false);

    emit removeNotification(2);
    QCOMPARE(m_subject->notifierEnabled, false);
}

#ifdef HAVE_QMSYSTEM
void Ut_NotifierNotificationSink::testSignalConnections()
{
    QVERIFY(disconnect(&m_subject->displayState, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)), m_subject, SLOT(updateStatusOfLedFeedback())));
}

void Ut_NotifierNotificationSink::testLedFeedbackStatus()
{
    // Display off, notifier disabled: no feedbacks
    m_subject->setNotifierEnabled(false);
    meeGoQmDisplayState = MeeGo::QmDisplayState::Off;
    m_subject->updateStatusOfLedFeedback();
    QCOMPARE(gNGFAdapterStub->stubCallCount("play"), 0);
    QCOMPARE(gNGFAdapterStub->stubCallCount("stop"), 0);

    // Display off, notifier enabled: feedback played
    m_subject->setNotifierEnabled(true);
    QCOMPARE(gNGFAdapterStub->stubCallCount("play"), 1);
    QCOMPARE(gNGFAdapterStub->stubCallCount("stop"), 0);

    // Updating the status of the led feedback again should not play a feedback again
    m_subject->updateStatusOfLedFeedback();
    QCOMPARE(gNGFAdapterStub->stubCallCount("play"), 1);
    QCOMPARE(gNGFAdapterStub->stubCallCount("stop"), 0);

    // Display on, notifier enabled: feedback stopped
    meeGoQmDisplayState = MeeGo::QmDisplayState::On;
    m_subject->updateStatusOfLedFeedback();
    QCOMPARE(gNGFAdapterStub->stubCallCount("play"), 1);
    QCOMPARE(gNGFAdapterStub->stubCallCount("stop"), 1);
}
#endif //HAVE_QMSYSTEM

QTEST_APPLESS_MAIN(Ut_NotifierNotificationSink)
