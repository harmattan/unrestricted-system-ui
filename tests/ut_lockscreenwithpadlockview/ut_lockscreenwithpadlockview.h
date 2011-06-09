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
#ifndef UT_LOCKSCREENWITHPADLOCKVIEW_H
#define UT_LOCKSCREENWITHPADLOCKVIEW_H

#include <QObject>

class MApplication;
class LockScreenWithPadlockView;
class LockScreen;
class NotificationManagerInterface;
class QPointF;

class Ut_LockScreenWithPadlockView : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testInitialState();
    void testDraggableIconMovement_data();
    void testDraggableIconMovement();
    void testNotificationAreaVisibility();
    void testCreatedItemsAreRemovedFromTheControllerAndTheScene();

private:
    void checkOverlayPos(const QPointF &eventPos);

    LockScreenWithPadlockView *m_subject;
    MApplication *app;
    NotificationManagerInterface *notificationManager;
    LockScreen *controller;
};

#endif
