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

#include "ut_eventeater.h"
#include "eventeater.h"
#include <QEvent>
#include <QShowEvent>
#include <MApplication>
#include <MWindow>
#include <QWidget>
#include "x11wrapper_stub.h"

// QWidget stubs
static const WId WINDOW_ID = 69;
WId QWidget::winId() const
{
    return WINDOW_ID;
}

void Ut_EventEater::init()
{
    m_subject = new EventEater;
}

void Ut_EventEater::cleanup()
{
}

void Ut_EventEater::initTestCase()
{
}

void Ut_EventEater::cleanupTestCase()
{
    delete m_subject;
}

void Ut_EventEater::testButtonEvents_data()
{
    QTest::addColumn<int>("eventType");
    QTest::addColumn<WId>("winId");
    QTest::addColumn<bool>("isVisible");
    QTest::addColumn<int>("verifyButtonEventSpyCount");

    QTest::newRow("Button press") << ButtonPress << WINDOW_ID << true << 1;
    QTest::newRow("Button release") << ButtonRelease << WINDOW_ID << true << 1;
    QTest::newRow("Button press for wrong window id") << ButtonPress << WId(1) << true <<  0;
    QTest::newRow("Button press when not visible") << ButtonPress << WINDOW_ID << false << 0;
}

void Ut_EventEater::testButtonEvents()
{
    QFETCH(int, eventType);
    QFETCH(WId, winId);
    QFETCH(bool, isVisible);
    QFETCH(int, verifyButtonEventSpyCount);

    m_subject->setVisible(isVisible);

    XEvent event;
    event.type = eventType;
    event.xany.window = winId;

    QSignalSpy spy(m_subject, SIGNAL(inputEventReceived()));
    QAbstractEventDispatcher::instance()->filterEvent(&event);

    QCOMPARE(spy.count(), verifyButtonEventSpyCount);
}

QTEST_MAIN(Ut_EventEater)
