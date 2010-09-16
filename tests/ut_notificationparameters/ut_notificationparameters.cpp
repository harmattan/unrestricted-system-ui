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
#include "ut_notificationparameters.h"
#include "notificationparameters.h"
#include "notificationparameter.h"
#include "genericnotificationparameterfactory.h"
#include "qdbusargument_fake.h"

static NotificationParameter createParameter(QString key, int value) {
    return NotificationParameter(key, QVariant(value));
}

static NotificationParameter createParameter(QString key, const QString &value) {
    return NotificationParameter(key, QVariant(value));
}

void Ut_NotificationParameters::initTestCase()
{
}

void Ut_NotificationParameters::cleanupTestCase()
{
}

void Ut_NotificationParameters::init()
{
}

void Ut_NotificationParameters::cleanup()
{
}

void Ut_NotificationParameters::testKeyValueAPI()
{
    NotificationParameters params;

    QVariant v1(5);
    QVariant v2("Test");
    params.add("test1", v1);
    params.add("test2", v2);
    QCOMPARE(params.value("test1"), v1);
    QCOMPARE(params.value("test2"), v2);
    QCOMPARE(params.value("test3").isNull(), true);
}

void Ut_NotificationParameters::testParameterAPI()
{
    NotificationParameters params;

    params.add(createParameter("test1", 5));
    params.add(createParameter("test2", "Test"));

    QCOMPARE(params.value("test1"), QVariant(5));
    QCOMPARE(params.value("test2"), QVariant("Test"));
    QCOMPARE(params.value("test3").isNull(), true);
}

void Ut_NotificationParameters::testWhenUpdatingParametersThenTheParametersGetUpdated()
{
    NotificationParameters params;
    params.add(GenericNotificationParameterFactory::unseenKey(), "true");

    QCOMPARE(params.value(GenericNotificationParameterFactory::unseenKey()).toBool(), true);

    NotificationParameters updated;
    updated.add(GenericNotificationParameterFactory::unseenKey(), "false");
    params.update(updated);

    QCOMPARE(params.value(GenericNotificationParameterFactory::unseenKey()).toBool(), false);
}

void Ut_NotificationParameters::testWhenUpdatingParametersThenTheExistingParametersRemain()
{
    NotificationParameters params;
    params.add(GenericNotificationParameterFactory::unseenKey(), "true");

    QCOMPARE(params.value(GenericNotificationParameterFactory::unseenKey()).toBool(), true);

    NotificationParameters updated;
    updated.add("updatedKey", "updatedValue");
    params.update(updated);

    QCOMPARE(params.value(GenericNotificationParameterFactory::unseenKey()).toBool(), true);
    QCOMPARE(params.value("updatedKey").toString(), QString("updatedValue"));
}

void Ut_NotificationParameters::testSerialization()
{
    NotificationParameters params1;
    NotificationParameters params2;

    params1.add(createParameter("test1", 5));
    params1.add(createParameter("test2", "Test"));

    // Transfer the parameters from params1 to params2
    // through a byte array
    QByteArray ba;
    QDataStream stream(&ba, QIODevice::ReadWrite);
    stream << params1;
    stream.device()->seek(0);
    stream >> params2;

    QCOMPARE(params2.count(), params1.count());
    QCOMPARE(params2.value("test1"), QVariant(5));
    QCOMPARE(params2.value("test2"), QVariant("Test"));
    QCOMPARE(params2.value("test3").isNull(), true);
}

void Ut_NotificationParameters::testDBusSerialization()
{
    NotificationParameters params1;
    NotificationParameters params2;

    params1.add(createParameter("test1", 5));
    params1.add(createParameter("test2", "Test"));

    // Transfer the parameters from params1 to params2
    // serializing to a QDBusArgument
    QDBusArgument arg;
    arg << params1;
    arg >> params2;

    QCOMPARE(params2.count(), params1.count());
    QCOMPARE(params2.value("test1"), QVariant(5));
    QCOMPARE(params2.value("test2"), QVariant("Test"));
    QCOMPARE(params2.value("test3").isNull(), true);
}

QTEST_MAIN(Ut_NotificationParameters)
