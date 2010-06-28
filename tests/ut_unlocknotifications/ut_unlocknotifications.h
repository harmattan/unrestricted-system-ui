/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
/***************************************************************************
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
#ifndef UT_UNLOCKNOTIFICATIONS_H
#define UT_UNLOCKNOTIFICATIONS_H

#include <QObject>
#include <QGraphicsLayoutItem>

#include "../common/MTester.h"

#include "unlocknotifications.h"

class MApplication;

class SignalSink : public QObject
{
    Q_OBJECT

public:
    SignalSink ();

public slots:
    void needToShow (bool visible);

public:
    bool m_NeedToShow;
    bool m_NeedToShowCame;
};

class Ut_UnlockNotifications : public QObject
{
    Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testDefaultValues ();
    void testOrientationChanged ();
    void testUpdateContentsWithNothing ();
    void testUpdateContentsWithOneSMS ();
    void testUpdateContentsWithManySMS ();
    void testUpdateContentsWithOneCall ();
    void testUpdateContentsWithManyCall ();
    void testUpdateContentsWithManyEmail ();
    void testUpdateContentsWithManyMessages ();

private:
    QString notificationLabelText (int nth);
    QString notificationIconName (int nth);
    QString notificationLastSubject ();

    MTester                 m_Tester;
    MApplication           *app;
    UnlockNotifications    *m_Subject;
    SignalSink              m_SignalSink;
};

#endif

