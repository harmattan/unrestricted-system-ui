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
#include <QGraphicsSceneMouseEvent>
#include <MSceneManager>
#include <MSceneWindow>
#include <MApplicationExtensionArea>
#include <MEscapeButtonPanel>
#include <MButton>
#include <MOverlay>
#include <MApplicationIfProxy>
#include <MPannableViewport>
#include <QGraphicsLinearLayout>
#include "pluginlist.h"
#include "notificationarea.h"
#include "statusindicatormenuwindow.h"
#include <MWidgetView>

void EventEaterWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
}

StatusIndicatorMenuSceneWindow::StatusIndicatorMenuSceneWindow(QGraphicsItem *parent) : MSceneWindow(parent)
{
}

void StatusIndicatorMenuSceneWindow::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(event->scenePos());
}

PannedWidgetController::PannedWidgetController(QGraphicsItem *parent) :
    MWidgetController(parent)
{
}

void PannedWidgetController::setGeometry(const QRectF &rect)
{
    MWidgetController::setGeometry(rect);
    emit positionOrSizeChanged();
}

const QString StatusIndicatorMenuWindow::CONTROL_PANEL_SERVICE_NAME = "com.nokia.DuiControlPanel";

StatusIndicatorMenuWindow::StatusIndicatorMenuWindow(QWidget *parent) :
    MWindow(parent),
    sceneWindow(new StatusIndicatorMenuSceneWindow),
    statusBar(new MStatusBar),
    pannableViewport(new MPannableViewport),
    pannableLayout(new QGraphicsLinearLayout(Qt::Vertical)),
    notificationArea(new NotificationArea),
    closeButtonOverlay(new MOverlay)
{
    setTranslucentBackground(true);
    setWindowTitle("Status Indicator Menu");
    connect(this, SIGNAL(displayEntered()), this, SLOT(displayActive()));
    connect(this, SIGNAL(displayExited()), this, SLOT(displayInActive()));
    connect(sceneWindow.data(), SIGNAL(clicked(QPointF)), this, SLOT(hideIfPointBeyondMenu(QPointF)));

    // Show status bar
    sceneManager()->appearSceneWindowNow(statusBar.data());

    // Set up a notification area
    notificationArea->setVisible(false);
    connect(notificationArea, SIGNAL(notificationCountChanged(int)), this, SLOT(setNotificationCount(int)));
    connect(notificationArea, SIGNAL(bannerClicked()), this, SLOT(hideStatusIndicatorMenu()));

    // Put all the stuff into the scene window layout
    pannableViewport = createPannableArea();
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addItem(createTopRow());
    layout->addItem(pannableViewport);
    sceneWindow->setLayout(layout);
    sceneWindow->setObjectName("StatusIndicatorMenuWindow");
    sceneManager()->appearSceneWindowNow(sceneWindow.data());

    // Create close button overlay
    closeButtonOverlay = QSharedPointer<MOverlay>(createCloseButtonOverlay());

    // Set the X window type, so that the window does not appear in the switcher and 
    // home screen con provide the correct UI flow
    setAttribute(Qt::WA_X11NetWmWindowTypeMenu);
}

StatusIndicatorMenuWindow::~StatusIndicatorMenuWindow()
{
    if(!notificationArea->isVisible()) {
        // If the notification area is not in the layout destroy it manually
        delete notificationArea;
    }
}

QGraphicsWidget* StatusIndicatorMenuWindow::createTopRow()
{
    // Create an extension area for the top row plugins
    MApplicationExtensionArea *extensionArea = new MApplicationExtensionArea("com.meego.core.MStatusIndicatorMenuExtensionInterface/1.0");
    extensionArea->setObjectName("StatusIndicatorMenuTopRowExtensionArea");
    connect(extensionArea, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)), this, SLOT(setStatusIndicatorMenuInterface(MApplicationExtensionInterface*)));
    extensionArea->init();

    // Create a button for accessing the full settings
    //% "Settings"
    MButton *settingsButton = new MButton(qtTrId("qtn_stat_menu_settings"));
    settingsButton->setObjectName("StatusIndicatorMenuTopRowExtensionButton");
    settingsButton->setViewType(MButton::iconType);
    settingsButton->setIconID("icon-m-common-settings");
    connect(settingsButton, SIGNAL(clicked()), this, SLOT(launchControlPanelAndHide()));

    // Put the extension area and the settings button to a horizontal layout
    QGraphicsLinearLayout *topRowLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    topRowLayout->setContentsMargins(0, 0, 0, 0);
    topRowLayout->setSpacing(0);
    topRowLayout->addStretch();
    topRowLayout->addItem(extensionArea);
    topRowLayout->addItem(settingsButton);
    topRowLayout->addStretch();

    // Create a container widget for extension area and settings button layout
    MWidgetController *topRowWidget = new MWidgetController;
    topRowWidget->setView(new MWidgetView(topRowWidget));
    topRowWidget->setObjectName("StatusIndicatorMenuExtensionAreaWidget");
    topRowWidget->setLayout(topRowLayout);

    return topRowWidget;
}

MPannableViewport* StatusIndicatorMenuWindow::createPannableArea()
{
    // Create pannable area contents
    pannableLayout = new QGraphicsLinearLayout(Qt::Vertical);
    pannableLayout->setContentsMargins(0, 0, 0, 0);
    pannableLayout->setSpacing(0);
    pannableLayout->addItem(new PluginList(this, sceneWindow.data()));
    pannableLayout->addItem(createCloseButtonRow());

    // Create a container widget for the pannable area
    PannedWidgetController *pannedWidget = new PannedWidgetController;
    pannedWidget->setView(new MWidgetView(pannedWidget));
    pannedWidget->setObjectName("StatusIndicatorMenuPannableWidget");
    pannedWidget->setLayout(pannableLayout);
    connect(pannedWidget, SIGNAL(positionOrSizeChanged()), this, SLOT(setPannabilityAndLayout()));

    // Setup the pannable viewport
    pannableViewport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pannableViewport->setWidget(pannedWidget);

    return pannableViewport;
}

QGraphicsWidget* StatusIndicatorMenuWindow::createCloseButtonRow()
{
    // Create a close button for the pannable area
    MButton *closeButton = new MButton;
    closeButton->setViewType("icon");
    closeButton->setObjectName("StatusIndicatorMenuCloseButton");
    closeButton->setIconID("icon-m-framework-close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(hide()));

    // Add two overlay widgets that will not allow mouse events to pass through them next to the close button
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addItem(new EventEaterWidget);
    layout->addItem(closeButton);
    layout->addItem(new EventEaterWidget);

    // Create the area itself
    MWidgetController *closeButtonArea = new MWidgetController;
    closeButtonArea->setView(new MWidgetView(closeButtonArea));
    closeButtonArea->setObjectName("CloseButtonArea");
    closeButtonArea->setLayout(layout);

    return closeButtonArea;
}

MOverlay *StatusIndicatorMenuWindow::createCloseButtonOverlay()
{
    // Create a close button
    MButton *closeButton = new MButton;
    closeButton->setViewType("icon");
    closeButton->setObjectName("StatusIndicatorMenuCloseButton");
    closeButton->setIconID("icon-m-framework-close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(hide()));

    // Add two overlay widgets that will not allow mouse events to pass through them next to the close button
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addItem(new EventEaterWidget);
    layout->addItem(closeButton);
    layout->addItem(new EventEaterWidget);

    // Create the overlay itself
    MOverlay *closeButtonOverlay = new MOverlay;
    closeButtonOverlay->setLayout(layout);
    closeButtonOverlay->setObjectName("CloseButtonOverlay");
    sceneManager()->appearSceneWindowNow(closeButtonOverlay);
    sceneManager()->disappearSceneWindowNow(closeButtonOverlay);

    return closeButtonOverlay;
}

void StatusIndicatorMenuWindow::setPannabilityAndLayout()
{
    // Enable pannability if there is too much content to fit on the screen
    if (pannableViewport->geometry().height() > pannableViewport->widget()->geometry().height()) {
        pannableViewport->setEnabled(false);
    } else {
        pannableViewport->setEnabled(true);
    }

    // Appear or disappear the close button overlay based on close area position
    QGraphicsWidget *widget = pannableViewport->widget();
    qreal screenHeight = sceneManager()->visibleSceneSize().height();
    qreal yPos = widget->mapToItem(sceneWindow.data(), QPointF(widget->geometry().width(), widget->geometry().height())).y();

    if (yPos < screenHeight) {
        sceneManager()->disappearSceneWindowNow(closeButtonOverlay.data());
    } else {
        sceneManager()->appearSceneWindowNow(closeButtonOverlay.data());
    }
}

void StatusIndicatorMenuWindow::displayActive()
{
    emit visibilityChanged(true);
}

void StatusIndicatorMenuWindow::displayInActive()
{
    emit visibilityChanged(false);
}

void StatusIndicatorMenuWindow::setStatusIndicatorMenuInterface(MApplicationExtensionInterface *extension)
{
    MStatusIndicatorMenuExtensionInterface *menuExtension = static_cast<MStatusIndicatorMenuExtensionInterface *>(extension);
    menuExtension->setStatusIndicatorMenuInterface(*this);
}

void StatusIndicatorMenuWindow::makeVisible()
{
    if (!isVisible()) {
        // If status indicator window is not visible, then show it
        show();
    } else {
        // Otherwise, raise it
        raise();
    }
}

void StatusIndicatorMenuWindow::showStatusIndicatorMenu()
{
    sceneWindow->appear();
}

void StatusIndicatorMenuWindow::hideStatusIndicatorMenu()
{
    hide();
}

void StatusIndicatorMenuWindow::setNotificationCount(int notificationCount)
{
    if(notificationCount > 0) {
        if(!notificationArea->isVisible()) {
            notificationArea->setVisible(true);
            pannableLayout->insertItem(0, notificationArea);
        }
    } else {
        if(notificationArea->isVisible()) {
            notificationArea->setVisible(false);
            pannableLayout->removeItem(notificationArea);
        }
    }
}

void StatusIndicatorMenuWindow::launchControlPanelAndHide()
{
    MApplicationIfProxy mApplicationIfProxy(CONTROL_PANEL_SERVICE_NAME, this);

    if (mApplicationIfProxy.connection().isConnected()) {
        mApplicationIfProxy.launch();
    } else {
        qWarning() << "Could not launch" << CONTROL_PANEL_SERVICE_NAME << "- DBus not connected?";
    }

    hideStatusIndicatorMenu();
}

void StatusIndicatorMenuWindow::hideIfPointBeyondMenu(QPointF point)
{
    QGraphicsWidget *widget = pannableViewport->widget();
    if (widget != NULL) {
        QRectF menuRect(sceneWindow->mapToScene(QPointF()), widget->mapToScene(QPointF(widget->geometry().width(), widget->geometry().height())));
        if (!menuRect.contains(point)) {
           hide();
        }
    }
}
