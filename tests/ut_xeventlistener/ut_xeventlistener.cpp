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

#include "ut_xeventlistener.h"

QHash<int, int> gCalledDummyXEventFilters;
class DummyXEventFilter : public XEventListenerFilterInterface
{
public:
    DummyXEventFilter(int id) {
        filterId = id;
        gCalledDummyXEventFilters.insert(filterId, 0);
    }
    ~DummyXEventFilter() {}

    int filterId;

    bool xEventFilter(const XEvent &) {
        qDebug() << "filtered: " << filterId;
        gCalledDummyXEventFilters[filterId]++;
        return false;
    }
};

bool gPreviousFilterCalled;
static bool previousEventFilter(void *) {
    gPreviousFilterCalled = true;
    return false;
}

QAbstractEventDispatcher::EventFilter currentFilter;
QAbstractEventDispatcher::EventFilter QAbstractEventDispatcher::setEventFilter(QAbstractEventDispatcher::EventFilter filter)
{
    currentFilter = filter;
    return previousEventFilter;
}

bool Ut_XEventListener::xEventFilter(const XEvent &)
{
    return false;
}

void Ut_XEventListener::init()
{
    gPreviousFilterCalled = false;
    gCalledDummyXEventFilters.clear();

    XEventListener::filterList.clear();
}

void Ut_XEventListener::cleanup()
{
}

void Ut_XEventListener::initTestCase()
{
    QAbstractEventDispatcher::instance()->setEventFilter(previousEventFilter);
}

void Ut_XEventListener::cleanupTestCase()
{
}

void Ut_XEventListener::testInitializingEventListener()
{
    XEventListener::registerEventFilter(this, 0);
    QVERIFY(XEventListener::previousEventFilter == previousEventFilter);
    QVERIFY(currentFilter == XEventListener::xEventFilter);
}

void Ut_XEventListener::testReceivingEvents_data()
{
    QTest::addColumn<int>("eventType");
    QTest::addColumn<bool>("expectedFiltered0");
    QTest::addColumn<bool>("expectedFiltered1");

    QTest::newRow("KeyPress") << KeyPress << true << false;
    QTest::newRow("ButtonPress") << ButtonPress << true << false;
    QTest::newRow("PropertyNotify") << PropertyNotify << false << true;
    QTest::newRow("LASTEvent") << LASTEvent << false << false;
}

void Ut_XEventListener::testReceivingEvents()
{
    DummyXEventFilter filter0(0);
    DummyXEventFilter filter1(1);

    QFETCH(int, eventType);
    QFETCH(bool, expectedFiltered0);
    QFETCH(bool, expectedFiltered1);
    XEventListener::registerEventFilter(&filter0, ButtonPressMask|KeyPressMask);
    XEventListener::registerEventFilter(&filter1, PropertyChangeMask);

    // Verify unregistered event type
    XEvent event;
    event.type = eventType;
    XEventListener::xEventFilter(&event);
    QCOMPARE((gCalledDummyXEventFilters[0] == 1), expectedFiltered0);
    QCOMPARE((gCalledDummyXEventFilters[1] == 1), expectedFiltered1);

    QCOMPARE(gPreviousFilterCalled, true);
}

QTEST_MAIN(Ut_XEventListener)
