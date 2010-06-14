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
#include <QDBusArgument>
#include "ut_notification.h"
#include "notificationparameters.h"
#include "notification.h"


QList<uint> qdbusUIntArgs;

QDBusArgument &QDBusArgument::operator<<(uint arg)
{
    qdbusUIntArgs.append(arg);

    return *this;
}

QList<QString> qdbusStringArgs;

QDBusArgument &QDBusArgument::operator<<(const QString &arg)
{
    qdbusStringArgs.append(arg);

    return *this;
}

void Ut_Notification::initTestCase()
{
}

void Ut_Notification::cleanupTestCase()
{
}

void Ut_Notification::init()
{
    qdbusUIntArgs.clear();
    qdbusStringArgs.clear();
}

void Ut_Notification::cleanup()
{
}

void Ut_Notification::testSerializationAndDeserialization()
{
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");

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
}


void Ut_Notification::testDBusSerialization()
{
    QDBusArgument arg;

    NotificationParameters parameters0;
    parameters0.add("imageId", "icon");
    parameters0.add("summary", "summary");
    parameters0.add("body", "body");
    parameters0.add("action", "action");
    parameters0.add("count",  456);

    Notification n1(1234, 20, 678, parameters0, Notification::ApplicationEvent, 0);

    arg << n1;

    QCOMPARE(qdbusUIntArgs.count(), 3);
    QCOMPARE(qdbusUIntArgs.at(0), uint(1234));
    QCOMPARE(qdbusUIntArgs.at(1), uint(20));
    QCOMPARE(qdbusUIntArgs.at(2), uint(456));
    QCOMPARE(qdbusStringArgs.count(), 4);
    QCOMPARE(qdbusStringArgs.at(0), QString("summary"));
    QCOMPARE(qdbusStringArgs.at(1), QString("body"));
    QCOMPARE(qdbusStringArgs.at(2), QString("icon"));
    QCOMPARE(qdbusStringArgs.at(3), QString("action"));
}

QTEST_MAIN(Ut_Notification)
