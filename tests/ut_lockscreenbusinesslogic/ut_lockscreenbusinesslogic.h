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
#ifndef UT_LOCKSCREENBUSINESSLOGIC_H
#define UT_LOCKSCREENBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class MApplicationWindow;
class LockScreenBusinessLogic;

class EventSink : public QObject
{
    Q_OBJECT

    
public:
    EventSink ();

public slots:
    void screenIsLocked (bool toggle);
    void timeout ();

public:
    bool         m_ScreenIsLockedCame;
    bool         m_ScreenIsLocked;
    int          m_Timeouts;
};

class Ut_LockScreenBusinessLogic : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testLockScreenBusinessLogicSignals ();
    void testLockScreenBusinessLogicTimer ();
    void testLockScreenBusinessLogicUnlock ();

private:
    EventSink                 m_EventSink;
    MApplicationWindow       *m_MainWindow;
    MApplication             *m_App;
    LockScreenBusinessLogic  *m_LockScreenBusinessLogic;
};

#endif

