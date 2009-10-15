#include "batterybusinesslogic.h"
#include <qmsystem/qmled.h>

#include <QTimer>
#include <QDebug>

#include <DuiLocale>
#include <DuiApplicationWindow>

#include "sysuid.h"

namespace {
    const QString PSMActivateText = trid("qtn_ener_aps", "Activate power save now");
    const QString PSMDeactivateText = trid("qtn_ener_dps", "Deactivate power save now");
    const QString ChargingText = trid("qtn_ener_char", "Charging");
    const QString ChargingCompleteText = trid("qtn_ener_charcomp", "Charging complete");
    const QString DisconnectChargerText = trid("qtn_ener_remcha", "Disconnect charger from power supply to save energy");
    const QString LowBatteryText = trid("qtn_ener_lowbatt", "Low battery");

}

BatteryBusinessLogic::BatteryBusinessLogic(SystemUIGConf *systemUIGConf) :
        systemUIGConf(systemUIGConf)
{    
    battery = new QmBattery();
    deviceMode = new QmDeviceMode();    
    uiNotif = Sysuid::notifier();

    /* init the battery levels */
    batteryLevels.insert(QmBattery::LevelFull, 5);
    batteryLevels.insert(QmBattery::Level4, 4);
    batteryLevels.insert(QmBattery::Level3, 3);
    batteryLevels.insert(QmBattery::Level2, 2);
    batteryLevels.insert(QmBattery::LevelLow, 1);
    batteryLevels.insert(QmBattery::LevelCritical, 0);

    /* check if gconfvalues need to initialized */
    initSystemUIGConfKeys();

    /* connect to QmSystem signals */
    connect(battery, SIGNAL(batteryLevelChanged(Maemo::QmBattery::Level)),
            this, SLOT(batteryLevelChanged(Maemo::QmBattery::Level)));
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
        systemUIGConf->setValue(SystemUIGConf::BatteryPSMThresholdKey, QVariant(batteryLevels.value(QmBattery::LevelLow)));
    }    
}


// This method should be called also when the device is returned from sleep mode
void BatteryBusinessLogic::initBattery()
{
    //init the charging status
    batteryStatusChanged(battery->getState());

    //init the battery level
    batteryLevelChanged(battery->getLevel());
}

int BatteryBusinessLogic::batteryLevelValue()
{   
    return batteryLevels.value(battery->getLevel());
}

void BatteryBusinessLogic::batteryStatusChanged(Maemo::QmBattery::State state)
{
    switch(state) {        
        case QmBattery::StateCharging:
            qDebug() << "Charging";
            emit batteryCharging();
            utiliseLED(true, QString("PatternBatteryCharging"));            
            uiNotif->showNotification(ChargingText);
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
    qDebug() << "BatteryBusinessLogic::batteryLevelChanged(" << batteryLevels.value(level) << ")";
    emit batteryLevelValueChanged(batteryLevels.value(level));
    checkPSMThreshold(level);

    switch(level) {
        case QmBattery::LevelFull:
            qDebug() << "BatteryBusinessLogic::batteryLevelChanged(" << batteryLevels.value(level) << ") battery is full";
            if(isBatteryCharging()) {
                qDebug() << "BatteryBusinessLogic::batteryLevelChanged(" << batteryLevels.value(level) << ") battery is charging";
                utiliseLED(true, QString("PatternBatteryFull"));
                //how long will this be shown?
                QTimer::singleShot(5000, this, SLOT(utiliseLED(false, QString("PatternBatteryFull"))));
                uiNotif->showNotification(ChargingCompleteText);
                //what about the time gap between these notifs?
                uiNotif->showNotification(DisconnectChargerText);
            }            
            break;
        case QmBattery::LevelLow:
            qDebug() << "BatteryBusinessLogic::batteryLevelChanged(" << batteryLevels.value(level) << ") battery is low";
            if(!isBatteryCharging()) {
                qDebug() << "BatteryBusinessLogic::batteryLevelChanged(" << batteryLevels.value(level) << ") battery is not charging";
                uiNotif->showNotification(LowBatteryText);
            }
            break;
        default:
            break;
   }
}

void BatteryBusinessLogic::devicePSMStateChanged(Maemo::QmDeviceMode::PSMState PSMState)
{    
    if(PSMState == QmDeviceMode::PSMStateOff)
        PSMValueChanged(PSMActivateText);
    else if(PSMState == QmDeviceMode::PSMStateOn)
        PSMValueChanged(PSMDeactivateText);
}

void BatteryBusinessLogic::checkPSMThreshold(Maemo::QmBattery::Level level)
{
    qDebug() << "BatteryBusinessLogic::checkPSMThreshold(" << batteryLevels.value(level) << ", "
            << systemUIGConf->value(SystemUIGConf::BatteryPSMThresholdKey).toInt() << ", "
            << systemUIGConf->value(SystemUIGConf::BatteryPSMAutoKey).toBool() << ")";
    if(batteryLevels.value(level) <= systemUIGConf->value(SystemUIGConf::BatteryPSMThresholdKey).toInt()
        && systemUIGConf->value(SystemUIGConf::BatteryPSMAutoKey).toBool()) {
        if(deviceMode->getPSMState() == QmDeviceMode::PSMStateOff)
            deviceMode->setPSMState(QmDeviceMode::PSMStateOn);
    }
    else {
        if(deviceMode->getPSMState() == QmDeviceMode::PSMStateOn)
            deviceMode->setPSMState(QmDeviceMode::PSMStateOff);
    }
}

QString BatteryBusinessLogic::PSMValue()
{
    qDebug() << "BatteryBusinessLogic::PSMValue()";
    return ( deviceMode->getPSMState() == QmDeviceMode::PSMStateOn ? PSMDeactivateText : PSMActivateText );
}

void BatteryBusinessLogic::togglePSM(const QString &value)
{
    qDebug() << "BatteryBusinessLogic::togglePSM(" << value << ")";
    if(value == PSMActivateText)
        deviceMode->setPSMState(QmDeviceMode::PSMStateOn); //turn on the PSM
    else
        deviceMode->setPSMState(QmDeviceMode::PSMStateOff); //turn off the PSM

    //when ever we toggle PSM manually, we turn off the automatic PSM
    togglePSMAuto(false);
    emit PSMAutoValueChanged(false);

}

void BatteryBusinessLogic::togglePSMAuto(bool toggle)
{
    qDebug() << "BatteryBusinessLogic::togglePSMAuto(" << toggle << ")";
    systemUIGConf->setValue(SystemUIGConf::BatteryPSMAutoKey, QVariant(toggle));
    if(toggle) // if we trun on the Auto PSM, we must check the threshold
        checkPSMThreshold(battery->getLevel());
}

QStringList BatteryBusinessLogic::remainingTimeValues()
{          
    qDebug() << "BatteryBusinessLogic::remainingTimeValues()";

    QStringList values;
    values << QString("%1").arg(battery->remainingTalkTime() / 60) << QString("%1").arg(battery->remainingTime() / 60);
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
    checkPSMThreshold(battery->getLevel());
}

QVariant BatteryBusinessLogic::GConfItemValue(SystemUIGConf::GConfKey key)
{
    return systemUIGConf->value(key);
}

QStringList BatteryBusinessLogic::PSMThresholdValues()
{
    //TODO: replace hardcoded values with real ones when they are available
    QStringList values;    
    values << QString("%1").arg(batteryLevels.value(QmBattery::LevelCritical))
            << QString("%1").arg(batteryLevels.value(QmBattery::LevelLow))
            << QString("%1").arg(batteryLevels.value(QmBattery::Level2))
            << QString("%1").arg(batteryLevels.value(QmBattery::Level3))
            << QString("%1").arg(batteryLevels.value(QmBattery::Level4))
            << QString("%1").arg(batteryLevels.value(QmBattery::LevelFull));
    return values;
}

bool BatteryBusinessLogic::isBatteryCharging()
{
    return battery->getState() == QmBattery::StateCharging;
}
