/****************************************************************************
**
** Copyright(C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation(directui@nokia.com)
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
#ifndef UT_LOCKSCREENWINDOW_H
#define UT_LOCKSCREENWINDOW_H

#include <QGraphicsWidget>
#include "screenlockextensioninterface.h"

class MApplication;
class ScreenLockWindow;

class LockScreen : public QGraphicsWidget
{
    Q_OBJECT

signals:
    void unlocked();
};

class ScreenLockExtension : public QObject, public ScreenLockExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(ScreenLockExtensionInterface MApplicationExtensionInterface)

public:
    ScreenLockExtension();
    virtual ~ScreenLockExtension();

    //! Methods derived from ScreenLockExtensionInterface
    virtual void reset();
    virtual void setNotificationManagerInterface(NotificationManagerInterface &notificationManager);

    //! Methods derived from MApplicationExtensionInterface
    virtual bool initialize(const QString &interface);
    virtual QGraphicsWidget *widget();

private:
    QGraphicsWidget *widget_;
};

class Ut_LockScreenWindow : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testWhenExtensionIsRegisteredUnlockedSignalFromLockScreenIsChainedToUnlockedSignal();
    void testWhenWindowIsCreatedLockScreenAppears();
    void testWhenWindowIsShownItIsExcludedFromTaskbar();
    void testOrientationLocking_data();
    void testOrientationLocking();
    void testReset();

private:
    MApplication *app;
    ScreenLockWindow *lockScreenWindow;
};

#endif
