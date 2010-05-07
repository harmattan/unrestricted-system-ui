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


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ClockStub : public StubBase {
  public:
  virtual void ClockConstructor(ApplicationContext &context, QGraphicsItem *parent);
  virtual void ClockDestructor();

  virtual void updateModelAndSetupTimer();
  virtual void updateSettings(Maemo::QmTimeWhatChanged whatChanged);
  virtual void updateAlarmSet();
  virtual void enterDisplayEvent();
  virtual void exitDisplayEvent();
}; 

// 2. IMPLEMENT STUB
void ClockStub::ClockConstructor(ApplicationContext &context, QGraphicsItem *parent) {
  Q_UNUSED(context);
  Q_UNUSED(parent);
}

void ClockStub::ClockDestructor() {
}

void ClockStub::updateModelAndSetupTimer() {
  stubMethodEntered("updateModelAndSetupTimer");
}

void ClockStub::updateSettings(Maemo::QmTimeWhatChanged whatChanged) {
  QList<ParameterBase*> params;
  params.append( new Parameter<Maemo::QmTimeWhatChanged >(whatChanged));
  stubMethodEntered("updateSettings",params);
}

void ClockStub::updateAlarmSet() {
  stubMethodEntered("updateAlarmSet");
}

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
Clock::Clock(ApplicationContext &context, QGraphicsItem *parent) {
  gClockStub->ClockConstructor(context, parent);
}

Clock::~Clock() {
  gClockStub->ClockDestructor();
}

void Clock::updateModelAndSetupTimer() {
  gClockStub->updateModelAndSetupTimer();
}

void Clock::updateSettings(Maemo::QmTimeWhatChanged whatChanged) {
  gClockStub->updateSettings(whatChanged);
}

void Clock::updateAlarmSet() {
  gClockStub->updateAlarmSet();
}

void Clock::enterDisplayEvent() {
  gClockStub->enterDisplayEvent();
}

void Clock::exitDisplayEvent() {
  gClockStub->exitDisplayEvent();
}


#endif
