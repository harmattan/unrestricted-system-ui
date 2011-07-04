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

#include <QtTest/QtTest>
#include <QDBusConnection>
#include <MNotification>
#include "ut_diskspacenotifier.h"
#include "diskspacenotifier.h"

QString qDBusConnectionConnectService;
QString qDBusConnectionConnectPath;
QString qDBusConnectionConnectInterface;
QString qDBusConnectionConnectName;
QObject *qDBusConnectionConnectReceiver = NULL;
QString qDBusConnectionConnectSlot;
bool QDBusConnection::connect(const QString &service, const QString &path, const QString &interface, const QString &name, QObject *receiver, const char *slot)
{
    qDBusConnectionConnectService = service;
    qDBusConnectionConnectPath = path;
    qDBusConnectionConnectInterface = interface;
    qDBusConnectionConnectName = name;
    qDBusConnectionConnectReceiver = receiver;
    qDBusConnectionConnectSlot = QString(slot);
    return true;
}

int mNotificationsPublished = 0;
bool MNotification::publish()
{
    mNotificationsPublished++;
    return true;
}

void Ut_DiskSpaceNotifier::initTestCase()
{
}

void Ut_DiskSpaceNotifier::cleanupTestCase()
{
}

void Ut_DiskSpaceNotifier::init()
{
    m_subject = new DiskSpaceNotifier();
}

void Ut_DiskSpaceNotifier::cleanup()
{
    delete m_subject;
    qDBusConnectionConnectService.clear();
    qDBusConnectionConnectPath.clear();
    qDBusConnectionConnectInterface.clear();
    qDBusConnectionConnectName.clear();
    qDBusConnectionConnectReceiver = NULL;
    qDBusConnectionConnectSlot.clear();
    mNotificationsPublished = 0;
}

void Ut_DiskSpaceNotifier::testSystemBusConnection()
{
    QCOMPARE(qDBusConnectionConnectService, QString());
    QCOMPARE(qDBusConnectionConnectPath, QString("/com/nokia/diskmonitor/signal"));
    QCOMPARE(qDBusConnectionConnectInterface, QString("com.nokia.diskmonitor.signal"));
    QCOMPARE(qDBusConnectionConnectName, QString("disk_space_change_ind"));
    QCOMPARE(qDBusConnectionConnectReceiver, m_subject);
    QCOMPARE(qDBusConnectionConnectSlot, QString(SLOT(handleDiskSpaceChange(QString, int))));
}

void Ut_DiskSpaceNotifier::testNotifications_data()
{
    QTest::addColumn<QString>("diskSpaceChangePath1");
    QTest::addColumn<int>("diskSpaceChangePercentage1");
    QTest::addColumn<QString>("diskSpaceChangePath2");
    QTest::addColumn<int>("diskSpaceChangePercentage2");
    QTest::addColumn<int>("notificationsPublished");

    QTest::newRow("Disk space of / reached threshold but not 100%") << "/" << 90 << "/" << 99 << 1;
    QTest::newRow("Disk space of / reached threshold and then 100%") << "/" << 90 << "/" << 100 << 2;
    QTest::newRow("Disk space of / reached 100% twice") << "/" << 100 << "/" << 100 << 1;
    QTest::newRow("Disk space of / and /home reached threshold") << "/" << 90 << "/home" << 90 << 2;
    QTest::newRow("Disk space of /home and /home/user/MyDocs reached 100%") << "/home" << 100 << "/home/user/MyDocs" << 100 << 2;
}

void Ut_DiskSpaceNotifier::testNotifications()
{
    QFETCH(QString, diskSpaceChangePath1);
    QFETCH(int, diskSpaceChangePercentage1);
    QFETCH(QString, diskSpaceChangePath2);
    QFETCH(int, diskSpaceChangePercentage2);
    QFETCH(int, notificationsPublished);

    m_subject->handleDiskSpaceChange(diskSpaceChangePath1, diskSpaceChangePercentage1);
    m_subject->handleDiskSpaceChange(diskSpaceChangePath2, diskSpaceChangePercentage2);

    QCOMPARE(mNotificationsPublished, notificationsPublished);
}

void Ut_DiskSpaceNotifier::testDestruction()
{
    m_subject->handleDiskSpaceChange("/", 100);

    delete m_subject;
    m_subject = NULL;
}

QTEST_APPLESS_MAIN(Ut_DiskSpaceNotifier)
