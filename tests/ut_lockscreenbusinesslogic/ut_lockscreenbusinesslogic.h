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

#include <QObject>

class MApplication;
class MApplicationWindow;

class Ut_LockScreenBusinessLogic : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testToggleScreenLockUI();
    void testToggleEventEater();
    void testUnlockScreen();
    void testHideEventEater();
#ifdef HAVE_QMSYSTEM
    void testDisplayStateChanged();
#endif

private:
    MApplication             *m_App;
};

#endif

