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

#include <DuiApplication>
#include <DuiApplicationPage>
#include <DuiScene>
#include <DuiSceneManager>
#include <DuiEscapeButtonPanel>
#include "pluginlist.h"
#include "statusindicatormenuwindow.h"

StatusIndicatorMenuWindow::StatusIndicatorMenuWindow(QWidget *parent) :
    DuiWindow(parent),
    scene(new DuiScene),
    applicationPage(new DuiApplicationPage),
    escapeButtonPanel(new DuiEscapeButtonPanel)
{
    // Take the scene into use and create a scene manager for it. Must be done in this order.
    setScene(scene.data());
    setSceneRect(QRectF(QPointF(), visibleSceneSize(Dui::Landscape)));
    centerOn(sceneRect().center());
    sceneManager =
    QSharedPointer<DuiSceneManager>(new DuiSceneManager(scene.data()));

    // Create an application page for the plugin list
    applicationPage->setObjectName("StatusIndicatorMenuPage");
    applicationPage->setTitle("Status Indicator Menu");
    applicationPage->setComponentsDisplayMode(DuiApplicationPage::NavigationBar, DuiApplicationPageModel::Hide);
    applicationPage->setComponentsDisplayMode(DuiApplicationPage::HomeButton, DuiApplicationPageModel::Hide);
    applicationPage->setComponentsDisplayMode(DuiApplicationPage::EscapeButton, DuiApplicationPageModel::Show);
    applicationPage->setCentralWidget(new PluginList(this,
                             applicationPage.data()));
    sceneManager->showWindowNow(applicationPage.data());

    // Create an escape button
    escapeButtonPanel->connect(escapeButtonPanel.data(),
                   SIGNAL(buttonClicked()), this, SLOT(lower()));
    sceneManager->showWindowNow(escapeButtonPanel.data());

    // Set the X window properties so that the window does not appear in the task bar
    excludeFromTaskBar();
}

StatusIndicatorMenuWindow::~StatusIndicatorMenuWindow()
{
}

void StatusIndicatorMenuWindow::setOrientationAngle(Dui::OrientationAngle angle)
{
    sceneManager->setOrientationAngle(angle);
    DuiWindow::setOrientationAngle(angle);
}

void StatusIndicatorMenuWindow::excludeFromTaskBar()
{
    // Tell the window to not to be shown in the switcher
    Atom skipTaskbarAtom = XInternAtom(QX11Info::display(), "_NET_WM_STATE_SKIP_TASKBAR", False);
    changeNetWmState(this, true, skipTaskbarAtom);

    // TODO: setting this property by hand can be removed when duicompositor
    // sets the _NET_WM_STATE attribute according to the message.
    Atom netWmStateAtom = XInternAtom(QX11Info::display(), "_NET_WM_STATE", False);
    QVector<Atom> atoms;
    atoms.append(skipTaskbarAtom);
    XChangeProperty(QX11Info::display(), internalWinId(), netWmStateAtom, XA_ATOM, 32, PropModeReplace, (unsigned char *)atoms.data(), atoms.count());
}

void StatusIndicatorMenuWindow::changeNetWmState(const QWidget* w, bool set, Atom one, Atom two)
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
