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
#include "mlocale_stub.h"
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
}

// Called after the last testfunction was executed
void Ut_ClockView::cleanupTestCase()
{
    delete app;
}

// Called before each testfunction is executed
void Ut_ClockView::init()
{
    gMLocaleStub = &gDefaultMLocaleStub;

    testClock = new Clock();
    m_subject = new TestClockView(testClock);
    connect(this, SIGNAL(localeChanged()), m_subject->locale.data(), SIGNAL(settingsChanged()));
    m_subject->setModel(&clockModel);
}

// Called after every testfunction
void Ut_ClockView::cleanup()
{
    disconnect(m_subject->locale.data());
    delete testClock;  // this also deletes the view
    testClock = NULL;
    m_subject = NULL;

    gMLocaleStub->stubReset();
}

void Ut_ClockView::testWhenConstructedThenCurrent24HourModeIsTakenInToUse()
{
    gMLocaleStub->stubSetReturnValue("timeFormat24h", MLocale::TwelveHourTimeFormat24h);

    Clock clock;
    TestClockView *view = new TestClockView(&clock);
    view->setModel(&clockModel);

    QCOMPARE(view->styleContainer().currentMode(), QString("twelve-hour"));
}

void Ut_ClockView::testWhen24HourModeChangesThenThe24HourModeIsTakenInToUse()
{
    gMLocaleStub->stubSetReturnValue("timeFormat24h", MLocale::TwelveHourTimeFormat24h);
    emit localeChanged();
    QCOMPARE(m_subject->styleContainer().currentMode(), QString("twelve-hour"));

    gMLocaleStub->stubSetReturnValue("timeFormat24h", MLocale::TwentyFourHourTimeFormat24h);
    emit localeChanged();
    QCOMPARE(m_subject->styleContainer().currentMode(), QString(""));
}

void Ut_ClockView::testWhenDefault24HourModeIsInUseThenDefault24HourTimeFormatIsUsed()
{
    gMLocaleStub->stubSetReturnValue("timeFormat24h", MLocale::LocaleDefaultTimeFormat24h);
    gMLocaleStub->stubSetReturnValue("defaultTimeFormat24h", MLocale::TwentyFourHourTimeFormat24h);
    emit localeChanged();
    QCOMPARE(m_subject->styleContainer().currentMode(), QString(""));

    gMLocaleStub->stubSetReturnValue("defaultTimeFormat24h", MLocale::TwelveHourTimeFormat24h);
    emit localeChanged();
    QCOMPARE(m_subject->styleContainer().currentMode(), QString("twelve-hour"));
}

void Ut_ClockView::testUpdateTime()
{
    const QString FORMAT_STRING("format string");
    const QString FORMATTED_DATE_TIME("formatted date/time");

    m_subject->modifiableStyle()->setTimeFormat(FORMAT_STRING);
    QDateTime time(QDate(), QTime(23, 23));

    gMLocaleStub->stubSetReturnValue("formatDateTime", FORMATTED_DATE_TIME);
    clockModel.setTime(time);
    QVERIFY(gMLocaleStub->stubCallCount("formatDateTime") > 0);
    QCOMPARE(gMLocaleStub->stubLastCallTo("formatDateTime").parameter<QDateTime>(0), time);
    QCOMPARE(gMLocaleStub->stubLastCallTo("formatDateTime").parameter<QString>(1), FORMAT_STRING);
    QCOMPARE(Ut_ClockView::timeAsString, FORMATTED_DATE_TIME);
}

const QString TO_REMOVE("toremove");
const QString FORMAT("format");

class EnhancedMLocaleStub : public MLocaleStub {
public:
    // Returns TO_REMOVE string if formatString parameter is "%p",
    // otherwise returns the formatString parameter as is.
    virtual QString formatDateTime(const QDateTime &dateTime, const QString &formatString) const
    {
        QString ret = MLocaleStub::formatDateTime(dateTime, formatString);

        if (formatString == "%p") {
            ret = TO_REMOVE;
        } else {
            ret = formatString;
        }

        return ret;
    }
};

EnhancedMLocaleStub gEnhancedMLocaleStub;

void Ut_ClockView::testAlignment()
{
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
