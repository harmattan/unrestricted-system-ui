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
#ifndef LOCKSCREENBUSINESSLOGIC_H
#define LOCKSCREENBUSINESSLOGIC_H

#include <qmsystem/qmlocks.h>
#include <qmsystem/qmdisplaystate.h>
//#include "eventeater.h"

#include <QObject>
#include <QTimer>

class LockScreenUI;
class QDBusInterface;

using namespace Maemo;

class LockScreenBusinessLogic : public QObject
{
    Q_OBJECT

public:
    LockScreenBusinessLogic (QObject* parent = 0);
    virtual ~LockScreenBusinessLogic ();

public slots:
    // from dbus
    void updateMissedEventAmounts(int, int, int, int);

    void toggleScreenLockUI (bool toggle);
    void toggleEventEater   (bool toggle);

private slots:
    //void displayStateChanged(Maemo::QmDisplayState::DisplayState state);
    //void locksChanged(Maemo::QmLocks::Lock what, Maemo::QmLocks::State how);
    void unlockScreen();

signals:
    void updateTime ();
    void unlockConfirmed ();

private: //methods
    //void toggleScreenLockUI(bool toggle);
    void mayStartTimer();
    void stopTimer();
    void hidefromTaskBar();

private: 
    //QmDisplayState   *display;
    //QmLocks          *locks;
    LockScreenUI     *lockUI;

    QTimer           timer;

    QmLocks::State   knownLock;
    QmDisplayState::DisplayState knownDisplay;
};

#endif // LOCKSCREENBUSINESSLOGIC_H
