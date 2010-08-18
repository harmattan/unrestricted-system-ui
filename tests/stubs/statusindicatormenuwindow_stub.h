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

#ifndef STATUSINDICATORMENUWINDOW_STUB
#define STATUSINDICATORMENUWINDOW_STUB

#include "statusindicatormenuwindow.h"
#include <stubbase.h>

#ifdef HAVE_QMSYSTEM
#include <qmlocks.h>
#endif

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class StatusIndicatorMenuWindowStub : public StubBase {
  public:
  virtual void StatusIndicatorMenuWindowConstructor(QWidget *parent);
  virtual void StatusIndicatorMenuWindowDestructor();
  virtual void makeVisible();
  virtual void showStatusIndicatorMenu();
  virtual void hideStatusIndicatorMenu();
  virtual void displayActive();
  virtual void displayInActive();
  virtual void setStatusIndicatorMenuInterface(MApplicationExtensionInterface *extension);
  virtual void launchControlPanelAndHide();
  virtual void setPannabilityAndLayout();
#ifdef HAVE_QMSYSTEM
  virtual void setWindowStateAccordingToDeviceLockState(Maemo::QmLocks::Lock what, Maemo::QmLocks::State how);
#endif
};

// 2. IMPLEMENT STUB
void StatusIndicatorMenuWindowStub::StatusIndicatorMenuWindowConstructor(QWidget *parent) {
  Q_UNUSED(parent);

}
void StatusIndicatorMenuWindowStub::StatusIndicatorMenuWindowDestructor() {

}
void StatusIndicatorMenuWindowStub::makeVisible() {
  stubMethodEntered("makeVisible");
}

void StatusIndicatorMenuWindowStub::showStatusIndicatorMenu() {
  stubMethodEntered("showStatusIndicatorMenu");
}

void StatusIndicatorMenuWindowStub::hideStatusIndicatorMenu() {
  stubMethodEntered("hideStatusIndicatorMenu");
}

void StatusIndicatorMenuWindowStub::displayActive() {
  stubMethodEntered("displayActive");
}

void StatusIndicatorMenuWindowStub::displayInActive() {
  stubMethodEntered("displayInActive");
}

void StatusIndicatorMenuWindowStub::setStatusIndicatorMenuInterface(MApplicationExtensionInterface *extension) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MApplicationExtensionInterface * >(extension));
  stubMethodEntered("setStatusIndicatorMenuInterface",params);
}

void StatusIndicatorMenuWindowStub::launchControlPanelAndHide() {
  stubMethodEntered("launchControlPanelAndHide");
}

void StatusIndicatorMenuWindowStub::setPannabilityAndLayout() {
  stubMethodEntered("setPannabilityAndLayout");
}

#ifdef HAVE_QMSYSTEM
void StatusIndicatorMenuWindowStub::setWindowStateAccordingToDeviceLockState(Maemo::QmLocks::Lock what, Maemo::QmLocks::State how) {
  QList<ParameterBase*> params;
  params.append( new Parameter<Maemo::QmLocks::Lock>(what));
  params.append( new Parameter<Maemo::QmLocks::State>(how));
  stubMethodEntered("setWindowStateAccordingToDeviceLockState",params);
}
#endif


// 3. CREATE A STUB INSTANCE
StatusIndicatorMenuWindowStub gDefaultStatusIndicatorMenuWindowStub;
StatusIndicatorMenuWindowStub* gStatusIndicatorMenuWindowStub = &gDefaultStatusIndicatorMenuWindowStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
StatusIndicatorMenuWindow::StatusIndicatorMenuWindow(QWidget *parent) {
  gStatusIndicatorMenuWindowStub->StatusIndicatorMenuWindowConstructor(parent);
}

StatusIndicatorMenuWindow::~StatusIndicatorMenuWindow() {
  gStatusIndicatorMenuWindowStub->StatusIndicatorMenuWindowDestructor();
}

void StatusIndicatorMenuWindow::makeVisible() {
  gStatusIndicatorMenuWindowStub->makeVisible();
}

void StatusIndicatorMenuWindow::showStatusIndicatorMenu() {
  gStatusIndicatorMenuWindowStub->showStatusIndicatorMenu();
}

void StatusIndicatorMenuWindow::hideStatusIndicatorMenu() {
  gStatusIndicatorMenuWindowStub->hideStatusIndicatorMenu();
}

void StatusIndicatorMenuWindow::displayActive() {
  gStatusIndicatorMenuWindowStub->displayActive();
}

void StatusIndicatorMenuWindow::displayInActive() {
  gStatusIndicatorMenuWindowStub->displayInActive();
}

void StatusIndicatorMenuWindow::setStatusIndicatorMenuInterface(MApplicationExtensionInterface *extension) {
  gStatusIndicatorMenuWindowStub->setStatusIndicatorMenuInterface(extension);
}

void StatusIndicatorMenuWindow::launchControlPanelAndHide() {
  gStatusIndicatorMenuWindowStub->launchControlPanelAndHide();
}

void StatusIndicatorMenuWindow::setPannabilityAndLayout() {
  gStatusIndicatorMenuWindowStub->setPannabilityAndLayout();
}

#ifdef HAVE_QMSYSTEM
void StatusIndicatorMenuWindow::setWindowStateAccordingToDeviceLockState(Maemo::QmLocks::Lock what, Maemo::QmLocks::State how) {
  gStatusIndicatorMenuWindowStub->setWindowStateAccordingToDeviceLockState(what, how);
}
#endif

#endif
