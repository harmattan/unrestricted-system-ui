/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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

#include "ut_lockscreenui.h"
#include "unlockwidgets.h"
#include "lockscreenui.h"
#include "unlockmissedevents_stub.h"
#include "sysuid_stub.h"

#include <QEvent>
#include <QShowEvent>
#include <QVariant>
#include <QPixmap>

#include <MApplication>
#include <MWindow>
#include <MGConfItem>
#include <MTheme>

#define GCONF_BG_LANDSCAPE \
    "/desktop/meego/background/landscape/picture_filename"
#define GCONF_BG_PORTRAIT \
    "/desktop/meego/background/portrait/picture_filename"

#define DEBUG
#include "../../src/debug.h"

/******************************************************************************
 * The stub for MFeedback class.
 */
#include <MFeedback>

static QString nameOfLastFeedback;

void
MFeedback::play () const 
{
    /*
     * Seems that the MWindow sends empty name feedbacks sometimes, we filter
     * those out. I hope these will not cause problems in the future... but we
     * can filter them by name too...
     */
    if (name().isEmpty())
        return;

    SYS_DEBUG ("*** name = %s", SYS_STR(name()));
    nameOfLastFeedback = name();
}

/******************************************************************************
 * Stub for the MGConfItem
 */
QVariant
MGConfItem::value () const 
{
    SYS_DEBUG ("*** key   = %s", SYS_STR(key()));
    return QVariant(QString("/some/path/to/image.img"));
}

#if 0
bool considerLockScreenUIVisible = false;

bool 
MWindow::isVisible ()
{
    return considerLockScreenUIVisible;
}
#endif

/******************************************************************************
 * Stub for the QPixmap
 */
bool failImageLoads = false;

bool
QPixmap::load (
        const QString  &fileName, 
        const char     *format, 
        Qt::ImageConversionFlags flags)
{
    Q_UNUSED (format);
    Q_UNUSED (flags);
    SYS_DEBUG ("*** fileName = %s", SYS_STR(fileName));

    if (failImageLoads)
        return false;
    
    *this = QPixmap (100, 100);

    return true;
}

/******************************************************************************
 * Stub for the QPainter
 */
QPixmap* qpainter_pixmap = 0;

void
QPainter::drawPixmap (const QRectF &targetRect, const QPixmap &pixmap, const QRectF &sourceRect)
{
    SYS_DEBUG ("");
    Q_UNUSED (targetRect);
    Q_UNUSED (sourceRect);

    qpainter_pixmap = (QPixmap *) &pixmap;
}

/******************************************************************************
 * The helper class to watch the signals.
 */
LockScreenUIEventSink::LockScreenUIEventSink() :
    m_OneInputCame (false),
    m_UnlockedCame (false)
{
}

void
LockScreenUIEventSink::OneInput ()
{
    m_OneInputCame = true;
}

void
LockScreenUIEventSink::unlocked ()
{
    m_UnlockedCame = true;
}

/******************************************************************************
 * The Ut_LockScreenUI implements the unit tests.
 */
void Ut_LockScreenUI::init()
{
}

void Ut_LockScreenUI::cleanup()
{
}


int   argc = 1;
char *argv[] = {
    (char *) "./ut_lockscreenui",
    NULL };

void Ut_LockScreenUI::initTestCase()
{
    m_EventEaterUI = 0;
    m_LockScreenUI = 0;
    m_MainWindow = 0;

    SYS_DEBUG ("+++ Creating application.");
    m_App = new MApplication(argc, argv);
    m_App->setQuitOnLastWindowClosed (false);
}

void 
Ut_LockScreenUI::cleanupTestCase()
{
    if (m_LockScreenUI)
        delete m_LockScreenUI;

    if (m_EventEaterUI)
        delete m_EventEaterUI;

    if (m_MainWindow)
        delete m_MainWindow; 

    m_App->deleteLater ();
}

/*
 * The LockScreenWindow is a simple window that shows two sperate images, one
 * for portrait and one for landscape. 
 */
void 
Ut_LockScreenUI::testLockScreenWindow ()
{
    MWindow          *parentWindow;
    UnlockHeader     *lockLiftArea;
    UnlockArea       *lockLandArea;
    LockScreenWindow *window;
    bool              connectSuccess;

    /*
     * Creating the window and doing some basic checks.
     */
    parentWindow = new MWindow;
    lockLiftArea = new UnlockHeader ();
    lockLandArea = new UnlockArea ();
    
    window = new LockScreenWindow (parentWindow, lockLiftArea, lockLandArea);
    connectSuccess = connect (
            window, SIGNAL(unlocked()),
            &m_EventSink, SLOT(unlocked()));

    m_EventSink.m_UnlockedCame = false;
    QVERIFY(connectSuccess);
    QVERIFY (window->m_Window);
    /*
     * Checking if the window is watching the right gconf keys.
     */
    QVERIFY (window->m_confBgLandscape != NULL);
    QVERIFY (window->m_confBgPortrait != NULL);
    QVERIFY (window->m_confBgLandscape->key() == GCONF_BG_LANDSCAPE);
    QVERIFY (window->m_confBgPortrait->key() == GCONF_BG_PORTRAIT);

    /*
     * Checking what happens when the image loads failed.
     */
    failImageLoads = true;
    window->reloadLandscapeBackground ();
    window->reloadPortraitBackground ();
    failImageLoads = false;
    // Should check something...

    /*
     * Checking if the background images are loaded with the right size. The
     * QPixmap::load() method is stubbed and so is the MGConfItem, so we can do
     * this.
     */
//    #ifdef LOT_DEBUG
    SYS_DEBUG ("*** m_bgLandscape size = %dx%d", 
            window->m_bgLandscape.width(),
            window->m_bgLandscape.height());
    SYS_DEBUG ("*** m_bgPortrait  size = %dx%d", 
            window->m_bgPortrait.width(),
            window->m_bgPortrait.height());
//    #endif
    QCOMPARE (window->m_bgLandscape.width(), 100);
    QCOMPARE (window->m_bgLandscape.height(), 100);
    
    QCOMPARE (window->m_bgPortrait.width(), 100);
    QCOMPARE (window->m_bgPortrait.height(), 100);

    /*
     * Creating some mouse events that will be sent to the window.
     */
    QGraphicsSceneMouseEvent *pressEvent;
    QGraphicsSceneMouseEvent *moveEvent;
    QGraphicsSceneMouseEvent *releaseEvent;
    QGraphicsSceneMouseEvent *moveEventNotActive;
    QPointF                   pressAt (window->m_LockLiftArea->preferredWidth (), 
                                       window->m_LockLiftArea->pos ().y ());
    QPointF                   moveToNotActive (400, 10);
    QPointF                   moveTo (400, 240);

    /*
     * Now we send three mouse events that ultimately will send the unlocked()
     * signal from the window.
     */
    pressEvent = new QGraphicsSceneMouseEvent (QEvent::GraphicsSceneMousePress);
    pressEvent->setPos (pressAt);
    
    moveEvent = new QGraphicsSceneMouseEvent (QEvent::GraphicsSceneMouseMove);
    moveEvent->setPos (moveTo);

    moveEventNotActive = new QGraphicsSceneMouseEvent (QEvent::GraphicsSceneMouseMove);
    moveEvent->setPos (moveToNotActive);

    releaseEvent = new QGraphicsSceneMouseEvent (
            QEvent::GraphicsSceneMouseRelease);

    // Sending a press event the place where it should activate the draggable
    // icon.
    window->mousePressEvent (pressEvent);
    QCOMPARE (window->m_DnDstate, (int) LockScreenWindow::STATE_MOVING);
    QVERIFY (nameOfLastFeedback == "start-dragndrop");

    //
    // Then we move the mouse right into the middle of the screen
    window->mouseMoveEvent (moveEvent);
    QCOMPARE (window->m_DnDstate, (int) LockScreenWindow::STATE_MOVING_ACTIVE);
    QVERIFY (nameOfLastFeedback == "enter-dragndrop-dropzone");

    // Move back to some non-active place
    window->mouseMoveEvent (moveEventNotActive);
    QCOMPARE (window->m_DnDstate, (int) LockScreenWindow::STATE_MOVING);
    QVERIFY (nameOfLastFeedback == "exit-dragndrop-dropzone");

    // ... again move to active area:
    window->mouseMoveEvent (moveEvent);
    QCOMPARE (window->m_DnDstate, (int) LockScreenWindow::STATE_MOVING_ACTIVE);
    QVERIFY (nameOfLastFeedback == "enter-dragndrop-dropzone");

    //
    // And then the mouse is released. And this concludes the interaction, the
    // unlocked() signal should be sent.
    //
    window->mouseReleaseEvent (releaseEvent);
    QVERIFY (m_EventSink.m_UnlockedCame);
    QVERIFY (nameOfLastFeedback == "release-inside-dragndrop-dropzone");

    /*
     * Test also the RTL layout direction
     */
    lockLiftArea->setLayoutDirection (Qt::RightToLeft);

    pressAt = QPointF (30., 30.);

    pressEvent = new QGraphicsSceneMouseEvent (QEvent::GraphicsSceneMousePress);
    pressEvent->setPos (pressAt);
    
    moveEvent = new QGraphicsSceneMouseEvent (QEvent::GraphicsSceneMouseMove);
    moveEvent->setPos (moveTo);

    releaseEvent = new QGraphicsSceneMouseEvent (
            QEvent::GraphicsSceneMouseRelease);

    // Sending a press event the place where it should activate the draggable
    // icon.
    window->mousePressEvent (pressEvent);
    QVERIFY (window->m_DnDstate == LockScreenWindow::STATE_MOVING);
    QVERIFY (nameOfLastFeedback == "start-dragndrop");

    //
    // Then we move the mouse right into th emiddle of the screen
    window->mouseMoveEvent (moveEvent);
    QVERIFY (nameOfLastFeedback == "enter-dragndrop-dropzone");

    //
    // And then the mouse is released. And this concludes the interaction, the
    // unlocked() signal should be sent.
    //
    window->mouseReleaseEvent (releaseEvent);
    QVERIFY (nameOfLastFeedback == "release-inside-dragndrop-dropzone");

    QVERIFY (m_EventSink.m_UnlockedCame);

    // Set the layout dir. back to LTR
    lockLiftArea->setLayoutDirection (Qt::LeftToRight);


#if 0
    // FIXME: It seems geometry cannot be changes by some reason

    // Check the packground painter:
    QPainter painter;
    window->m_bgLandscape = QPixmap (200, 100);
    window->m_bgPortrait = QPixmap (100, 200);

    // Landscape:
    window->setGeometry (QRectF (0, 0, 200, 100));

    window->paint (&painter, 0, 0);
    QVERIFY (qpainter_pixmap != 0);
    QCOMPARE (qpainter_pixmap->size ().width (), 200);

    SYS_DEBUG ("size: %2f, %2f", window->geometry ().width (), window->geometry ().height ());

    // Portrait:
    window->setGeometry (QRectF (0, 0, 100, 200));

    window->paint (&painter, 0, 0);
    QVERIFY (qpainter_pixmap != 0);
    QCOMPARE (qpainter_pixmap->size ().width (), 100);

    SYS_DEBUG ("size: %2f, %2f", window->geometry ().width (), window->geometry ().height ());
#endif

    delete window;
    delete parentWindow;
    delete lockLiftArea;
    delete lockLandArea;
}

void
Ut_LockScreenUI::testLockScreenUI ()
{
    Q_ASSERT (m_LockScreenUI == 0);
    
    /*
     * When the lockscreenUI is created it just delays its realize function. We
     * wait a small amount of time then we chack that the internal widgets are
     * created. For this delayed realization the widget does not need to be
     * shown, so the widget will be shown fast even at the first time.
     */
    m_LockScreenUI = new LockScreenUI;
    QTest::qWait (30);

    QVERIFY (m_LockScreenUI->m_Realized);
    QVERIFY (m_LockScreenUI->m_policy != NULL);
    QVERIFY (m_LockScreenUI->m_LockLiftArea != NULL);
    QVERIFY (m_LockScreenUI->m_LockLandArea != NULL);
    QVERIFY (m_LockScreenUI->m_SceneWindow != NULL);

    delete m_LockScreenUI;

    /*
     * Now let's do it with the early manual realization. The createContent()
     * hopefully will be called when the window shown...
     */
    m_LockScreenUI = new LockScreenUI;
    m_LockScreenUI->createContent();
    QVERIFY (m_LockScreenUI->m_Realized);
    QVERIFY (m_LockScreenUI->m_policy != NULL);
    QVERIFY (m_LockScreenUI->m_LockLiftArea != NULL);
    QVERIFY (m_LockScreenUI->m_LockLandArea != NULL);
    QVERIFY (m_LockScreenUI->m_SceneWindow != NULL);

    /*
     * Check whether the reset-state of DnD works properly
     */
    // Set the DnDstate to icon-moving state...
    m_LockScreenUI->m_SceneWindow->m_DnDstate =
        LockScreenWindow::STATE_MOVING;

    // Call reset function
    m_LockScreenUI->reset ();

    QVERIFY (m_LockScreenUI->m_SceneWindow->m_DnDstate ==
             LockScreenWindow::STATE_NONE);

    // FIXME: I will enhance this part... 
    //m_LockScreenUI->showEvent (NULL);
    //considerLockScreenUIVisible = true;
    m_LockScreenUI->showHideNotifications (true);
    //QVERIFY (m_LockScreenUI->m_notificationArea->isVisible());

    m_LockScreenUI->showHideNotifications (false);
    //QVERIFY (!m_LockScreenUI->m_notificationArea->isVisible());
    delete m_LockScreenUI;
    
    /*
     * Test the extreme case of the early destruction of the object. We had a
     * creash in this case, this is what checked here.
     */
    m_LockScreenUI = new LockScreenUI;
    delete m_LockScreenUI;
}

/*
 * This test will check if the lockscreenui will send the signal when the
 * lockscreenwindow emits the unlocked() signal.
 */
void
Ut_LockScreenUI::testLockScreenUISignals ()
{
    bool              connectSuccess;

    m_LockScreenUI = new LockScreenUI;
    m_LockScreenUI->createContent();
    connectSuccess = connect (m_LockScreenUI, SIGNAL(unlocked()),
            &m_EventSink, SLOT(unlocked()));
    QVERIFY(connectSuccess);
    m_EventSink.m_UnlockedCame = false;

    
    emit m_LockScreenUI->m_SceneWindow->unlocked();
    QVERIFY (m_EventSink.m_UnlockedCame);

    delete m_LockScreenUI;
}


void
Ut_LockScreenUI::testLockScreenUIWindowName ()
{
    Q_ASSERT (m_LockScreenUI == 0);
    
    m_LockScreenUI = new LockScreenUI;

    /*
     * I'm not sure what could we test here except that the program will not
     * crash if the event is invalid and the internal window ID is also 
     * invalid.
     */
    QShowEvent event;
    m_LockScreenUI->showEvent(&event);

    delete m_LockScreenUI;
}

void
Ut_LockScreenUI::testEventEaterUI ()
{
    bool connectSuccess;

    /*
     * We need a new event eater widget.
     */
    Q_ASSERT (m_EventEaterUI == 0);
    m_EventEaterUI = new EventEaterUI;
    
    /*
     * We test if we can connect to the OneInput() signal.
     */
    connectSuccess = connect (m_EventEaterUI, SIGNAL(OneInput()),
            &m_EventSink, SLOT(OneInput()));
    QVERIFY (connectSuccess);
    
    /*
     * Let's see if the event is actually generates a signal. 
     */
    m_EventSink.m_OneInputCame = false;
    m_EventEaterUI->mousePressEvent ((QMouseEvent *)NULL);
    QVERIFY (m_EventSink.m_OneInputCame);
    
    m_EventSink.m_OneInputCame = false;
    m_EventEaterUI->mouseReleaseEvent ((QMouseEvent *)NULL);
    QVERIFY (m_EventSink.m_OneInputCame);

    delete m_EventEaterUI;
    m_EventEaterUI = 0;
}

void
Ut_LockScreenUI::testEventEaterUIWindowName ()
{
    /*
     * We need a new event eater widget.
     */
    Q_ASSERT (m_EventEaterUI == 0);
    m_EventEaterUI = new EventEaterUI;

    /*
     * I'm not sure what could we test here except that the program will not
     * crash if the event is invalid and the internal window ID is also 
     * invalid.
     */
    QShowEvent event;
    m_EventEaterUI->showEvent(&event);

    delete m_EventEaterUI;
    m_EventEaterUI = 0;
}

QTEST_APPLESS_MAIN(Ut_LockScreenUI)
