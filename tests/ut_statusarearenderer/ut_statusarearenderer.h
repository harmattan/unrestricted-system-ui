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
#ifndef _UT_STATUSAREARENDERER_
#define _UT_STATUSAREARENDERER_

#include <QObject>
#include <QRectF>

#ifdef HAVE_QMSYSTEM
#include "qmdisplaystate.h"
#endif

class StatusAreaRenderer;

class Ut_StatusAreaRenderer : public QObject
{
    Q_OBJECT

private:
    void setupXGetPropertiesToDefault();
    void setStatusBarVisibility(bool visible);

    StatusAreaRenderer *statusAreaRenderer;

signals:
    void changed(QList<QRectF> rectList);

#ifdef HAVE_QMSYSTEM
    void displayStateChanged(MeeGo::QmDisplayState::DisplayState);
#endif

private slots:
    // Executed once before every test case
    void init();
    // Executed once after every test case
    void cleanup();
    // Executed once before first test case
    void initTestCase();
    // Executed once after last test case
    void cleanupTestCase();
    // Test setuping status bar visible property listening
    void testSetupStatusBarVisiblePropertyListening();
    void testWhenStatusBarVisiblePropertySetupUnsuccessfulThenAssumingStatusBarVisible();
    void testStatusBarInvisibleStopsRendering();
    void testWhenStatusBarBecomesVisibleRenderAccumulatedChanges();
    void testUnregisteringEventFilterWhenDestroyed();
    // Test scene changed events render the scene
    void testSceneChanged();
    // Test shared pixmap handle
    void testSharedPixmapHandle();

    void testAccumulationTimer();

    void testRenderingWithMeeGo();
    void testRenderingWithoutMeeGo();

    void testStatusBarPropertyWindowCreation();
    void testStatusBarPixmapPropertiesAreSetCorrectly();
    void testStatusBarPixmapPropertiesAreDeletedInDestructor();
    void testStatusBarPixmapSharingIsAllowed();

    void testWMWindowUnavailableInStartUp();
    void testWMWindowBecomingUnavailable();
    void testWMWindowBecomingAvailable_data();
    void testWMWindowBecomingAvailable();
    void testBug285985Workaround();
    void testSetupStatusBarVisibleListenerFreesDataCorrectly_data();
    void testSetupStatusBarVisibleListenerFreesDataCorrectly();
    void testUpdateStatusBarVisiblePropertyFreesDataCorrectly_data();
    void testUpdateStatusBarVisiblePropertyFreesDataCorrectly();

#ifdef HAVE_QMSYSTEM
    // Test rendering happens when display state is on
    void testSceneRenderControlDisplayStateOn();
    // Test rendering does not happen when display state is off
    void testSceneRenderControlDisplayStateOff();
    // Test rendering does not happen when display state is dimmed
    void testSceneRenderControlDisplayStateDimmed();
    // Test MOnDisplayChangeEvent dispatching
    void testMOnDisplayChangeEvent();
    // Test that scene is not rendered if initial display state is off
    void testSceneRenderControlWhenInitialDisplayStateOff();
    void testWhenDisplaySwitchedOnRenderAccumulatedChanges();
#endif
};

class RenderTestsHelper
{
public:
    RenderTestsHelper();
    ~RenderTestsHelper();
    QList<QRectF>* setupRenderTests(Ut_StatusAreaRenderer* testClass, StatusAreaRenderer* statusAreaRenderer);
private:
    QList<QRectF>* rectList;
    void setupRect();
};

#endif //_UT_STATUSAREARENDERER_
