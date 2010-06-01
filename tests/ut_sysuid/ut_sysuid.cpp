/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of system ui.
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

#include "ut_sysuid.h"

#include <QDBusConnection>
#include <MApplication>
#include "mcompositornotificationsink.h"
#include "ngfnotificationsink.h"
#include "testcontextitem.h"
#include "contextframeworkcontext.h"
#include "sysuid.h"
#include "widgetnotificationsink_stub.h"
#include "eventtypestore_stub.h"
#include "notificationmanager_stub.h"
#include "statusarearenderer_stub.h"
#include "unlockwidgets_stub.h"
#include "sysuidrequest_stub.h"
#include "statusindicatormenuwindow_stub.h"
#include "mcompositornotificationsink_stub.h"
#include "unlocknotificationsinkstub.h"
#include "pannedwidgetcontroller_stub.h"

maemosec::storage::~storage()
{
}

Notification::~Notification()
{
}

Notification::Notification()
{
}

NotificationGroup::~NotificationGroup()
{
}

NotificationParameters::NotificationParameters()
{
}

NotificationParameters::~NotificationParameters()
{
}

// NotificationSink stubs (used by MCompositorNotificationSink)
NotificationSink::NotificationSink()
{
}

void NotificationSink::addGroup(uint, const NotificationParameters &)
{
}

void NotificationSink::removeGroup(uint)
{
}

bool Ut_SysuidCompositorNotificationState = false;
bool Ut_SysuidFeedbackNotificationState = false;

void NotificationSink::setApplicationEventsEnabled(bool enabled)
{
    if(dynamic_cast<MCompositorNotificationSink*>(this) != NULL) {
        Ut_SysuidCompositorNotificationState = enabled;
    } else if(dynamic_cast<NGFNotificationSink*>(this) != NULL) {
        Ut_SysuidFeedbackNotificationState = enabled;
    }
}

// NGFNotificationSink stubs (used by Sysuid)
NGFNotificationSink *mNGFNotificationSink = 0;
NGFNotificationSink::NGFNotificationSink()
{
    mNGFNotificationSink = this;
}

NGFNotificationSink::~NGFNotificationSink()
{
}

void NGFNotificationSink::addNotification(const Notification &)
{
}

void NGFNotificationSink::removeNotification(uint)
{
}

// QDBusConnection stubs (used by Sysuid)
bool QDBusConnection::registerService(QString const &)
{
    return true;
}

bool QDBusConnection::registerObject(QString const &, QObject *, QFlags<QDBusConnection::RegisterOption>)
{
    return true;
}

// ContextItem & ContextFrameworkItem stubs
TestContextItem *testContextItem;
ContextItem *ContextFrameworkContext::createContextItem(const QString&)
{
    testContextItem = new TestContextItem;
    return testContextItem;
}

ContextFrameworkItem::ContextFrameworkItem(const QString &key)
    : property(key)
{
    connect(&property, SIGNAL(valueChanged()), this, SIGNAL(contentsChanged()));
}

QVariant ContextFrameworkItem::value() const
{
    return property.value();
}

void Ut_Sysuid::initTestCase()
{
}

void Ut_Sysuid::cleanupTestCase()
{
}

void Ut_Sysuid::init()
{
    static char *args[] = {(char *) "./ut_sysuid"};
    static int argc = sizeof(args) / sizeof(char *);

    app = new MApplication(argc, args);
    sysuid = new Sysuid(NULL);
    Ut_SysuidCompositorNotificationState = false;
    Ut_SysuidFeedbackNotificationState = false;
}

void Ut_Sysuid::cleanup()
{
    delete sysuid;
    delete app;
}

void Ut_Sysuid::testUseMode()
{
    testContextItem->setValue("");
    QVERIFY(Ut_SysuidCompositorNotificationState);
    QVERIFY(Ut_SysuidFeedbackNotificationState);
    testContextItem->setValue("recording");
    QVERIFY(!Ut_SysuidCompositorNotificationState);
    QVERIFY(!Ut_SysuidFeedbackNotificationState);
}

QTEST_APPLESS_MAIN(Ut_Sysuid)
