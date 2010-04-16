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
#ifndef _UT_STATUSAREAWINDOW_
#define _UT_STATUSAREAWINDOW_

#include <QtTest/QtTest>
#include "qmdisplaystate.h"

class StatusAreaWindow;
class MApplication;

class Ut_StatusAreaWindow : public QObject
{
    Q_OBJECT

private:
    StatusAreaWindow *statusAreaWindow;
    MApplication *app;

signals:
    void changed(QList<QRectF> rectList);
    void displayStateChanged(Maemo::QmDisplayState::DisplayState);

private slots:
    // Executed once before every test case
    void init();
    // Executed once after every test case
    void cleanup();
    // Executed once before first test case
    void initTestCase();
    // Executed once after last test case
    void cleanupTestCase();
    // Test scene changed events render the scene
    void testSceneChanged();
    // Test rendering happens when display state is on
    void testSceneRenderControlDisplayStateOn();
    // Test rendering does not happen when display state is off
    void testSceneRenderControlDisplayStateOff();
    // Test rendering does not happen when display state is dimmed
    void testSceneRenderControlDisplayStateDimmed();
};

class RenderTestsHelper: public QObject
{
    Q_OBJECT
public:
    RenderTestsHelper(){}
    ~RenderTestsHelper();
    QList<QRectF>* setupRenderTests(Ut_StatusAreaWindow* testClass, StatusAreaWindow* statusAreaWindow);
private:
    QList<QRectF>* rectList;
    void setupRect();
};

#endif //_UT_STATUSAREAWINDOW_
