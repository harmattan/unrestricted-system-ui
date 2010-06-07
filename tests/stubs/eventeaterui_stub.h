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

#include "lockscreenui.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class EventEaterUIStub : public StubBase {
  public:
  virtual void EventEaterUIConstructor();
  virtual void mousePressEvent(QMouseEvent *event);
  virtual void mouseReleaseEvent(QMouseEvent *event);
  virtual void showEvent(QShowEvent *event);
}; 

// 2. IMPLEMENT STUB
void EventEaterUIStub::EventEaterUIConstructor() {

}
void EventEaterUIStub::mousePressEvent(QMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QMouseEvent * >(event));
  stubMethodEntered("mousePressEvent",params);
}

void EventEaterUIStub::mouseReleaseEvent(QMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QMouseEvent * >(event));
  stubMethodEntered("mouseReleaseEvent",params);
}

void EventEaterUIStub::showEvent(QShowEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QShowEvent * >(event));
  stubMethodEntered("showEvent",params);
}



// 3. CREATE A STUB INSTANCE
EventEaterUIStub gDefaultEventEaterUIStub;
EventEaterUIStub* gEventEaterUIStub = &gDefaultEventEaterUIStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
EventEaterUI::EventEaterUI() {
  gEventEaterUIStub->EventEaterUIConstructor();
}

void EventEaterUI::mousePressEvent(QMouseEvent *event) {
  gEventEaterUIStub->mousePressEvent(event);
}

void EventEaterUI::mouseReleaseEvent(QMouseEvent *event) {
  gEventEaterUIStub->mouseReleaseEvent(event);
}

void EventEaterUI::showEvent(QShowEvent *event) {
  gEventEaterUIStub->showEvent(event);
}


#endif
