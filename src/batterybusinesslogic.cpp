#include "batterybusinesslogic.h"

#include <QTimer>

#include <DuiLocale>
#include <DuiApplicationWindow>

BatteryBusinessLogic::BatteryBusinessLogic()
{
    uiNotif = new Notifier();
    battery = new QmBattery();
    deviceMode = new QmDeviceMode();
    batteryGConf = new BatteryGConf();

    /* init the battery levels */
    batteryLevels.insert(QmBattery::LevelFull, 100);
    //batteryLevels.insert(QmBattery::Level75, 85); //not yet implemented
    //batteryLevels.insert(QmBattery::Level50, 60); //not yet implemented
    //batteryLevels.insert(QmBattery::Level25, 35); //not yet implemented
    batteryLevels.insert(QmBattery::LevelLow, 15);
    batteryLevels.insert(QmBattery::LevelCritical, 5);

    /* check if gconfvalues need to initialized */
    initBatteryGConfKeys();

    qDebug() << "JAKE: " << batteryLevels.value(QmBattery::LevelFull);

    /* connect to QmSystem signals */
    connect(battery, SIGNAL(batteryLevelChanged(Maemo::QmBattery::Level)),
            this, SLOT(batteryLevelChanged(Maemo::QmBattery::Level)));
    connect(battery, SIGNAL(batteryStateChanged(Maemo::QmBattery::State)),
            this, SLOT(batteryStateChanged(Maemo::QmBattery::State)));

    /*
    connect(battery, SIGNAL(remainingTalkTimeChanged(int)),
            this, SLOT(remainingTalkTimeChanged(int)));
    */

    /* connect to GConf signals */
    connect(batteryGConf, SIGNAL(valueChanged(BatteryGConf::GConfKey,QVariant)),
            this, SLOT(gConfValueChanged(BatteryGConf::GConfKey, QVariant)));

    DuiApplicationWindow *win = new DuiApplicationWindow();    
    win->setWindowOpacity(0);
    win->show();

    /* just testing
    connect(uiNotif, SIGNAL(notifTimeout()), this, SLOT(activatePSM()));    
    QHash<QString,QString> staticVariables;
    staticVariables.insert(QString("%a"), QString("10"));

            uiNotif->showCancellableNotification(trid("qtn_ener_psnote", "Battery charge level less than %a%. Switching to power save in %b seconds"),
                                                 10,
                                                 QString("%b"),
                                                 staticVariables);
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

void BatteryBusinessLogic::initBatteryGConfKeys()
{
    if(batteryGConf->value(BatteryGConf::PSMThresholdValuesKey).toList().empty()) {
        batteryGConf->setValue(BatteryGConf::BatteryLevelKey, QVariant(battery->chargeLevel()));
        batteryGConf->setValue(BatteryGConf::BatterySystemSettingInUseKey, QVariant(false));
        batteryGConf->setValue(BatteryGConf::ChargingKey, QVariant(battery->isCharging()));
        batteryGConf->setValue(BatteryGConf::PSMDisabledKey, QVariant(false));
       batteryGConf->setValue(BatteryGConf::PSMThresholdKey, QVariant(batteryLevels.value(QmBattery::LevelLow)));
        batteryGConf->setValue(BatteryGConf::PSMToggleKey, QVariant(deviceMode->getPSMState() == Maemo::QmDeviceMode::PSMStateOn ? true : false));
        QList<QVariant> values;
        values << QVariant(batteryLevels.value(QmBattery::LevelCritical))
                << QVariant(batteryLevels.value(QmBattery::LevelLow))

                // TEMP
                << QVariant(35)
                << QVariant(60)
                << QVariant(85)

                //<< QVariant(batteryLevels.value(QmBattery::Level25))
                //<< QVariant(batteryLevels.value(QmBattery::Level50))
                //<< QVariant(batteryLevels.value(QmBattery::Level75))
                << QVariant(batteryLevels.value(QmBattery::LevelFull));

        batteryGConf->setValue(BatteryGConf::PSMThresholdValuesKey, QVariant(values));

        updateTimes();
    }
}


// This method should be called also when the device is returned from sleep mode
void BatteryBusinessLogic::checkBattery()
{
    checkChargingState();
    checkBatteryLevel();
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

/*
void BatteryBusinessLogic::remainingTalkTimeChanged(int secondsLeft)
{  
    batteryGConf->setValue(BatteryGConf::PSMremainingTalkTime, secondsLeft / 60);

    if(secondsLeft <= batteryGConf->value(BatteryGConf::PSMThresholdKey).toInt() * 60) {
        if(deviceMode->getPSMState() == QmDeviceMode::PSMStateOff
           && batteryGConf->value(BatteryGConf::PSMDisabledKey).toBool() == false) {
            // Send a notification that can be cancelled.
            // If it's not cancelled, after certain time the notifier emits a signal.
            // We catch this signal to set the PSM                       
            connect(uiNotif, SIGNAL(notifTimedOut()), this, SLOT(activatePSM()));

            QHash<QString,QString> staticVariables;
            QString number;
            staticVariables.insert(QString("%a"), number.setNum(battery->chargeLevelPercentage()));
            uiNotif->showCancellableNotification(trid("qtn_ener_psnote", "Battery charge level less than %a%. Switching to power save in %b seconds"),
                                                 10,
                                                 QString("%b"),
                                                 staticVariables);
        }            
    }
    else {
        togglePSM(false);
    }
}
*/

void BatteryBusinessLogic::batteryStateChanged(Maemo::QmBattery::State state)
{
    switch(state) {        
        case QmBattery::StateCharging:
            qDebug() << "Charging";
            batteryGConf->setValue(BatteryGConf::ChargingKey, QVariant(true));
            uiNotif->showNotification(trid("qtn_ener_char", "Charging"));            
            emit charging();
            break;
        case QmBattery::StateNotCharging:
            batteryGConf->setValue(BatteryGConf::ChargingKey, QVariant(false));
            break;
        default:
            break;
    }    
}

void BatteryBusinessLogic::batteryLevelChanged(Maemo::QmBattery::Level level)
{    
    batteryGConf->setValue(BatteryGConf::BatteryLevelKey, QVariant(level));

    checkPSMThreshold(level);

    switch(level) {
        case QmBattery::LevelFull:        
            if(battery->isCharging()) {                
                //how to show these? combined or right after each other?
                uiNotif->showNotification(trid("qtn_ener_charcomp", "Charging complete"));                
                uiNotif->showNotification(trid("qtn_ener_remcha", "Disconnect charger from power supply to save energy"));
            }            
        break;
        case QmBattery::LevelLow:
            if(!battery->isCharging())
                uiNotif->showNotification(trid("qtn_ener_lowbatt", "Low battery"));
        break;      
        default:
        break;
   }
}

void BatteryBusinessLogic::checkPSMThreshold(Maemo::QmBattery::Level level)
{       
    if(level <= batteryGConf->value(BatteryGConf::PSMThresholdKey).toInt()) {
        if(deviceMode->getPSMState() == QmDeviceMode::PSMStateOff
           && batteryGConf->value(BatteryGConf::PSMDisabledKey).toBool() == false) {
            // Send a notification that can be cancelled.
            // If it's not cancelled, after certain time the notifier emits a signal.
            // We catch this signal to set the PSM
            connect(uiNotif, SIGNAL(notifTimedOut()), this, SLOT(activatePSM()));

            QHash<QString,QString> staticVariables;
            QString number;
            staticVariables.insert(QString("%a"), number.setNum(battery->chargeLevelPercentage()));
            uiNotif->showCancellableNotification(trid("qtn_ener_psnote", "Battery charge level less than %a%. Switching to power save in %b seconds"),
                                                 10,
                                                 QString("%b"),
                                                 staticVariables);
        }
    }
    else {
        togglePSM(false);
    }
}

void BatteryBusinessLogic::activatePSM()
{
    qDebug() << "Activate PSM";
    disconnect(uiNotif, SIGNAL(notifTimeout()), this, SLOT(activatePSM()));
    togglePSM(true);
}

void BatteryBusinessLogic::togglePSM(bool toggle)
{
    if(toggle) { //turn on the PSM
        if(deviceMode->getPSMState() == QmDeviceMode::PSMStateOff
           && batteryGConf->value(BatteryGConf::PSMDisabledKey).toBool() == false) {
            qDebug() << "Turn ON The PSM";
            deviceMode->setPSMState(QmDeviceMode::PSMStateOn);            
        }
    }
    else { //turn off the PSM
        if(deviceMode->getPSMState() == QmDeviceMode::PSMStateOn) {
            qDebug() << "Turn OFF The PSM";
            deviceMode->setPSMState(QmDeviceMode::PSMStateOff);            
        }
    }
}

void BatteryBusinessLogic::pollTimes(bool toggle)
{    
    if(toggle) {
        updateTimes();
        timer = new QTimer(0);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateTimes()));
        timer->start(10000); //update times every 10th second
    }
    else {
        timer->stop();
        delete timer;
        timer = NULL;
    }
}

void  BatteryBusinessLogic::updateTimes()
{
    //TODO: remove the stub values when remaining-methods are in use
    qDebug() << "Update TIMES";
    batteryGConf->setValue(BatteryGConf::RemainingTalkTimeKey, 120/*battery->remainingTalkTime() * 60*/);
    batteryGConf->setValue(BatteryGConf::RemainingStandByTimeKey, 300 /*battery->remainingStandByTime() * 60 */);
}

void BatteryBusinessLogic::gConfValueChanged(BatteryGConf::GConfKey key, QVariant value)
{
    switch(key) {
        case BatteryGConf::PSMToggleKey: // PSM was toggled manually
            togglePSM(value.toBool());
            break;            
        case BatteryGConf::PSMDisabledKey:
            if(value.toBool()) // PSM was disabled
                togglePSM(false);
            else // PSM was enabled
                checkPSMThreshold((Maemo::QmBattery::Level)battery->chargeLevel());
            break;
        case BatteryGConf::PSMThresholdKey: // threshold value was changed
            checkPSMThreshold((Maemo::QmBattery::Level)battery->chargeLevel());
            break;
        case BatteryGConf::BatterySystemSettingInUseKey: // battery system setting is in use
            if(value.toBool())
                pollTimes(true);
            else
                pollTimes(false);
        default:
            break;
    }
}
