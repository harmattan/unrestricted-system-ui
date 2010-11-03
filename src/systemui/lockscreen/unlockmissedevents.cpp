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
#include "unlockmissedevents.h"
#include <QString>

UnlockMissedEvents UnlockMissedEvents::m_instance;

UnlockMissedEvents::UnlockMissedEvents () : QObject (),
    lastType (NotifyLast)
{
    clearAll ();
}

UnlockMissedEvents::~UnlockMissedEvents ()
{

}

UnlockMissedEvents&
UnlockMissedEvents::getInstance ()
{
    return m_instance;
}

UnlockMissedEvents::Types
UnlockMissedEvents::getLastType ()
{
    return lastType;
}

QString
UnlockMissedEvents::getLastSubject (Types atype)
{
    if (atype == NotifyLast)
        return QString ();

    return lastSubjects[atype];
}

int
UnlockMissedEvents::getCount (Types atype)
{
    if (atype == NotifyLast)
        return 0;

    return notificationCounts[atype];
}

void
UnlockMissedEvents::addNotification (Types type, QString subject)
{
    lastSubjects[type] = subject;
    notificationCounts[type]++;
    lastType = type;

    emit updated ();
}

void
UnlockMissedEvents::clearAll ()
{
    for (int i = 0; i < NotifyLast; i++)
    {
        lastSubjects[i] = "";
        notificationCounts[i] = 0;
    }

    if (lastType != NotifyLast)
    {
        lastType = NotifyLast;
        emit updated ();
    }
}

