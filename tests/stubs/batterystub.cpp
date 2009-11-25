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
    levelIndex = 0;
    levelIndexInc = 1;
    energyLevel = 100;
    state = QmBattery::StateNotCharging;
    type = QmBattery::Wall;
}

int QmBattery::getRemainingIdleTime(QmBattery::RemainingTimeMode mode) const
{
    if(mode == QmBattery::PowersaveMode)
        return energyLevel * 3 / 2;
    else
        return energyLevel * 3;
}

int QmBattery::getRemainingTalkTime(QmBattery::RemainingTimeMode mode) const
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

QmBattery::State QmBattery::getState()
{
    return state;
}

QmBattery::Level QmBattery::getLevel()
{
    return levels.at(levelIndex);
}

void QmBattery::changeLevel()
{
    if(levelIndex == 2) {
        levelIndexInc = -1;        
    }    
    else if(levelIndex == 0) {
        levelIndexInc = 1;        
    }

    levelIndex += levelIndexInc;

    emit batteryLevelChanged(levels.at(levelIndex));

    switch(levelIndex) {
        case 0:
            energyLevel = 90;
            state = QmBattery::StateNotCharging;
            emit batteryStatusChanged(QmBattery::StateNotCharging);
            break;
        case 1:
            energyLevel = 15;            
            break;
        default: //2
            energyLevel = 5;
            state = QmBattery::StateCharging;
            emit batteryStatusChanged(QmBattery::StateCharging);
            break;
    }
    emit batteryEnergyLevelChanged(energyLevel);
}

void QmBattery::setBatteryEnergyLevel(int level)
{
    energyLevel = level;
}

}

