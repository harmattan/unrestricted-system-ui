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
#include "ut_volumeoverlay.h"
#include <volumeoverlay.h>
#include <QDebug>
#include <MSceneWindow>
#include <MApplication>
#include <MSceneManager>
#include <QGraphicsLinearLayout>
#include <MWindow>

int   argc = 1;
char *argv[] = {
    (char *) "./ut_volumeoverlay",
    NULL };

/*********************************************************************************
 * Stub for QTimer
 */
int t_interval = 0;
bool t_isStarted = false;

QTimer::QTimer(QObject *parent)
{
    Q_UNUSED(parent);
    QTest::qWarn("QTimer::QTimer");
}

void
QTimer::setInterval(int msec)
{
    t_interval = msec;
    QTest::qWarn("QTimer::setInterval");
}

void
QTimer::start()
{
    t_isStarted = true;
    QTest::qWarn("QTimer::start");
}

void
QTimer::stop()
{
    t_isStarted = false;
    QTest::qWarn("QTimer::stop");
}

/*********************************************************************************
 * Stub for MWindow
 */
M::Orientation mw_orientation = M::Portrait;
QSize
MWindow::visibleSceneSize(M::Orientation orientation) const
{
    mw_orientation = orientation;
    QTest::qWarn("MWindow::visibleSceneSize");

    return QSize (100, 100);
}

/*********************************************************************************
 * The Ut_VolumeOverlay implements the unit tests.
 */
void
Ut_VolumeOverlay::init ()
{
    t_interval = 0;
    t_isStarted = false;
    mw_orientation = M::Portrait;
}

void
Ut_VolumeOverlay::cleanup ()
{

}

void
Ut_VolumeOverlay::initTestCase ()
{
    QTest::qWarn("+++ Creating application.");
    m_App = new MApplication(argc, argv);
    m_App->setQuitOnLastWindowClosed (false);

    m_Api = new VolumeOverlay();
    QVERIFY(m_Api->m_timer != NULL);
    QVERIFY(m_Api->m_slider != NULL);
    QVERIFY(m_Api->m_window != NULL);
}

void
Ut_VolumeOverlay::testUpdateVolume ()
{
    m_Api->UpdateVolume(20, 100);
    QCOMPARE (m_Api->m_value, 20);
    QCOMPARE (m_Api->m_valueMax, 100);
    QVERIFY(t_isStarted);
}

void
Ut_VolumeOverlay::testHideMe()
{
    m_Api->hideWindow();

    QCOMPARE(m_Api->m_window->isVisible (), false);
}

void
Ut_VolumeOverlay::cleanupTestCase ()
{
    delete m_Api;
}

QTEST_APPLESS_MAIN(Ut_VolumeOverlay)

