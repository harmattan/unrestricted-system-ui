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
#ifndef FT_LOCKSCREENUIWIDGET_H
#define FT_LOCKSCREENUIWIDGET_H

#include <QtTest/QtTest>
#include <QObject>
#include "xchecker.h"

class MApplication;
class MApplicationWindow;
class ScreenLockWindow;
class EventEater;

class Ft_LockScreenUI : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testEventEaterUIShowHide ();
    void testLockScreenUIShowHide ();
    void testLockScreenUIShowHideWithMainWindow ();
    void testEventEaterUIShowHideWithMainWindow ();
    void testLockScreenUIWithTSOff ();
    // Not fully implemented yet
    //void testMissedEvents ();
    
private:
    void createLockScreenUI ();
    void createEventEaterUI ();

private:
    XChecker             m_XChecker;
    MApplicationWindow  *m_MainWindow;
    ScreenLockWindow    *m_LockScreenWindow;
    EventEater          *m_EventEaterUI;
    MApplication        *m_App;
};

#endif
