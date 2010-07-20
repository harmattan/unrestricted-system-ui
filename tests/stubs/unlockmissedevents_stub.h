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
  virtual void getInstance();
  virtual UnlockMissedEvents::Types getLastType();
  virtual QString getLastSubject(int type);
  virtual int getCount(int type);
  virtual void addNotification(int type, QString subject);
  virtual void clearAll();
}; 

// 2. IMPLEMENT STUB
void UnlockMissedEventsStub::UnlockMissedEventsConstructor() {

}
void UnlockMissedEventsStub::UnlockMissedEventsDestructor() {

}
void UnlockMissedEventsStub::getInstance() {
  stubMethodEntered("getInstance");
}

UnlockMissedEvents::Types
UnlockMissedEventsStub::getLastType() {
  stubMethodEntered("getLastType");
  return stubReturnValue<UnlockMissedEvents::Types > ("getLastType");
}

QString
UnlockMissedEventsStub::getLastSubject(int type) 
{
  QList<ParameterBase*> params;
  params.append (new Parameter<int>(type));
  stubMethodEntered("getLastSubject", params);
  return stubReturnValue<QString> ("getLastSubject");
}

int UnlockMissedEventsStub::getCount(int type) {
  QList<ParameterBase*> params;
  params.append (new Parameter<int>(type));
  stubMethodEntered("getCount",params);
  
  return stubReturnValue<int> ("getCount");
}
void UnlockMissedEventsStub::addNotification(int type, QString subject) {
  QList<ParameterBase*> params;
  params.append (new Parameter<int>(type));
  params.append (new Parameter<QString>(subject));
  stubMethodEntered("addNotification",params);
}
void UnlockMissedEventsStub::clearAll() {
  stubMethodEntered("clearAll");
}

// 3. CREATE A STUB INSTANCE
UnlockMissedEventsStub gDefaultUnlockMissedEventsStub;
UnlockMissedEventsStub* gUnlockMissedEventsStub = &gDefaultUnlockMissedEventsStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB

UnlockMissedEvents UnlockMissedEvents::m_instance;

UnlockMissedEvents::UnlockMissedEvents() {
  lastType = NotifyLast; 
  gUnlockMissedEventsStub->UnlockMissedEventsConstructor();

  for (int i = 0; i < NotifyLast; i++)
    notificationCounts[i] = 0;
}

UnlockMissedEvents::~UnlockMissedEvents() {
  gUnlockMissedEventsStub->UnlockMissedEventsDestructor();
}

UnlockMissedEvents& UnlockMissedEvents::getInstance()
{
  gUnlockMissedEventsStub->getInstance();
  return m_instance;
}

UnlockMissedEvents::Types 
UnlockMissedEvents::getLastType() {
  return gUnlockMissedEventsStub->getLastType();
}

QString UnlockMissedEvents::getLastSubject(Types atype) {
  return gUnlockMissedEventsStub->getLastSubject((int) atype);
}

int UnlockMissedEvents::getCount(Types atype) {
  return gUnlockMissedEventsStub->getCount ((int) atype);
}

void UnlockMissedEvents::addNotification(Types type, QString subject) {
  gUnlockMissedEventsStub->addNotification((int) type, subject);
}

void UnlockMissedEvents::clearAll() {
  gUnlockMissedEventsStub->clearAll();
}

#endif
