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
#include <QGraphicsLayout>
#include <QGraphicsLinearLayout>
#include "applicationcontext.h"
#include "testcontextitem.h"

QHash<QString, TestContextItem *> testContextItems;

// Test context
class TestContext : public ApplicationContext
{
public:
    virtual ContextItem *createContextItem(const QString &key) {
        Q_UNUSED(key);

        if (testContextItems[key] == NULL) {
            testContextItems[key] = new TestContextItem;
        }

        return testContextItems[key];
    }
};

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
}

// Called after the last testfunction was executed
void Ut_ClockView::cleanupTestCase()
{
    delete app;
}

// Called before each testfunction is executed
void Ut_ClockView::init()
{
    testContext = new TestContext();
    testContextItems.clear();

    testClock = new Clock(*testContext);
    m_subject = new TestClockView(testClock);

    // Set model defaults
    clockModel.setTimeFormat24h(true);
    clockModel.setAlarmSet(false);
}

// Called after every testfunction
void Ut_ClockView::cleanup()
{
    delete testClock;  // this also deletes the view
    testClock = NULL;
    m_subject = NULL;
    delete testContext;
    testContext = NULL;
}

void Ut_ClockView::testUpdateTime()
{
    m_subject->setModel(&clockModel);
    m_subject->modifiableStyle()->setTimeFormat(QString("hh:mmap"));
    clockModel.setTime(QTime(1, 1));
    QCOMPARE(Ut_ClockView::timeAsString, QString("01:01am"));
}

void Ut_ClockView::testUpdateAlarmSet()
{
    m_subject->setModel(&clockModel);
    m_subject->modifiableStyle()->setTimeFormat(QString("hh:mmap"));
    m_subject->modifiableStyle()->setTimeFormatWithAlarm(QString("hh:mm"));
    clockModel.setTime(QTime(1, 1));
    QCOMPARE(Ut_ClockView::timeAsString, QString("01:01am"));

    clockModel.setAlarmSet(true);
    QCOMPARE(Ut_ClockView::timeAsString, QString("01:01"));
}

void Ut_ClockView::testUpdateTimeFormat()
{
    m_subject->setModel(&clockModel);
    clockModel.setTimeFormat24h(false);
    QCOMPARE(m_subject->styleContainer().currentMode(), QString("twelve-hour"));
    clockModel.setTimeFormat24h(true);
    QCOMPARE(m_subject->styleContainer().currentMode(), QString());
}

void Ut_ClockView::testTwelveHour()
{
    m_subject->setModel(&clockModel);
    clockModel.setTimeFormat24h(false);
    m_subject->modifiableStyle()->setTimeFormat(QString("hh:mm"));
    m_subject->modifiableStyle()->setTimeFormatWithAlarm(QString("hh:mm"));
    clockModel.setTime(QTime(23, 10));
    QCOMPARE(Ut_ClockView::timeAsString, QString("11:10"));
    clockModel.setAlarmSet(true);
    QCOMPARE(Ut_ClockView::timeAsString, QString("11:10"));
}


QTEST_APPLESS_MAIN(Ut_ClockView)
