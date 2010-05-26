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

#include <MApplication>
#include <MSceneWindow>
#include <MButton>
#include <QGraphicsLinearLayout>
#include "ut_pluginlist.h"
#include "pluginlist.h"
#include "pannedwidgetcontroller_stub.h"
#include "statusindicatormenuwindow_stub.h"
#include "statusindicatormenuscenewindow_stub.h"

// QPluginLoader stubs (used by PluginList)
QObject *QPluginLoader::instance()
{
    if(!QFileInfo(fileName()).absolutePath().endsWith(APPLICATION_EXTENSION_DIR)) {
        Ut_PluginList::loadedPlugins.append(fileName());
    }

    return new TestPlugin;
}

bool QPluginLoader::load()
{
    return true;
}

MWidget *TestPlugin::constructWidget(MStatusIndicatorMenuInterface &)
{
    return new MWidget;
}

bool Ut_PluginList::windowMinimized;
QStringList Ut_PluginList::loadedPlugins;

void Ut_PluginList::initTestCase()
{
    int argc = 1;
    char *app_name = (char *)"./ut_pluginlist";
    app = new MApplication(argc, &app_name);
    window = new StatusIndicatorMenuWindow;
}

void Ut_PluginList::cleanupTestCase()
{
    delete window;
    delete app;
}

void Ut_PluginList::init()
{
    gStatusIndicatorMenuWindowStub->stubReset();
    loadedPlugins.clear();
    pluginList = new PluginList(window);
}

void Ut_PluginList::cleanup()
{
    delete pluginList;
}

void Ut_PluginList::testInitialization()
{
    for (int i = 0; i < 10; i++) {
        // Process the delayed loading timer events
        QCoreApplication::processEvents();
    }

    // The layout should contain a widget for each plugin plus one for the top row
    QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout *>(pluginList->layout());
    QVERIFY(layout != NULL);
    QCOMPARE(loadedPlugins.count(), 10);
    QCOMPARE(layout->count(), loadedPlugins.count());
}

void Ut_PluginList::testShowStatusIndicatorMenu()
{
    pluginList->showStatusIndicatorMenu();

    QCOMPARE(gStatusIndicatorMenuWindowStub->stubCallCount("showStatusIndicatorMenu"), 1);
}

void Ut_PluginList::testHideStatusIndicatorMenu()
{
    pluginList->hideStatusIndicatorMenu();

    QCOMPARE(gStatusIndicatorMenuWindowStub->stubCallCount("hideStatusIndicatorMenu"), 1);
}

QTEST_APPLESS_MAIN(Ut_PluginList)
