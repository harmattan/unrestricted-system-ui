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

#include "lockscreenui.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LockScreenUIStub : public StubBase {
  public:
  virtual void LockScreenUIConstructor();
  virtual void LockScreenUIDestructor();
  virtual void updateDateTime();
  virtual void updateMissedEvents(int emails, int messages, int calls, int im);
  virtual void createContent();
  virtual void showEvent(QShowEvent *event);
  virtual void realize();
  virtual void sliderUnlocked();
}; 

// 2. IMPLEMENT STUB
void LockScreenUIStub::LockScreenUIConstructor() {

}
void LockScreenUIStub::LockScreenUIDestructor() {

}
void LockScreenUIStub::updateDateTime() {
  stubMethodEntered("updateDateTime");
}

void LockScreenUIStub::updateMissedEvents(int emails, int messages, int calls, int im) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(emails));
  params.append( new Parameter<int >(messages));
  params.append( new Parameter<int >(calls));
  params.append( new Parameter<int >(im));
  stubMethodEntered("updateMissedEvents",params);
}

void LockScreenUIStub::createContent() {
  stubMethodEntered("createContent");
}

void LockScreenUIStub::showEvent(QShowEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QShowEvent * >(event));
  stubMethodEntered("showEvent",params);
}

void LockScreenUIStub::realize() {
  stubMethodEntered("realize");
}

void LockScreenUIStub::sliderUnlocked() {
  stubMethodEntered("sliderUnlocked");
}



// 3. CREATE A STUB INSTANCE
LockScreenUIStub gDefaultLockScreenUIStub;
LockScreenUIStub* gLockScreenUIStub = &gDefaultLockScreenUIStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
LockScreenUI::LockScreenUI() {
  gLockScreenUIStub->LockScreenUIConstructor();
}

LockScreenUI::~LockScreenUI() {
  gLockScreenUIStub->LockScreenUIDestructor();
}

void LockScreenUI::updateDateTime() {
  gLockScreenUIStub->updateDateTime();
}

void LockScreenUI::updateMissedEvents(int emails, int messages, int calls, int im) {
  gLockScreenUIStub->updateMissedEvents(emails, messages, calls, im);
}

void LockScreenUI::createContent() {
  gLockScreenUIStub->createContent();
}

void LockScreenUI::showEvent(QShowEvent *event) {
  gLockScreenUIStub->showEvent(event);
}

void LockScreenUI::realize() {
  gLockScreenUIStub->realize();
}

void LockScreenUI::sliderUnlocked() {
  gLockScreenUIStub->sliderUnlocked();
}


#endif
