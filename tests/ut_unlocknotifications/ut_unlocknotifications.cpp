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
#include "ut_unlocknotifications.h"

#include <notification.h>
#include <notificationparameters.h>
#include <genericnotificationparameterfactory.h>

#include "unlockmissedevents_stub.h"
#include "unlocknotifications.h"

#include <MApplication>
#include <MOrientationChangeEvent>

#include <QGraphicsLinearLayout>
#include <MStylableWidget>
#include <MImageWidget>
#include <MLabel>

#include <QtTest/QtTest>
#include <QList>
#include <QVariant>

static int argc = 1;
static char *app_name = (char *) "./ut_unlocknotificationsink";

/******************************************************************************
 * SignalSink implementation.
 */
SignalSink::SignalSink () :
    m_NeedToShow (false),
    m_NeedToShowCame (false)
{
}

void
SignalSink::needToShow (
        bool visible)
{
    m_NeedToShow = false;
    m_NeedToShowCame = true;
}

/******************************************************************************
 * Ut_UnlockNotifications implements the actual testcase.
 */
void
Ut_UnlockNotifications::initTestCase ()
{
    // Create a MAapplication
    app = new MApplication (argc, &app_name);
}

void
Ut_UnlockNotifications::cleanupTestCase ()
{
    // Destroy MApplication
    delete app;
}

void
Ut_UnlockNotifications::init ()
{
    bool connectSuccess;

    m_Subject = new UnlockNotifications ();

    connectSuccess = connect (
            m_Subject, SIGNAL(needToShow(bool)),
            &m_SignalSink, SLOT(needToShow(bool)));
    QVERIFY (connectSuccess);
}

void
Ut_UnlockNotifications::cleanup ()
{
    delete m_Subject;
}

void
Ut_UnlockNotifications::testDefaultValues ()
{
    if (m_Subject) {
        delete m_Subject;
    }

    m_Subject = new UnlockNotifications ();

    QVERIFY (m_Subject->m_otherevents_area);
    QVERIFY (m_Subject->m_mostrecent_layout);
    QVERIFY (m_Subject->m_last_icon);
    QVERIFY (m_Subject->m_last_subject);
    QVERIFY (m_Subject->m_icon_layout);
    QVERIFY (m_Subject->m_mostrecent_area);
    QVERIFY (m_Subject->m_vbox);

    QVERIFY (m_Tester.objectnameIs (
                m_Subject->m_otherevents_area,
                "LockOtherEventsArea"));
    QVERIFY (m_Tester.objectnameIs (
                m_Subject->m_last_icon,
                "LockMostRecentIcon"));
    QVERIFY (m_Tester.objectnameIs (
                m_Subject->m_last_subject,
                "LockMostRecentLabel"));
    QVERIFY (m_Tester.objectnameIs (
                m_Subject->m_mostrecent_area,
                "LockMostRecentArea"));
}

void
Ut_UnlockNotifications::testOrientationChanged ()
{
    if (!m_Subject)
        m_Subject = new UnlockNotifications ();

    /*
     * Testing the layout in landscape.
     */
    MOrientationChangeEvent landscapeEvent (M::Landscape);
    m_Subject->orientationChangeEvent (&landscapeEvent);
    QVERIFY (m_Tester.isLayoutContains(
                m_Subject->m_icon_layout,
                m_Subject->m_last_icon));
    QVERIFY (m_Tester.isLayoutContains(
                m_Subject->m_icon_layout,
                m_Subject->m_last_subject));

    /*
     * testing the layout in portrait.
     */
    MOrientationChangeEvent portraitEvent (M::Portrait);
    m_Subject->orientationChangeEvent (&portraitEvent);
    
    QVERIFY (m_Tester.isLayoutContains(
                m_Subject->m_mostrecent_layout,
                m_Subject->m_last_icon));
    QVERIFY (m_Tester.isLayoutContains(
                m_Subject->m_mostrecent_layout,
                m_Subject->m_last_subject));
    
    QVERIFY (m_Tester.isLayoutContains(
                m_Subject->m_vbox,
                m_Subject->m_mostrecent_area));
}

void
Ut_UnlockNotifications::testUpdateContentsWithNothing ()
{
    /*
     * Initializing...
     */
    m_SignalSink.m_NeedToShow = false;
    m_SignalSink.m_NeedToShowCame = false;
    gUnlockMissedEventsStub->stubSetReturnValue (
            "getLastType", 
            UnlockMissedEvents::NotifyLast);

    /*
     * Calling the updateContents so the widget is refreshed.
     */
    m_Subject->updateContents ();

    /*
     * The checks...
     */
    QVERIFY (!m_SignalSink.m_NeedToShow);
    QVERIFY (m_SignalSink.m_NeedToShowCame);
    QVERIFY (m_Subject->m_labels.size() == 0);
    QVERIFY (m_Subject->m_icons.size() == 0);
    QVERIFY (m_Subject->m_last_subject->text().isEmpty());
    QVERIFY (m_Subject->m_last_icon->image().isEmpty());
}

/*
 * Checking what happens when we have one SMS to show.
 */
void
Ut_UnlockNotifications::testUpdateContentsWithOneSMS ()
{
    /*
     * Initializing...
     */
    m_SignalSink.m_NeedToShow = false;
    m_SignalSink.m_NeedToShowCame = false;
    gUnlockMissedEventsStub->stubSetReturnValue ("getCount", 1);
    gUnlockMissedEventsStub->stubSetReturnValue (
            "getLastSubject", 
            QString("lastSubject"));
    gUnlockMissedEventsStub->stubSetReturnValue (
            "getLastType", 
            UnlockMissedEvents::NotifySms);

    /*
     * Calling the updateContents so the widget is refreshed.
     */
    m_Subject->updateContents ();

    /*
     * The checks...
     */
    QVERIFY (!m_SignalSink.m_NeedToShow);
    QVERIFY (m_SignalSink.m_NeedToShowCame);
    QVERIFY (notificationLastSubject() == "lastSubject");
    QVERIFY (notificationLabelText(UnlockMissedEvents::NotifySms) == "1");
    QVERIFY (notificationIconName(UnlockMissedEvents::NotifySms) == 
            "icon-m-content-sms");
}

void
Ut_UnlockNotifications::testUpdateContentsWithManySMS ()
{
    /*
     * Initializing...
     */
    m_SignalSink.m_NeedToShow = false;
    m_SignalSink.m_NeedToShowCame = false;
    gUnlockMissedEventsStub->stubSetReturnValue ("getCount", 8);
    gUnlockMissedEventsStub->stubSetReturnValue (
            "getLastSubject", 
            QString("lastSubject"));
    gUnlockMissedEventsStub->stubSetReturnValue (
            "getLastType", 
            UnlockMissedEvents::NotifySms);

    m_Subject->updateContents ();

    QVERIFY (!m_SignalSink.m_NeedToShow);
    QVERIFY (m_SignalSink.m_NeedToShowCame);
    QVERIFY (notificationLastSubject() == "qtn_scrl_sms");
    QVERIFY (notificationLabelText(UnlockMissedEvents::NotifySms) == "8");
    QVERIFY (notificationIconName(UnlockMissedEvents::NotifySms) == 
            "icon-m-content-sms");
}

void
Ut_UnlockNotifications::testUpdateContentsWithOneCall ()
{
    /*
     * Initializing...
     */
    m_SignalSink.m_NeedToShow = false;
    m_SignalSink.m_NeedToShowCame = false;
    gUnlockMissedEventsStub->stubSetReturnValue ("getCount", 1);
    gUnlockMissedEventsStub->stubSetReturnValue (
            "getLastSubject", 
            QString("lastCaller"));
    gUnlockMissedEventsStub->stubSetReturnValue (
            "getLastType", 
            UnlockMissedEvents::NotifyCall);

    m_Subject->updateContents ();
    
    QVERIFY (!m_SignalSink.m_NeedToShow);
    QVERIFY (m_SignalSink.m_NeedToShowCame);
    QVERIFY (notificationLastSubject() == "lastCaller");
    QVERIFY (notificationLabelText(UnlockMissedEvents::NotifyCall) == "1");
    QVERIFY (notificationIconName(UnlockMissedEvents::NotifyCall) == 
            "icon-m-content-call");
}

void
Ut_UnlockNotifications::testUpdateContentsWithManyCall ()
{
    /*
     * Initializing...
     */
    m_SignalSink.m_NeedToShow = false;
    m_SignalSink.m_NeedToShowCame = false;
    gUnlockMissedEventsStub->stubSetReturnValue ("getCount", 3);
    gUnlockMissedEventsStub->stubSetReturnValue (
            "getLastSubject", 
            QString("lastCaller"));
    gUnlockMissedEventsStub->stubSetReturnValue (
            "getLastType", 
            UnlockMissedEvents::NotifyCall);

    m_Subject->updateContents ();
    
    QVERIFY (!m_SignalSink.m_NeedToShow);
    QVERIFY (m_SignalSink.m_NeedToShowCame);
    QVERIFY (notificationLastSubject() == "qtn_scrl_missed_call");
    QVERIFY (notificationLabelText(UnlockMissedEvents::NotifyCall) == "3");
    QVERIFY (notificationIconName(UnlockMissedEvents::NotifyCall) == 
            "icon-m-content-call");
}

void
Ut_UnlockNotifications::testUpdateContentsWithManyEmail ()
{
    /*
     * Initializing...
     */
    m_SignalSink.m_NeedToShow = false;
    m_SignalSink.m_NeedToShowCame = false;
    gUnlockMissedEventsStub->stubSetReturnValue ("getCount", 89);
    gUnlockMissedEventsStub->stubSetReturnValue (
            "getLastSubject", 
            QString("lastEmail"));
    gUnlockMissedEventsStub->stubSetReturnValue (
            "getLastType", 
            UnlockMissedEvents::NotifyEmail);

    m_Subject->updateContents ();
    
    QVERIFY (!m_SignalSink.m_NeedToShow);
    QVERIFY (m_SignalSink.m_NeedToShowCame);
    QVERIFY (notificationLastSubject() == "qtn_scrl_email");
    QVERIFY (notificationLabelText(UnlockMissedEvents::NotifyEmail) == "89");
    QVERIFY (notificationIconName(UnlockMissedEvents::NotifyEmail) == 
            "icon-m-content-email");
}

void
Ut_UnlockNotifications::testUpdateContentsWithManyMessages ()
{
    /*
     * Initializing...
     */
    m_SignalSink.m_NeedToShow = false;
    m_SignalSink.m_NeedToShowCame = false;
    gUnlockMissedEventsStub->stubSetReturnValue ("getCount", 77);
    gUnlockMissedEventsStub->stubSetReturnValue (
            "getLastSubject", 
            QString("lastChatMessage"));
    gUnlockMissedEventsStub->stubSetReturnValue (
            "getLastType", 
            UnlockMissedEvents::NotifyMessage);

    m_Subject->updateContents ();
    
    QVERIFY (!m_SignalSink.m_NeedToShow);
    QVERIFY (m_SignalSink.m_NeedToShowCame);
    QVERIFY (notificationLastSubject() == "qtn_scrl_chat");
    QVERIFY (notificationLabelText(UnlockMissedEvents::NotifyMessage) == "77");
    QVERIFY (notificationIconName(UnlockMissedEvents::NotifyMessage) == 
            "icon-m-content-chat");
   
    /*
     * One more time with a different number of events.
     */
    gUnlockMissedEventsStub->stubSetReturnValue ("getCount", 88);
    m_Subject->updateContents ();
    
    QVERIFY (!m_SignalSink.m_NeedToShow);
    QVERIFY (m_SignalSink.m_NeedToShowCame);
    QVERIFY (notificationLastSubject() == "qtn_scrl_chat");
    QVERIFY (notificationLabelText(UnlockMissedEvents::NotifyMessage) == "88");
    QVERIFY (notificationIconName(UnlockMissedEvents::NotifyMessage) == 
            "icon-m-content-chat");
}


/*!
 * \returns the Nth label text in the UnlockNotifications object.
 */
QString
Ut_UnlockNotifications::notificationLabelText (
        int nth)
{
    return m_Subject->m_labels[nth]->text();
}

/*!
 * \returns the Nth label text in the UnlockNotifications object.
 */
QString
Ut_UnlockNotifications::notificationIconName (
        int nth)
{
    return m_Subject->m_icons[nth]->image();
}

/*!
 * \returns The last subject text from the UnlockNotifications object.
 */
QString
Ut_UnlockNotifications::notificationLastSubject ()
{
    return m_Subject->m_last_subject->text();
}

QTEST_APPLESS_MAIN(Ut_UnlockNotifications)

