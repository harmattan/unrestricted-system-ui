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
#ifndef EVENTTYPESTORE_STUB
#define EVENTTYPESTORE_STUB

#include "eventtypestore.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class EventTypeStoreStub : public StubBase {
  public:
  virtual void EventTypeStoreConstructor(const QString &eventTypesPath, uint maxStoredEventTypes);
  virtual bool eventTypeExists(const QString &eventType) const;
  virtual QList<QString> allKeys(const QString &eventType) const;
  virtual bool contains(const QString &eventType, const QString &key) const;
  virtual QString value(const QString &eventType, const QString &key) const;
};

// 2. IMPLEMENT STUB
void EventTypeStoreStub::EventTypeStoreConstructor(const QString &eventTypesPath, uint maxStoredEventTypes)
{
    Q_UNUSED(eventTypesPath);
    Q_UNUSED(maxStoredEventTypes);

}

bool EventTypeStoreStub::eventTypeExists(const QString &eventType) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(eventType));
    stubMethodEntered("eventTypeExists", params);
    return stubReturnValue<bool>("eventTypeExists");
}

QList<QString> EventTypeStoreStub::allKeys(const QString &eventType) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(eventType));
    stubMethodEntered("allKeys", params);
    return stubReturnValue<QList<QString> >("allKeys");
}

bool EventTypeStoreStub::contains(const QString &eventType, const QString &key) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(eventType));
    params.append(new Parameter<QString>(key));
    stubMethodEntered("contains", params);
    return stubReturnValue<bool>("contains");
}

QString EventTypeStoreStub::value(const QString &eventType, const QString &key) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(eventType));
    params.append(new Parameter<QString>(key));
    stubMethodEntered("value", params);
    return stubReturnValue<QString>("value");
}

// 3. CREATE A STUB INSTANCE
EventTypeStoreStub gDefaultEventTypeStoreStub;
EventTypeStoreStub *gEventTypeStoreStub = &gDefaultEventTypeStoreStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
EventTypeStore::EventTypeStore(const QString &eventTypesPath, uint maxStoredEventTypes)
{
    gEventTypeStoreStub->EventTypeStoreConstructor(eventTypesPath, maxStoredEventTypes);
}

bool EventTypeStore::eventTypeExists(const QString &eventType) const
{
    return gEventTypeStoreStub->eventTypeExists(eventType);
}

QList<QString> EventTypeStore::allKeys(const QString &eventType) const
{
    return gEventTypeStoreStub->allKeys(eventType);
}

bool EventTypeStore::contains(const QString &eventType, const QString &key) const
{
    return gEventTypeStoreStub->contains(eventType, key);
}

QString EventTypeStore::value(const QString &eventType, const QString &key) const
{
    return gEventTypeStoreStub->value(eventType, key);
}

void EventTypeStore::loadSettings(const QString&)
{
}

void EventTypeStore::updateEventTypeFileList()
{
}

void EventTypeStore::updateEventTypeFile(const QString&)
{
}

#endif
