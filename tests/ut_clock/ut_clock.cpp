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

#include <QtTest/QtTest>
#include <MOnDisplayChangeEvent>
#include <MLocale>
#include <QScopedPointer>
#include "ut_clock.h"
#include "clock.h"

Q_DECLARE_METATYPE(MLocale::TimeFormat24h);

#ifdef HAVE_QMSYSTEM
Q_DECLARE_METATYPE(Maemo::QmTime::TimeFormat);
Q_DECLARE_METATYPE(Maemo::QmTimeWhatChanged);

Maemo::QmTime::TimeFormat Maemo::QmTime::getTimeFormat()
{
    return Ut_Clock::expectedTimeFormat;
}
#endif

QDateTime QDateTime::currentDateTime()
{
    return Ut_Clock::expectedDateTime;
}

void QTimer::start(int msec)
{
    Ut_Clock::timerTimeout = msec;
}

void QTimer::stop()
{
    Ut_Clock::timerTimeout = -1;
}


static MLocale::TimeFormat24h g_timeFormat24h = MLocale::TwelveHourTimeFormat24h;
MLocale::TimeFormat24h MLocale::timeFormat24h() const
{
    return g_timeFormat24h;
}

static MLocale::TimeFormat24h g_defaultTimeFormat24h = MLocale::TwelveHourTimeFormat24h;
MLocale::TimeFormat24h MLocale::defaultTimeFormat24h() const
{
    return g_defaultTimeFormat24h;
}

static bool g_localeDestroyed = false;
MLocale::~MLocale()
{
    g_localeDestroyed = true;
}

static bool g_localeSettingsConnected = false;
void MLocale::connectSettings()
{
    g_localeSettingsConnected = true;
}

int Ut_Clock::timerTimeout;
QDateTime Ut_Clock::expectedDateTime;
#ifdef HAVE_QMSYSTEM
Maemo::QmTime::TimeFormat Ut_Clock::expectedTimeFormat;
#endif

// Called before the first testfunction is executed
void Ut_Clock::initTestCase()
{
    expectedDateTime = QDateTime(QDate(2000, 1, 1));
}

// Called after the last testfunction was executed
void Ut_Clock::cleanupTestCase()
{
}

// Called before each testfunction is executed
void Ut_Clock::init()
{
    g_localeDestroyed = false;
    g_localeSettingsConnected = false;
#ifdef HAVE_QMSYSTEM
    expectedTimeFormat = Maemo::QmTime::format12h;
#endif

    m_subject = new Clock;
    connect(this, SIGNAL(shortDisplayMode(bool)),
            m_subject, SLOT(setShortDisplay(bool)));
    connect(this, SIGNAL(localeSettingsChanged()),
            m_subject, SLOT(updateLocaleSettings()));

#ifdef HAVE_QMSYSTEM
    connect(this, SIGNAL(timeOrSettingsChanged(Maemo::QmTimeWhatChanged)),
            m_subject, SLOT(updateSettings(Maemo::QmTimeWhatChanged)));
#endif
}

// Called after every testfunction
void Ut_Clock::cleanup()
{
    delete m_subject;
}

void Ut_Clock::testConstruction()
{
    QVERIFY(m_subject->locale);
#ifdef HAVE_QMSYSTEM
    QVERIFY(disconnect(&m_subject->qmTime,
                       SIGNAL(timeOrSettingsChanged(Maemo::QmTimeWhatChanged)),
                       m_subject,
                       SLOT(updateSettings(Maemo::QmTimeWhatChanged))));
#endif
    QVERIFY(disconnect(m_subject->locale.data(),
                       SIGNAL(settingsChanged()),
                       m_subject,
                       SLOT(updateLocaleSettings())));
    QVERIFY(g_localeSettingsConnected);
    QVERIFY(disconnect(&m_subject->timer,
                       SIGNAL(timeout()),
                       m_subject, SLOT(updateModelAndSetupTimer())));
    QVERIFY(m_subject->timer.isSingleShot());
    QDateTime nextUpdateTime = expectedDateTime.addSecs(60);
    QTime time = nextUpdateTime.time();
    time.setHMS(time.hour(), time.minute(), 0);
    nextUpdateTime.setTime(time);
    QVERIFY(timerTimeout > expectedDateTime.msecsTo(nextUpdateTime));
}

void Ut_Clock::testDestruction()
{
    Clock* subject(new Clock);
    delete subject;
    QVERIFY(g_localeDestroyed);
}


static void columnsFor24HourModeTests()
{
    QTest::addColumn<MLocale::TimeFormat24h>("formatToSet");
    QTest::addColumn<MLocale::TimeFormat24h>("defaultFormatToSet");
    QTest::addColumn<bool>("expected24h");
}

static QVector<QTestData*> rowsFor24HourModeTests()
{
    QVector<QTestData*> returnValue;
    QTestData& row1 = QTest::newRow("12h forced");
    row1
        << MLocale::TwelveHourTimeFormat24h
        << MLocale::TwentyFourHourTimeFormat24h
        << false;
    returnValue.append(&row1);
    QTestData& row2 = QTest::newRow("24h forced");
    row2
        << MLocale::TwentyFourHourTimeFormat24h
        << MLocale::TwelveHourTimeFormat24h
        << true;
    returnValue.append(&row2);
    QTestData& row3 = QTest::newRow("12h locale default");
    row3
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::TwelveHourTimeFormat24h
        << false;
    returnValue.append(&row3);
    QTestData& row4 = QTest::newRow("24h locale default");
    row4
        << MLocale::LocaleDefaultTimeFormat24h
        << MLocale::TwentyFourHourTimeFormat24h
        << true;
    returnValue.append(&row4);
    return returnValue;
}

void Ut_Clock::test24HourModeDuringCreation_data()
{
    columnsFor24HourModeTests();
    rowsFor24HourModeTests();
}

void Ut_Clock::test24HourModeDuringCreation()
{
    QFETCH(MLocale::TimeFormat24h, formatToSet);
    g_timeFormat24h = formatToSet;
    QFETCH(MLocale::TimeFormat24h, defaultFormatToSet);
    g_defaultTimeFormat24h = defaultFormatToSet;
    QScopedPointer<Clock> subject(new Clock);
    QFETCH(bool, expected24h);
    QCOMPARE(subject->model()->timeFormat24h(),
             expected24h);
}

void Ut_Clock::test24HourModeToggling_data()
{
    columnsFor24HourModeTests();
#ifdef HAVE_QMSYSTEM
    QTest::addColumn<Maemo::QmTime::TimeFormat>("expectedFormat");
    QTest::addColumn<Maemo::QmTimeWhatChanged>("whatChanged");
#endif
    QVector<QTestData*> rows = rowsFor24HourModeTests();
#ifdef HAVE_QMSYSTEM
    *rows[0] << Maemo::QmTime::format24h
            << Maemo::QmTimeOnlySettingsChanged;
    *rows[1] << Maemo::QmTime::format12h
            << Maemo::QmTimeOnlySettingsChanged;
    *rows[2] << Maemo::QmTime::format24h
            << Maemo::QmTimeTimeChanged;
    *rows[3] << Maemo::QmTime::format12h
            << Maemo::QmTimeTimeChanged;
#endif
}

void Ut_Clock::test24HourModeToggling()
{
    QFETCH(MLocale::TimeFormat24h, formatToSet);
    g_timeFormat24h = formatToSet;
    QFETCH(MLocale::TimeFormat24h, defaultFormatToSet);
    g_defaultTimeFormat24h = defaultFormatToSet;
    emit localeSettingsChanged();
    QFETCH(bool, expected24h);
    QCOMPARE(m_subject->model()->timeFormat24h(),
             expected24h);

#ifdef HAVE_QMSYSTEM
    // QMSystem settings changes no longer affect clock 24h mode,
    // only MLocale settings changes will, so we test with a variety
    // of values that QMSystem signals don't change the 24h format
    // at all
    bool oldValue = m_subject->model()->timeFormat24h();
    QFETCH(Maemo::QmTime::TimeFormat, expectedFormat);
    expectedTimeFormat = expectedFormat;
    QFETCH(Maemo::QmTimeWhatChanged, whatChanged);
    emit timeOrSettingsChanged(whatChanged);
    QCOMPARE(m_subject->model()->timeFormat24h(), oldValue);
#endif
}

void Ut_Clock::testTimeUpdate()
{
    // Check that time was initialized correctly
    QCOMPARE(m_subject->model()->time(), expectedDateTime);

#ifdef HAVE_QMSYSTEM
    // If qmsystem notifies up that time changed, model should be updated accordingly
    expectedDateTime = QDateTime(QDate(2010, 1, 1));
    emit timeOrSettingsChanged(Maemo::QmTimeTimeChanged);
    QCOMPARE(m_subject->model()->time(), expectedDateTime);
#endif
}

void Ut_Clock::testModelUpdates()
{
    MOnDisplayChangeEvent exitDisplayEvent(MOnDisplayChangeEvent::FullyOffDisplay, QRectF());
    MOnDisplayChangeEvent enterDisplayEvent(MOnDisplayChangeEvent::FullyOnDisplay, QRectF());

    // The timer should be running by default and the model should contain the current time
    QVERIFY(timerTimeout >= 0);
    QCOMPARE(m_subject->model()->time(), expectedDateTime);

    // When the application becomes invisible the timer should stop
    qApp->sendEvent(m_subject, &exitDisplayEvent);
    QCOMPARE(timerTimeout, -1);

    // When the application becomes visible the timer should start and the model should be updated to contain the current time
    expectedDateTime = QDateTime(QDate(2001, 1, 1));
    qApp->sendEvent(m_subject, &enterDisplayEvent);
    QVERIFY(timerTimeout >= 0);
    QCOMPARE(m_subject->model()->time(), expectedDateTime);
}

void Ut_Clock::testShortDisplayToggling()
{
    // Check that the "setShortDisplay" slot sets the correct model field
    emit shortDisplayMode(true);
    QCOMPARE(m_subject->model()->shortDisplay(), true);
    emit shortDisplayMode(false);
    QCOMPARE(m_subject->model()->shortDisplay(), false);
}

QTEST_MAIN(Ut_Clock)
