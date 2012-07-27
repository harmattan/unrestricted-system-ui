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
#ifndef BATTERYPERCENTAGESTATUSINDICATOR_STUB
#define BATTERYPERCENTAGESTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>

#ifdef HAVE_QMSYSTEM
#include <qmdevicemode.h>
#endif

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class BatteryPercentageStatusIndicatorStub : public StubBase
{
public:
    virtual void BatteryPercentageStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent);
    virtual void BatteryPercentageStatusIndicatorDestructor();
    virtual void batteryPercentageChanged();
};

// 2. IMPLEMENT STUB
void BatteryPercentageStatusIndicatorStub::BatteryPercentageStatusIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);

}
void BatteryPercentageStatusIndicatorStub::BatteryPercentageStatusIndicatorDestructor()
{

}
void BatteryPercentageStatusIndicatorStub::batteryPercentageChanged()
{
    stubMethodEntered("batteryPercentageChanged");
}

// 3. CREATE A STUB INSTANCE
BatteryPercentageStatusIndicatorStub gDefaultBatteryPercentageStatusIndicatorStub;
BatteryPercentageStatusIndicatorStub *gBatteryPercentageStatusIndicatorStub = &gDefaultBatteryPercentageStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
BatteryPercentageStatusIndicator::BatteryPercentageStatusIndicator(ApplicationContext &context, QGraphicsItem *parent)
{
    gBatteryPercentageStatusIndicatorStub->BatteryPercentageStatusIndicatorConstructor(context, parent);
}

BatteryPercentageStatusIndicator::~BatteryPercentageStatusIndicator()
{
    gBatteryPercentageStatusIndicatorStub->BatteryPercentageStatusIndicatorDestructor();
}

void BatteryPercentageStatusIndicator::batteryPercentageChanged()
{
    gBatteryPercentageStatusIndicatorStub->batteryPercentageChanged();
}

#endif // BATTERYPERCENTAGESTATUSINDICATOR_STUB
