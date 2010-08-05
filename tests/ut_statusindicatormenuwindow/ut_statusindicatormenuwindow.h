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
#ifndef _UT_STATUSINDICATORMENUWINDOW_
#define _UT_STATUSINDICATORMENUWINDOW_

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include "statusindicatormenuwindow.h"

class StatusIndicatorMenuWindow;
class MApplication;

class Ut_StatusIndicatorMenuWindow : public QObject
{
    Q_OBJECT

signals:
    void clicked();
    void settingsButtonClicked();
    void clicked(QPointF point);
    void positionOrSizeChanged();
    void displayExited();

private:
    StatusIndicatorMenuWindow *statusIndicatorMenuWindow;
    MApplication *app;

private slots:
    // Executed once before every test case
    void init();
    // Executed once after every test case
    void cleanup();
    // Executed once before first test case
    void initTestCase();
    // Executed once after last test case
    void cleanupTestCase();

    // Test cases
    void testMakeVisible();
    void testCloseButtonOverlay();
    void testSettingsButtonClicked();
    void testWhenPressedBelowMenuContentsThenWindowShouldHide();
    void testCloseButtonPosition();
    void testSetPannability();
    void testWindowType();
    void testPannableAreaBackgroundWidget();
    void testTopRowInitialization();
    void testVerticalExtensionArea();
    void testWhenFullScreenWindowComesOnTopStatusMenuIsClosed();
};

#endif //_UT_STATUSINDICATORMENUWINDOW_
