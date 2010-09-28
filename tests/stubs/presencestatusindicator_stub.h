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
#ifndef PRESENCESTATUSINDICATOR_STUB
#define PRESENCESTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class PresenceStatusIndicatorStub : public StubBase
{
public:
    virtual void PresenceStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent);
    virtual void PresenceStatusIndicatorDestructor();
    virtual void presenceChanged();
    ContextItem *presence ;
};

// 2. IMPLEMENT STUB
void PresenceStatusIndicatorStub::PresenceStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);

}
void PresenceStatusIndicatorStub::PresenceStatusIndicatorDestructor()
{

}
void PresenceStatusIndicatorStub::presenceChanged()
{
    stubMethodEntered("presenceChanged");
}



// 3. CREATE A STUB INSTANCE
PresenceStatusIndicatorStub gDefaultPresenceStatusIndicatorStub;
PresenceStatusIndicatorStub *gPresenceStatusIndicatorStub = &gDefaultPresenceStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
PresenceStatusIndicator::PresenceStatusIndicator(ApplicationContext &context, QGraphicsItem *parent)
{
    gPresenceStatusIndicatorStub->PresenceStatusIndicatorConstructor(context, parent);
}

PresenceStatusIndicator::~PresenceStatusIndicator()
{
    gPresenceStatusIndicatorStub->PresenceStatusIndicatorDestructor();
}

void PresenceStatusIndicator::presenceChanged()
{
    gPresenceStatusIndicatorStub->presenceChanged();
}


#endif
