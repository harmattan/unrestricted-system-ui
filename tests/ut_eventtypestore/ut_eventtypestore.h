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
#ifndef UT_EVENTTYPESTORE_H
#define UT_EVENTTYPESTORE_H

#include <QObject>
#include <QMetaType>

class EventTypeStore;

Q_DECLARE_METATYPE(EventTypeStore *);

class Ut_EventTypeStore : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void testEventTypeSettingsValues();
    void testEventTypeStoreMaxFileSizeHandling();
    void testEventTypeUninstalling();

private:
    EventTypeStore *m_subject;

signals:
    void directoryChanged(const QString &path);
};

#endif
