#include "batterybusinesslogic.h"
#include <qmsystem/qmled.h>

#include <QTimer>

#include <DuiLocale>
#include <DuiApplicationWindow>

#include "sysuid.h"

BatteryBusinessLogic::BatteryBusinessLogic(SystemUIGConf *systemUIGConf) :
        systemUIGConf(systemUIGConf)
{    
    battery = new QmBattery();
    deviceMode = new QmDeviceMode();    
    uiNotif = Sysuid::notifier();

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
    connect(battery, SIGNAL(batteryStateChanged(int, int)),
            this, SLOT(batteryLevelChanged(int, int)));
    connect(battery, SIGNAL(batteryStatusChanged(Maemo::QmBattery::State)),
            this, SLOT(batteryStatusChanged(Maemo::QmBattery::State)));
    connect(deviceMode, SIGNAL(devicePSMStateChanged(Maemo::QmDeviceMode::PSMState)),
            this, SLOT(devicePSMStateChanged(Maemo::QmDeviceMode::PSMState)));

    initBattery();
}

BatteryBusinessLogic::~BatteryBusinessLogic()
{
    uiNotif = NULL; // don't delete, not owned!
    delete battery;
    battery = NULL;       
}


void BatteryBusinessLogic::initSystemUIGConfKeys()
{
    if(systemUIGConf->keyCount(SystemUIGConf::Battery) < 3) {
        /* GConf keys have not yet been set. */        
        systemUIGConf->setValue(SystemUIGConf::BatteryPSMAutoKey, QVariant(true));

        //TODO: Replace the hardcoded value
        systemUIGConf->setValue(SystemUIGConf::BatteryPSMThresholdKey, QVariant("10"));
    }    
}


// This method should be called also when the device is returned from sleep mode
void BatteryBusinessLogic::initBattery()
{
    //init the charging status
    batteryStatusChanged(battery->getState());

    //init the battery level
    batteryLevelChanged(battery->bars(), battery->maxBars());
}

int BatteryBusinessLogic::batteryLevelValue()
{
    /*
    int chargeLevelPercentage = batteryLevelPercentage();
    QmBattery::Level level;
    if(chargeLevelPercentage >= 85)
        level = QmBattery::LevelFull;    
    else if(chargeLevelPercentage < 85 && chargeLevelPercentage >= 60)
        level = QmBattery::Level75;
    else if(chargeLevelPercentage < 60 && chargeLevelPercentage >= 35)
        level = QmBattery::Level50;
    else if(chargeLevelPercentage < 35 && chargeLevelPercentage >= 15)
        level = QmBattery::Level25;        
    else if(chargeLevelPercentage < 15 && chargeLevelPercentage >= 5)
        level = QmBattery::LevelLow;
    else //chargeLevelPercentage < 5
        level = QmBattery::LevelCritical;

    return (int)level; //type has to be int. called elsewhere as well
    */
    return 2;
    return battery->bars();
}

void BatteryBusinessLogic::batteryStatusChanged(Maemo::QmBattery::State state)
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

void BatteryBusinessLogic::batteryLevelChanged(int bars, int maxBars)
{
    emit batteryLevelValueChanged((int)bars);
    checkPSMThreshold(bars);

    switch(bars) {
        case 6: //currently hard coded
            if(batteryChargingState()) {
                utiliseLED(true, QString("PatternBatteryFull"));
                //how long will this be shown?
                QTimer::singleShot(5000, this, SLOT(utiliseLED(false, QString("PatternBatteryFull"))));
                uiNotif->showNotification(trid("qtn_ener_charcomp", "Charging complete"));
                //what about the time gap between these notifs?
                uiNotif->showNotification(trid("qtn_ener_remcha", "Disconnect charger from power supply to save energy"));
            }            
            break;
        case 2: //currently hard coded
            if(!batteryChargingState())
                uiNotif->showNotification(trid("qtn_ener_lowbatt", "Low battery"));
            break;
        default:
            break;
   }
}

void BatteryBusinessLogic::devicePSMStateChanged(Maemo::QmDeviceMode::PSMState PSMState)
{
    if(PSMState == QmDeviceMode::PSMStateOff)
        PSMValueChanged(false);
    else if(PSMState == QmDeviceMode::PSMStateOn)
        PSMValueChanged(true);
}

void BatteryBusinessLogic::checkPSMThreshold(int bars)
{
    qDebug() << "BatteryBusinessLogic::checkPSMThreshold(" << bars << ")";
    if(bars <= systemUIGConf->value(SystemUIGConf::BatteryPSMThresholdKey).toInt()) {
        if(deviceMode->getPSMState() == QmDeviceMode::PSMStateOff)
            deviceMode->setPSMState(QmDeviceMode::PSMStateOn);
    }
    else {
        if(deviceMode->getPSMState() == QmDeviceMode::PSMStateOn)
            deviceMode->setPSMState(QmDeviceMode::PSMStateOff);
    }
}

bool BatteryBusinessLogic::PSMValue()
{
    qDebug() << "BatteryBusinessLogic::PSMValue()";    
    return deviceMode->getPSMState() == QmDeviceMode::PSMStateOn;
}

void BatteryBusinessLogic::togglePSM(bool toggle)
{
    qDebug() << "BatteryBusinessLogic::togglePSM(" << toggle << ")";    
    if(toggle) { //turn on the PSM
        deviceMode->setPSMState(QmDeviceMode::PSMStateOn);

        //TEMP!!! Remove when deviceMode really sends the signals
        emit PSMValueChanged(true);
    }
    else { //turn off the PSM
        deviceMode->setPSMState(QmDeviceMode::PSMStateOff);

        //TEMP!!! Remove when deviceMode really sends the signals
        emit PSMValueChanged(false);
    }    

}

void BatteryBusinessLogic::togglePSMAuto(bool toggle)
{
    qDebug() << "BatteryBusinessLogic::togglePSMAuto(" << toggle << ")";
    systemUIGConf->setValue(SystemUIGConf::BatteryPSMAutoKey, QVariant(toggle));
    checkPSMThreshold(battery->bars());
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
    //TODO: replace hardcoded values with real ones when they are available
    QStringList values;
    values << QString("10") << QString("20") << QString("30") << QString("40") << QString("50");

    /*
    values << batteryLevels.value(QmBattery::LevelCritical)
            << batteryLevels.value(QmBattery::LevelLow)            
            << batteryLevels.value(QmBattery::Level75)
            << batteryLevels.value(QmBattery::Level50)
            << batteryLevels.value(QmBattery::Level25)            
            << batteryLevels.value(QmBattery::LevelFull);
    */
    return values;

}

bool BatteryBusinessLogic::batteryChargingState()
{
    return true;
    return battery->getState() == QmBattery::StateCharging;
}
