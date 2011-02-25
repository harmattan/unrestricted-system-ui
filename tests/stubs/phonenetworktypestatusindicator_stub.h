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
#ifndef PHONENETWORKTYPESTATUSINDICATOR_STUB
#define PHONENETWORKTYPESTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class PhoneNetworkTypeStatusIndicatorStub : public StubBase
{
public:
    virtual void PhoneNetworkTypeStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent);
    virtual void PhoneNetworkTypeStatusIndicatorDestructor();
    virtual void setNetworkAvailability(bool available);
    virtual void setNetworkType();
};

// 2. IMPLEMENT STUB
void PhoneNetworkTypeStatusIndicatorStub::PhoneNetworkTypeStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);

}
void PhoneNetworkTypeStatusIndicatorStub::PhoneNetworkTypeStatusIndicatorDestructor()
{

}

void PhoneNetworkTypeStatusIndicatorStub::setNetworkAvailability(bool available)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QVariant >(available));
    stubMethodEntered("setNetworkAvailability", params);
}

void PhoneNetworkTypeStatusIndicatorStub::setNetworkType()
{
    stubMethodEntered("setNetworkType");
}


// 3. CREATE A STUB INSTANCE
PhoneNetworkTypeStatusIndicatorStub gDefaultPhoneNetworkTypeStatusIndicatorStub;
PhoneNetworkTypeStatusIndicatorStub *gPhoneNetworkTypeStatusIndicatorStub = &gDefaultPhoneNetworkTypeStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
PhoneNetworkTypeStatusIndicator::PhoneNetworkTypeStatusIndicator(ApplicationContext &context, QGraphicsItem *parent)
{
    gPhoneNetworkTypeStatusIndicatorStub->PhoneNetworkTypeStatusIndicatorConstructor(context, parent);
}

PhoneNetworkTypeStatusIndicator::~PhoneNetworkTypeStatusIndicator()
{
    gPhoneNetworkTypeStatusIndicatorStub->PhoneNetworkTypeStatusIndicatorDestructor();
}

void PhoneNetworkTypeStatusIndicator::setNetworkAvailability(bool available)
{
    gPhoneNetworkTypeStatusIndicatorStub->setNetworkAvailability(available);
}


void PhoneNetworkTypeStatusIndicator::setNetworkType()
{
    gPhoneNetworkTypeStatusIndicatorStub->setNetworkType();
}



#endif
