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

#ifndef UT_CLOCK_H
#define UT_CLOCK_H

#include <QObject>
#include <QDateTime>

class Clock;

class Ut_Clock : public QObject
{
    Q_OBJECT

public:
    static int timerTimeout;
    static QDateTime expectedDateTime;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstruction_data();
    void testConstruction();
    void testTimeUpdate();
    void testModelUpdates();

private:
    Clock *m_subject;
};

#endif
