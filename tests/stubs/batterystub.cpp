#include "batterystub.h"
#include <QTimer>
#include <QDebug>

namespace Maemo {

QmBattery::QmBattery(QObject *parent) :
        QObject(parent)
{
    initValues();    
}

QmBattery::~QmBattery()
{
}

void QmBattery::initValues()
{
    levels << QmBattery::LevelFull << QmBattery::LevelLow << QmBattery::LevelCritical;
    energyLevel = 100;
}

int QmBattery::remainingTime(QmBattery::RemainingTimeMode mode)
{
    if(mode == QmBattery::PowersaveMode)
        return energyLevel * 3 / 2;
    else
        return energyLevel * 3;
}

int QmBattery::remainingTalkTime(QmBattery::RemainingTimeMode mode)
{
    if(mode == QmBattery::PowersaveMode)
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

void QmBattery::setBatteryEnergyLevel(int level)
{
    energyLevel = level;
}

QmBattery::State QmBattery::getState()
{
    return state;
}

QmBattery::Level QmBattery::getLevel()
{
    return levels.at(0);
}


}
