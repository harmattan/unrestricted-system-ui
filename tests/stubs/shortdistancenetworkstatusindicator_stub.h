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
#ifndef SHORTDISTANCENETWORKSTATUSINDICATOR_STUB
#define SHORTDISTANCENETWORKSTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class ShortDistanceNetworkStatusIndicatorStub : public StubBase
{
public:
    virtual void ShortDistanceNetworkStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent);
    virtual void ShortDistanceNetworkStatusIndicatorDestructor();
    virtual void activityChanged();
    ContextItem *ShortDistanceNetwork;
};

// 2. IMPLEMENT STUB
void ShortDistanceNetworkStatusIndicatorStub::ShortDistanceNetworkStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);

}
void ShortDistanceNetworkStatusIndicatorStub::ShortDistanceNetworkStatusIndicatorDestructor()
{

}
void ShortDistanceNetworkStatusIndicatorStub::activityChanged()
{
    stubMethodEntered("activityChanged");
}



// 3. CREATE A STUB INSTANCE
ShortDistanceNetworkStatusIndicatorStub gDefaultShortDistanceNetworkStatusIndicatorStub;
ShortDistanceNetworkStatusIndicatorStub *gShortDistanceNetworkStatusIndicatorStub = &gDefaultShortDistanceNetworkStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
ShortDistanceNetworkStatusIndicator::ShortDistanceNetworkStatusIndicator(ApplicationContext &context, QGraphicsItem *parent)
{
    gShortDistanceNetworkStatusIndicatorStub->ShortDistanceNetworkStatusIndicatorConstructor(context, parent);
}

ShortDistanceNetworkStatusIndicator::~ShortDistanceNetworkStatusIndicator()
{
    gShortDistanceNetworkStatusIndicatorStub->ShortDistanceNetworkStatusIndicatorDestructor();
}

void ShortDistanceNetworkStatusIndicator::activityChanged()
{
    gShortDistanceNetworkStatusIndicatorStub->activityChanged();
}


#endif
