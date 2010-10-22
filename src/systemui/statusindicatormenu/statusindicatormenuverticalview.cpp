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
#include <MSceneManager>
#include <MSeparator>
#include <MApplicationExtensionArea>
#include <MButton>
#include <QGraphicsLinearLayout>
#include <mscenewindowview.h>
#include "notificationarea.h"
#include <mstatusbar.h>
#include "statusindicatormenu.h"
#include "statusindicatormenuverticalview.h"


StatusIndicatorMenuVerticalView::StatusIndicatorMenuVerticalView(StatusIndicatorMenu *controller) :
    MSceneWindowView(controller),
    statusBar(new MStatusBar)
{
    // Show status bar
    controller->sceneManager()->appearSceneWindowNow(statusBar);

    // Create an extension area for the top row plugins
    MApplicationExtensionArea *extensionArea = new MApplicationExtensionArea("com.meego.core.MStatusIndicatorMenuExtensionInterface/1.0");
    connect(extensionArea, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)), controller, SLOT(setStatusIndicatorMenuInterface(MApplicationExtensionInterface*)));
    connect(extensionArea, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)), this, SLOT(setExtensionLayoutPosition(MApplicationExtensionInterface*)));
    extensionArea->setObjectName("StatusIndicatorMenuExtensionArea");
    extensionArea->setInProcessFilter(QRegExp("/statusindicatormenu-(alarms|internetconnection|presence|profile).desktop$"));
    extensionArea->setOutOfProcessFilter(QRegExp("$^"));
    extensionArea->setOrder((QStringList() << "statusindicatormenu-alarms.desktop" << "statusindicatormenu-internetconnection.desktop" << "statusindicatormenu-presence.desktop" << "statusindicatormenu-profile.desktop"));
    extensionArea->init();

    // Create a button for accessing the full settings
    //% "Settings"
    MButton *settingsButton = new MButton(qtTrId("qtn_stat_menu_settings"));
    settingsButton->setObjectName("StatusIndicatorMenuTopRowExtensionButton");
    settingsButton->setViewType(MButton::iconType);
    settingsButton->setIconID("icon-m-status-menu-settings");
    settingsButton->setLayoutPosition(M::VerticalBottomPosition);
    connect(settingsButton, SIGNAL(clicked()), controller, SLOT(launchControlPanelAndHide()));

    // Put the extension area and the settings button to a horizontal layout
    QGraphicsLinearLayout *vlayout = new QGraphicsLinearLayout(Qt::Vertical);
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setSpacing(0);
    vlayout->addItem(extensionArea);
    vlayout->addItem(settingsButton);

    // Create a container widget for extension area and settings button layout
    containerWidget = new MWidgetController(controller);
    containerWidget->setView(new MWidgetView(containerWidget));
    containerWidget->setObjectName("StatusIndicatorMenuExtensionAreaWidget");
    containerWidget->setLayout(vlayout);

    QGraphicsLinearLayout *hlayout = new QGraphicsLinearLayout(Qt::Horizontal);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->setSpacing(0);
    hlayout->addStretch();
    hlayout->addItem(containerWidget);
    hlayout->addStretch();

    controller->setLayout(hlayout);
}

void StatusIndicatorMenuVerticalView::setExtensionLayoutPosition(MApplicationExtensionInterface *extension)
{
    MWidgetController *widget = dynamic_cast<MWidgetController*>(extension->widget());

    if (widget) {
        widget->setLayoutPosition(M::VerticalCenterPosition);
    }
}

StatusIndicatorMenuVerticalView::~StatusIndicatorMenuVerticalView()
{
    delete containerWidget;
    delete statusBar;
}

M_REGISTER_VIEW_NEW(StatusIndicatorMenuVerticalView, StatusIndicatorMenu)
