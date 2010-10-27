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
#ifndef UT_LOCKSCREENWITHPADLOCKVIEW_H
#define UT_LOCKSCREENWITHPADLOCKVIEW_H

#include <QtTest/QtTest>
#include <QObject>
#include <MSceneWindow>

class MApplication;
class LockScreenWithPadlockView;
class LockScreen;

class Ut_LockScreenWithPadlockView : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void checkOverlayPos(const QPointF &eventPos);

    void testInitialState();
    void testDraggableIconMovement_data();
    void testDraggableIconMovement();
    void testNotificationAreaVisibility();
    void testCreatedItemsAreRemovedFromTheControllerAndTheScene();

private:
    LockScreenWithPadlockView *m_subject;
    MApplication* app;
    LockScreen *controller;
};

#endif
