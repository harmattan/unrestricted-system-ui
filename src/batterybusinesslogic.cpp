#include "batterybusinesslogic.h"
#include <DuiLocale>

BatteryBusinessLogic::BatteryBusinessLogic()
{
    uiNotif = new Notifier();
    battery = new QmBattery();
    deviceMode = new QmDeviceMode();

    connect(battery, SIGNAL(batteryLevelChanged(QmBattery::Level level)),
            this, SLOT(batteryLevelChanged(QmBattery::Level level)));
    connect(battery, SIGNAL(batteryStateChanged(QmBattery::State state)),
            this, SLOT(batteryStateChanged(QmBattery::State state)));

    /* Not yet implemented in QmBattery
    connect(battery, SIGNAL(remainingTalkTimeChanged(int secondsLeft)),
            this, SLOT(remainingTalkTimeChanged(int secondsLeft)));
            */

    checkBattery();
}

BatteryBusinessLogic::~BatteryBusinessLogic()
{
    delete uiNotif;
    uiNotif = NULL;
    delete battery;
    battery = NULL;
}

// This method should be called also when the device is returned from sleep mode
void BatteryBusinessLogic::checkBattery()
{
    checkChargingState();
    checkBatteryLevel();
}

void BatteryBusinessLogic::remainingTalkTimeChanged(int secondsLeft)
{
    int powerSaveTimeSetByUser = 600; //dummy 600 seconds == 10 minutes

    if( secondsLeft <= powerSaveTimeSetByUser) {   
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

void BatteryBusinessLogic::checkBatteryLevel()
{
    int chargeLevelPercentage = battery->chargeLevelPercentage();
    QmBattery::Level level;

    if(chargeLevelPercentage >= 85)
        level = QmBattery::LevelFull;

    /* Not yet implemented in QmBattery (not necessarily even needed)
    else if(chargeLevelPercentage < 85 && chargeLevelPercentage >= 60)
        level = QmBattery::Level75;
    else if(chargeLevelPercentage < 60 && chargeLevelPercentage >= 35)
        level = QmBattery::Level50;
    else if(chargeLevelPercentage < 35 && chargeLevelPercentage >= 15)
        level = QmBattery::Level25;
        */
    else if(chargeLevelPercentage < 15 && chargeLevelPercentage >= 5)
        level = QmBattery::LevelLow;
    else //chargeLevelPercentage < 5
        level = QmBattery::LevelCritical;

    batteryLevelChanged(level);

}

void BatteryBusinessLogic::batteryStateChanged(QmBattery::State state)
{
    switch(state) {        
        case QmBattery::StateCharging:            
            uiNotif->showNotification(trid("qtn_ener_char", "Charging"));
        break;        
        default: //QmBattery::StateNotCharging
        break;
    }    
}

void BatteryBusinessLogic::batteryLevelChanged(QmBattery::Level level)
{
    switch(level) {
        case QmBattery::LevelFull:
            if(battery->isCharging()) {
                uiNotif->showNotification(trid("qtn_ener_charcomp", "Charging complete"));
                uiNotif->showNotification(trid("qtn_ener_remcha", "Disconnect charger from power supply to save energy"));
            }            
        break;
        case QmBattery::LevelLow:
            uiNotif->showNotification(trid("qtn_ener_lowbatt", "Low battery"));
        default:
        break;
   }
}
