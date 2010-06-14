/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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
#include <MImageWidget>
#include <MLabel>

#include <QtTest/QtTest>
#include <QList>
#include <QVariant>


#define DEBUG
#include "../../src/debug.h"

static int argc = 1;
static char *app_name = (char *) "./ut_unlocknotificationsink";

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
    m_Subject = new UnlockNotifications ();
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
Ut_UnlockNotifications::testUpdateContents ()
{
    if (!m_Subject)
        m_Subject = new UnlockNotifications ();

    m_Subject->updateContents ();
}


QTEST_APPLESS_MAIN(Ut_UnlockNotifications)

