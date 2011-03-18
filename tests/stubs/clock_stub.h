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
#ifndef CLOCK_STUB
#define CLOCK_STUB

#include "clock.h"
#include <stubbase.h>
#include <MLocale>

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ClockStub : public StubBase {
  public:
  virtual void ClockConstructor(QGraphicsItem *parent);
  virtual void ClockDestructor();
  virtual void updateModelAndSetupTimer();
#ifdef HAVE_QMSYSTEM
  virtual void updateSettings(MeeGo::QmTime::WhatChanged whatChanged);
#endif
  virtual void enterDisplayEvent();
  virtual void exitDisplayEvent();
};

// 2. IMPLEMENT STUB
void ClockStub::ClockConstructor(QGraphicsItem *parent) {
  Q_UNUSED(parent);
}

void ClockStub::ClockDestructor() {
}

void ClockStub::updateModelAndSetupTimer() {
  stubMethodEntered("updateModelAndSetupTimer");
}

#ifdef HAVE_QMSYSTEM
void ClockStub::updateSettings(MeeGo::QmTime::WhatChanged whatChanged) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MeeGo::QmTime::WhatChanged >(whatChanged));
  stubMethodEntered("updateSettings",params);
}
#endif

void ClockStub::enterDisplayEvent() {
  stubMethodEntered("enterDisplayEvent");
}

void ClockStub::exitDisplayEvent() {
  stubMethodEntered("exitDisplayEvent");
}



// 3. CREATE A STUB INSTANCE
ClockStub gDefaultClockStub;
ClockStub* gClockStub = &gDefaultClockStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
Clock::Clock(QGraphicsItem *parent) {
  gClockStub->ClockConstructor(parent);
}

Clock::~Clock() {
}

void Clock::updateModelAndSetupTimer() {
  gClockStub->updateModelAndSetupTimer();
}

#ifdef HAVE_QMSYSTEM
void Clock::updateSettings(MeeGo::QmTime::WhatChanged whatChanged) {
  gClockStub->updateSettings(whatChanged);
}
#endif

void Clock::enterDisplayEvent() {
  gClockStub->enterDisplayEvent();
}

void Clock::exitDisplayEvent() {
  gClockStub->exitDisplayEvent();
}


#endif
