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

#ifndef UT_STATUSINDICATORANIMATIONVIEW_H
#define UT_STATUSINDICATORANIMATIONVIEW_H

#include <QtTest/QtTest>
#include <QObject>

#include "statusindicatoranimationview.h"

class MApplication;

class TestStatusIndicatorAnimationView : public StatusIndicatorAnimationView
{
    Q_OBJECT
    M_VIEW(StatusIndicatorModel, StatusIndicatorAnimationStyle)

public:
    TestStatusIndicatorAnimationView(StatusIndicator *controller);

    StatusIndicatorModel *getModel();

    void executeStyleChanged();

    void callableDrawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

    StatusIndicatorAnimationStyle *modifiableStyle();

};


class Ut_StatusIndicatorAnimationView : public QObject
{
    Q_OBJECT

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test cases
    void testImageListInitialized();
    void testSetAnimationFrame();
    void testSetAnimationFrameToInvalid();
    void testSetAnimationDuration();
    void testValueChanged();
    void testValueChangedToInvalid();
    void testChangingAnimate();

private:
    // MApplication
    MApplication *app;
    // A controller for the test subject
    StatusIndicator *controller;
    // The object being tested
    TestStatusIndicatorAnimationView *m_subject;
};

#endif
