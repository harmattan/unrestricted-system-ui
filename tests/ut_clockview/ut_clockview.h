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
#ifndef UT_CLOCKVIEW_H
#define UT_CLOCKVIEW_H

#include <QtTest/QtTest>
#include <QObject>
#include "clockstyle.h"
#include "clockview.h"

class Clock;
class MApplication;

class TestClockStyle : public ClockStyle
{
};

class TestClockStyleContainer : public ClockStyleContainer
{
public:
    QString currentMode()
    {
        return ClockStyleContainer::currentMode();
    }
};

class TestClockView : public ClockView
{
    M_VIEW(ClockModel, TestClockStyle)

public:
    TestClockView(Clock *controller);

    ClockStyle *modifiableStyle() {
        ClockStyleContainer &sc = style();
        const ClockStyle *const_s = sc.operator ->();
        ClockStyle *s = const_cast<ClockStyle *>(const_s);
        return s;
    }
    TestClockStyleContainer& styleContainer() {
        return style();
    }
};

class Ut_ClockView : public QObject
{
    Q_OBJECT

signals:
    void localeChanged();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testWhenConstructedThenCurrent24HourModeIsTakenInToUse();
    void testWhen24HourModeChangesThenThe24HourModeIsTakenInToUse();
    void testWhenDefault24HourModeIsInUseThenDefault24HourTimeFormatIsUsed();
    void testUpdateTime();
    void testAlignment();

public:
    static Qt::Alignment alignment;
    static QString timeAsString;

private:
    MApplication *app;
    TestClockView *m_subject;
    Clock *testClock;
};

#endif
