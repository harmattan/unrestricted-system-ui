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
#ifndef CALLSTATUSINDICATOR_STUB
#define CALLSTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class CallStatusIndicatorStub : public StubBase
{
public:
    virtual void CallStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent);
    virtual void CallStatusIndicatorDestructor();
    virtual void callOrMutedChanged();
};

// 2. IMPLEMENT STUB
void CallStatusIndicatorStub::CallStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);

}
void CallStatusIndicatorStub::CallStatusIndicatorDestructor()
{

}
void CallStatusIndicatorStub::callOrMutedChanged()
{
    stubMethodEntered("callOrMutedChanged");
}



// 3. CREATE A STUB INSTANCE
CallStatusIndicatorStub gDefaultCallStatusIndicatorStub;
CallStatusIndicatorStub *gCallStatusIndicatorStub = &gDefaultCallStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
CallStatusIndicator::CallStatusIndicator(ApplicationContext &context, QGraphicsItem *parent)
{
    gCallStatusIndicatorStub->CallStatusIndicatorConstructor(context, parent);
}

CallStatusIndicator::~CallStatusIndicator()
{
    gCallStatusIndicatorStub->CallStatusIndicatorDestructor();
}

void CallStatusIndicator::callOrMutedChanged()
{
    gCallStatusIndicatorStub->callOrMutedChanged();
}


#endif
