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

#include "ut_date.h"
#include "date.h"
#include <MApplication>
#include <MLabel>
#include <MLocale>

QDateTime gCurrentDateTime;

QDateTime QDateTime::currentDateTime()
{
    return gCurrentDateTime;
}

void Ut_Date::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_date";
    app = new MApplication(argc, &app_name);
}

void Ut_Date::cleanupTestCase()
{
    delete app;
}

void Ut_Date::init()
{
    locale = MLocale::createSystemMLocale();
    gCurrentDateTime = QDateTime(QDate(2042, 12, 21), QTime(12, 32));
    m_subject = new Date;
    m_subject->setFormat("\%A \%B \%d");
}

void Ut_Date::cleanup()
{
    delete m_subject;
}

void Ut_Date::checkNextUpdate()
{
    QDateTime nextUpdateTime = gCurrentDateTime.addDays(1);
    nextUpdateTime.setTime(QTime());
    QCOMPARE(m_subject->timer.interval(), (gCurrentDateTime.secsTo(nextUpdateTime) + 1) * 1000);
    QVERIFY(m_subject->timer.isActive());
}

void Ut_Date::testInitialState()
{
    QCOMPARE(m_subject->label->text(), locale->formatDateTime(gCurrentDateTime, "\%A \%B \%d"));

    checkNextUpdate();

    QVERIFY(disconnect(&m_subject->qmTime, SIGNAL(timeOrSettingsChanged(Maemo::QmTimeWhatChanged)),
                        m_subject, SLOT(updateSettings(Maemo::QmTimeWhatChanged))));

    QVERIFY(disconnect(&m_subject->timer, SIGNAL(timeout()), m_subject, SLOT(updateDate())));
}

void Ut_Date::testDateUpdates()
{
    gCurrentDateTime = QDateTime(QDate(2100, 4, 13), QTime(1, 23));
    m_subject->updateDate();

    QCOMPARE(m_subject->label->text(), locale->formatDateTime(gCurrentDateTime, "\%A \%B \%d"));

    checkNextUpdate();
}

void Ut_Date::testTimeSettingChanged()
{
    gCurrentDateTime = QDateTime(QDate(2100, 4, 13), QTime(1, 23));
    m_subject->updateSettings(Maemo::QmTimeTimeChanged);

    QCOMPARE(m_subject->label->text(), locale->formatDateTime(gCurrentDateTime, "\%A \%B \%d"));

    checkNextUpdate();
}


QTEST_APPLESS_MAIN(Ut_Date)
