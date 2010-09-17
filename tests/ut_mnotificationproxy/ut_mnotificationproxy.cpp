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
#include <QtDBus/QtDBus>
#include "ut_mnotificationproxy.h"
#include "notification.h"
#include "notificationparameters.h"
#include "notificationgroup.h"
#include "mnotificationproxy.h"
#include "genericnotificationparameterfactory.h"
#include "notificationwidgetparameterfactory.h"

#define EVENT_TYPE GenericNotificationParameterFactory::eventTypeKey()
#define COUNT      GenericNotificationParameterFactory::countKey()

#define SUMMARY    NotificationWidgetParameterFactory::summaryKey()
#define BODY       NotificationWidgetParameterFactory::bodyKey()
#define IMAGE      NotificationWidgetParameterFactory::imageIdKey()
#define ICON       NotificationWidgetParameterFactory::iconIdKey()
#define ACTION     NotificationWidgetParameterFactory::actionKey()

Q_DECLARE_METATYPE(MNotificationProxy);
Q_DECLARE_METATYPE(MNotificationGroupProxy);

void Ut_MNotificationProxy::init()
{
    qRegisterMetaType<MNotificationProxy>("MNotificationProxy");
    qRegisterMetaType<MNotificationGroupProxy>("MNotificationGroupProxy");

    qDBusRegisterMetaType<MNotificationProxy>();
    qDBusRegisterMetaType<MNotificationGroupProxy>();
}

void Ut_MNotificationProxy::cleanup()
{
}

void Ut_MNotificationProxy::initTestCase()
{
    // MApplications must be created manually these days due to theme system changes
    static int argc = 1;
    static char *app_name = (char *)"./ut_statusindicator";
    app = new MApplication(argc, &app_name);
}

void Ut_MNotificationProxy::cleanupTestCase()
{
    // Destroy the MApplication
    delete app;
}

void Ut_MNotificationProxy::testMNotificationProxy()
{
    NotificationParameters param1;
    param1.add(EVENT_TYPE, "type");
    param1.add(COUNT, 5);
    param1.add(SUMMARY, "summary");
    param1.add(BODY, "body");
    param1.add(IMAGE, "image");
    param1.add(ACTION, "action");

    // Create a Notification and an MNotificationProxy out of it
    Notification n1(10, 100, 200, param1, Notification::ApplicationEvent, 0);
    MNotificationProxy proxy(n1);

    // Verify that the MNotificationProxy contains all
    // the correct Notification data
    QCOMPARE(n1.notificationId(), proxy.notificationId);
    QCOMPARE(n1.groupId(), proxy.groupId);
    QCOMPARE(n1.parameters().value(EVENT_TYPE).toString(), proxy.eventType);
    QCOMPARE(n1.parameters().value(SUMMARY).toString(), proxy.summary);
    QCOMPARE(n1.parameters().value(BODY).toString(), proxy.body);
    QCOMPARE(n1.parameters().value(IMAGE).toString(), proxy.imageId);
    QCOMPARE(n1.parameters().value(ACTION).toString(), proxy.action);
    QCOMPARE(n1.parameters().value(COUNT).toUInt(), proxy.count);

    // Check the DBus argument signature
    QString signature =
        QDBusMetaType::typeToSignature(qMetaTypeId<MNotificationProxy>());
    QCOMPARE(QString("(uusssssu)"), signature);
}

void Ut_MNotificationProxy::testMNotificationGroupProxy()
{
    NotificationParameters param1;
    param1.add(EVENT_TYPE, "type");
    param1.add(COUNT, 5);
    param1.add(SUMMARY, "summary");
    param1.add(BODY, "body");
    param1.add(IMAGE, "image");
    param1.add(ACTION, "action");

    // Create a NotificationGroup and an MNotificationGroupProxy out of it
    NotificationGroup g1(100, 200, param1);
    MNotificationGroupProxy proxy(g1);

    // Verify that the MNotificationGroupProxy contains all
    // the correct NotificationGroup data
    QCOMPARE(g1.groupId(), proxy.groupId);
    QCOMPARE(g1.parameters().value(EVENT_TYPE).toString(), proxy.eventType);
    QCOMPARE(g1.parameters().value(SUMMARY).toString(), proxy.summary);
    QCOMPARE(g1.parameters().value(BODY).toString(), proxy.body);
    QCOMPARE(g1.parameters().value(IMAGE).toString(), proxy.imageId);
    QCOMPARE(g1.parameters().value(ACTION).toString(), proxy.action);
    QCOMPARE(g1.parameters().value(COUNT).toUInt(), proxy.count);

    // Check the DBus argument signature
    QString signature =
        QDBusMetaType::typeToSignature(qMetaTypeId<MNotificationGroupProxy>());
    QCOMPARE(QString("(usssssu)"), signature);
}

QTEST_APPLESS_MAIN(Ut_MNotificationProxy)
