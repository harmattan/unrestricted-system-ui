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
    levels << QmBattery::LevelFull << QmBattery::Level4 << QmBattery::Level3 << QmBattery::Level2 << QmBattery::LevelLow << QmBattery::LevelCritical;
    levelIndex = 0;
    levelIndexIncrease = 1;

    charging = false;
    QTimer::singleShot(10000, this, SLOT(updateValues()));
}

void QmBattery::updateValues()
{
    levelIndex += levelIndexIncrease;
    if ( levelIndex == 6 ) {
        levelIndexIncrease = -1;
        emit batteryStatusChanged(QmBattery::StateCharging);
        charging = true;        
        QTimer::singleShot(10000, this, SLOT(updateValues()));
        return;
    }
    else if( levelIndex == -1 ) {
        levelIndexIncrease = 1;
        emit batteryStatusChanged(QmBattery::StateNotCharging);
        charging = false;
        levelIndex = 0;
        QTimer::singleShot(10000, this, SLOT(updateValues()));
        return;
    }    

    emit batteryLevelChanged(levels.at(levelIndex));    

    QTimer::singleShot(10000, this, SLOT(updateValues()));
}

int QmBattery::remainingTime()
{
    if(!charging) {
        return 60 * 60 * 2 + 60 * 20;
    }
    else {
        return -1;
    }
}

int QmBattery::remainingTalkTime()
{
    if(!charging) {
        return 60 * 60 * 3 + 60 * 44;
    }
    else {
        return -1;
    }
}

QmBattery::State QmBattery::getState()
{
    if(charging)
        return QmBattery::StateCharging;
    else
        return QmBattery::StateNotCharging;
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
