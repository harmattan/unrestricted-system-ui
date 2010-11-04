/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
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
#include <MApplication>
#include "closeeventeater.h"
#include "ut_closeeventeater.h"

bool qObjectEventFilterReturnValue = false;
bool QObject::eventFilter(QObject *, QEvent *)
{
    return qObjectEventFilterReturnValue;
}

void Ut_CloseEventEater::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_closeeventeater" };
    app = new MApplication(argc, app_name);
}

void Ut_CloseEventEater::cleanupTestCase()
{
    delete app;
}

void Ut_CloseEventEater::init()
{
    m_subject = new CloseEventEater();
}

void Ut_CloseEventEater::cleanup()
{
    delete m_subject;
    qObjectEventFilterReturnValue = false;
}

void Ut_CloseEventEater::testCloseEventIsIgnored()
{
    QCloseEvent event;
    event.accept();
    QVERIFY(m_subject->eventFilter(this, &event));
    QVERIFY(!event.isAccepted());
}

void Ut_CloseEventEater::testOtherEventsArePassedOn_data()
{
    QTest::addColumn<bool>("eventFilterReturnValue");
    QTest::addColumn<bool>("accepted");

    QTest::newRow("Event filter returns false, accepted was false") << false << false;
    QTest::newRow("Event filter returns false, accepted was true") << false << true;
    QTest::newRow("Event filter returns true, accepted was false") << true << false;
    QTest::newRow("Event filter returns true, accepted was true") << true << true;
}

void Ut_CloseEventEater::testOtherEventsArePassedOn()
{
    QFETCH(bool, eventFilterReturnValue);
    QFETCH(bool, accepted);

    qObjectEventFilterReturnValue = eventFilterReturnValue;

    QEvent event(QEvent::None);
    event.setAccepted(accepted);
    QCOMPARE(m_subject->eventFilter(this, &event), eventFilterReturnValue);
    QCOMPARE(event.isAccepted(), accepted);
}

QTEST_APPLESS_MAIN(Ut_CloseEventEater)
