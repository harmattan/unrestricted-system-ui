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

#include <MApplicationPage>
#include <MSceneManager>
#include <MEscapeButtonPanel>
#include <QX11Info>
#include "pluginlist.h"
#include "statusindicatormenuwindow.h"

#include <X11/Xlib.h>

// TODO: this include can be removed when mcompositor
// sets the _NET_WM_STATE attribute according to the message.
#include <X11/Xatom.h>


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


StatusIndicatorMenuWindow::StatusIndicatorMenuWindow(QWidget *parent) :
    MWindow(parent),
    applicationPage(new MApplicationPage),
    escapeButtonPanel(new MEscapeButtonPanel)
{
    // Create an application page for the plugin list
    applicationPage->setObjectName("StatusIndicatorMenuPage");
    applicationPage->setTitle("Status Indicator Menu");
    applicationPage->setComponentsDisplayMode(MApplicationPage::NavigationBar, MApplicationPageModel::Hide);
    applicationPage->setComponentsDisplayMode(MApplicationPage::HomeButton, MApplicationPageModel::Hide);
    applicationPage->setComponentsDisplayMode(MApplicationPage::EscapeButton, MApplicationPageModel::Show);
    applicationPage->setCentralWidget(new PluginList(this, applicationPage.data()));
    sceneManager()->appearSceneWindowNow(applicationPage.data());

    // Create an escape button
    escapeButtonPanel->connect(escapeButtonPanel.data(),
                   SIGNAL(buttonClicked()), this, SLOT(hide()));
    sceneManager()->appearSceneWindowNow(escapeButtonPanel.data());

    // Set the X window properties so that the window does not appear in the task bar
    excludeFromTaskBar();
    connect(this, SIGNAL(displayEntered()), this, SLOT(displayActive()));
    connect(this, SIGNAL(displayExited()), this, SLOT(displayInActive()));
}

void StatusIndicatorMenuWindow::displayActive()
{
    emit visibilityChanged(true);
}

void StatusIndicatorMenuWindow::displayInActive()
{
    emit visibilityChanged(false);
}

StatusIndicatorMenuWindow::~StatusIndicatorMenuWindow()
{
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

    // Make sure the application page is centered on display
    centerOn(applicationPage.data());
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
