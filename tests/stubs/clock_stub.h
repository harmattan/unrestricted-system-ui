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
  virtual void ClockConstructor(QGraphicsItem *parent);

  virtual void updateModelAndSetupTimer();
#ifdef HAVE_QMSYSTEM
  virtual void updateSettings(Maemo::QmTimeWhatChanged whatChanged);
#endif
  virtual void setShortDisplay(bool isShort);
  virtual void enterDisplayEvent();
  virtual void exitDisplayEvent();
}; 

// 2. IMPLEMENT STUB
void ClockStub::ClockConstructor(QGraphicsItem *parent) {
  Q_UNUSED(parent);
}

void ClockStub::updateModelAndSetupTimer() {
  stubMethodEntered("updateModelAndSetupTimer");
}

#ifdef HAVE_QMSYSTEM
void ClockStub::updateSettings(Maemo::QmTimeWhatChanged whatChanged) {
  QList<ParameterBase*> params;
  params.append( new Parameter<Maemo::QmTimeWhatChanged >(whatChanged));
  stubMethodEntered("updateSettings",params);
}
#endif

void ClockStub::setShortDisplay(bool isShort) {
  QList<ParameterBase*> params;
  params.append(new Parameter<bool>(isShort));
  stubMethodEntered("setShortDisplay", params);
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
Clock::Clock(QGraphicsItem *parent) {
  gClockStub->ClockConstructor(parent);
}

void Clock::updateModelAndSetupTimer() {
  gClockStub->updateModelAndSetupTimer();
}

#ifdef HAVE_QMSYSTEM
void Clock::updateSettings(Maemo::QmTimeWhatChanged whatChanged) {
  gClockStub->updateSettings(whatChanged);
}
#endif

void Clock::setShortDisplay(bool isShort) {
  gClockStub->setShortDisplay(isShort);
}

void Clock::enterDisplayEvent() {
  gClockStub->enterDisplayEvent();
}

void Clock::exitDisplayEvent() {
  gClockStub->exitDisplayEvent();
}


#endif
