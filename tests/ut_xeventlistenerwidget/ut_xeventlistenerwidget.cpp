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
#include <QX11Info>
#include "xeventlistenerwidget.h"
#include "xeventlistener_stub.h"
#include "ut_xeventlistenerwidget.h"

WId qWidgetCreateWindow = 0;
bool qWidgetCreateInitializeWindow = true;
bool qWidgetCreateDestroyOldWindow = true;
void QWidget::create(WId window, bool initializeWindow, bool destroyOldWindow)
{
    qWidgetCreateWindow = window;
    qWidgetCreateInitializeWindow = initializeWindow;
    qWidgetCreateDestroyOldWindow = destroyOldWindow;
}

bool qWidgetDestroyDestroyWindow = true;
bool qWidgetDestroyDestroySubWindows = true;
void QWidget::destroy(bool destroyWindow, bool destroySubWindows)
{
    qWidgetDestroyDestroyWindow = destroyWindow;
    qWidgetDestroyDestroySubWindows = destroySubWindows;
}

void Ut_XEventListenerWidget::init()
{
}

void Ut_XEventListenerWidget::cleanup()
{
    gXEventListenerStub->stubReset();
    qWidgetCreateWindow = 0;
    qWidgetCreateInitializeWindow = true;
    qWidgetCreateDestroyOldWindow = true;
    qWidgetDestroyDestroyWindow = true;
    qWidgetDestroyDestroySubWindows = true;
}

void Ut_XEventListenerWidget::initTestCase()
{
}

void Ut_XEventListenerWidget::cleanupTestCase()
{
}

void Ut_XEventListenerWidget::testConstructionAndDestruction()
{
    Qt::HANDLE window = QX11Info::appRootWindow(QX11Info::appScreen());
    XEventListenerWidget *widget = new XEventListenerWidget(NULL, window);
    QCOMPARE(qWidgetCreateWindow, (WId)window);
    QCOMPARE(qWidgetCreateInitializeWindow, false);
    QCOMPARE(qWidgetCreateDestroyOldWindow, false);

    delete widget;
    QCOMPARE(qWidgetDestroyDestroyWindow, false);
    QCOMPARE(qWidgetDestroyDestroySubWindows, false);
}

void Ut_XEventListenerWidget::testX11Event_data()
{
    QTest::addColumn<bool>("returnValue");

    QTest::newRow("Return true") << true;
    QTest::newRow("Return false") << false;
}

void Ut_XEventListenerWidget::testX11Event()
{
    QFETCH(bool, returnValue);

    gXEventListenerStub->stubSetReturnValue("xEventFilter", returnValue);
    XEventListenerWidget widget(NULL, 0);
    XEvent event;
    QCOMPARE(widget.x11Event(&event), returnValue);
    QCOMPARE(gXEventListenerStub->stubCallCount("xEventFilter"), 1);
    QCOMPARE(gXEventListenerStub->stubLastCallTo("xEventFilter").parameter<void *>(0), &event);
}

QTEST_MAIN(Ut_XEventListenerWidget)
