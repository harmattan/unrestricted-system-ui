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
#ifndef INTERNETCONNECTIONSTATUSINDICATOR_STUB
#define INTERNETCONNECTIONSTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class InternetConnectionStatusIndicatorStub : public StubBase
{
public:
    virtual void InternetConnectionStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent);
    virtual void InternetConnectionStatusIndicatorDestructor();
    virtual void updateStatus();
};

// 2. IMPLEMENT STUB
void InternetConnectionStatusIndicatorStub::InternetConnectionStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);

}
void InternetConnectionStatusIndicatorStub::InternetConnectionStatusIndicatorDestructor()
{

}
void InternetConnectionStatusIndicatorStub::updateStatus()
{
    stubMethodEntered("updateStatus");
}


// 3. CREATE A STUB INSTANCE
InternetConnectionStatusIndicatorStub gDefaultInternetConnectionStatusIndicatorStub;
InternetConnectionStatusIndicatorStub *gInternetConnectionStatusIndicatorStub = &gDefaultInternetConnectionStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
InternetConnectionStatusIndicator::InternetConnectionStatusIndicator(ApplicationContext &context, QGraphicsItem *parent)
{
    gInternetConnectionStatusIndicatorStub->InternetConnectionStatusIndicatorConstructor(context, parent);
}

InternetConnectionStatusIndicator::~InternetConnectionStatusIndicator()
{
    gInternetConnectionStatusIndicatorStub->InternetConnectionStatusIndicatorDestructor();
}

void InternetConnectionStatusIndicator::updateStatus()
{
    gInternetConnectionStatusIndicatorStub->updateStatus();
}


#endif
