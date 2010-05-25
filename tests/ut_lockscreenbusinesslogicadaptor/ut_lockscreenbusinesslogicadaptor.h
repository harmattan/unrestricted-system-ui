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
#ifndef _UT_LOCKSCREENBUSINESSLOGICADAPTOR_H
#define _UT_LOCKSCREENBUSINESSLOGICADAPTOR_H

#include <QtTest/QtTest>
#include <QObject>

#define ADAPTOR_TEST

class LockScreenBusinessLogic;
#include <lockscreenbusinesslogicadaptor.h>

class Ut_LockScreenBusinessLogicAdaptor : public QObject
{
    Q_OBJECT

    private slots:
        void initTestCase ();
        void cleanupTestCase ();

        void MCECallBack ();
        void EventEaterIf ();
        void UnlockScreenIf ();

    private:
        LockScreenBusinessLogic          *m_logic;
        LockScreenBusinessLogicAdaptor   *m_subject;
};

#endif
