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

#include <MApplication>
#include <QByteArray>
#include "qdbusargument_fake.h"
#include "ut_notification.h"
#include "notificationparameters.h"
#include "notification.h"

static QDateTime timestamp = QDateTime::fromString("M1d1y9911:11:11", "'M'M'd'd'y'yyhh:mm:ss"); //Fri Jan 1 11:11:11 1999

void Ut_Notification::initTestCase()
{
}

void Ut_Notification::cleanupTestCase()
{
}

void Ut_Notification::init()
{
}

void Ut_Notification::cleanup()
{
}

void Ut_Notification::testWhenUpdatingNotificationParametersToNotificationThenTheParametersGetUpdated()
{
    NotificationParameters parameters;
    parameters.add("parameter", "initialParameterValue");
    Notification notification(1234, 20, 678, parameters, Notification::ApplicationEvent, 2345);

    QCOMPARE(notification.parameters().value("parameter").toString(), QString("initialParameterValue"));

    NotificationParameters updateParameters;
    updateParameters.add("parameter", "updatedParameterValue");
    notification.updateParameters(updateParameters);

    QCOMPARE(notification.parameters().value("parameter").toString(), QString("updatedParameterValue"));
}

void Ut_Notification::testWhenUpdatingNotificationParametersToNotificationThenTheOldParametersRemain()
{
    NotificationParameters parameters;
    parameters.add("initialParameter", "initialParameterValue");
    Notification notification(1234, 20, 678, parameters, Notification::ApplicationEvent, 2345);

    QCOMPARE(notification.parameters().value("initialParameter").toString(), QString("initialParameterValue"));

    NotificationParameters updateParameters;
    updateParameters.add("updateParameter", "updateParameterValue");
    notification.updateParameters(updateParameters);

    QCOMPARE(notification.parameters().value("initialParameter").toString(), QString("initialParameterValue"));
    QCOMPARE(notification.parameters().value("updateParameter").toString(), QString("updateParameterValue"));
}

void Ut_Notification::testSerializationAndDeserialization()
{
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");
    parameters0.add("timestamp", timestamp);

    Notification n1(1234, 20, 678, parameters0, Notification::ApplicationEvent, 2345);
    Notification n2;

    // Transfer the state of notification n1 to n2
    // through a byte array
    QByteArray ba;
    QDataStream stream(&ba, QIODevice::ReadWrite);
    stream << n1;
    stream.device()->seek(0);
    stream >> n2;

    QCOMPARE(n2.notificationId(), uint(1234));
    QCOMPARE(n2.groupId(), uint(20));
    QCOMPARE(n2.userId(), uint(678));
    QCOMPARE(n2.type(), Notification::ApplicationEvent);
    QCOMPARE(n2.timeout(), int(2345));
    QCOMPARE(n2.parameters().value("imageId").toString(), QString("icon0"));
    QCOMPARE(n2.parameters().value("timestamp").toDateTime(), timestamp);
}


void Ut_Notification::testDBusSerialization()
{
    NotificationParameters parameters0;
    parameters0.add("eventType", "type");
    parameters0.add("imageId", "icon");
    parameters0.add("summary", "summary");
    parameters0.add("body", "body");
    parameters0.add("action", "action");
    parameters0.add("count",  456);
    parameters0.add("timestamp", timestamp);

    Notification n1(1234, 20, 678, parameters0, Notification::ApplicationEvent, 0);
    Notification n2;

    // Transfer a Notification from n1 to n2 by serializing it
    // to a QDBusArgument and unserializing it
    QDBusArgument arg;
    arg << n1;
    arg >> n2;

    QCOMPARE(n1.notificationId(), n2.notificationId());
    QCOMPARE(n1.userId(), n2.userId());
    QCOMPARE(n1.groupId(), n2.groupId());
    QCOMPARE(n1.type(), n2.type());
    QCOMPARE(n1.timeout(), n2.timeout());
    QCOMPARE(n1.parameters().count(), n2.parameters().count());
    QCOMPARE(n1.parameters().value("eventType"),
             n2.parameters().value("eventType"));
    QCOMPARE(n1.parameters().value("imageId"),
             n2.parameters().value("imageId"));
    QCOMPARE(n1.parameters().value("summary"),
             n2.parameters().value("summary"));
    QCOMPARE(n1.parameters().value("body"),
             n2.parameters().value("body"));
    QCOMPARE(n1.parameters().value("action"),
             n2.parameters().value("action"));
    QCOMPARE(n1.parameters().value("count"),
             n2.parameters().value("count"));
    QCOMPARE(n1.parameters().value("timestamp"),
             n2.parameters().value("timestamp"));
}

QTEST_MAIN(Ut_Notification)
