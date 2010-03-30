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
 * ut_pluginlist.cpp
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

#include <DuiApplication>
#include <DuiSceneWindow>
#include <DuiApplicationWindow>
#include <DuiApplicationPage>
#include <DuiButton>
#include <QGraphicsLinearLayout>
#include <DuiApplicationIfProxy>
#include "ut_pluginlist.h"
#include "pluginlist.h"
#include "notificationarea_stub.h"

// DuiApplicationIfProxy stubs (used by StatusArea)
QDBusPendingReply<> DuiApplicationIfProxy::launch()
{
    Ut_PluginList::duiApplicationIfProxyLaunchCalled = true;

    return QDBusPendingReply<>();
}

// DuiApplicationPage stubs (used by PluginList)
void DuiSceneWindow::appear(enum DeletionPolicy)
{
    Ut_PluginList::applicationPageShown = true;
}

// QPluginLoader stubs (used by PluginList)
QObject *QPluginLoader::instance()
{
    Ut_PluginList::loadedPlugins.append(fileName());
    return new TestPlugin;
}

DuiWidget *TestPlugin::constructWidget(DuiStatusIndicatorMenuInterface &)
{
    return new DuiWidget;
}

bool Ut_PluginList::duiApplicationIfProxyLaunchCalled;
bool Ut_PluginList::applicationWindowMinimized;
bool Ut_PluginList::applicationPageShown;
QStringList Ut_PluginList::loadedPlugins;

void Ut_PluginList::initTestCase()
{
    int argc = 1;
    char *app_name = (char *)"./ut_pluginlist";
    app = new DuiApplication(argc, &app_name);
    applicationWindow = new DuiApplicationWindow;
    applicationPage = new DuiApplicationPage;
}

void Ut_PluginList::cleanupTestCase()
{
    delete applicationPage;
    delete applicationWindow;
    delete app;
}

void Ut_PluginList::init()
{
    duiApplicationIfProxyLaunchCalled = false;
    applicationPageShown = false;
    loadedPlugins.clear();
    pluginList = new PluginList(applicationWindow, applicationPage);
    connect(this, SIGNAL(settingsButtonClicked()), pluginList, SLOT(settingsButtonClicked()));
}

void Ut_PluginList::cleanup()
{
    delete pluginList;
}

void Ut_PluginList::testInitialization()
{
    // The loaded plugin list should be known
    QCOMPARE(loadedPlugins.count(), 8);
    QCOMPARE(loadedPlugins.at(0), QDir(DUISTATUSINDICATORMENU_PLUGIN_DIR "/libprofile.so").canonicalPath());
    QCOMPARE(loadedPlugins.at(1), QDir(DUISTATUSINDICATORMENU_PLUGIN_DIR "/libdatetime.so").canonicalPath());
    QCOMPARE(loadedPlugins.at(2), QDir(DUISTATUSINDICATORMENU_PLUGIN_DIR "/libclockalarm.so").canonicalPath());
    QCOMPARE(loadedPlugins.at(3), QDir(DUISTATUSINDICATORMENU_PLUGIN_DIR "/libconnectivity.so").canonicalPath());
    QCOMPARE(loadedPlugins.at(4), QDir(DUISTATUSINDICATORMENU_PLUGIN_DIR "/libpresence.so").canonicalPath());
    QCOMPARE(loadedPlugins.at(5), QDir(DUISTATUSINDICATORMENU_PLUGIN_DIR "/libbattery.so").canonicalPath());
    QCOMPARE(loadedPlugins.at(6), QDir(DUISTATUSINDICATORMENU_PLUGIN_DIR "/libvolume.so").canonicalPath());
    QCOMPARE(loadedPlugins.at(7), QDir(DUISTATUSINDICATORMENU_PLUGIN_DIR "/libaccessories.so").canonicalPath());

    // The layout should contain a widget for each plugin plus one widget for the button
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(pluginList->layout());
    QVERIFY(layout != NULL);
    QCOMPARE(layout->count(), loadedPlugins.count() + 1);
}

void Ut_PluginList::testShowStatusIndicatorMenu()
{
    pluginList->showStatusIndicatorMenu();

    QVERIFY(applicationPageShown);
}

void Ut_PluginList::testHideStatusIndicatorMenu()
{
    pluginList->hideStatusIndicatorMenu();

    QVERIFY(!applicationWindow->isVisible());
}

void Ut_PluginList::testSettingsButtonClicked()
{
    emit settingsButtonClicked();

    QVERIFY(duiApplicationIfProxyLaunchCalled);
}

void Ut_PluginList::testNotificationAreaVisibility()
{
    NotificationArea *notificationArea =
            gNotificationAreaStub->stubLastCallTo("notificationAreaConstructor").parameter<NotificationArea*>(0);

    QVERIFY(!notificationArea->isVisible());
    QMetaObject::invokeMethod(notificationArea, "notificationCountChanged", Q_ARG(int, 1));
    QVERIFY(notificationArea->isVisible());
    QMetaObject::invokeMethod(notificationArea, "notificationCountChanged", Q_ARG(int, 10));
    QVERIFY(notificationArea->isVisible());
    QMetaObject::invokeMethod(notificationArea, "notificationCountChanged", Q_ARG(int, 0));
    QVERIFY(!notificationArea->isVisible());
}

QTEST_APPLESS_MAIN(Ut_PluginList)
