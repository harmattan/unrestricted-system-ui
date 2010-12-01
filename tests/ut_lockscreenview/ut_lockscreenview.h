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
**
****************************************************************************/
#ifndef UT_LOCKSCREENVIEW_H
#define UT_LOCKSCREENVIEW_H

#include <QtTest/QtTest>
#include <QObject>
#include <MSceneWindow>
#include "lockscreenview.h"

class MApplication;

class Ut_LockScreenView : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void testGConfKeyConnections();
    void testUpdatePixmapWithNoImage();
    void testUpdatePixmapFromTheme();
    void testUpdatePixmapFromFile();
    void testInitialImageLoading();
    void testDrawBackgroundWithDefaultPixmap();
    void testDrawBackgroundWithLoadedPixmap_data();
    void testDrawBackgroundWithLoadedPixmap();

private:
    MApplication *app;
    MWidgetController *controller;
    LockScreenView *lockScreenView;
};

#endif
