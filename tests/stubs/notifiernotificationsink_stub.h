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
#ifndef NOTIFIERNOTIFICATIONSINK_STUB
#define NOTIFIERNOTIFICATIONSINK_STUB

#include "notifiernotificationsink.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NotifierNotificationSinkStub : public StubBase {
  public:
  virtual void NotifierNotificationSinkConstructor(QObject *parent);
  virtual void NotifierNotificationSinkDestructor();
  virtual void addGroup(uint groupId, const NotificationParameters &parameters);
  virtual void removeGroup(uint groupId);
  virtual void addNotification(const Notification &notification);
  virtual void removeNotification(uint notificationId);
  virtual void setNotifierEnabled(bool active);
  virtual void updateStatusOfLedFeedback();

  void clearSink();
  void disableNotificationAdditions(bool disable);
};

// 2. IMPLEMENT STUB
void NotifierNotificationSinkStub::NotifierNotificationSinkConstructor(QObject *) {

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

void NotifierNotificationSinkStub::clearSink() {
  stubMethodEntered("clearSink");
}

void NotifierNotificationSinkStub::disableNotificationAdditions(bool disable)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(disable));
  stubMethodEntered("disableNotificationAdditions", params);
}

void NotifierNotificationSinkStub::setNotifierEnabled(bool enabled)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<bool>(enabled));
  stubMethodEntered("setNotifierEnabled", params);
}

void NotifierNotificationSinkStub::updateStatusOfLedFeedback()
{
  stubMethodEntered("updateStatusOfLedFeedback");
}

// 3. CREATE A STUB INSTANCE
NotifierNotificationSinkStub gDefaultNotifierNotificationSinkStub;
NotifierNotificationSinkStub* gNotifierNotificationSinkStub = &gDefaultNotifierNotificationSinkStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
NotifierNotificationSink::NotifierNotificationSink(QObject *parent) : NotificationSink(parent) {
  gNotifierNotificationSinkStub->NotifierNotificationSinkConstructor(parent);
}

NotifierNotificationSink::~NotifierNotificationSink() {
  gNotifierNotificationSinkStub->NotifierNotificationSinkDestructor();
}

void NotifierNotificationSink::addNotification(const Notification &notification) {
  gNotifierNotificationSinkStub->addNotification(notification);
}

void NotifierNotificationSink::removeNotification(uint notificationId) {
  gNotifierNotificationSinkStub->removeNotification(notificationId);
}

void NotifierNotificationSink::clearSink() {
  gNotifierNotificationSinkStub->clearSink();
}

void NotifierNotificationSink::disableNotificationAdditions(bool disable)
{
  gNotifierNotificationSinkStub->disableNotificationAdditions(disable);
}

void NotifierNotificationSink::setNotifierEnabled(bool enabled)
{
    gNotifierNotificationSinkStub->setNotifierEnabled(enabled);
}

void NotifierNotificationSink::updateStatusOfLedFeedback()
{
    gNotifierNotificationSinkStub->updateStatusOfLedFeedback();
}

#endif
