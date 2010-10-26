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
#include "ut_clockview.h"
#include "clockview.h"
#include "clock_stub.h"
#include <MApplication>
#include <MLabel>
#include <MLocale>
#include <QGraphicsLayout>
#include <QGraphicsLinearLayout>
#include "applicationcontext.h"

// MWidgetController stubs
ClockModel clockModel;
MWidgetModel *MWidgetController::model()
{
    return &clockModel;
}
const MWidgetModel *MWidgetController::model() const
{
    return &clockModel;
}

// MLabel stub
void MLabel::setAlignment(Qt::Alignment alignment)
{
    Ut_ClockView::alignment = alignment;
}

Qt::Alignment Ut_ClockView::alignment;

// QGraphicsLinearLayout stubs (prevents crashing)
void QGraphicsLinearLayout::insertItem(int , QGraphicsLayoutItem *)
{
}

void MLabel::setText(const QString &text)
{
    Ut_ClockView::timeAsString = text;
}

TestClockView::TestClockView(Clock *controller) : ClockView(controller)
{
}

QString Ut_ClockView::timeAsString;

// Called before the first testfunction is executed
void Ut_ClockView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_clockview";
    app = new MApplication(argc, &app_name);

    locale = new MLocale();
}

// Called after the last testfunction was executed
void Ut_ClockView::cleanupTestCase()
{
    delete locale;
    delete app;
}

// Called before each testfunction is executed
void Ut_ClockView::init()
{
    testClock = new Clock();
    m_subject = new TestClockView(testClock);

    // Set model defaults
    clockModel.setTimeFormat24h(true);
    clockModel.setShortDisplay(false);
}

// Called after every testfunction
void Ut_ClockView::cleanup()
{
    delete testClock;  // this also deletes the view
    testClock = NULL;
    m_subject = NULL;
}

void Ut_ClockView::testUpdateTime()
{
    m_subject->setModel(&clockModel);
    m_subject->modifiableStyle()->setTimeFormat(QString("%R"));
    QDateTime time(QDate(), QTime(23, 23));

    clockModel.setTime(time);
    QCOMPARE(Ut_ClockView::timeAsString, locale->formatDateTime(time, "%R"));
}

void Ut_ClockView::testSetShortDisplay()
{
    m_subject->setModel(&clockModel);
    m_subject->modifiableStyle()->setShortRemoveAmPmIndicator(true);
    QDateTime time(QDate(), QTime(1, 1));

    clockModel.setShortDisplay(true);
    clockModel.setTime(time);

    // Test am/pm indicator behind
    m_subject->modifiableStyle()->setTimeFormat(QString("%I:%M%p"));
    clockModel.setShortDisplay(false);
    QCOMPARE(Ut_ClockView::timeAsString, locale->formatDateTime(time, "%I:%M%p"));
    clockModel.setShortDisplay(true);
    QCOMPARE(Ut_ClockView::timeAsString, locale->formatDateTime(time, "%I:%M"));

    // Test am/pm indicator behind, with a space
    m_subject->modifiableStyle()->setTimeFormat(QString("%I:%M %p"));
    clockModel.setShortDisplay(false);
    QCOMPARE(Ut_ClockView::timeAsString, locale->formatDateTime(time, "%I:%M %p"));
    clockModel.setShortDisplay(true);
    QCOMPARE(Ut_ClockView::timeAsString, locale->formatDateTime(time, "%I:%M"));

    // Test am/pm indicator in front
    m_subject->modifiableStyle()->setTimeFormat(QString("%p%I:%M"));
    clockModel.setShortDisplay(false);
    QCOMPARE(Ut_ClockView::timeAsString, locale->formatDateTime(time, "%p%I:%M"));
    clockModel.setShortDisplay(true);
    QCOMPARE(Ut_ClockView::timeAsString, locale->formatDateTime(time, "%I:%M"));

    // Test am/pm indicator in front, with a space
    m_subject->modifiableStyle()->setTimeFormat(QString("%p %I:%M"));
    clockModel.setShortDisplay(false);
    QCOMPARE(Ut_ClockView::timeAsString, locale->formatDateTime(time, "%p %I:%M"));
    clockModel.setShortDisplay(true);
    QCOMPARE(Ut_ClockView::timeAsString, locale->formatDateTime(time, "%I:%M"));
}

void Ut_ClockView::testUpdateTimeFormat()
{
    m_subject->setModel(&clockModel);
    clockModel.setTimeFormat24h(false);
    QCOMPARE(m_subject->styleContainer().currentMode(), QString("twelve-hour"));
    clockModel.setTimeFormat24h(true);
    QCOMPARE(m_subject->styleContainer().currentMode(), QString());
}

void Ut_ClockView::testAlignment()
{
    m_subject->setModel(&clockModel);
    Qt::Alignment alignment = Qt::AlignLeft;
    m_subject->modifiableStyle()->setHorizontalAlign(alignment);
    m_subject->applyStyle();
    QCOMPARE(Ut_ClockView::alignment, alignment);

    alignment = Qt::AlignRight;
    m_subject->modifiableStyle()->setHorizontalAlign(alignment);
    m_subject->applyStyle();
    QCOMPARE(Ut_ClockView::alignment, alignment);
}

QTEST_APPLESS_MAIN(Ut_ClockView)
