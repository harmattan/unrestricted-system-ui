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

#ifndef EVENTEATERWIDGET_STUB
#define EVENTEATERWIDGET_STUB

#include "statusindicatormenuwindow.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class EventEaterWidgetStub : public StubBase {
  public:
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

// 2. IMPLEMENT STUB
void EventEaterWidgetStub::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mousePressEvent",params);
}

// 3. CREATE A STUB INSTANCE
EventEaterWidgetStub gDefaultEventEaterWidgetStub;
EventEaterWidgetStub* gEventEaterWidgetStub = &gDefaultEventEaterWidgetStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
void EventEaterWidget::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  gEventEaterWidgetStub->mousePressEvent(event);
}

#endif
