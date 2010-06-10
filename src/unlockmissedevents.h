/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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
#ifndef UNLOCKMISSEDEVENTS_H
#define UNLOCKMISSEDEVENTS_H

#include <QObject>
class QString;

class UnlockMissedEvents : public QObject
{
    Q_OBJECT

public:
    // These types of notifications are shown in
    // the unlock-screen window
    typedef enum
    {
        NotifyEmail = 0,
        NotifySms,
        NotifyCall,
        NotifyMessage,
        NotifyOther,
        NotifyLast
    } Types;

    static UnlockMissedEvents& getInstance ();
    // These are used by UnlockNotifications widget
    Types   getLastType ();
    QString getLastSubject (Types atype);
    int     getCount (Types atype);
    // These are used by UnlockNotificationSink
    void    addNotification (Types type, QString subject);
    void    clearAll ();

signals:
    void    updated ();

private:
    // This is a singleton class, so these're private
    UnlockMissedEvents ();
    ~UnlockMissedEvents ();

    static UnlockMissedEvents m_instance;

    QString lastSubjects[NotifyLast];
    int     notificationCounts[NotifyLast];
    Types   lastType;
};

#endif
