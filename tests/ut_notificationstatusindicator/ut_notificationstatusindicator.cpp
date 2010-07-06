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

#include "ut_notificationstatusindicator.h"
#include "notification_stub.h"
#include "statusindicator.h"
#include <mapplication.h>
#include "sysuid_stub.h"
#include "notificationsink_stub.h"
#include "notifiernotificationsink_stub.h"
#include "mcompositornotificationsink_stub.h"
#include "widgetnotificationsink_stub.h"
#include "inputmethodstatusindicatoradaptor_stub.h"
#include "notificationmanager_stub.h"
#include "notificationgroup_stub.h"
#include "eventtypestore_stub.h"

void Ut_NotificationStatusIndicator::init()
{
    m_subject = new NotificationStatusIndicator;
}

void Ut_NotificationStatusIndicator::cleanup()
{
    gNotifierNotificationSinkStub->stubReset();

    delete m_subject;
}

void Ut_NotificationStatusIndicator::initTestCase()
{
    // MApplications must be created manually these days due to theme system changes
    static int argc = 1;
    static char *app_name = (char *)"./Ut_NotificationStatusIndicator";
    app = new MApplication(argc, &app_name);
}

void Ut_NotificationStatusIndicator::cleanupTestCase()
{
    // Destroy the MApplication
    delete app;
}

void Ut_NotificationStatusIndicator::testSetActive()
{
    connect(this, SIGNAL(notifierSinkActive(bool)), m_subject, SLOT(setActive(bool)));

    emit notifierSinkActive(true);
    QVERIFY(m_subject->objectName().indexOf("On") >= 0);

    emit notifierSinkActive(false);
     QVERIFY(m_subject->objectName().indexOf("On") < 0);
}

void Ut_NotificationStatusIndicator::testMenuVisibilityChange()
{
    // verify that we don't do anything when visibility is false
    m_subject->statusIndicatorMenuVisibilityChange(false);
    QCOMPARE(gNotifierNotificationSinkStub->stubCallCount("clearSink"), 0);
    QCOMPARE(gNotifierNotificationSinkStub->stubCallCount("disableNotificationAdditions"), 1);
    QCOMPARE(gNotifierNotificationSinkStub->stubLastCallTo("disableNotificationAdditions").parameter<bool>(0), false);

    // verify that sink is cleared when status menu comes visible
    m_subject->statusIndicatorMenuVisibilityChange(true);
    QCOMPARE(gNotifierNotificationSinkStub->stubCallCount("clearSink"), 1);
    QCOMPARE(gNotifierNotificationSinkStub->stubCallCount("disableNotificationAdditions"), 2);
    QCOMPARE(gNotifierNotificationSinkStub->stubLastCallTo("disableNotificationAdditions").parameter<bool>(0), true);
}

QTEST_APPLESS_MAIN(Ut_NotificationStatusIndicator)
