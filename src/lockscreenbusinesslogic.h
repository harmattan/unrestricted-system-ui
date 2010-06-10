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

#include <QObject>
#include <QTimer>

#ifdef HAVE_QMSYSTEM
#include <qmdisplaystate.h>
#endif

class LockScreenUI;
class EventEaterUI;

class LockScreenBusinessLogic : public QObject
{
    Q_OBJECT

public:
    LockScreenBusinessLogic (QObject* parent = 0);
    virtual ~LockScreenBusinessLogic ();

public slots:
    void toggleScreenLockUI (bool toggle);
    void toggleEventEater   (bool toggle);

private slots:
    void unlockScreen();
    void hideEventEater();
#ifdef HAVE_QMSYSTEM
    void displayStateChanged (Maemo::QmDisplayState::DisplayState state);
#endif

signals:
    void updateTime ();
    void unlockConfirmed ();
    void screenIsLocked (bool locked);

private: //methods
    void mayStartTimer();
    void mayStopTimer();
    bool displayIsOn ();

private: 
    LockScreenUI     *lockUI;
    EventEaterUI     *eaterUI;
#ifdef HAVE_QMSYSTEM
    Maemo::QmDisplayState    m_QmDisplay;
#endif
    QTimer            timer;
#ifdef UNIT_TEST
    friend class Ut_LockScreenBusinessLogic;
    friend class Ft_LockScreenBusinessLogic;
#endif

};

#endif
