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
#ifndef EVENTEATERUI_STUB
#define EVENTEATERUI_STUB

#include "eventeater.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class EventEaterStub : public StubBase {
  public:
  virtual void EventEaterConstructor();
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void showEvent(QShowEvent *event);
}; 

// 2. IMPLEMENT STUB
void EventEaterStub::EventEaterConstructor() {

}
void EventEaterStub::mousePressEvent(QMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QMouseEvent * >(event));
  stubMethodEntered("mousePressEvent",params);
}

void EventEaterStub::mouseReleaseEvent(QMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QMouseEvent * >(event));
  stubMethodEntered("mouseReleaseEvent",params);
}

void EventEaterStub::showEvent(QShowEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QShowEvent * >(event));
  stubMethodEntered("showEvent",params);
}



// 3. CREATE A STUB INSTANCE
EventEaterStub gDefaultEventEaterStub;
EventEaterStub* gEventEaterStub = &gDefaultEventEaterStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
EventEater::EventEater() {
  gEventEaterStub->EventEaterConstructor();
}

void EventEater::mousePressEvent(QMouseEvent *event) {
  gEventEaterStub->mousePressEvent(event);
}

void EventEater::mouseReleaseEvent(QMouseEvent *event) {
  gEventEaterStub->mouseReleaseEvent(event);
}

void EventEater::showEvent(QShowEvent *event) {
  gEventEaterStub->showEvent(event);
}


#endif
