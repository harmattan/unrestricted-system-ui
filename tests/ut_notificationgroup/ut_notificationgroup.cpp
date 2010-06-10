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
#include <QDBusArgument>
#include "ut_notificationgroup.h"
#include "notificationparameters.h"
#include "notificationgroup.h"


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

void Ut_NotificationGroup::initTestCase()
{
}

void Ut_NotificationGroup::cleanupTestCase()
{
}

void Ut_NotificationGroup::init()
{
    qdbusUIntArgs.clear();
    qdbusStringArgs.clear();
}

void Ut_NotificationGroup::cleanup()
{
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
    QDBusArgument arg;

    NotificationParameters parameters0;
    parameters0.add("imageId", "icon");
    parameters0.add("body", "<p><b>summary</b></p><p>body</p>");
    parameters0.add("action", "action");
    parameters0.add("count",  456);

    NotificationGroup n1(20, 678, parameters0);

    arg << n1;

    QCOMPARE(qdbusUIntArgs.count(), 2);
    QCOMPARE(qdbusUIntArgs.at(0), uint(20));
    QCOMPARE(qdbusUIntArgs.at(1), uint(456));
    QCOMPARE(qdbusStringArgs.count(), 4);
    QCOMPARE(qdbusStringArgs.at(0), QString("summary"));
    QCOMPARE(qdbusStringArgs.at(1), QString("body"));
    QCOMPARE(qdbusStringArgs.at(2), QString("icon"));
    QCOMPARE(qdbusStringArgs.at(3), QString("action"));
}


QTEST_MAIN(Ut_NotificationGroup)
