#include "batterystub.h"
#include <QTimer>

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
    percentages << 90 << 80 << 70 << 60 << 40 << 30 << 20 << 10 << 5 << 3;

    levelIndex = 0;
    levelIndexIncrease = 1;

    emit batteryLevelChanged(QmBattery::LevelFull);

    state = QmBattery::StateNotCharging;
    type = QmBattery::Wall;
    QTimer::singleShot(10000, this, SLOT(updateValues()));
}

void QmBattery::updateValues()
{    
    previousIndex = levelIndex;
    levelIndex += levelIndexIncrease;
    switch(levelIndex) {
        case 0:
            if(previousIndex == 1)
                emit batteryLevelChanged(QmBattery::LevelFull);
            break;
        case 8:            
            emit batteryLevelChanged(QmBattery::LevelLow);
            break;
        case 10:
            if(previousIndex == 10) {
                levelIndexIncrease = -1;
                state = QmBattery::StateCharging;
                emit batteryStatusChanged(state);
            }
            else {
                levelIndexIncrease = 0;
                state = QmBattery::StateChargingFailed;
                emit batteryStatusChanged(state);
            }
            QTimer::singleShot(10000, this, SLOT(updateValues()));
            return;
        case -1:
            levelIndexIncrease = 1;
            state = QmBattery::StateNotCharging;
            emit batteryStatusChanged(state);
            emit chargerEvent(QmBattery::ChargerDisconnected);
            levelIndex = 0;
            QTimer::singleShot(10000, this, SLOT(updateValues()));
            return;
    }    

    emit batteryEnergyLevelChanged(percentages.at(levelIndex));

    QTimer::singleShot(10000, this, SLOT(updateValues()));
}

int QmBattery::remainingTime()
{
    if(!state ==  QmBattery::StateCharging) {
        return (10 - levelIndex) * 60 * 60  + 60 * 20;
    }
    else {
        return -1;
    }
}

int QmBattery::remainingTalkTime()
{
    if(!state ==  QmBattery::StateCharging) {
        return (10 - levelIndex) / 2 * 60 * 60 + 60 * 44;
    }
    else {
        return -1;
    }
}

QmBattery::ChargerType QmBattery::getChargerType()
{
    return type;
}

int QmBattery::getBatteryEnergyLevel()
{
    if(levelIndex == 10)
        return percentages.at(9);
    return percentages.at(levelIndex);
}

QmBattery::State QmBattery::getState()
{
    return state;
}

QmBattery::Level QmBattery::getLevel()
{
    if(levelIndex == 6)
        return levels.at(5);
    else if(levelIndex == -1)
        return levels.at(0);
    else
        return levels.at(levelIndex);
}


}
