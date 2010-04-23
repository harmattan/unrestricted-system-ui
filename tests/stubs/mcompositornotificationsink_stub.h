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

#ifndef MCOMPOSITORNOTIFICATIONSINK_STUB
#define MCOMPOSITORNOTIFICATIONSINK_STUB

#include "mcompositornotificationsink.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MCompositorNotificationSinkStub : public StubBase {
  public:
  virtual void MCompositorNotificationSinkConstructor();
  virtual void MCompositorNotificationSinkDestructor();
  virtual void addNotification(const Notification &notification);
  virtual void removeNotification(uint notificationId);
  virtual void timeout();
  virtual void setDisabled(bool disabled);
  virtual void makeNotificationWindow();
  virtual void updateNotification(const Notification &notification);
  virtual void notificationDone(uint notificationId, bool notificationIdInUse);
  virtual void addInfoBannerToWindow();
  virtual void removeWindow();
   bool sinkDisabled ;
   MWindow *window ;
   Notification newNotification ;
}; 

// 2. IMPLEMENT STUB
void MCompositorNotificationSinkStub::MCompositorNotificationSinkConstructor() {

}
void MCompositorNotificationSinkStub::MCompositorNotificationSinkDestructor() {

}

void MCompositorNotificationSinkStub::addNotification(const Notification &notification) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const Notification & >(notification));
  stubMethodEntered("addNotification",params);
}

void MCompositorNotificationSinkStub::removeNotification(uint notificationId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(notificationId));
  stubMethodEntered("removeNotification",params);
}

void MCompositorNotificationSinkStub::timeout() {
  stubMethodEntered("timeout");
}

void MCompositorNotificationSinkStub::setDisabled(bool disabled) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(disabled));
  stubMethodEntered("setDisabled",params);
}

void MCompositorNotificationSinkStub::makeNotificationWindow() {
  stubMethodEntered("makeNotificationWindow");
}

void MCompositorNotificationSinkStub::addInfoBannerToWindow() {
    stubMethodEntered("addInfoBannerToWindow");
}

void MCompositorNotificationSinkStub::removeWindow() {
    stubMethodEntered("removeWindow");
}

void MCompositorNotificationSinkStub::updateNotification(const Notification &notification) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const Notification & >(notification));
  stubMethodEntered("updateNotification",params);
}

void MCompositorNotificationSinkStub::notificationDone(uint notificationId, bool notificationIdInUse) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(notificationId));
  params.append( new Parameter<bool >(notificationIdInUse));
  stubMethodEntered("notificationDone",params);
}



// 3. CREATE A STUB INSTANCE
MCompositorNotificationSinkStub gDefaultMCompositorNotificationSinkStub;
MCompositorNotificationSinkStub* gMCompositorNotificationSinkStub = &gDefaultMCompositorNotificationSinkStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MCompositorNotificationSink::MCompositorNotificationSink() {
  gMCompositorNotificationSinkStub->MCompositorNotificationSinkConstructor();
}

MCompositorNotificationSink::~MCompositorNotificationSink() {
  gMCompositorNotificationSinkStub->MCompositorNotificationSinkDestructor();
}

void MCompositorNotificationSink::addNotification(const Notification &notification) {
  gMCompositorNotificationSinkStub->addNotification(notification);
}

void MCompositorNotificationSink::removeNotification(uint notificationId) {
  gMCompositorNotificationSinkStub->removeNotification(notificationId);
}

void MCompositorNotificationSink::timeout() {
  gMCompositorNotificationSinkStub->timeout();
}

void MCompositorNotificationSink::setDisabled(bool disabled) {
  gMCompositorNotificationSinkStub->setDisabled(disabled);
}

void MCompositorNotificationSink::updateNotification(const Notification &notification) {
  gMCompositorNotificationSinkStub->updateNotification(notification);
}

void MCompositorNotificationSink::notificationDone(uint notificationId, bool notificationIdInUse) {
  gMCompositorNotificationSinkStub->notificationDone(notificationId, notificationIdInUse);
}

void MCompositorNotificationSink::addInfoBannerToWindow() {
    gMCompositorNotificationSinkStub->addInfoBannerToWindow();
}

void MCompositorNotificationSink::hideWindow() {
    gMCompositorNotificationSinkStub->removeWindow();
}


#endif
