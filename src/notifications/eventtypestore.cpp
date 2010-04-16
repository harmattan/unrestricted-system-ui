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

#include "eventtypestore.h"
#include <QFileInfo>
#include <QDir>

const QString EventTypeStore::FILE_EXTENSION = ".conf";
const uint EventTypeStore::FILE_MAX_SIZE = 32768;

EventTypeStore::EventTypeStore(const QString &eventTypesPath, uint maxStoredEventTypes) :
    eventTypesPath(eventTypesPath),
    maxStoredEventTypes(maxStoredEventTypes)
{
    if (!this->eventTypesPath.endsWith('/')) {
        this->eventTypesPath.append('/');
    }

    // Watch for changes in event type files
    eventTypePathWatcher.addPath(this->eventTypesPath);
    connect(&eventTypePathWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(updateEventTypeFileList()));
    connect(&eventTypePathWatcher, SIGNAL(fileChanged(QString)), this, SLOT(updateEventTypeFile(QString)));
    updateEventTypeFileList();
}

void EventTypeStore::updateEventTypeFileList()
{
    QDir eventTypesDir(eventTypesPath);

    if(eventTypesDir.exists()) {
        QStringList filter("*" + FILE_EXTENSION);

        QSet<QString> files = eventTypesDir.entryList(filter, QDir::Files).toSet();
        QSet<QString> removedFiles = eventTypeFiles - files;

        foreach(const QString &removedEventType, removedFiles) {
            QString eventType = QFileInfo(removedEventType).baseName();
            QString eventFilePath = eventTypesPath + removedEventType;
            eventTypePathWatcher.removePath(eventFilePath);
            eventTypesMap.remove(eventType);
            emit eventTypeUninstalled(eventType);
        }

        eventTypeFiles = files;

        // add event type files to watcher
        foreach(QString file, eventTypeFiles){
            QString eventTypeFilePath = eventTypesPath + file;
            if (!eventTypePathWatcher.files().contains(eventTypeFilePath)) {
                eventTypePathWatcher.addPath(eventTypeFilePath);
            }
        }
    }
}

void EventTypeStore::updateEventTypeFile(const QString &path)
{
    QFileInfo fileInfo(path);
    if (fileInfo.exists()) {
       QString eventType = fileInfo.baseName();
       loadSettings(eventType);
       emit eventTypeModified(eventType);
    }
}

const QSettings *EventTypeStore::settingsForEventType(const QString &eventType) const
{
    if (!eventTypesMap.contains(eventType)) {
        // If the event type has not been loaded yet load it
        const_cast<EventTypeStore *>(this)->loadSettings(eventType);
    }

    const QSettings *eventTypeSettings = eventTypesMap.value(eventType).data();

    if (eventTypeSettings != NULL) {
        // If there are settings for the event type mark it as recently used by moving it to the beginning of the usage list
        eventTypeUsage.removeAll(eventType);
        eventTypeUsage.insert(0, eventType);

        // If there are too many event types in memory get rid of the extra ones
        while (eventTypeUsage.count() > (int)maxStoredEventTypes) {
            eventTypesMap.remove(eventTypeUsage.takeLast());
        }
    }

    return eventTypeSettings;
}

void EventTypeStore::loadSettings(const QString &eventType)
{
    QFileInfo file(QString(eventTypesPath).append(eventType).append(FILE_EXTENSION));
    if (file.exists() && file.size() != 0 && file.size() <= FILE_MAX_SIZE) {
        QSharedPointer<QSettings> eventTypeSettings(new QSettings(file.filePath(), QSettings::IniFormat));
        if (eventTypeSettings->status() == QSettings::NoError) {
            eventTypesMap.insert(eventType, eventTypeSettings);
        }
    }
}
