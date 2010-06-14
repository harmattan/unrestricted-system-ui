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
#include "clock.h"

#ifdef HAVE_QMSYSTEM
#include <qmtime.h>
#endif

class TestContext;

class Ut_Clock : public QObject
{
    Q_OBJECT

public:
    static int timerTimeout;
    static QDateTime expectedDateTime;
#ifdef HAVE_QMSYSTEM
    static Maemo::QmTime::TimeFormat expectedTimeFormat;
#endif

signals:
#ifdef HAVE_QMSYSTEM
    void timeOrSettingsChanged(Maemo::QmTimeWhatChanged);
#endif
    void shortDisplayMode(bool isSet);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void test24HourModeDuringCreation();
    void test24HourModeToggling();
    void testTimeUpdate();
    void testModelUpdates();
    void testShortDisplayToggling();

private:
    Clock *m_subject;
};

#endif
