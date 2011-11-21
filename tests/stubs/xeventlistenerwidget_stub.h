/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
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
#ifndef XEVENTLISTENERWIDGET_STUB
#define XEVENTLISTENERWIDGET_STUB

#include "xeventlistenerwidget.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class XEventListenerWidgetStub : public StubBase {
  public:
  virtual void XEventListenerWidgetConstructor(QWidget *parent, Window id);
  virtual void XEventListenerWidgetDestructor();
  virtual bool x11Event(XEvent *event);
}; 

// 2. IMPLEMENT STUB
void XEventListenerWidgetStub::XEventListenerWidgetConstructor(QWidget *parent, Window id) {
  Q_UNUSED(parent);
  Q_UNUSED(id);

}
void XEventListenerWidgetStub::XEventListenerWidgetDestructor() {

}
bool XEventListenerWidgetStub::x11Event(XEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<XEvent * >(event));
  stubMethodEntered("x11Event",params);
  return stubReturnValue<bool>("x11Event");
}



// 3. CREATE A STUB INSTANCE
XEventListenerWidgetStub gDefaultXEventListenerWidgetStub;
XEventListenerWidgetStub* gXEventListenerWidgetStub = &gDefaultXEventListenerWidgetStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
XEventListenerWidget::XEventListenerWidget(QWidget *parent, Window id) {
  gXEventListenerWidgetStub->XEventListenerWidgetConstructor(parent, id);
}

XEventListenerWidget::~XEventListenerWidget() {
  gXEventListenerWidgetStub->XEventListenerWidgetDestructor();
}

bool XEventListenerWidget::x11Event(XEvent *event) {
  return gXEventListenerWidgetStub->x11Event(event);
}


#endif
