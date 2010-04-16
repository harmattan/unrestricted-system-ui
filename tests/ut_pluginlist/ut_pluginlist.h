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
/*
 * ut_pluginlist.h
 *
 * This file is part of system ui
 *
 * Copyright (C) 2009 Nokia Corporation. All rights reserved.
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved.
 * Copying, including reproducing, storing, adapting or translating,
 * any or all of this material requires the prior written consent of
 * Nokia Corporation. This material also contains confidential
 * information which may not be disclosed to others without the prior
 * written consent of Nokia.
 */

#ifndef UT_PLUGINLIST_H
#define UT_PLUGINLIST_H

#include <QtTest/QtTest>
#include <QObject>
#include <MStatusIndicatorMenuPluginInterface>

class MApplication;
class MApplicationWindow;
class MApplicationPage;
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
    static bool mApplicationIfProxyLaunchCalled;
    static bool applicationWindowMinimized;
    static bool applicationPageShown;
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
    // Test clicking the settings button
    void testSettingsButtonClicked();
    // Test that the notification area shows/hides based on available notifications
    void testNotificationAreaVisibility();

signals:
    void settingsButtonClicked();

private:
    // MApplication
    MApplication *app;
    // MApplicationWindow for testing
    MApplicationWindow *applicationWindow;
    // MApplicationPage for testing
    MApplicationPage *applicationPage;
    // The object being tested
    PluginList *pluginList;
};

#endif
