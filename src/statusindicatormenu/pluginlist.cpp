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

#include "pluginlist.h"
#include <MButton>
#include <QTimer>
#include <MSceneWindow>
#include <QStringList>
#include <QPluginLoader>
#include <QGraphicsLinearLayout>
#include <QDebug>
#include "statusindicatormenuwindow.h"

PluginList::PluginList(StatusIndicatorMenuWindow *window, QGraphicsItem *parent) :
    MWidget(parent),
    mainLayout(new QGraphicsLinearLayout(Qt::Vertical)),
    window(window)
{
    // Create a widget for laying out the widgets
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);

    pluginsToBeLoaded <<
	    "/libprofile.so" <<
	    "/libdatetime.so" << 
	    "/libclockalarm.so" << 
	    "/libconnectivity.so" << 
	    "/libpresence.so" << 
	    "/libbattery.so" << 
	    "/libaccessories.so" << 
	    "/libcallui.so" << 
	    "/libtransferui.so";

    QTimer::singleShot(0, this, SLOT(loadPlugins()));
}

PluginList::~PluginList()
{
}

void PluginList::loadPlugins()
{
    // Load the plugins
    if (!pluginsToBeLoaded.isEmpty()) {
        const QString fullPath = STATUSINDICATORMENU_PLUGIN_DIR + pluginsToBeLoaded.takeFirst();

        qDebug() << "Loading status indicator menu plugin" << fullPath;
        addPlugin (fullPath);
        qDebug() << "Loaded status indicator menu plugin" << fullPath;
        
        QTimer::singleShot(0, this, SLOT(loadPlugins()));
    }
}

void PluginList::showStatusIndicatorMenu()
{
    window->showStatusIndicatorMenu();
}

void PluginList::hideStatusIndicatorMenu()
{
    window->hideStatusIndicatorMenu();
}

/*!
 * This function will actually load a plugin.
 */
void PluginList::addPlugin(const QString &path)
{
    bool success;
    QObject *object;
    QPluginLoader loader(path);

    /*
     * We have to resolve all the symbols early, so we will not abort when there
     * are unresolved symbols in the library. It is really important!
     */
    loader.setLoadHints(QLibrary::ResolveAllSymbolsHint);
    success = loader.load();
    if (!success) {
        qDebug() << "Error loading plugin " << path << ": " << loader.errorString();
        return;
    }

    object = loader.instance();

    MStatusIndicatorMenuPluginInterface* plugin = qobject_cast<MStatusIndicatorMenuPluginInterface*> (object);
    if (plugin != NULL) {
        MWidget *widget = plugin->constructWidget(*this);
        if (widget != NULL) {
            mainLayout->addItem(widget);
        }
    }

    delete object;
}
