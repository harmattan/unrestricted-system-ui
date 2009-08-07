#include "batterybusinesslogic.h"

BatteryBusinessLogic::BatteryBusinessLogic()
{
    uiNotif = new Notifier();
    battery = new QmBattery();
    deviceMode = new QmDeviceMode();

    connect(battery, SIGNAL( batteryLevelChanged(QmBattery::Level level) ),
            this, SLOT( batteryLevelChanged(QmBattery::Level level)));
    connect(battery, SIGNAL( batteryStateChanged(QmBattery::State state)),
            this, SLOT( batteryStateChanged(QmBattery::State state)));

//    charging = false;
    checkRemainingTime();
    checkChargingState();
}

BatteryBusinessLogic::~BatteryBusinessLogic()
{
    delete uiNotif;
    uiNotif = NULL;
    delete battery;
    battery = NULL;
}

void BatteryBusinessLogic::batteryLevelChanged(QmBattery::Level level)
{
    // Actually we don't do anything with the level in it's current format.
    // The level to turn on the power saving should be adjustable by the user.
    // Also user can switch off the power saving completely.

    // Anyway, the level can be LevelCritical, LevelFull or LevelLow

    checkRemainingTime();

    //if(level == QmBattery::LevelFull && charging)

}

void BatteryBusinessLogic::checkRemainingTime()
{
    int powerSaveTimeSetByUser = 600; //dummy 600 seconds == 10 minutes

    if( battery->remainingTime() <= powerSaveTimeSetByUser) {
        /* replace with this when implemented in QmBattery
    if( battery->remainingTalkTime() <= powerSaveTimeSetByUser) {
    */
        if(deviceMode->getPSMState() != QmDeviceMode::PSMStateOff)
            deviceMode->setPSMState(QmDeviceMode::PSMStateOn);
    }
    else if(deviceMode->getPSMState() != QmDeviceMode::PSMStateOn)
        deviceMode->setPSMState(QmDeviceMode::PSMStateOff);
}

void BatteryBusinessLogic::checkChargingState()
{    
    QmBattery::State state = QmBattery::StateNotCharging;
    if(battery->isCharging())
        state = QmBattery::StateCharging;
    batteryStateChanged(state);
}

void BatteryBusinessLogic::batteryStateChanged(QmBattery::State state)
{
    switch(state) {        
        case QmBattery::StateCharging:            
            //send signal to indicate battery charging
            //show a notification about charging
        break;        
        default: //QmBattery::StateNotCharging            
            //send signal to stop indicating battery charging
        break;
    }    
}

