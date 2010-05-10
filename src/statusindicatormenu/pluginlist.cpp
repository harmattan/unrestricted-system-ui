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

#include "pluginlist.h"
#include <MButton>
#include <QTimer>
#include "mstatusindicatormenuplugininterface.h"
#include <MWindow>
#include <MApplicationPage>
#include <QStringList>
#include <QPluginLoader>
#include <QGraphicsLinearLayout>
#include <MApplicationIfProxy>
#include "notificationarea.h"

#define DEBUG
#define WARNING
#include "debug.h"

#ifndef UNIT_TEST
static const int LoadDelay = 100;
#else
static const int LoadDelay = 0;
#endif

const QString PluginList::CONTROL_PANEL_SERVICE_NAME = "com.nokia.DuiControlPanel";

PluginList::PluginList(MWindow *applicationWindow, MApplicationPage *applicationPage, QGraphicsItem *parent) :
    MWidget(parent),
    mainLayout(new QGraphicsLinearLayout(Qt::Vertical)),
    applicationWindow(applicationWindow),
    applicationPage(applicationPage)
{
    // Create a widget for laying out the widgets
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);

    // Create the notification area
    notificationArea = new NotificationArea(this);
    notificationArea->setVisible(false);


    connect(notificationArea, SIGNAL(notificationCountChanged(int)), this, SLOT(setNotificationCount(int)));
    connect(notificationArea, SIGNAL(bannerClicked()), this, SLOT(hideStatusIndicatorMenu()));

    m_LoadingPluginNumber = 0;
    m_PluginFiles << 
	    "/libprofile.so" <<
	    "/libdatetime.so" << 
	    "/libclockalarm.so" << 
	    "/libconnectivity.so" << 
	    "/libpresence.so" << 
	    "/libbattery.so" << 
	    "/libvolume.so" << 
	    "/libaccessories.so" << 
	    "/libcallui.so" << 
	    "/libtransferui.so";

    QTimer::singleShot(LoadDelay, this, SLOT(loadPlugins()));
}

PluginList::~PluginList()
{
    delete notificationArea;
}

void PluginList::loadPlugins()
{
    // Load the plugins
    if (m_LoadingPluginNumber < m_PluginFiles.size()) {
        const QString fullPath = 
            STATUSINDICATORMENU_PLUGIN_DIR + 
            m_PluginFiles[m_LoadingPluginNumber];

        SYS_DEBUG ("********************************************");
        SYS_DEBUG ("*** loading : %s", SYS_STR(fullPath));
        SYS_DEBUG ("********************************************");
        addPlugin (fullPath);

        ++m_LoadingPluginNumber;
        QTimer::singleShot(LoadDelay, this, SLOT(loadPlugins()));

        return;
    }

    // Create a button for accessing the full settings
    addSettingsButton();
}

void PluginList::setNotificationCount(int notificationCount)
{
    if(notificationCount > 0) {
        if(!notificationArea->isVisible()) {
            notificationArea->setVisible(true);
            mainLayout->insertItem(0, notificationArea);
        }
    } else {
        if(notificationArea->isVisible()) {
            notificationArea->setVisible(false);
            mainLayout->removeItem(notificationArea);
        }
    }
}

/*!
 * This function will actually load a plugin.
 */
void
PluginList::addPlugin(
        const QString &path)
{
    bool           success;
    QObject       *object;
    QPluginLoader  loader(path);

    SYS_DEBUG ("Loading plugin from %s", SYS_STR(path));
    /*
     * We have to resolve all the symbols early, so we will not abort when there
     * are unresolved symbols in the library. It is really important!
     */
    loader.setLoadHints(QLibrary::ResolveAllSymbolsHint);
    success = loader.load();
    if (!success) {
        qDebug() << "Error loading plugin: " << loader.errorString();
	return;
    }

    object = loader.instance();

    MStatusIndicatorMenuPluginInterface* plugin = 
	    qobject_cast<MStatusIndicatorMenuPluginInterface*>(object);
    if (plugin != NULL) {
        MWidget *widget = plugin->constructWidget(*this);
        if (widget != NULL) {
            mainLayout->addItem(widget);
        }
    }

    delete object;
}

void PluginList::addSettingsButton()
{
    // Create a button for accessing the full settings
    MButton *settingsButton = new MButton("Settings");
    settingsButton->setObjectName("StatusIndicatorMenuSettingsButton");
    connect(settingsButton, SIGNAL(clicked()), this, SLOT(settingsButtonClicked()));

    // Add the settings button to a horizontal mainLayout for centering
    QGraphicsLinearLayout *settingsLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    settingsLayout->setContentsMargins(0, 0, 0, 0);
    settingsLayout->addStretch();
    settingsLayout->addItem(settingsButton);
    settingsLayout->addStretch();

    // Add the horizontal mainLayout to the main mainLayout
    mainLayout->addItem(settingsLayout);
}

void PluginList::showStatusIndicatorMenu()
{
    if (applicationPage != NULL) {
        applicationPage->appear();
    }
}

void PluginList::hideStatusIndicatorMenu()
{
    if (applicationWindow != NULL) {
        applicationWindow->hide();
    }
}

void PluginList::settingsButtonClicked()
{
    MApplicationIfProxy mApplicationIfProxy(CONTROL_PANEL_SERVICE_NAME, this);

    if (mApplicationIfProxy.connection().isConnected()) {
        mApplicationIfProxy.launch();
    } else {
        qWarning() << "Could not launch" << CONTROL_PANEL_SERVICE_NAME << "- DBus not connected?";
    }

    hideStatusIndicatorMenu();
}
