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
#ifndef _UT_STATUSINDICATOR_
#define _UT_STATUSINDICATOR_

#include <QtGui>
#include <QtTest/QtTest>
#include <QObject>
#include <mapplication.h>
#include <mwidget.h>
#include "statusindicator.h"
#include "statusindicatoriconview.h"
#include "applicationcontext.h"

class TestStatusIndicatorIconView : public StatusIndicatorIconView
{
    Q_OBJECT
    M_VIEW(StatusIndicatorModel, StatusIndicatorIconStyle)

public:
    TestStatusIndicatorIconView(StatusIndicator *controller);

protected slots:
    virtual void updateData(const QList<const char *>& modifications);
};

class TestStatusIndicatorLabelView : public StatusIndicatorIconView
{
public:
    TestStatusIndicatorLabelView(StatusIndicator *controller);
};


class TestContext;

class Ut_StatusIndicator : public QObject
{
    Q_OBJECT

private:
    MApplication  *app;
    TestContext     *testContext;

private slots:
    // Executed once before every test case
    void init();

    // Executed once after every test case
    void cleanup();

    // Executed once before first test case
    void initTestCase();

    // Executed once after last test case
    void cleanupTestCase();

    void testModelUpdates();
    void testPhoneNetworkSignalStrength();
    void testBattery();
    void testAlarm();
    void testBluetooth();
    void testInternetConnection();
    void testAnimation();
    void testPhoneNetwork();
    void testInputMethod();
    void testCall();
    void testProfile();
};

#endif //_UT_STATUSINDICATOR_
