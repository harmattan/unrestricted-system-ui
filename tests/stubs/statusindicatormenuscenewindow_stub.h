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
#ifndef STATUSINDICATORMENUSCENEWINDOW_STUB
#define STATUSINDICATORMENUSCENEWINDOW_STUB

#include "statusindicatormenuwindow.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class StatusIndicatorMenuSceneWindowStub : public StubBase {
  public:
  virtual void StatusIndicatorMenuSceneWindowConstructor(QGraphicsItem *parent);
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
}; 

// 2. IMPLEMENT STUB
void StatusIndicatorMenuSceneWindowStub::StatusIndicatorMenuSceneWindowConstructor(QGraphicsItem *parent) {
  Q_UNUSED(parent);

}
void StatusIndicatorMenuSceneWindowStub::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mousePressEvent",params);
}

// 3. CREATE A STUB INSTANCE
StatusIndicatorMenuSceneWindowStub gDefaultStatusIndicatorMenuSceneWindowStub;
StatusIndicatorMenuSceneWindowStub* gStatusIndicatorMenuSceneWindowStub = &gDefaultStatusIndicatorMenuSceneWindowStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
StatusIndicatorMenuSceneWindow::StatusIndicatorMenuSceneWindow(QGraphicsItem *parent) {
  gStatusIndicatorMenuSceneWindowStub->StatusIndicatorMenuSceneWindowConstructor(parent);
}

void StatusIndicatorMenuSceneWindow::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  gStatusIndicatorMenuSceneWindowStub->mousePressEvent(event);
}

#endif
