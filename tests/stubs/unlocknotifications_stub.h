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
#ifndef UNLOCKNOTIFICATIONS_STUB
#define UNLOCKNOTIFICATIONS_STUB

#include "unlocknotifications.h"
#include <stubbase.h>


// 1. DECLARE STUB
class UnlockNotificationsStub : public StubBase {
  public:
  virtual void UnlockNotificationsConstructor();
  virtual void UnlockNotificationsDestructor();
  virtual void updateContents();
  virtual void orientationChangeEvent(MOrientationChangeEvent *event);
  virtual void sizeHint(Qt::SizeHint which, const QSizeF& constraint);
}; 

// 2. IMPLEMENT STUB
void UnlockNotificationsStub::UnlockNotificationsConstructor() {

}
void UnlockNotificationsStub::UnlockNotificationsDestructor() {

}
void UnlockNotificationsStub::updateContents() {
  stubMethodEntered("updateContents");
}
void UnlockNotificationsStub::orientationChangeEvent(MOrientationChangeEvent *event) {
  QList<ParameterBase*> params;
  params.append (new Parameter<MOrientationChangeEvent *>(event));
  stubMethodEntered("orientationChangeEvent",params);
}
void UnlockNotificationsStub::sizeHint(Qt::SizeHint which, const QSizeF& constraint) {
  QList<ParameterBase*> params;
  params.append (new Parameter<Qt::SizeHint>(which));
  params.append (new Parameter<QSizeF>(constraint));
  stubMethodEntered("sizeHint",params);
}

// 3. CREATE A STUB INSTANCE
UnlockNotificationsStub gDefaultUnlockNotificationsStub;
UnlockNotificationsStub* gUnlockNotificationsStub = &gDefaultUnlockNotificationsStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB
UnlockNotifications::UnlockNotifications() {
  gUnlockNotificationsStub->UnlockNotificationsConstructor();
}

UnlockNotifications::~UnlockNotifications() {
  gUnlockNotificationsStub->UnlockNotificationsDestructor();
}

void UnlockNotifications::updateContents()
{
  gUnlockNotificationsStub->updateContents();
}

void UnlockNotifications::orientationChangeEvent (MOrientationChangeEvent *event) {
  gUnlockNotificationsStub->orientationChangeEvent(event);
}

QSizeF UnlockNotifications::sizeHint (Qt::SizeHint which, const QSizeF& constraint) const {
  gUnlockNotificationsStub->sizeHint (which, constraint);
  return QSizeF(1.0,1.0);
}

#endif
