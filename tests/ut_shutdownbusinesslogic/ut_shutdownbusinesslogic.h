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
#ifndef UT_SHUTDOWNBUSINESSLOGIC_H
#define UT_SHUTDOWNBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>
#include <QPointer>

class MApplication;
class MApplicationWindow;
class ShutdownBusinessLogic;

class ShutdownUI : public QObject 
{
    Q_OBJECT

public:
    ShutdownUI ();
    bool showWindow (const QString &text1, const QString &text2, int delay);

    QString m_Text1;
    QString m_Text2;
    int     m_Delay;
};

class Ut_ShutdownBusinessLogic : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    
    void testShowUI ();
#ifdef HAVE_QMSYSTEM
    void testSystemStateChanged ();
    void testShutdownWindow();
#endif
    
private:
    MApplication          *m_App;
    QPointer <ShutdownBusinessLogic> m_Api;
};

#endif

