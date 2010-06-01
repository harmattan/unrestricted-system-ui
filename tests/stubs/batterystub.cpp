/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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
#include "batterystub.h"
#include <QTimer>
#include <QDebug>

#define DEBUG
#include "../../../src/debug.h"

namespace Maemo
{

QmBattery::QmBattery(QObject *parent) :
        QObject(parent)
{
    SYS_DEBUG ("");
    initValues();
}

QmBattery::~QmBattery()
{
}

void QmBattery::initValues()
{
    levels << QmBattery::LevelFull << QmBattery::LevelLow << QmBattery::LevelCritical;
    states << QmBattery::StateFull << QmBattery::StateLow << QmBattery::StateEmpty;
    levelIndex = 0;
    levelIndexInc = 1;
    energyLevel = 100;
    state = QmBattery::StateNotCharging;
    type = QmBattery::Wall;
}

int QmBattery::getRemainingIdleTime(QmBattery::RemainingTimeMode mode) const
{
    if (mode == QmBattery::PowersaveMode)
        return energyLevel * 3 / 2;
    else
        return energyLevel * 3;
}

int QmBattery::getRemainingTalkTime(QmBattery::RemainingTimeMode mode) const
{
    if (mode == QmBattery::PowersaveMode)
        return energyLevel * 2 / 2;
    else
        return energyLevel * 2;
}

QmBattery::ChargerType QmBattery::getChargerType()
{
    return type;
}

int QmBattery::getBatteryEnergyLevel()
{
    return energyLevel;
}

QmBattery::State 
QmBattery::getState ()
{
    return state;
}

QmBattery::ChargingState QmBattery::getChargingState()
{
    return state;
}

QmBattery::Level QmBattery::getLevel()
{
    return levels.at(levelIndex);
}

QmBattery::BatteryState QmBattery::getBatteryState()
{
    return states.at(levelIndex);
}

void QmBattery::changeLevel()
{
    if (levelIndex == 2) {
        levelIndexInc = -1;
    } else if (levelIndex == 0) {
        levelIndexInc = 1;
    }

    levelIndex += levelIndexInc;

    // Deprecated:
    emit batteryLevelChanged(levels.at(levelIndex));

    emit batteryStateChanged(states.at(levelIndex));

    switch (levelIndex) {
    case 0:
        energyLevel = 90;
        state = QmBattery::StateNotCharging;
        // Deprecated:
        emit batteryStatusChanged(QmBattery::StateNotCharging);

        emit chargingStateChanged(QmBattery::StateNotCharging);
        break;
    case 1:
        energyLevel = 15;
        break;
    default: //2
        energyLevel = 5;
        state = QmBattery::StateCharging;
        // Deprecated:
        emit batteryStatusChanged(QmBattery::StateCharging);

        emit chargingStateChanged(QmBattery::StateCharging);
        break;
    }
    emit batteryEnergyLevelChanged(energyLevel);
}

void 
QmBattery::setBatteryEnergyLevel(int level)
{
    energyLevel = level;
}

void 
QmBattery::emitBatteryStateChanged (
        Maemo::QmBattery::BatteryState state)
{
    SYS_DEBUG ("emitting batteryStateChanged (%d)", state);
    emit batteryStateChanged (state);
}
void
QmBattery::emitChargerEvent (
        Maemo::QmBattery::ChargerType type)
{
    emit chargerEvent (type);
}

void 
QmBattery::emitChargingStateChanged (
        Maemo::QmBattery::ChargingState state)
{
    emit chargingStateChanged (state);
}

}

