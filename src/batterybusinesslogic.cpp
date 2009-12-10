#include "batterybusinesslogic.h"

#include <DuiLocale>
#include <QTimer>
#include <QTime>

/* TODO List

   1) Create notification for RechargeBatteryText. Connect it to the signal from QmSystemState which inforsm the reason for shut down.
   2) What are correct animation rates when charging with USB / Wall?
   3) If USB 100 mA is used, do we show animation at all? In Fremantle not.
   4) Connect sounds with notifications   

*/

namespace {    
    const QString ChargingText = trid("qtn_ener_char", "Charging");
    const QString ChargingNotStartedText = trid("qtn_ener_repcharger", "Charging not started. Replace charger.");
    const QString ChargingCompleteText = trid("qtn_ener_charcomp", "Charging complete");
    const QString DisconnectChargerText = trid("qtn_ener_remcha", "Disconnect charger from power supply to save energy");
    const QString LowBatteryText = trid("qtn_ener_lowbatt", "Low battery");
    const QString EnterPSMText = trid("qtn_ener_ent_psnote", "Entering power save mode");
    const QString ExitPSMText = trid("qtn_ener_exit_psnote", "Exiting power save mode");
    const QString RechargeBatteryText = trid("qtn_ener_rebatt", "Recharge battery");
    const int LowBatteryActiveInterval = 30 * 60 * 1000; //30 mins
    const int LowBatteryInactiveInterval = 2 * 60 * 60 * 1000; //2 hours
    const int ChargingAnimationRateUSB = 800; // 800 ms
    const int ChargingAnimationRateWall = 400; // 400 ms
}

LowBatteryNotifier::LowBatteryNotifier(QObject* parent) :
        QObject(parent),
        display(new QmDisplayState()),        
        timer(new QTimer(this))
{
    qDebug() << Q_FUNC_INFO;
    sleep = ( display->get() == Maemo::QmDisplayState::Off ? true : false );
    activeInterval = LowBatteryActiveInterval;
    inactiveInterval = LowBatteryInactiveInterval;
    time.start();
    connect(display, SIGNAL(displayStateChanged(Maemo::QmDisplayState::DisplayState)),
            this, SLOT(displayStateChanged(Maemo::QmDisplayState::DisplayState)));
    connect(timer, SIGNAL(timeout()), this, SLOT(showLowBatteryNotification()));       
}

LowBatteryNotifier::~LowBatteryNotifier()
{
}

void LowBatteryNotifier::showLowBatteryNotification()
{    
    qDebug() << Q_FUNC_INFO;    
    emit showNotification(LowBatteryText);
    time.start(); //restart time
    switch(display->get()) {
        case Maemo::QmDisplayState::On:
        case Maemo::QmDisplayState::Dimmed:
            sleep = false;            
            timer->start(activeInterval);
            break;
        case Maemo::QmDisplayState::Off:
            sleep = true;
            timer->start(inactiveInterval);
            break;        
    }    
}

void LowBatteryNotifier::displayStateChanged(Maemo::QmDisplayState::DisplayState state)
{    
    qDebug() << Q_FUNC_INFO;
    switch(state) {
        case Maemo::QmDisplayState::On:
            if(!sleep)
                break;
            if(time.elapsed() < activeInterval)
                timer->setInterval(activeInterval - time.elapsed());
            else
                showLowBatteryNotification();
            sleep = false;
            break;
        case Maemo::QmDisplayState::Dimmed:
            sleep = false;
            break;
        case Maemo::QmDisplayState::Off:           
            timer->setInterval(inactiveInterval - time.elapsed());
            sleep = true;
            break;        
    }
}

BatteryBusinessLogic::BatteryBusinessLogic(SystemUIGConf *systemUIGConf, QObject* parent) :
        QObject(parent),
        systemUIGConf(systemUIGConf),        
        battery(NULL),
        deviceMode(NULL),
        led(NULL),
        lowBatteryNotifier(NULL)
{
    battery = new QmBattery();
    deviceMode = new QmDeviceMode();
    led = new QmLED();

     /* init the PSM thresholds */
    PSMThresholds << QString("5") << QString("10") << QString("15") << QString("25") << QString("35")
            << QString("50") << QString("60") << QString("75") << QString("85") << QString("100");

    /* check if gconfvalues need to initialized */
    initSystemUIGConfKeys();

    /* connect to QmSystem signals */
    connect(battery, SIGNAL(batteryLevelChanged(Maemo::QmBattery::Level)),
            this, SLOT(batteryLevelChanged(Maemo::QmBattery::Level)));
    connect(battery, SIGNAL(batteryStatusChanged(Maemo::QmBattery::State)),
            this, SLOT(batteryStatusChanged(Maemo::QmBattery::State)));
    connect(battery, SIGNAL(batteryEnergyLevelChanged(int)),
            this, SLOT(batteryEnergyLevelChanged(int)));
    connect(battery, SIGNAL(chargerEvent(Maemo::QmBattery::ChargerType)),
            this, SLOT(batteryChargerEvent(Maemo::QmBattery::ChargerType)));
    connect(deviceMode, SIGNAL(devicePSMStateChanged(Maemo::QmDeviceMode::PSMState)),
            this, SLOT(devicePSMStateChanged(Maemo::QmDeviceMode::PSMState)));

    initBattery();
}


BatteryBusinessLogic::~BatteryBusinessLogic()
{        
    delete battery;
    battery = NULL;
    delete deviceMode;
    deviceMode = NULL;
    delete led;
    led = NULL;
}

void BatteryBusinessLogic::initSystemUIGConfKeys()
{
    if(systemUIGConf->keyCount(SystemUIGConf::Battery) < 2) {
        /* GConf keys have not yet been set. */        
        systemUIGConf->setValue(SystemUIGConf::BatteryPSMAutoKey, QVariant(true));       
        systemUIGConf->setValue(SystemUIGConf::BatteryPSMThresholdKey, QVariant(1));
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

int BatteryBusinessLogic::batteryBarValue(int percentage)
{   
    if(percentage == -1)
        percentage = battery->getBatteryEnergyLevel();

    int index = 0;
    if(percentage >= 88)
        index = PSMThresholds.indexOf("100");
    else if(percentage < 88 && percentage >= 75)
        index = PSMThresholds.indexOf("85");
    else if(percentage < 75 && percentage >= 62)
        index = PSMThresholds.indexOf("75");
    else if(percentage < 62 && percentage >= 50)
        index = PSMThresholds.indexOf("60");
    else if(percentage < 50 && percentage >= 38)
        index = PSMThresholds.indexOf("50");
    else if(percentage < 38 && percentage >= 25)
        index = PSMThresholds.indexOf("35");
    else if(percentage < 25 && percentage >= 13)
        index = PSMThresholds.indexOf("25");
    else if(percentage < 13 && percentage >= 10)
        index = PSMThresholds.indexOf("15");
    else if(percentage < 10 && percentage >= 5)
        index = PSMThresholds.indexOf("10");
    else if(percentage < 5)
        index = PSMThresholds.indexOf("5");

    return index;
}

void BatteryBusinessLogic::batteryStatusChanged(Maemo::QmBattery::State state)
{
    //qDebug() << "BatteryBusinessLogic::batteryStatusChanged(" << state << ")";
    switch(state) {        
        case QmBattery::StateCharging:
            qDebug() << "Charging";
            emit batteryCharging(animationRate(battery->getChargerType()));
            utiliseLED(true, QString("PatternBatteryCharging"));            
            emit showNotification(ChargingText);
            if(lowBatteryNotifier != NULL) {                
                delete lowBatteryNotifier;
                lowBatteryNotifier = NULL;
            }
            break;
        case QmBattery::StateNotCharging:
            qDebug() << "Not charging";
            emit batteryNotCharging();
            utiliseLED(false, QString("PatternBatteryCharging"));
            break;
        case QmBattery::StateChargingFailed:
            qDebug() << "Charging not started";
            emit batteryNotCharging();
            utiliseLED(false, QString("PatternBatteryCharging"));
            emit showNotification(ChargingNotStartedText);
            break;
        default:
            break;
    }
    emit remainingTimeValuesChanged(remainingTimeValues());
}

void BatteryBusinessLogic::batteryLevelChanged(Maemo::QmBattery::Level level)
{
    qDebug() << "BatteryBusinessLogic::batteryLevelChanged(" << level << ")";
    switch(level) {
        case QmBattery::LevelFull:            
            emit showNotification(ChargingCompleteText);
            utiliseLED(true, QString("PatternBatteryFull"));
            break;
        case QmBattery::LevelLow:                           
            if(battery->getState() != QmBattery::StateCharging) {
                if(lowBatteryNotifier == NULL) {
                    lowBatteryNotifier = new LowBatteryNotifier();
                    connect(lowBatteryNotifier, SIGNAL(showNotification(QString)), this, SIGNAL(showNotification(QString)));
                }
                lowBatteryNotifier->showLowBatteryNotification();
            }
            break;
        default:
            break;
   }
}

void BatteryBusinessLogic::batteryEnergyLevelChanged(int percentage)
{
    qDebug() << "BatteryBusinessLogic::batteryEnergyLevelChanged(" << percentage << ")";
    emit batteryBarValueChanged(batteryBarValue(percentage));
    emit remainingTimeValuesChanged(remainingTimeValues());
    checkPSMThreshold();
}

void BatteryBusinessLogic::batteryChargerEvent(Maemo::QmBattery::ChargerType type)
{
    switch(type) {
        case QmBattery::None: // No  charger connected
            if(battery->getLevel() == QmBattery::LevelFull)
                 emit showNotification(DisconnectChargerText); //show reminder
            break;
        case QmBattery::Wall: // Wall charger
        case QmBattery::USB_500mA: // USB with 500mA output
        case QmBattery::USB_100mA: // USB with 100mA output
            emit batteryCharging(animationRate(type));
            break;
        default: //Maemo::QmBattery::Unknown
            break;    
    }
}

void BatteryBusinessLogic::devicePSMStateChanged(Maemo::QmDeviceMode::PSMState PSMState)
{    
    qDebug() << "BatteryBusinessLogic::devicePSMStateChanged(" << PSMState << ")";
    if(PSMState == QmDeviceMode::PSMStateOff) {
        emit showNotification(ExitPSMText);
        emit PSMValueChanged(false);
    }
    else if(PSMState == QmDeviceMode::PSMStateOn) {
        emit showNotification(EnterPSMText);
        emit PSMValueChanged(true);
    }
    emit remainingTimeValuesChanged(remainingTimeValues());
}

void BatteryBusinessLogic::checkPSMThreshold()
{
    qDebug() << "BatteryBusinessLogic::checkPSMThreshold()";
    if(systemUIGConf->value(SystemUIGConf::BatteryPSMAutoKey).toBool()) { // we only handle this if the auto PSM is on        

        if(batteryBarValue(battery->getBatteryEnergyLevel()) <= systemUIGConf->value(SystemUIGConf::BatteryPSMThresholdKey).toInt()) {
            if(deviceMode->getPSMState() == QmDeviceMode::PSMStateOff)
                deviceMode->setPSMState(QmDeviceMode::PSMStateOn);
        }
        else {
            if(deviceMode->getPSMState() == QmDeviceMode::PSMStateOn)
                deviceMode->setPSMState(QmDeviceMode::PSMStateOff);
        }
    }
}

bool BatteryBusinessLogic::PSMValue()
{
    qDebug() << "BatteryBusinessLogic::PSMValue()";    
    return ( deviceMode->getPSMState() == QmDeviceMode::PSMStateOn ? true : false );
}

void BatteryBusinessLogic::togglePSM(bool toggle)
{
    qDebug() << "BatteryBusinessLogic::togglePSM(" << toggle << ")";

    if(toggle)
        deviceMode->setPSMState(QmDeviceMode::PSMStateOn); //turn on the PSM
    else
        deviceMode->setPSMState(QmDeviceMode::PSMStateOff); //turn off the PSM

    //when ever we toggle PSM manually, we turn off the automatic PSM
    systemUIGConf->setValue(SystemUIGConf::BatteryPSMAutoKey, QVariant(false));
    emit PSMAutoDisabled();
}

void BatteryBusinessLogic::togglePSMAuto(bool toggle)
{
    qDebug() << "BatteryBusinessLogic::togglePSMAuto(" << toggle << ")";
    systemUIGConf->setValue(SystemUIGConf::BatteryPSMAutoKey, QVariant(toggle));
    if(toggle) // if we turn on the Auto PSM, we must check the threshold
        checkPSMThreshold();
    else { // if we turn off the Auto PSM, we must disable the PSM in all cases
        if(deviceMode->getPSMState() == QmDeviceMode::PSMStateOn)
            deviceMode->setPSMState(QmDeviceMode::PSMStateOff);     
    }

}

QStringList BatteryBusinessLogic::remainingTimeValues()
{          
    qDebug() << "BatteryBusinessLogic::remainingTimeValues()";

    QStringList values;
    if(battery->getState() == QmBattery::StateCharging)
        values << ChargingText << ChargingText;
    else {
        Maemo::QmDeviceMode::PSMState state = deviceMode->getPSMState();
        Maemo::QmBattery::RemainingTimeMode mode;
        switch(state) {
            case Maemo::QmDeviceMode::PSMStateOn:
                mode = Maemo::QmBattery::PowersaveMode;
                break;
            case Maemo::QmDeviceMode::PSMStateOff:
                mode = Maemo::QmBattery::NormalMode;
                break;
            default:
                return QStringList() << "N/A" << "N/A";
        }
        values << QString("%1").arg(battery->getRemainingTalkTime(mode) / 60)
                << QString("%1").arg(battery->getRemainingIdleTime(mode) / 60);
    }
    return values;
}

void BatteryBusinessLogic::utiliseLED(bool activate, const QString &pattern)
{    
    if(activate)
        led->activate(pattern);
    else
        led->deactivate(pattern);
}

void BatteryBusinessLogic::setPSMThreshold(const QString &threshold)
{
    qDebug() << "BatteryBusinessLogic::setPSMthreshold(" << threshold << ")";
    systemUIGConf->setValue(SystemUIGConf::BatteryPSMThresholdKey, QVariant(PSMThresholds.indexOf(threshold)));
    checkPSMThreshold();
}

QVariant BatteryBusinessLogic::GConfItemValue(SystemUIGConf::GConfKey key)
{
    return systemUIGConf->value(key);
}

QStringList BatteryBusinessLogic::PSMThresholdValues()
{
    //TODO: replace hardcoded values with real ones when they are available
    QStringList values;
    for(int i=0; i<5; ++i)
        values << PSMThresholds.at(i);
    return values;
}

QString BatteryBusinessLogic::PSMThresholdValue()
{
    return PSMThresholds.at(systemUIGConf->value(SystemUIGConf::BatteryPSMThresholdKey).toInt());
}

void BatteryBusinessLogic::batteryStatus()
{
    switch(battery->getState()) {
        case QmBattery::StateCharging:
            emit batteryCharging(animationRate(battery->getChargerType()));
            break;
        case QmBattery::StateNotCharging:
        case QmBattery::StateChargingFailed:
            emit batteryNotCharging();
            break;
        default:
            break;
    }
}

int BatteryBusinessLogic::animationRate(Maemo::QmBattery::ChargerType type)
{
    int rate = -1;
    switch(type) {
        case QmBattery::USB_500mA:
            rate = ChargingAnimationRateUSB;
            break;
        case QmBattery::Wall:
            rate = ChargingAnimationRateWall;
            break;
        default: // QmBattery::USB_100mA etc.
            break;
    }
    return rate;
}
