#include "batterystub.h"
#include <QTimer>
#include <QDebug>

namespace Maemo {

#ifdef UTILISE_BATTERY_USER
void QmBatteryUser::start()
{
    emit changeLevel();
    QTimer::singleShot(5000, this, SLOT(start()));
}
#endif

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
    energyLevel = 100;

#ifdef UTILISE_BATTERY_USER
    batteryUser = new QmBatteryUser();
    connect(batteryUser, SIGNAL(changeLevel()), this, SLOT(changeLevel()));
    batteryUser->start();
#endif

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
    if(levelIndex < 2)
        levelIndex++;
    else
        levelIndex = 0;
    emit batteryLevelChanged(levels.at(levelIndex));

    switch(levelIndex) {
        case 0:
            energyLevel = 90;
        case 1:
            energyLevel = 15;
        default: //2
            energyLevel = 5;
    }
    emit batteryEnergyLevelChanged(energyLevel);

}

}

