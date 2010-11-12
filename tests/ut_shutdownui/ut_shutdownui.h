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
#ifndef UT_SHUTDOWNUI_H
#define UT_SHUTDOWNUI_H

#include <QObject>

class MApplication;
class ShutdownUI;

class Ut_ShutdownUI : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    
    void testInitialization();
    void testRealize();
    void testShowWindow();
    
private:
    MApplication *app;
    ShutdownUI *m_subject;
};

#endif

