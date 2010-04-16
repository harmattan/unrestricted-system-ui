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

#include <MApplication>
#include <MSceneWindow>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MButton>
#include <QGraphicsLinearLayout>
#include <MApplicationIfProxy>
#include "ut_pluginlist.h"
#include "pluginlist.h"
#include "notificationarea_stub.h"

// MApplicationIfProxy stubs (used by StatusArea)
QDBusPendingReply<> MApplicationIfProxy::launch()
{
    Ut_PluginList::mApplicationIfProxyLaunchCalled = true;

    return QDBusPendingReply<>();
}

// MApplicationPage stubs (used by PluginList)
void MSceneWindow::appear(enum DeletionPolicy)
{
    Ut_PluginList::applicationPageShown = true;
}

// QPluginLoader stubs (used by PluginList)
QObject *QPluginLoader::instance()
{
    Ut_PluginList::loadedPlugins.append(fileName());
    return new TestPlugin;
}

MWidget *TestPlugin::constructWidget(MStatusIndicatorMenuInterface &)
{
    return new MWidget;
}

bool Ut_PluginList::mApplicationIfProxyLaunchCalled;
bool Ut_PluginList::applicationWindowMinimized;
bool Ut_PluginList::applicationPageShown;
QStringList Ut_PluginList::loadedPlugins;

void Ut_PluginList::initTestCase()
{
    int argc = 1;
    char *app_name = (char *)"./ut_pluginlist";
    app = new MApplication(argc, &app_name);
    applicationWindow = new MApplicationWindow;
    applicationPage = new MApplicationPage;
}

void Ut_PluginList::cleanupTestCase()
{
    delete applicationPage;
    delete applicationWindow;
    delete app;
}

void Ut_PluginList::init()
{
    mApplicationIfProxyLaunchCalled = false;
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
    QCOMPARE(loadedPlugins.at(0), QDir(STATUSINDICATORMENU_PLUGIN_DIR "/libprofile.so").canonicalPath());
    QCOMPARE(loadedPlugins.at(1), QDir(STATUSINDICATORMENU_PLUGIN_DIR "/libdatetime.so").canonicalPath());
    QCOMPARE(loadedPlugins.at(2), QDir(STATUSINDICATORMENU_PLUGIN_DIR "/libclockalarm.so").canonicalPath());
    QCOMPARE(loadedPlugins.at(3), QDir(STATUSINDICATORMENU_PLUGIN_DIR "/libconnectivity.so").canonicalPath());
    QCOMPARE(loadedPlugins.at(4), QDir(STATUSINDICATORMENU_PLUGIN_DIR "/libpresence.so").canonicalPath());
    QCOMPARE(loadedPlugins.at(5), QDir(STATUSINDICATORMENU_PLUGIN_DIR "/libbattery.so").canonicalPath());
    QCOMPARE(loadedPlugins.at(6), QDir(STATUSINDICATORMENU_PLUGIN_DIR "/libvolume.so").canonicalPath());
    QCOMPARE(loadedPlugins.at(7), QDir(STATUSINDICATORMENU_PLUGIN_DIR "/libaccessories.so").canonicalPath());

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

    QVERIFY(mApplicationIfProxyLaunchCalled);
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
