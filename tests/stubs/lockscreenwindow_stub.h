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

#include "lockscreenwindow.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LockScreenWindowStub : public StubBase {
  public:
  virtual void LockScreenWindowConstructor();
  virtual void LockScreenWindowDestructor();
  virtual void updateDateTime();
  virtual void reset();
  virtual void appear();
  virtual void disappear();
};

// 2. IMPLEMENT STUB
void LockScreenWindowStub::LockScreenWindowConstructor() {
}

void LockScreenWindowStub::LockScreenWindowDestructor() {
}

void LockScreenWindowStub::updateDateTime() {
  stubMethodEntered("updateDateTime");
}

void LockScreenWindowStub::reset() {
  stubMethodEntered("reset");
}

void LockScreenWindowStub::appear() {
  stubMethodEntered("appear");
}

void LockScreenWindowStub::disappear() {
  stubMethodEntered("disappear");
}


// 3. CREATE A STUB INSTANCE
LockScreenWindowStub gDefaultLockScreenWindowStub;
LockScreenWindowStub* gLockScreenWindowStub = &gDefaultLockScreenWindowStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
LockScreenWindow::LockScreenWindow()
{
  gLockScreenWindowStub->LockScreenWindowConstructor();
}

LockScreenWindow::~LockScreenWindow() {
  gLockScreenWindowStub->LockScreenWindowDestructor();
}

void LockScreenWindow::updateDateTime() {
  gLockScreenWindowStub->updateDateTime();
}

void LockScreenWindow::reset() {
  gLockScreenWindowStub->reset();
}

void LockScreenWindow::appear()
{
    gLockScreenWindowStub->appear();
}

void LockScreenWindow::disappear()
{
    gLockScreenWindowStub->disappear();
}

#endif
