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
#ifndef QMBATTERY_STUB
#define QMBATTERY_STUB

#include "stubbase.h"
/*
 * XXX: This stub contains only those methods which are 
 * used by the  BatteryBusinessLogic class ...
 */

/*
 * Declare stub
 */
class QmBatteryStub : public StubBase
{
public:
    virtual Maemo::QmBattery::ChargerType getChargerType ();
    virtual Maemo::QmBattery::ChargingState getChargingState ();
    virtual Maemo::QmBattery::BatteryState getBatteryState ();
    virtual int getRemainingTalkTime (Maemo::QmBattery::RemainingTimeMode mode);
    virtual int getRemainingIdleTime (Maemo::QmBattery::RemainingTimeMode mode);
    virtual int getRemainingCapacityPct ();
    virtual int getMaxBars ();
};

Maemo::QmBattery::ChargingState
QmBatteryStub::getChargingState ()
{
    stubMethodEntered ("getChargingState");
    return stubReturnValue<Maemo::QmBattery::ChargingState> ("getChargingState");
}

Maemo::QmBattery::ChargerType
QmBatteryStub::getChargerType ()
{
    stubMethodEntered ("getChargerType");
    return stubReturnValue<Maemo::QmBattery::ChargerType> ("getChargerType");
}


Maemo::QmBattery::BatteryState
QmBatteryStub::getBatteryState ()
{
    stubMethodEntered ("getBatteryState");
    return stubReturnValue<Maemo::QmBattery::BatteryState> ("getBatteryState");
}


int
QmBatteryStub::getRemainingTalkTime (Maemo::QmBattery::RemainingTimeMode mode)
{
    QList<ParameterBase*> params;
    params.append (new Parameter<Maemo::QmBattery::RemainingTimeMode> (mode));
    stubMethodEntered ("getRemainingTalkTime", params);
    return stubReturnValue<int> ("getRemainingTalkTime");
}

int
QmBatteryStub::getRemainingIdleTime (Maemo::QmBattery::RemainingTimeMode mode)
{
    QList<ParameterBase*> params;
    params.append (new Parameter<Maemo::QmBattery::RemainingTimeMode> (mode));
    stubMethodEntered ("getRemainingIdleTime", params);
    return stubReturnValue<int> ("getRemainingIdleTime");
}

int
QmBatteryStub::getRemainingCapacityPct ()
{
    stubMethodEntered ("getRemainingCapacityPct");
    return stubReturnValue<int> ("getRemainingCapacityPct");
}

int
QmBatteryStub::getMaxBars ()
{
    stubMethodEntered ("getMaxBars");
    return stubReturnValue<int> ("getMaxBars");
}


/*
 * Create a stub instance
 */
QmBatteryStub gDefaultQmBatteryStub;
QmBatteryStub *gQmBatteryStub = &gDefaultQmBatteryStub;

/*
 * Create a proxy which calls the stub
 */
namespace Maemo
{

QmBattery::QmBattery (QObject *parent)
{
    Q_UNUSED (parent);
    /*
     * Do nothing, we don't want to instantiate a real QmBattery...
     */
}

QmBattery::~QmBattery ()
{

}

QmBattery::ChargerType
QmBattery::getChargerType () const
{
    return gQmBatteryStub->getChargerType ();
}

QmBattery::ChargingState
QmBattery::getChargingState() const
{
    return gQmBatteryStub->getChargingState ();
}

QmBattery::BatteryState
QmBattery::getBatteryState () const
{
    return gQmBatteryStub->getBatteryState ();
}

int
QmBattery::getRemainingTalkTime (QmBattery::RemainingTimeMode mode) const
{
    return gQmBatteryStub->getRemainingTalkTime (mode);
}

int
QmBattery::getRemainingIdleTime (QmBattery::RemainingTimeMode mode) const
{
    return gQmBatteryStub->getRemainingIdleTime (mode);
}

int
QmBattery::getRemainingCapacityPct () const
{
    return gQmBatteryStub->getRemainingCapacityPct ();
}

int
QmBattery::getMaxBars () const
{
    return gQmBatteryStub->getMaxBars ();
}

} /* namespace Maemo */

#endif
