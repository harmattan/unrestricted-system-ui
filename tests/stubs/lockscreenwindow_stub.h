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
#ifndef LOCKSCREENWINDOW_STUB
#define LOCKSCREENWINDOW_STUB

#include "screenlockwindow.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ScreenLockWindowStub : public StubBase {
  public:
  virtual void ScreenLockWindowConstructor();
  virtual void ScreenLockWindowDestructor();
  virtual void applyStyle();
  virtual void showEvent(QShowEvent *event);
  virtual void setLowPowerMode(bool enable);
  virtual void paintEvent(QPaintEvent *event);
};

// 2. IMPLEMENT STUB
void ScreenLockWindowStub::ScreenLockWindowConstructor() {
}

void ScreenLockWindowStub::ScreenLockWindowDestructor() {
}

void ScreenLockWindowStub::applyStyle() {
  stubMethodEntered("applyStyle");
}

void ScreenLockWindowStub::showEvent(QShowEvent *event)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QShowEvent * >(event));
  stubMethodEntered("showEvent", params);
}

void ScreenLockWindowStub::setLowPowerMode(bool enable)
{
    QList<ParameterBase*> params;
    params.append( new Parameter<bool>(enable));
    stubMethodEntered("setLowPowerMode", params);
}

void ScreenLockWindowStub::paintEvent(QPaintEvent *event)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QPaintEvent * >(event));
  stubMethodEntered("paintEvent", params);
}


// 3. CREATE A STUB INSTANCE
ScreenLockWindowStub gDefaultScreenLockWindowStub;
ScreenLockWindowStub* gScreenLockWindowStub = &gDefaultScreenLockWindowStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
ScreenLockWindow::ScreenLockWindow(MApplicationExtensionArea *, QWidget *)
{
  gScreenLockWindowStub->ScreenLockWindowConstructor();
}

ScreenLockWindow::~ScreenLockWindow() {
  gScreenLockWindowStub->ScreenLockWindowDestructor();
}

void ScreenLockWindow::applyStyle() {
  gScreenLockWindowStub->applyStyle();
}

void ScreenLockWindow::showEvent(QShowEvent *event)
{
    gScreenLockWindowStub->showEvent(event);
}

void ScreenLockWindow::setLowPowerMode(bool enable)
{
    gScreenLockWindowStub->setLowPowerMode(enable);
}

void ScreenLockWindow::paintEvent(QPaintEvent *event)
{
    gScreenLockWindowStub->paintEvent(event);
}

#endif
