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

#ifndef EVENTTYPESTORE_H_
#define EVENTTYPESTORE_H_

#include <QString>
#include <QMap>
#include <QSet>
#include <QSettings>
#include <QSharedPointer>
#include <QStringList>
#include <QFileSystemWatcher>


/*!
 * A class that represents a notification event type store. The event type
 * store will store all the event types stored in the given path.
 *
 * The event type store will limit the number of configuration
 * files it will read. The rationale is to constrain memory usage and startup
 * time in case a huge number of event types are defined by a misbehaving
 * package.
 *
 */
class EventTypeStore : public QObject
{
    Q_OBJECT

public:
    /*!
     * Creates a notification event types store to store the notifications event
     * types.
     *
     * \param eventTypesPath The path where the different event types are defined
     * \param maxStoredEventTypes The maximum number of event types to keep in memory
     */
    explicit EventTypeStore(const QString &eventTypesPath, uint maxStoredEventTypes = 100);

    /*!
      * Returns the QSettings object for a given event type.
      *
      * \param eventType The event type whose QSettings object is desired.
      */
    const QSettings *settingsForEventType(const QString &eventType) const;

private slots:
    /*!
     * Updates the list of available event type files
     */
    void updateEventTypeFileList();

    /*!
     * Updates the event type representing the given file
     *
     * Only updates the event type if file is modified. Removing an event type file is handled
     * by updateEventTypeFileList() slot
     */
    void updateEventTypeFile(const QString &path);

signals:
    /*!
     * A signal sent whenever an event type has been modified
     * \param eventType the event type that was modified
     */
    void eventTypeModified(const QString &eventType);

    /*!
     * A signal sent whenever an event type has been uninstalled
     * \param eventType the event type that was removed
     */
    void eventTypeUninstalled(const QString &eventType);

private:
    //! The file extension for the event configuration files
    static const QString FILE_EXTENSION;

    //! The maximum size of the event configuration file
    static const uint FILE_MAX_SIZE;

    //! The path where the event type configuration files are stored
    QString eventTypesPath;

    //! The maximum number of event types to keep in memory
    uint maxStoredEventTypes;

    //! Map for storing event types and corresponding QSettings object
    mutable QMap<QString, QSharedPointer<QSettings> > eventTypesMap;

    //! List for keeping track of which event types have been most recently used
    mutable QStringList eventTypeUsage;

    //! Load the data into our internal map
    void loadSettings(const QString &eventType);

    //! File system watcher to notice changes in installed event types
    QFileSystemWatcher eventTypePathWatcher;

    //! List of available event type files
    QSet<QString> eventTypeFiles;
};

#endif /* EVENTTYPESTORE_H_ */
