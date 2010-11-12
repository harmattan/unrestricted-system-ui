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
#ifndef FT_SHUTDOWNUI_H
#define FT_SHUTDOWNUI_H

#include <QtTest/QtTest>
#include <QObject>
#include "xchecker.h"

class MApplication;
class MApplicationWindow;
class ShutdownUI;

class Ft_ShutdownUI : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testConstructDestruct ();
    void testShutdownUIShowHide ();

private:
    XChecker                   m_XChecker;
    MApplicationWindow        *m_MainWindow;
    MApplication              *m_App;
    ShutdownUI                *m_ShutDownUI;
};

#endif

