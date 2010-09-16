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

#include <QByteArray>
#include "qdbusargument_fake.h"
#include "ut_notificationgroup.h"
#include "notificationparameters.h"
#include "notificationgroup.h"

void Ut_NotificationGroup::initTestCase()
{
}

void Ut_NotificationGroup::cleanupTestCase()
{
}

void Ut_NotificationGroup::init()
{
}

void Ut_NotificationGroup::cleanup()
{
}

void Ut_NotificationGroup::testWhenUpdatingNotificationParametersToNotificationGroupThenTheParametersGetUpdated()
{
    NotificationParameters parameters;
    parameters.add("parameter", "initialParameterValue");
    NotificationGroup notificationGroup(0, 0, parameters);

    QCOMPARE(notificationGroup.parameters().value("parameter").toString(), QString("initialParameterValue"));

    NotificationParameters updateParameters;
    updateParameters.add("parameter", "updatedParameterValue");
    notificationGroup.updateParameters(updateParameters);

    QCOMPARE(notificationGroup.parameters().value("parameter").toString(), QString("updatedParameterValue"));
}

void Ut_NotificationGroup::testWhenUpdatingNotificationParametersToNotificationGroupThenTheOldParametersRemain()
{
    NotificationParameters parameters;
    parameters.add("initialParameter", "initialParameterValue");
    NotificationGroup notificationGroup(0, 0, parameters);

    QCOMPARE(notificationGroup.parameters().value("initialParameter").toString(), QString("initialParameterValue"));

    NotificationParameters updateParameters;
    updateParameters.add("updateParameter", "updateParameterValue");
    notificationGroup.updateParameters(updateParameters);

    QCOMPARE(notificationGroup.parameters().value("initialParameter").toString(), QString("initialParameterValue"));
    QCOMPARE(notificationGroup.parameters().value("updateParameter").toString(), QString("updateParameterValue"));
}

void Ut_NotificationGroup::testSerializationAndDeserialization()
{
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");

    NotificationGroup ng1(20, 1234, parameters0);
    NotificationGroup ng2;

    // Transfer the state of notification group ng1 to ng2
    // through a byte array
    QByteArray ba;
    QDataStream stream(&ba, QIODevice::ReadWrite);
    stream << ng1;
    stream.device()->seek(0);
    stream >> ng2;

    QCOMPARE(ng2.groupId(), uint(20));
    QCOMPARE(ng2.userId(), uint(1234));
    QCOMPARE(ng2.parameters().value("imageId").toString(), QString("icon0"));
}

void Ut_NotificationGroup::testDBusSerialization()
{
    NotificationParameters parameters0;
    parameters0.add("eventType", "type");
    parameters0.add("imageId", "icon");
    parameters0.add("summary", "summary");
    parameters0.add("body", "body");
    parameters0.add("action", "action");
    parameters0.add("count",  456);

    NotificationGroup n1(20, 678, parameters0);
    NotificationGroup n2;

    // Transfer a NotificationGroup from n1 to n2 by serializing it
    // to a QDBusArgument and unserializing it
    QDBusArgument arg;
    arg << n1;
    arg >> n2;

    QCOMPARE(n1.groupId(), n2.groupId());
    QCOMPARE(n1.userId(), n2.userId());
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

}


QTEST_MAIN(Ut_NotificationGroup)
