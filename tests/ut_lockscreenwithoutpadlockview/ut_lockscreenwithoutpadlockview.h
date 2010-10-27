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
class LockScreenWithoutPadlockView;
class LockScreen;

class Ut_LockScreenWithoutPadlockView : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testNotificationArea();
    void testCreatedItemsAreRemovedFromTheControllerAndTheScene();

private:
    LockScreenWithoutPadlockView *m_subject;
    MApplication* app;
    LockScreen *controller;
};

#endif
