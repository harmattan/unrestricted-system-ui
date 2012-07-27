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

#include <QtTest/QtTest>
#include "ut_statusareaview.h"
#include "statusareaview.h"
#include "statusarea.h"
#include "clock_stub.h"
#include "statusindicator_stub.h"
#include "contextframeworkitem_stub.h"
#include "contextframeworkcontext_stub.h"
#include "alarmstatusindicator_stub.h"
#include "batterystatusindicator_stub.h"
#include "batterypercentagestatusindicator_stub.h"
#include "phonenetworksignalstrengthstatusindicator_stub.h"
#include "phonenetworktypestatusindicator_stub.h"
#include "shortdistancenetworkstatusindicator_stub.h"
#include "gpsstatusindicator_stub.h"
#include "presencestatusindicator_stub.h"
#include "statusindicator_stub.h"
#include "phonenetworkstatusindicator_stub.h"
#include "inputmethodstatusindicator_stub.h"
#include "transferstatusindicator_stub.h"
#include "callstatusindicator_stub.h"
#include "tetheringstatusindicator_stub.h"
#include "profilestatusindicator_stub.h"
#include "callforwardingstatusindicator_stub.h"
#include "dlnastatusindicator_stub.h"
#include "notificationarea_stub.h"
#include "notificationstatusindicator_stub.h"
#include "notificationsink_stub.h"
#include "notificationstatusindicatorsink_stub.h"
#include "inputmethodstatusindicatoradaptor_stub.h"
#include "sysuid_stub.h"
#include "x11wrapper_modified_stub.h"
#include <MApplication>

// Called before the first testfunction is executed
void Ut_StatusAreaView::initTestCase()
{
    int argc = 1;
    static char *app_name = (char *)"./ut_lockscreenstatusareaview";
    app = new MApplication(argc, &app_name);
    notificationStatusIndicatorSink = new NotificationStatusIndicatorSink;
    gSysuidStub->stubSetReturnValue("notificationStatusIndicatorSink", notificationStatusIndicatorSink);
}

// Called after the last testfunction was executed
void Ut_StatusAreaView::cleanupTestCase()
{
    delete notificationStatusIndicatorSink;
    delete app;
}

// Called before each testfunction is executed
void Ut_StatusAreaView::init()
{
    gContextFrameworkContextStub->stubSetReturnValue("createContextItem", static_cast<ContextItem *>(new ContextFrameworkItem(QString())));
    statusArea = new StatusArea;
    m_subject = new StatusAreaView(statusArea);
    statusArea->setView(m_subject);
}

// Called after every testfunction
void Ut_StatusAreaView::cleanup()
{
    delete statusArea;
    statusArea = NULL;
    gContextFrameworkContextStub->stubReset();
    gContextFrameworkItemStub->stubReset();
}

void Ut_StatusAreaView::testSignalConnections()
{
    QVERIFY(disconnect(&Sysuid::instance()->notificationStatusIndicatorSink(), SIGNAL(iconIdChanged(QString)), m_subject->landscapeNotificationIndicator, SLOT(setIconID(QString))));
    QVERIFY(disconnect(&Sysuid::instance()->notificationStatusIndicatorSink(), SIGNAL(iconIdChanged(QString)), m_subject->portraitNotificationIndicator, SLOT(setIconID(QString))));
}

void Ut_StatusAreaView::testObjectNames()
{
    QCOMPARE(m_subject->landscapePhoneNetworkIndicator->objectName(), QString("PhoneNetworkStatusIndicatorLandscape"));
    QCOMPARE(m_subject->portraitPhoneNetworkIndicator->objectName(), QString("PhoneNetworkStatusIndicatorPortrait"));
}

void Ut_StatusAreaView::testStyleNamesFollowCallState()
{
    QVERIFY(disconnect(m_subject->callContextItem, SIGNAL(contentsChanged()), m_subject, SLOT(setStyleNames())));
    QCOMPARE(gContextFrameworkItemStub->stubCallCount("subscribe"), 1);

    QCOMPARE(m_subject->landscapeWidget->styleName(), QString("StatusBarLandscapeWidget"));
    QCOMPARE(m_subject->portraitWidget->styleName(), QString("StatusBarPortraitWidget"));

    gContextFrameworkItemStub->stubSetReturnValue("value", QVariant("active"));
    m_subject->setStyleNames();

    QCOMPARE(m_subject->landscapeWidget->styleName(), QString("StatusBarLandscapeWidgetCall"));
    QCOMPARE(m_subject->portraitWidget->styleName(), QString("StatusBarPortraitWidgetCall"));
}

QTEST_APPLESS_MAIN(Ut_StatusAreaView)
