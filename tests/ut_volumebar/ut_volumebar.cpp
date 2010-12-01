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
#include "ut_volumebar.h"
#include <volumebar.h>
#include <QHash>
#include <QDebug>
#include <MWidget>
#include <MApplication>
#include <MSceneManager>
#include <QGraphicsSceneMouseEvent>
#include <QAbstractAnimation>

int   argc = 1;
char *argv[] = {
    (char *) "./ut_volumebar",
    NULL };

/*********************************************************************************
 * Stub for QAbstractAnimation
 */
QHash<QString,bool> animsRunning;

void QAbstractAnimation::start(QAbstractAnimation::DeletionPolicy policy)
{
    Q_UNUSED(policy);
    animsRunning[objectName()] = true;
}

void QAbstractAnimation::stop()
{
    animsRunning[objectName()] = false;
}

/*********************************************************************************
 * Stub for MWidget
 */
bool mwidget_isondisplay = false;

MSceneManager *sceneManagerInstance = 0;

bool MWidget::isOnDisplay () const
{
    return mwidget_isondisplay;
}

MSceneManager * MWidget::sceneManager () const
{
    return sceneManagerInstance;
}

/*********************************************************************************
 * The Ut_VolumeBar implements the unit tests.
 */
void Ut_VolumeBar::init ()
{
    volumeBar = new VolumeBar();

    // set animsRunning hash table up,
    // and animation object names ...
    volumeBar->animMove->setObjectName ("move");
    animsRunning["move"] = false;
    volumeBar->animFadeIn->setObjectName ("fade-in");
    animsRunning["fade-in"] = false;
    volumeBar->animFadeOut->setObjectName ("fade-out");
    animsRunning["fade-out"] = false;
}

void Ut_VolumeBar::cleanup ()
{
    delete volumeBar;
}

void Ut_VolumeBar::initTestCase ()
{
    QTest::qWarn("+++ Creating application.");
    app = new MApplication(argc, argv);
    sceneManagerInstance = new MSceneManager;
}

void Ut_VolumeBar::cleanupTestCase ()
{
    delete sceneManagerInstance;
    app->deleteLater ();
}

void Ut_VolumeBar::testUpdateVolume ()
{
    volumeBar->updateVolume(20, 100);

    QCOMPARE (volumeBar->value, 20);
    QCOMPARE (volumeBar->valueMax, 100);

    QVERIFY (volumeBar->timer.isActive ());
}

void Ut_VolumeBar::testChangeVolume()
{
    QSignalSpy spy (volumeBar, SIGNAL (volumeChanged (int)));
    QList<QVariant> arguments;

    // Make the window visible & set some known value
    volumeBar->updateVolume (5, 10);

    // make a fake-press event
    QGraphicsSceneMouseEvent *pressEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMousePress);
    pressEvent->setPos (QPointF (0, 0));

    // send it
    volumeBar->mousePressEvent (pressEvent);
    QVERIFY(pressEvent->isAccepted());

    // check whether we got a volumeChanged signal
    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    // and the value should be the max (maxcounts - 1) (as we tapped on left-top corner)
    QCOMPARE (arguments[0].toInt (), 9);

    // clean up our 'spy'
    spy.clear ();

    // send a fake mouse-move event (to simulate grabbing...)
    QGraphicsSceneMouseEvent *moveEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseMove);
    moveEvent->setPos (volumeBar->geometry().bottomRight());

    // send it
    volumeBar->mouseMoveEvent (moveEvent);

     // check whether we got a volumeChanged signal
    QCOMPARE (spy.count (), 1);
    arguments = spy.takeFirst ();
    // and the value should be smaller (as we tapped on right-bottom corner)
    QCOMPARE (arguments[0].toInt (), 0);
}

void Ut_VolumeBar::testAnimations()
{
    // at first opacity should be 0.0
    QVERIFY (volumeBar->opacity () <= 0.1);

    // lets update the volume now
    volumeBar->updateVolume (10, 100);

    // fade in animation should run, but fade out shuld be stopped 
    QCOMPARE (animsRunning["fade-in"], true);
    QCOMPARE (animsRunning["fade-out"], false);

    // isOnDisplay => true
    mwidget_isondisplay = true;

    // make a fake-press event
    QGraphicsSceneMouseEvent *pressEvent = new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMousePress);
    pressEvent->setPos (QPointF (0, 0));

    // send it
    volumeBar->mousePressEvent (pressEvent);

    // because we're on display, animation shuld be running
    QCOMPARE (animsRunning["move"], true);

    // stop the previously started animations
    volumeBar->animMove->stop ();
    volumeBar->animFadeIn->stop ();

    // isOnDisplay => false
    mwidget_isondisplay = false;

    // now send the press event without on being display
    volumeBar->mousePressEvent (pressEvent);

    // bar-moving animation should be stopped as we're not visible
    QCOMPARE (animsRunning["move"], false);

    QSignalSpy spy (volumeBar, SIGNAL (animationsFinished ()));
    // simulate the time-out event
    volumeBar->finishAnimations ();

    // animationsFinished should be emitted
    QCOMPARE (spy.count (), 1);
    // and everything should be stopped 
    // && opacity should be 0.0 again
    QCOMPARE (animsRunning["fade-in"], false);
    QCOMPARE (animsRunning["fade-out"], false);
    QCOMPARE (volumeBar->timer.isActive (), false);
    QVERIFY (volumeBar->opacity () <= 0.1);
}

QTEST_APPLESS_MAIN(Ut_VolumeBar)
