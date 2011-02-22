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
#ifndef CALLFORWARDINGSTATUSINDICATOR_STUB
#define CALLFORWARDINGSTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class CallForwardingStatusIndicatorStub : public StubBase
{
public:
    virtual void CallForwardingStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent);
    virtual void CallForwardingStatusIndicatorDestructor();
    virtual void callForwardingChanged();
};

// 2. IMPLEMENT STUB
void CallForwardingStatusIndicatorStub::CallForwardingStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);
}

void CallForwardingStatusIndicatorStub::CallForwardingStatusIndicatorDestructor()
{
}

void CallForwardingStatusIndicatorStub::callForwardingChanged()
{
    stubMethodEntered("callForwardingChanged");
}

// 3. CREATE A STUB INSTANCE
CallForwardingStatusIndicatorStub gDefaultCallForwardingStatusIndicatorStub;
CallForwardingStatusIndicatorStub *gCallForwardingStatusIndicatorStub = &gDefaultCallForwardingStatusIndicatorStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB
CallForwardingStatusIndicator::CallForwardingStatusIndicator(ApplicationContext &context, QGraphicsItem *parent)
{
    gCallForwardingStatusIndicatorStub->CallForwardingStatusIndicatorConstructor(context, parent);
}

CallForwardingStatusIndicator::~CallForwardingStatusIndicator()
{
    gCallForwardingStatusIndicatorStub->CallForwardingStatusIndicatorDestructor();
}

void CallForwardingStatusIndicator::callForwardingChanged()
{
    gCallForwardingStatusIndicatorStub->callForwardingChanged();
}

#endif
