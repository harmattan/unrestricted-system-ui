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

#ifndef UT_PLUGINLIST_H
#define UT_PLUGINLIST_H

#include <QtTest/QtTest>
#include <QObject>
#include <MStatusIndicatorMenuPluginInterface>

class MApplication;
class StatusIndicatorMenuWindow;
class PluginList;

class TestPlugin : public QObject, MStatusIndicatorMenuPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(MStatusIndicatorMenuPluginInterface)

public:
    virtual MWidget *constructWidget(MStatusIndicatorMenuInterface &);
};

class Ut_PluginList : public QObject
{
    Q_OBJECT

public:
    static bool windowMinimized;
    static QStringList loadedPlugins;

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test initialization
    void testInitialization();
    // Test returning from a subpage
    void testShowStatusIndicatorMenu();
    // Test hiding the application
    void testHideStatusIndicatorMenu();

private:
    // MApplication
    MApplication *app;
    // Window for testing
    StatusIndicatorMenuWindow *window;
    // The object being tested
    PluginList *pluginList;
};

#endif
