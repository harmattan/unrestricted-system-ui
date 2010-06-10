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
#ifndef UNLOCKMISSEDEVENTS_STUB
#define UNLOCKMISSEDEVENTS_STUB

#include "unlockmissedevents.h"
#include <stubbase.h>


// 1. DECLARE STUB
class UnlockMissedEventsStub : public StubBase {
  public:
  virtual void UnlockMissedEventsConstructor();
  virtual void UnlockMissedEventsDestructor();
  virtual void updateDateTime();
  virtual void createContent();
  virtual void showEvent(QShowEvent *event);
  virtual void realize();
  virtual void sliderUnlocked();
}; 

// 2. IMPLEMENT STUB
void UnlockMissedEventsStub::UnlockMissedEventsConstructor() {

}
void UnlockMissedEventsStub::UnlockMissedEventsDestructor() {

}
void UnlockMissedEventsStub::updateDateTime() {
  stubMethodEntered("updateDateTime");
}

void UnlockMissedEventsStub::createContent() {
  stubMethodEntered("createContent");
}

void UnlockMissedEventsStub::showEvent(QShowEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QShowEvent * >(event));
  stubMethodEntered("showEvent",params);
}

void UnlockMissedEventsStub::realize() {
  stubMethodEntered("realize");
}

void UnlockMissedEventsStub::sliderUnlocked() {
  stubMethodEntered("sliderUnlocked");
}



// 3. CREATE A STUB INSTANCE
UnlockMissedEventsStub gDefaultUnlockMissedEventsStub;
UnlockMissedEventsStub* gUnlockMissedEventsStub = &gDefaultUnlockMissedEventsStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
UnlockMissedEvents::UnlockMissedEvents() {
  gUnlockMissedEventsStub->UnlockMissedEventsConstructor();
}

UnlockMissedEvents::~UnlockMissedEvents() {
  gUnlockMissedEventsStub->UnlockMissedEventsDestructor();
}

void UnlockMissedEvents::updateDateTime() {
  gUnlockMissedEventsStub->updateDateTime();
}

void UnlockMissedEvents::createContent() {
  gUnlockMissedEventsStub->createContent();
}

void UnlockMissedEvents::showEvent(QShowEvent *event) {
  gUnlockMissedEventsStub->showEvent(event);
}

void UnlockMissedEvents::realize() {
  gUnlockMissedEventsStub->realize();
}

void UnlockMissedEvents::sliderUnlocked() {
  gUnlockMissedEventsStub->sliderUnlocked();
}


#endif
