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
#ifndef LOCKSCREENUI_STUB
#define LOCKSCREENUI_STUB

#include "lockscreen.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LockScreenStub : public StubBase {
  public:
  virtual void LockScreenConstructor();
  virtual void LockScreenDestructor();
  virtual void reset();
  virtual void unlock();
#ifdef HAVE_QMSYSTEM
  virtual void updateDisplayState(MeeGo::QmDisplayState::DisplayState state);
#endif
}; 

// 2. IMPLEMENT STUB
void LockScreenStub::LockScreenConstructor() {

}
void LockScreenStub::LockScreenDestructor() {

}

void LockScreenStub::unlock() {
  stubMethodEntered("unlock");
}

void LockScreenStub::reset() {
  stubMethodEntered("reset");
}

#ifdef HAVE_QMSYSTEM
void LockScreenStub::updateDisplayState(MeeGo::QmDisplayState::DisplayState state) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MeeGo::QmDisplayState::DisplayState >(state));
  stubMethodEntered("updateDisplayState",params);
}
#endif

// 3. CREATE A STUB INSTANCE
LockScreenStub gDefaultLockScreenStub;
LockScreenStub* gLockScreenStub = &gDefaultLockScreenStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
LockScreen::LockScreen(QGraphicsItem *)
{
  gLockScreenStub->LockScreenConstructor();
}

LockScreen::~LockScreen() {
  gLockScreenStub->LockScreenDestructor();
}

void LockScreen::unlock() {
  gLockScreenStub->unlock();
}

void LockScreen::reset() {
  gLockScreenStub->reset();
}

#ifdef HAVE_QMSYSTEM
void LockScreen::updateDisplayState(MeeGo::QmDisplayState::DisplayState state) {
  gLockScreenStub->updateDisplayState(state);
}
#endif

#endif
