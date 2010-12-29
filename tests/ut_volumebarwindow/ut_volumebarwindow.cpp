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
#include "ut_volumebarwindow.h"
#include <volumebarwindow.h>
#include <volumebarlogic_stub.h>
#include <volumebar_stub.h>
#include <MApplication>
#include <MWindow>

bool isWindowVisible = false;

void MWindow::setVisible(bool visible)
{
    isWindowVisible = visible;
}

void Ut_VolumeBarWindow::init()
{
    volumeBarWindow = new VolumeBarWindow(volumeBarLogic);
}

void Ut_VolumeBarWindow::cleanup()
{
    delete volumeBarWindow;
}

void Ut_VolumeBarWindow::initTestCase()
{
    static int argc = 1;
    static char *argv[] = { (char *) "./ut_volumebarwindow", 0 };
    app = new MApplication(argc, argv);
    volumeBarLogic = new VolumeBarLogic;
}

void Ut_VolumeBarWindow::cleanupTestCase()
{
    delete volumeBarLogic;
    delete app;
}

void Ut_VolumeBarWindow::testUpdateVolume()
{
    gVolumeBarLogicStub->stubSetReturnValue("volume", 1u);
    gVolumeBarLogicStub->stubSetReturnValue("maxVolume", 3u);
    volumeBarWindow->updateVolume();
    QCOMPARE(gVolumeBarStub->stubCallCount("setTargetPercentage"), 1);
    QCOMPARE(gVolumeBarStub->stubLastCallTo("setTargetPercentage").parameter<qreal>(0), 0.5);
}

void Ut_VolumeBarWindow::testVolumeBarChanged()
{
    gVolumeBarLogicStub->stubSetReturnValue("maxVolume", 65u);
    volumeBarWindow->setVolume(0.5);
    QCOMPARE(gVolumeBarLogicStub->stubCallCount("setVolume"), 1);
    QCOMPARE(gVolumeBarLogicStub->stubLastCallTo("setVolume").parameter<quint32>(0), 32u);
}

QTEST_APPLESS_MAIN(Ut_VolumeBarWindow)
