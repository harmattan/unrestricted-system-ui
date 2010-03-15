/*
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

#include "pluginlist.h"
#include <DuiButton>
#include "duistatusindicatormenuplugininterface.h"
#include <DuiWindow>
#include <DuiApplicationPage>
#include <QPluginLoader>
#include <QGraphicsLinearLayout>
#include <DuiApplicationIfProxy>
#include "notificationarea.h"

const QString PluginList::CONTROL_PANEL_SERVICE_NAME = "com.nokia.DuiControlPanel";

PluginList::PluginList(DuiWindow *applicationWindow, DuiApplicationPage *applicationPage, QGraphicsItem *parent) :
    DuiWidget(parent),
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

    // Load the plugins
    addPlugin(DUISTATUSINDICATORMENU_PLUGIN_DIR "/libprofile.so");
    addPlugin(DUISTATUSINDICATORMENU_PLUGIN_DIR "/libdatetime.so");
    addPlugin(DUISTATUSINDICATORMENU_PLUGIN_DIR "/libclockalarm.so");
    addPlugin(DUISTATUSINDICATORMENU_PLUGIN_DIR "/libconnectivity.so");
    addPlugin(DUISTATUSINDICATORMENU_PLUGIN_DIR "/libpresence.so");
    addPlugin(DUISTATUSINDICATORMENU_PLUGIN_DIR "/libbattery.so");
    addPlugin(DUISTATUSINDICATORMENU_PLUGIN_DIR "/libvolume.so");
    addPlugin(DUISTATUSINDICATORMENU_PLUGIN_DIR "/libaccessories.so");

    // Create a button for accessing the full settings
    addSettingsButton();
}

PluginList::~PluginList()
{
    delete notificationArea;
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

void PluginList::addPlugin(const QString &path)
{
    QPluginLoader loader(path);
    QObject* object = loader.instance();

    DuiStatusIndicatorMenuPluginInterface* plugin = qobject_cast<DuiStatusIndicatorMenuPluginInterface*>(object);
    if (plugin != NULL) {
        DuiWidget *widget = plugin->constructWidget(*this);
        if (widget != NULL) {
            mainLayout->addItem(widget);
        }
    }
    delete object;
}

void PluginList::addSettingsButton()
{
    // Create a button for accessing the full settings
    DuiButton *settingsButton = new DuiButton("Settings");
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
        applicationWindow->lower();
    }
}

void PluginList::settingsButtonClicked()
{
    DuiApplicationIfProxy duiApplicationIfProxy(CONTROL_PANEL_SERVICE_NAME, this);

    if (duiApplicationIfProxy.connection().isConnected()) {
        duiApplicationIfProxy.launch();
    } else {
        qWarning() << "Could not launch" << CONTROL_PANEL_SERVICE_NAME << "- DBus not connected?";
    }

    hideStatusIndicatorMenu();
}
