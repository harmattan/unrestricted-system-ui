#include "batterybusinesslogic.h"
#include <qmsystem/qmled.h>

#include <QTimer>

#include <DuiLocale>
#include <DuiApplicationWindow>

BatteryBusinessLogic::BatteryBusinessLogic(SystemUIGConf *systemUIGConf) :
        systemUIGConf(systemUIGConf)
{    
    battery = new QmBattery();
    deviceMode = new QmDeviceMode();    
    uiNotif = new Notifier();    

    /* init the battery levels */
    batteryLevels.insert(QmBattery::LevelFull, QString("100"));
    //batteryLevels.insert(QmBattery::Level75, 85); //not yet implemented
    //batteryLevels.insert(QmBattery::Level50, 60); //not yet implemented
    //batteryLevels.insert(QmBattery::Level25, 35); //not yet implemented
    batteryLevels.insert(QmBattery::LevelLow, QString("15"));
    batteryLevels.insert(QmBattery::LevelCritical, QString("5"));

    /* check if gconfvalues need to initialized */
    initSystemUIGConfKeys();

    /* connect to QmSystem signals */
    connect(battery, SIGNAL(batteryLevelChanged(Maemo::QmBattery::Level)),
            this, SLOT(batteryLevelChanged(Maemo::QmBattery::Level)));
    connect(battery, SIGNAL(batteryStateChanged(Maemo::QmBattery::State)),
            this, SLOT(batteryStateChanged(Maemo::QmBattery::State)));

    initBattery();
}

BatteryBusinessLogic::~BatteryBusinessLogic()
{
    delete uiNotif;
    uiNotif = NULL;
    delete battery;
    battery = NULL;       
}


void BatteryBusinessLogic::initSystemUIGConfKeys()
{
    if(systemUIGConf->keyCount(SystemUIGConf::Battery) < 3) {
        /* GConf keys have not yet been set. */
        systemUIGConf->setValue(SystemUIGConf::BatteryPSMToggleKey, QVariant(deviceMode->getPSMState() == Maemo::QmDeviceMode::PSMStateOn ? true : false));
        systemUIGConf->setValue(SystemUIGConf::BatteryPSMDisabledKey, QVariant(false));
        systemUIGConf->setValue(SystemUIGConf::BatteryPSMThresholdKey, QVariant(batteryLevels.value(QmBattery::LevelLow)));
    }    
}


// This method should be called also when the device is returned from sleep mode
void BatteryBusinessLogic::initBattery()
{
    //init the charging status
    batteryStateChanged(battery->getState());

    //init the battery level
    batteryLevelChanged((QmBattery::Level)batteryLevelValue());
}

int BatteryBusinessLogic::batteryLevelPercentage()
{
    return 55; //100*battery->bars()/battery->maxBars();
}

int BatteryBusinessLogic::batteryLevelValue()
{
    int chargeLevelPercentage = batteryLevelPercentage();
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

    return (int)level; //type has to be int. called elsewhere as well
}

void BatteryBusinessLogic::batteryStateChanged(Maemo::QmBattery::State state)
{
    switch(state) {        
        case QmBattery::StateCharging:
            qDebug() << "Charging";
            emit batteryCharging();
            utiliseLED(true, QString("PatternBatteryCharging"));            
            uiNotif->showNotification(trid("qtn_ener_char", "Charging"));                        
            break;
        case QmBattery::StateNotCharging:
            emit batteryNotCharging();
            utiliseLED(false, QString("PatternBatteryCharging"));
            break;
        default:
            break;
    }    
}

void BatteryBusinessLogic::batteryLevelChanged(Maemo::QmBattery::Level level)
{
    emit batteryLevelValueChanged((int)level);
    checkPSMThreshold(level);

    switch(level) {
        case QmBattery::LevelFull:
            if(battery->getState() == QmBattery::StateCharging) {
                utiliseLED(true, QString("PatternBatteryFull"));
                QTimer::singleShot(5000, this, SLOT(utiliseLED(false, QString("PatternBatteryFull"))));
                uiNotif->showNotification(trid("qtn_ener_charcomp", "Charging complete"));                
                uiNotif->showNotification(trid("qtn_ener_remcha", "Disconnect charger from power supply to save energy"));
            }            
            break;
        case QmBattery::LevelLow:
            if(battery->getState() != QmBattery::StateCharging)
                uiNotif->showNotification(trid("qtn_ener_lowbatt", "Low battery"));
            break;
        default:
            break;
   }
}

void BatteryBusinessLogic::checkPSMThreshold(Maemo::QmBattery::Level level)
{
    qDebug() << "BatteryBusinessLogic::checkPSMThreshold(" << level << ")";
    if(level <= systemUIGConf->value(SystemUIGConf::BatteryPSMThresholdKey).toInt()) {
        if(deviceMode->getPSMState() == QmDeviceMode::PSMStateOff
           && systemUIGConf->value(SystemUIGConf::BatteryPSMDisabledKey).toBool() == false) {
            // Send a notification that can be cancelled.
            // If it's not cancelled, after certain time the notifier emits a signal.
            // We catch this signal to set the PSM
            connect(uiNotif, SIGNAL(notifTimeout()), this, SLOT(activatePSM()));

            QHash<QString,QString> staticVariables;
            QString number;
            staticVariables.insert(QString("%a"), number.setNum(batteryLevelPercentage()));
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
    qDebug() << "BatteryBusinessLogic::togglePSM(" << toggle << ")";    
    if(toggle) { //turn on the PSM
        if(deviceMode->getPSMState() == QmDeviceMode::PSMStateOff
           && systemUIGConf->value(SystemUIGConf::BatteryPSMDisabledKey).toBool() == false) {
            qDebug() << "Turn ON The PSM";
            deviceMode->setPSMState(QmDeviceMode::PSMStateOn);
            systemUIGConf->setValue(SystemUIGConf::BatteryPSMToggleKey, QVariant(toggle));
            emit PSMToggleValueChanged(toggle);
        }
    }
    else { //turn off the PSM
        if(deviceMode->getPSMState() == QmDeviceMode::PSMStateOn) {
            qDebug() << "Turn OFF The PSM";
            deviceMode->setPSMState(QmDeviceMode::PSMStateOff);
            systemUIGConf->setValue(SystemUIGConf::BatteryPSMToggleKey, QVariant(toggle));
            emit PSMToggleValueChanged(toggle);
        }
    }    
}

QStringList BatteryBusinessLogic::remainingTimeValues()
{          
    qDebug() << "BatteryBusinessLogic::remainingTimeValues()";

    QStringList values;

    //TODO: replace hardcoded values with correct ones and remove temp var
    static int temp = 0; //temp
    if(temp<1) {
        values << (QString("%1").arg(120))/*battery->remainingTalkTime() * 60*/ << (QString("%1").arg(300)) /*battery->remainingStandByTime() * 60 */;
        ++temp;
    }
    else {
        values << (QString("%1").arg(130))/*battery->remainingTalkTime() * 60*/ << (QString("%1").arg(350)) /*battery->remainingStandByTime() * 60 */;
        temp = 0;
    }
    return values;
}

void BatteryBusinessLogic::utiliseLED(bool activate, const QString &pattern)
{
    QmLED led;
    if(activate)
        led.activate(pattern);
    else
        led.deactivate(pattern);
}

void BatteryBusinessLogic::togglePSMDisabled(bool disabled)
{    
    qDebug() << "BatteryBusinessLogic::togglePSMDisabled(" << disabled << ")";
    systemUIGConf->setValue(SystemUIGConf::BatteryPSMDisabledKey, QVariant(disabled));
    if(disabled) //PSM disabled
        togglePSM(false);
    else
        checkPSMThreshold((Maemo::QmBattery::Level)battery->bars());
}

void BatteryBusinessLogic::setPSMThreshold(const QString &threshold)
{
    qDebug() << "BatteryBusinessLogic::setPSMthreshold(" << threshold << ")";
    systemUIGConf->setValue(SystemUIGConf::BatteryPSMThresholdKey, QVariant(threshold));
    checkPSMThreshold((Maemo::QmBattery::Level)battery->bars());
}

QVariant BatteryBusinessLogic::GConfItemValue(SystemUIGConf::GConfKey key)
{
    return systemUIGConf->value(key);
}

QStringList BatteryBusinessLogic::PSMThresholdValues()
{
    QStringList values;
    values << batteryLevels.value(QmBattery::LevelCritical)
            << batteryLevels.value(QmBattery::LevelLow)
            /* Not yet implemented in QmBattery
            << batteryLevels.value(QmBattery::Level75)
            << batteryLevels.value(QmBattery::Level50)
            << batteryLevels.value(QmBattery::Level25)
            */
            << batteryLevels.value(QmBattery::LevelFull);
    return values;

}

bool BatteryBusinessLogic::batteryChargingState()
{
    return battery->getState() == QmBattery::StateCharging;
}
