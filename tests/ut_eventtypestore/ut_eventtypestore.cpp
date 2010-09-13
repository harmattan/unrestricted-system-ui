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
#include <QObject>
#include "ut_eventtypestore.h"

// List of event type files
QStringList eventTypeFilesList;
// Map of event type and map of its settings
QMap<QString, QMap<QString, QString> > eventTypeSettingsMap;
//Size of the event configuration file
uint eventTypeFileSize;

// QFileSystemWatcher stubs
void QFileSystemWatcher::addPath(const QString &)
{
}

// QFileInfo stubs
bool QFileInfo::exists() const
{
    return eventTypeFilesList.contains(fileName());
}

qint64 QFileInfo::size() const
{
    return eventTypeFileSize;
}

// QDir stubs
bool QDir::exists() const
{
    return true;
}

QStringList QDir::entryList(const QStringList &, Filters, SortFlags) const
{
    return eventTypeFilesList;
}

// Stubs of QSettings methods
QStringList QSettings::allKeys() const
{
    return QStringList(eventTypeSettingsMap.value(QFileInfo(fileName()).baseName()).keys());
}

bool QSettings::contains(const QString &key) const
{
    return eventTypeSettingsMap.value(QFileInfo(fileName()).baseName()).contains(key);
}

QVariant QSettings::value(const QString &key, const QVariant &defaultValue) const
{
    if (eventTypeSettingsMap.value(QFileInfo(fileName()).baseName()).contains(key)) {
        return eventTypeSettingsMap.value(QFileInfo(fileName()).baseName()).value(key);
    } else {
        return defaultValue;
    }
}

void Ut_EventTypeStore::init()
{
    eventTypeFilesList.clear();
    eventTypeSettingsMap.clear();
    eventTypeFileSize = 100;
}

void Ut_EventTypeStore::cleanup()
{
    delete m_subject;
    m_subject = NULL;
}

void Ut_EventTypeStore::testEventTypeSettingsValues()
{
    // Create three event type files
    eventTypeFilesList.append("smsEventType.conf");
    eventTypeFilesList.append("emailEventType.conf");
    eventTypeFilesList.append("chatEventType.conf");

    // Create settings map for storing the values for each event type and insert keys/values
    QMap<QString, QString> smsSettingsMap;
    QMap<QString, QString> emailSettingsMap;
    QMap<QString, QString> chatSettingsMap;
    smsSettingsMap.insert("iconId", "sms-icon");
    smsSettingsMap.insert("feedbackId", "sound-file");
    emailSettingsMap.insert("iconId", "email-icon");
    emailSettingsMap.insert("feedbackId", "sound-file-for-email");
    chatSettingsMap.insert("iconId", "chat-icon");
    chatSettingsMap.insert("feedbackId", "sound-file-for-chat");
    eventTypeSettingsMap.insert("smsEventType", smsSettingsMap);
    eventTypeSettingsMap.insert("emailEventType", emailSettingsMap);
    eventTypeSettingsMap.insert("chatEventType", chatSettingsMap);

    // Create a store that can hold up to 2 event types in memory at the same time
    m_subject = new EventTypeStore("/eventtypepath", 2);

    // Verify settings object is null for invalid key
    QCOMPARE(m_subject->eventTypeExists("idontexist"), false);

    // Verify that settings object for each event type contains the provided keys/values
    QCOMPARE(m_subject->eventTypeExists("smsEventType"), true);
    QCOMPARE(m_subject->allKeys("smsEventType").count(), 2);
    QCOMPARE(m_subject->contains("smsEventType", "iconId"), true);
    QCOMPARE(m_subject->value("smsEventType", "iconId"), QString("sms-icon"));
    QCOMPARE(m_subject->contains("smsEventType", "feedbackId"), true);
    QCOMPARE(m_subject->value("smsEventType", "feedbackId"), QString("sound-file"));
    QCOMPARE(m_subject->eventTypeExists("emailEventType"), true);
    QCOMPARE(m_subject->allKeys("emailEventType").count(), 2);
    QCOMPARE(m_subject->contains("emailEventType", "iconId"), true);
    QCOMPARE(m_subject->value("emailEventType", "iconId"), QString("email-icon"));
    QCOMPARE(m_subject->contains("emailEventType", "feedbackId"), true);
    QCOMPARE(m_subject->value("emailEventType", "feedbackId"), QString("sound-file-for-email"));

    // Verify that event types work even if the maximum amount of types stored in memory is reached
    QCOMPARE(m_subject->eventTypeExists("chatEventType"), true);
    QCOMPARE(m_subject->allKeys("chatEventType").count(), 2);
    QCOMPARE(m_subject->contains("chatEventType", "iconId"), true);
    QCOMPARE(m_subject->value("chatEventType", "iconId"), QString("chat-icon"));
    QCOMPARE(m_subject->contains("chatEventType", "feedbackId"), true);
    QCOMPARE(m_subject->value("chatEventType", "feedbackId"), QString("sound-file-for-chat"));
    QCOMPARE(m_subject->eventTypeExists("smsEventType"), true);
    QCOMPARE(m_subject->allKeys("smsEventType").count(), 2);
    QCOMPARE(m_subject->contains("smsEventType", "iconId"), true);
    QCOMPARE(m_subject->value("smsEventType", "iconId"), QString("sms-icon"));
    QCOMPARE(m_subject->contains("smsEventType", "feedbackId"), true);
    QCOMPARE(m_subject->value("smsEventType", "feedbackId"), QString("sound-file"));
}
void Ut_EventTypeStore::testEventTypeStoreMaxFileSizeHandling()
{
    // Verify if the event type file is above its maximum size, than its not loaded
    eventTypeFileSize = 90000;
    eventTypeFilesList.append("bigEventType.conf");
    QMap<QString, QString> bigEventTypeSettingsMap;
    bigEventTypeSettingsMap.insert("iconId", "sms-icon");
    bigEventTypeSettingsMap.insert("feedbackId", "sound-file");
    eventTypeSettingsMap.insert("bigEventType", bigEventTypeSettingsMap);

    m_subject = new EventTypeStore("/eventtypepath", 2);
    // Verify that settings object is NULL
    QCOMPARE(m_subject->eventTypeExists("bigEventType"), false);
}

void Ut_EventTypeStore::testEventTypeUninstalling()
{
    // Create existing event type file
    eventTypeFilesList.append("smsEventType.conf");

    m_subject = new EventTypeStore("/eventtypepath");
    QSignalSpy uninstallSpy(m_subject, SIGNAL(eventTypeUninstalled(QString)));
    connect(this, SIGNAL(directoryChanged(QString)), m_subject, SLOT(updateEventTypeFileList()));

    // Add new event type file
    eventTypeFilesList.append("chatEventType.conf");
    emit directoryChanged("/eventtypepath");
    QCOMPARE(uninstallSpy.count(), 0);
    QCOMPARE(m_subject->eventTypeExists("chatEventType"), true);

    // Remove the added event type file
    eventTypeFilesList.removeOne("chatEventType.conf");
    emit directoryChanged("/eventtypepath");
    QCOMPARE(uninstallSpy.count(), 1);
    QCOMPARE(m_subject->eventTypeExists("chatEventType"), false);

    // Remove the existing event type file
    eventTypeFilesList.removeOne("smsEventType.conf");
    emit directoryChanged("/eventtypepath");
    QCOMPARE(uninstallSpy.count(), 2);
    QCOMPARE(m_subject->eventTypeExists("smsEventType"), false);
}

QTEST_APPLESS_MAIN(Ut_EventTypeStore)
