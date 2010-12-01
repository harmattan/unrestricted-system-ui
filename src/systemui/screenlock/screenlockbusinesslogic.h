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
#ifndef SCREENLOCKBUSINESSLOGIC_H
#define SCREENLOCKBUSINESSLOGIC_H

#include <QObject>

#ifdef HAVE_QMSYSTEM
#include <qmlocks.h>
#include <qmdisplaystate.h>
#endif

class ScreenLockWindow;
class EventEater;

class ScreenLockBusinessLogic : public QObject
{
    Q_OBJECT

public:
    ScreenLockBusinessLogic(QObject* parent = 0);
    virtual ~ScreenLockBusinessLogic();

public slots:
    void toggleScreenLockUI(bool toggle);
    void toggleEventEater(bool toggle);

private slots:
    void unlockScreen();
    void hideEventEater();

#ifdef HAVE_QMSYSTEM
    void displayStateChanged(MeeGo::QmDisplayState::DisplayState state);
    void locksChanged(MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how);
#endif

signals:
    void updateTime();
    void unlockConfirmed();
    void screenIsLocked(bool locked);

private:
    bool displayIsOn();

    ScreenLockWindow *screenLockWindow;
    EventEater *eventEaterWindow;

#ifdef HAVE_QMSYSTEM
    MeeGo::QmDisplayState displayState;
    MeeGo::QmLocks locks;
#endif

#ifdef UNIT_TEST
    friend class Ut_LockScreenBusinessLogic;
    friend class Ft_LockScreenBusinessLogic;
#endif
};

#endif
