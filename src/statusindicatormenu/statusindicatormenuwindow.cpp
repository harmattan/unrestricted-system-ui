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
#include <QX11Info>
#include "pluginlist.h"
#include "notificationarea.h"
#include "statusindicatormenuwindow.h"

#include <X11/Xlib.h>

// TODO: this include can be removed when mcompositor
// sets the _NET_WM_STATE attribute according to the message.
#include <X11/Xatom.h>

void EventEaterWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
}

/*!
 * Changes the _NET_WM_STATE property of a widget's window.
 *
 * \param w the widget whose window's property to change
 * \param set \c true if the first data value will be 1, \c false if it will be 0
 * \param one the first Atom to put in the value
 * \param two the second Atom to put in the value
 */
void changeNetWmState(const QWidget* w, bool set, Atom one, Atom two = 0)
{
    XEvent e;
    e.xclient.type = ClientMessage;
    Display *display = QX11Info::display();
    Atom netWmStateAtom = XInternAtom(display, "_NET_WM_STATE", FALSE);
    e.xclient.message_type = netWmStateAtom;
    e.xclient.display = display;
    e.xclient.window = w->internalWinId();
    e.xclient.format = 32;
    e.xclient.data.l[0] = set ? 1 : 0;
    e.xclient.data.l[1] = one;
    e.xclient.data.l[2] = two;
    e.xclient.data.l[3] = 0;
    e.xclient.data.l[4] = 0;
    XSendEvent(display, RootWindow(display, w->x11Info().screen()), FALSE, (SubstructureNotifyMask | SubstructureRedirectMask), &e);
    XSync(display, FALSE);
}

const QString StatusIndicatorMenuWindow::CONTROL_PANEL_SERVICE_NAME = "com.nokia.DuiControlPanel";

StatusIndicatorMenuWindow::StatusIndicatorMenuWindow(QWidget *parent) :
    MWindow(parent),
    sceneWindow(new MSceneWindow),
    pannableLayout(new QGraphicsLinearLayout(Qt::Vertical)),
    notificationArea(new NotificationArea),
    closeButtonOverlay(new MOverlay)
{
    setWindowTitle("Status Indicator Menu");
    connect(this, SIGNAL(displayEntered()), this, SLOT(displayActive()));
    connect(this, SIGNAL(displayExited()), this, SLOT(displayInActive()));

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
    connect(settingsButton, SIGNAL(clicked()), this, SLOT(settingsButtonClicked()));

    // Put the extension area and the settings button to a horizontal layout
    QGraphicsLinearLayout *topRowLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    topRowLayout->addStretch();
    topRowLayout->addItem(extensionArea);
    topRowLayout->addItem(settingsButton);
    topRowLayout->addStretch();

    // Create a notification area
    notificationArea->setVisible(false);
    connect(notificationArea, SIGNAL(notificationCountChanged(int)), this, SLOT(setNotificationCount(int)));
    connect(notificationArea, SIGNAL(bannerClicked()), this, SLOT(hideStatusIndicatorMenu()));

    // Create a container widget for the pannable area
    QGraphicsWidget *pannableWidget = new QGraphicsWidget;
    pannableLayout->setContentsMargins(0, 0, 0, 0);
    pannableLayout->setSpacing(0);
    pannableLayout->addItem(new PluginList(this, sceneWindow.data()));
    pannableWidget->setLayout(pannableLayout);

    // Create a pannable viewport
    MPannableViewport *pannableViewport = new MPannableViewport;
    pannableViewport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pannableViewport->setWidget(pannableWidget);

    // Put all the stuff into the scene window layout
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addItem(topRowLayout);
    layout->addItem(pannableViewport);
    sceneWindow->setLayout(layout);
    sceneWindow->setObjectName("StatusIndicatorMenuWindow");
    sceneManager()->appearSceneWindowNow(sceneWindow.data());

    // Add an overlay that has the close button in it
    MButton *closeButton = new MButton(closeButtonOverlay.data());
    closeButton->setViewType("icon");
    closeButton->setObjectName("StatusIndicatorMenuCloseButton");
    closeButton->setIconID("icon-m-framework-close");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(hide()));

    // Add two overlay widgets that will not allow mouse events to pass through them next to the close button
    layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addItem(new EventEaterWidget);
    layout->addItem(closeButton);
    layout->addItem(new EventEaterWidget);

    closeButtonOverlay->setLayout(layout);
    closeButtonOverlay->setObjectName("CloseButtonOverlay");
    sceneManager()->appearSceneWindowNow(closeButtonOverlay.data());

    // Set the X window properties so that the window does not appear in the task bar
    excludeFromTaskBar();
}

StatusIndicatorMenuWindow::~StatusIndicatorMenuWindow()
{
    if(!notificationArea->isVisible()) {
        // If the notification area is not in the layout destroy it manually
        delete notificationArea;
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

void StatusIndicatorMenuWindow::excludeFromTaskBar()
{
    // Tell the window to not to be shown in the switcher
    Atom skipTaskbarAtom = XInternAtom(QX11Info::display(), "_NET_WM_STATE_SKIP_TASKBAR", False);
    changeNetWmState(this, true, skipTaskbarAtom);

    // TODO: setting this property by hand can be removed when mcompositor
    // sets the _NET_WM_STATE attribute according to the message.
    Atom netWmStateAtom = XInternAtom(QX11Info::display(), "_NET_WM_STATE", False);
    QVector<Atom> atoms;
    atoms.append(skipTaskbarAtom);
    XChangeProperty(QX11Info::display(), internalWinId(), netWmStateAtom, XA_ATOM, 32, PropModeReplace, (unsigned char *)atoms.data(), atoms.count());
}

void StatusIndicatorMenuWindow::settingsButtonClicked()
{
    MApplicationIfProxy mApplicationIfProxy(CONTROL_PANEL_SERVICE_NAME, this);

    if (mApplicationIfProxy.connection().isConnected()) {
        mApplicationIfProxy.launch();
    } else {
        qWarning() << "Could not launch" << CONTROL_PANEL_SERVICE_NAME << "- DBus not connected?";
    }

    hideStatusIndicatorMenu();
}
