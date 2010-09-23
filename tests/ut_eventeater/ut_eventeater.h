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
#ifndef Ut_EventEater_H
#define Ut_EventEater_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class EventEater;

class LockScreenUIEventSink : public QObject
{
    Q_OBJECT

public:
    LockScreenUIEventSink();

public slots:
    void inputReceived();
    void unlocked();

public:
    bool      m_OneInputCame;
    bool      m_UnlockedCame;
};


class Ut_EventEater : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testEventEater();
    void testEventEaterWindowName();

private:
    LockScreenUIEventSink  m_EventSink;
    EventEater          *m_EventEater;
    MApplication          *m_App;
};

#endif
