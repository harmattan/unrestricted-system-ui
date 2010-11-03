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
#ifndef FT_SHUTDOWNBUSINESSLOGIC_H
#define FT_SHUTDOWNBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>
#include "xchecker.h"

class MApplication;
class MApplicationWindow;

class Ft_ShutdownBusinessLogic : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

#ifdef HAVE_QMSYSTEM
    void testThermalState ();
    void testShutDownDenied ();
    void testBatteryStateEmpty ();
    void testShutDown ();
#endif
    
private:
    XChecker                   m_XChecker;
    MApplication              *m_App;
};

#endif


