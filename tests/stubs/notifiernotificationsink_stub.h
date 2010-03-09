/***************************************************************************
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

#ifndef NOTIFIERNOTIFICATIONSINK_STUB
#define NOTIFIERNOTIFICATIONSINK_STUB

#include "notifiernotificationsink.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotifierNotificationSinkStub : public StubBase {
  public:
  virtual void NotifierNotificationSinkConstructor();
  virtual void NotifierNotificationSinkDestructor();
  virtual void addGroup(uint groupId, const NotificationParameters &parameters);
  virtual void removeGroup(uint groupId);
  virtual void addNotification(const Notification &notification);
  virtual void removeNotification(uint notificationId);
   QHash<uint, uint> groupForNotification ;
   QHash<uint, uint> notificationCountForNotification ;
   QHash<uint, uint> notificationCountForGroup ;
   uint notificationCount ;
  virtual void updateNotificationCount();
}; 

// 2. IMPLEMENT STUB
void NotifierNotificationSinkStub::NotifierNotificationSinkConstructor() {

}
void NotifierNotificationSinkStub::NotifierNotificationSinkDestructor() {

}

void NotifierNotificationSinkStub::addGroup(uint groupId, const NotificationParameters &parameters) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(groupId));
  params.append( new Parameter<const NotificationParameters & >(parameters));
  stubMethodEntered("addGroup",params);
}

void NotifierNotificationSinkStub::removeGroup(uint groupId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(groupId));
  stubMethodEntered("removeGroup",params);
}

void NotifierNotificationSinkStub::addNotification(const Notification &notification) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const Notification & >(notification));
  stubMethodEntered("addNotification",params);
}

void NotifierNotificationSinkStub::removeNotification(uint notificationId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(notificationId));
  stubMethodEntered("removeNotification",params);
}

void NotifierNotificationSinkStub::updateNotificationCount() {
  stubMethodEntered("updateNotificationCount");
}



// 3. CREATE A STUB INSTANCE
NotifierNotificationSinkStub gDefaultNotifierNotificationSinkStub;
NotifierNotificationSinkStub* gNotifierNotificationSinkStub = &gDefaultNotifierNotificationSinkStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
NotifierNotificationSink::NotifierNotificationSink() {
  gNotifierNotificationSinkStub->NotifierNotificationSinkConstructor();
}

NotifierNotificationSink::~NotifierNotificationSink() {
  gNotifierNotificationSinkStub->NotifierNotificationSinkDestructor();
}

void NotifierNotificationSink::addGroup(uint groupId, const NotificationParameters &parameters) {
  gNotifierNotificationSinkStub->addGroup(groupId, parameters);
}

void NotifierNotificationSink::removeGroup(uint groupId) {
  gNotifierNotificationSinkStub->removeGroup(groupId);
}

void NotifierNotificationSink::addNotification(const Notification &notification) {
  gNotifierNotificationSinkStub->addNotification(notification);
}

void NotifierNotificationSink::removeNotification(uint notificationId) {
  gNotifierNotificationSinkStub->removeNotification(notificationId);
}

void NotifierNotificationSink::updateNotificationCount() {
  gNotifierNotificationSinkStub->updateNotificationCount();
}


#endif
