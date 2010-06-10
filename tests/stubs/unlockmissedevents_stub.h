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
  virtual void getLastType();
  virtual void getLastSubject(int type);
  virtual void getCount(int type);
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
void UnlockMissedEventsStub::getLastType() {
  stubMethodEntered("getLastType");
}
void UnlockMissedEventsStub::getLastSubject(int type) {
  QList<ParameterBase*> params;
  params.append (new Parameter<int>(type));
  stubMethodEntered("getLastSubject",params);
}
void UnlockMissedEventsStub::getCount(int type) {
  QList<ParameterBase*> params;
  params.append (new Parameter<int>(type));
  stubMethodEntered("getCount",params);
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
}

UnlockMissedEvents::~UnlockMissedEvents() {
  gUnlockMissedEventsStub->UnlockMissedEventsDestructor();
}

UnlockMissedEvents& UnlockMissedEvents::getInstance()
{
  gUnlockMissedEventsStub->getInstance();
  return m_instance;
}

UnlockMissedEvents::Types UnlockMissedEvents::getLastType() {
  gUnlockMissedEventsStub->getLastType();
  return lastType;
}

QString UnlockMissedEvents::getLastSubject(Types atype) {
  gUnlockMissedEventsStub->getLastSubject((int) atype);
  return QString ("");
}

int UnlockMissedEvents::getCount(Types atype) {
  gUnlockMissedEventsStub->getCount ((int) atype);
  return 1;
}

void UnlockMissedEvents::addNotification(Types type, QString subject) {
  gUnlockMissedEventsStub->addNotification((int) type, subject);
}

void UnlockMissedEvents::clearAll() {
  gUnlockMissedEventsStub->clearAll();
}

#endif
