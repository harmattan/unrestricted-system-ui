/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "batterybusinesslogic.h"

#include <DuiLocale>
#include <QTimer>
#include <QTime>
#include <DuiNotification>


/*
 * Needed to add some delay otherwise the QmSystem seems to return wrong value. 
 * But then it stopped to work with the next image...
 */
#define DELAY_FOR_INITIALIZATION 0

#define DEBUG
#include "../debug.h"

/* TODO List

   1) Create notification for RechargeBatteryText. Connect it to the signal 
      from QmSystemState which inforsm the reason for shut down.
   2) What are correct animation rates when charging with USB / Wall?
   3) If USB 100 mA is used, do we show animation at all? In Fremantle not.
   4) Connect sounds with notifications

*/

namespace
{
//% "Charging"
const char *ChargingText = QT_TRID_NOOP ("qtn_ener_char");
//% "Charging not started. Replace charger."
const char *ChargingNotStartedText = QT_TRID_NOOP ("qtn_ener_repcharger");
//% "Charging complete"
const char *ChargingCompleteText = QT_TRID_NOOP ("qtn_ener_charcomp");
//% "Disconnect charger from power supply to save energy"
const char *DisconnectChargerText = QT_TRID_NOOP ("qtn_ener_remcha");
//% "Low battery"
const char *LowBatteryText = QT_TRID_NOOP ("qtn_ener_lowbatt");
//% "Entering power save mode"
const char *EnterPSMText = QT_TRID_NOOP ("qtn_ener_ent_psnote");
//% "Exiting power save mode"
const char *ExitPSMText = QT_TRID_NOOP ("qtn_ener_exit_psnote");
/* TODO: ^^
//% "Recharge battery"
const char *RechargeBatteryText = QT_TRID_NOOP ("qtn_ener_rebatt");
 */
const int   LowBatteryActiveInterval = 30 * 60 * 1000; //30 mins
const int   LowBatteryInactiveInterval = 2 * 60 * 60 * 1000; //2 hours
const int   ChargingAnimationRateUSB = 800; // 800 ms
const int   ChargingAnimationRateWall = 400; // 400 ms
}

/******************************************************************************
 * Methods for the LowBatteryNotifier class.
 */
LowBatteryNotifier::LowBatteryNotifier (
        QObject *parent) :
        QObject (parent),
        m_Display (new QmDisplayState()),
        m_Timer (new QTimer(this))
{
    SYS_DEBUG ("----------------------------------------------");

    m_Sleep = m_Display->get() == Maemo::QmDisplayState::Off;
    m_ActiveInterval = LowBatteryActiveInterval;
    m_InactiveInterval = LowBatteryInactiveInterval;
    m_Time.start ();
    connect(m_Display, 
            SIGNAL(displayStateChanged(Maemo::QmDisplayState::DisplayState)),
            this, 
            SLOT(displayStateChanged(Maemo::QmDisplayState::DisplayState)));

    connect (m_Timer, SIGNAL(timeout()), 
            this, SLOT(showLowBatteryNotification()));
}

LowBatteryNotifier::~LowBatteryNotifier()
{
}


void 
LowBatteryNotifier::showLowBatteryNotification()
{
    SYS_DEBUG ("");

    DuiNotification("", "", qtTrId (LowBatteryText));
    m_Time.start(); //restart time

    switch (m_Display->get()) {
        case Maemo::QmDisplayState::On:
        case Maemo::QmDisplayState::Dimmed:
            m_Sleep = false;
            m_Timer->start (m_ActiveInterval);
            break;
        case Maemo::QmDisplayState::Off:
            m_Sleep = true;
            m_Timer->start (m_InactiveInterval);
            break;
    }
}

void 
LowBatteryNotifier::displayStateChanged (
        Maemo::QmDisplayState::DisplayState state)
{
    SYS_DEBUG ("");

    switch (state) {
    case Maemo::QmDisplayState::On:
        if (!m_Sleep)
            break;
        if (m_Time.elapsed() < m_ActiveInterval)
            m_Timer->setInterval (m_ActiveInterval - m_Time.elapsed());
        else
            showLowBatteryNotification();
        m_Sleep = false;
        break;

    case Maemo::QmDisplayState::Dimmed:
        m_Sleep = false;
        break;
    case Maemo::QmDisplayState::Off:
        m_Timer->setInterval (m_InactiveInterval - m_Time.elapsed());
        m_Sleep = true;
        break;
    }
}

/******************************************************************************
 * Methods for the BatteryBusinessLogic class.
 */
BatteryBusinessLogic::BatteryBusinessLogic (
        SystemUIGConf *systemUIGConf, 
        QObject       *parent) :
    QObject (parent),
    m_SystemUIGConf (systemUIGConf),
    m_Battery (new QmBattery),
    m_DeviceMode (new QmDeviceMode),
    m_Led (new QmLED),
    m_LowBatteryNotifier (NULL)
{
    /* init the PSM thresholds */
    m_PSMThresholds << 
	    QString("5") << 
	    QString("10") << 
	    QString("15") << 
	    QString("25") << 
	    QString("35") << 
	    QString("50") << 
	    QString("60") << 
	    QString("75") << 
	    QString("85") << 
	    QString("100");

    /* check if gconfvalues need to initialized */
    initSystemUIGConfKeys ();

    /* connect to QmSystem signals */
    connect (m_Battery, SIGNAL(batteryLevelChanged(Maemo::QmBattery::Level)),
            this, SLOT(batteryLevelChanged(Maemo::QmBattery::Level)));
    connect (m_Battery, SIGNAL(batteryStatusChanged(Maemo::QmBattery::State)),
            this, SLOT(batteryStatusChanged(Maemo::QmBattery::State)));
    connect (m_Battery, SIGNAL(batteryEnergyLevelChanged(int)),
            this, SLOT(batteryEnergyLevelChanged(int)));
    connect (m_Battery, SIGNAL(chargerEvent(Maemo::QmBattery::ChargerType)),
            this, SLOT(batteryChargerEvent(Maemo::QmBattery::ChargerType)));
    connect (m_DeviceMode, SIGNAL(devicePSMStateChanged(Maemo::QmDeviceMode::PSMState)),
            this, SLOT(devicePSMStateChanged(Maemo::QmDeviceMode::PSMState)));

    #if DELAY_FOR_INITIALIZATION
    /*
     * See the documentation of DELAY_FOR_INITIALIZATION for further details.
     */
    QTimer::singleShot (DELAY_FOR_INITIALIZATION, this, SLOT(initBattery()));
    #else
    initBattery ();
    #endif
}


BatteryBusinessLogic::~BatteryBusinessLogic()
{
    delete m_Battery;
    m_Battery = NULL;

    delete m_DeviceMode;
    m_DeviceMode = NULL;

    delete m_Led;
    m_Led = NULL;
}

void 
BatteryBusinessLogic::initSystemUIGConfKeys ()
{
    if (m_SystemUIGConf->keyCount(SystemUIGConf::Battery) < 2) {
        /* 
         * GConf keys have not yet been set. 
         */
        m_SystemUIGConf->setValue (
                SystemUIGConf::BatteryPSMAutoKey, 
                QVariant(true));
        m_SystemUIGConf->setValue(
                SystemUIGConf::BatteryPSMThresholdKey, 
                QVariant(1));
    }
}


// This method should be called also when the device is returned from sleep 
// mode
void
BatteryBusinessLogic::initBattery ()
{
    SYS_DEBUG ("");
    //init the charging status
    batteryStatusChanged (m_Battery->getState());

    //init the battery level
    batteryLevelChanged (m_Battery->getLevel());
}

int 
BatteryBusinessLogic::batteryBarValue (
        int percentage)
{
    if (percentage == -1)
        percentage = m_Battery->getBatteryEnergyLevel();

    int index = 0;
    if (percentage >= 88)
        index = m_PSMThresholds.indexOf("100");
    else if (percentage < 88 && percentage >= 75)
        index = m_PSMThresholds.indexOf("85");
    else if (percentage < 75 && percentage >= 62)
        index = m_PSMThresholds.indexOf("75");
    else if (percentage < 62 && percentage >= 50)
        index = m_PSMThresholds.indexOf("60");
    else if (percentage < 50 && percentage >= 38)
        index = m_PSMThresholds.indexOf("50");
    else if (percentage < 38 && percentage >= 25)
        index = m_PSMThresholds.indexOf("35");
    else if (percentage < 25 && percentage >= 13)
        index = m_PSMThresholds.indexOf("25");
    else if (percentage < 13 && percentage >= 10)
        index = m_PSMThresholds.indexOf("15");
    else if (percentage < 10 && percentage >= 5)
        index = m_PSMThresholds.indexOf("10");
    else if (percentage < 5)
        index = m_PSMThresholds.indexOf("5");

    return index;
}

void 
BatteryBusinessLogic::batteryStatusChanged (
        Maemo::QmBattery::State state)
{
    SYS_DEBUG ("");

    switch (state) {
        case QmBattery::StateCharging:
            SYS_DEBUG ("Charging");
            emit batteryCharging (animationRate (m_Battery->getChargerType()));
            utiliseLED (true, QString("PatternBatteryCharging"));
            DuiNotification ("", "", qtTrId (ChargingText));
            if (m_LowBatteryNotifier != NULL) {
                delete m_LowBatteryNotifier;
                m_LowBatteryNotifier = NULL;
            }
            break;

        case QmBattery::StateNotCharging:
            SYS_DEBUG ("Not charging");
            emit batteryNotCharging ();
            utiliseLED (false, QString("PatternBatteryCharging"));
            break;

        case QmBattery::StateChargingFailed:
            SYS_DEBUG ("Charging not started");
            emit batteryNotCharging ();
            utiliseLED (false, QString("PatternBatteryCharging"));
            DuiNotification ("", "", qtTrId (ChargingNotStartedText));
            break;

        default:
            SYS_WARNING ("Unhandled state: %d", state);
    }

    emit remainingTimeValuesChanged (remainingTimeValues());
}

void 
BatteryBusinessLogic::batteryLevelChanged (
        Maemo::QmBattery::Level level)
{
    SYS_DEBUG ("");

    switch (level) {
    case QmBattery::LevelFull:
        DuiNotification("", "", qtTrId (ChargingCompleteText));
        utiliseLED(true, QString("PatternBatteryFull"));
        break;

    case QmBattery::LevelLow:
        if (m_Battery->getState() != QmBattery::StateCharging) {
            if (m_LowBatteryNotifier == NULL) {
                m_LowBatteryNotifier = new LowBatteryNotifier();
                connect(m_LowBatteryNotifier, SIGNAL(showNotification(QString)), 
                        this, SIGNAL(showNotification(QString)));
            }
            m_LowBatteryNotifier->showLowBatteryNotification();
        }
        break;

    default:
        break;
    }
}

void 
BatteryBusinessLogic::batteryEnergyLevelChanged (
        int percentage)
{
    SYS_DEBUG ("*** percentage = %d", percentage);

    emit batteryBarValueChanged (batteryBarValue(percentage));
    emit remainingTimeValuesChanged (remainingTimeValues());
    checkPSMThreshold ();
}

void 
BatteryBusinessLogic::batteryChargerEvent (
        Maemo::QmBattery::ChargerType type)
{
    SYS_DEBUG ("");

    switch (type) {
    case QmBattery::None: // No  charger connected
        if (m_Battery->getLevel() == QmBattery::LevelFull)
            DuiNotification("", "", qtTrId (DisconnectChargerText)); //show reminder
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

void 
BatteryBusinessLogic::devicePSMStateChanged (
        Maemo::QmDeviceMode::PSMState PSMState)
{
    qDebug() << "BatteryBusinessLogic::devicePSMStateChanged(" << 
        PSMState << ")";

    if (PSMState == QmDeviceMode::PSMStateOff) {
        DuiNotification("", "", qtTrId (ExitPSMText));
        SYS_DEBUG ("Emitting DBus signal on PSM off");
        emit PSMValueChanged (false);
    } else if (PSMState == QmDeviceMode::PSMStateOn) {
        DuiNotification("", "", qtTrId (EnterPSMText));
        SYS_DEBUG ("Emitting DBus signal on PSM on");
        emit PSMValueChanged (true);
    }

    emit remainingTimeValuesChanged (remainingTimeValues());
}

void 
BatteryBusinessLogic::checkPSMThreshold ()
{
    qDebug() << "BatteryBusinessLogic::checkPSMThreshold()";

    if (m_SystemUIGConf->value(SystemUIGConf::BatteryPSMAutoKey).toBool()) { 
        // we only handle this if the auto PSM is on
        if (batteryBarValue(m_Battery->getBatteryEnergyLevel()) <= 
                m_SystemUIGConf->value(SystemUIGConf::BatteryPSMThresholdKey).toInt()) {
            if (m_DeviceMode->getPSMState() == QmDeviceMode::PSMStateOff)
                setPSMState (true);
        } else {
            if (m_DeviceMode->getPSMState() == QmDeviceMode::PSMStateOn)
                setPSMState (false);
        }
    }
}

bool 
BatteryBusinessLogic::PSMValue()
{
    qDebug() << "BatteryBusinessLogic::PSMValue()";
    return m_DeviceMode->getPSMState() == QmDeviceMode::PSMStateOn;
}

/*!
 * \param toggle true to go to power save mode, false to leave it
 * 
 * Enters or leaves the power saving mode. This function will also disable the
 * auto power save mode.
 */
void 
BatteryBusinessLogic::togglePSM (
        bool toggle)
{
    qDebug() << "BatteryBusinessLogic::togglePSM(" << toggle << ")";

    setPSMState (toggle);

    //when ever we toggle PSM manually, we turn off the automatic PSM
    m_SystemUIGConf->setValue (
            SystemUIGConf::BatteryPSMAutoKey, 
            QVariant (false));
    emit PSMAutoDisabled ();
}

void 
BatteryBusinessLogic::togglePSMAuto (
        bool toggle)
{
    qDebug() << "BatteryBusinessLogic::togglePSMAuto(" << toggle << ")";
    m_SystemUIGConf->setValue(
            SystemUIGConf::BatteryPSMAutoKey, 
            QVariant(toggle));
    if (toggle) // if we turn on the Auto PSM, we must check the threshold
        checkPSMThreshold();
    else { // if we turn off the Auto PSM, we must disable the PSM in all cases
        if (m_DeviceMode->getPSMState() == QmDeviceMode::PSMStateOn)
            setPSMState (false);
    }

}

QStringList 
BatteryBusinessLogic::remainingTimeValues ()
{
    SYS_DEBUG ("");

    QStringList values;
    if (m_Battery->getState() == QmBattery::StateCharging)
        values << qtTrId (ChargingText) << qtTrId (ChargingText);
    else {
        Maemo::QmDeviceMode::PSMState state = m_DeviceMode->getPSMState();
        Maemo::QmBattery::RemainingTimeMode mode;
        switch (state) {
            case Maemo::QmDeviceMode::PSMStateOn:
                mode = Maemo::QmBattery::PowersaveMode;
                break;
            case Maemo::QmDeviceMode::PSMStateOff:
            default:
                mode = Maemo::QmBattery::NormalMode;
                break;
                // This was the default...
                //return QStringList() << "N/A" << "N/A";
        }
        values << QString("%1").arg(m_Battery->getRemainingTalkTime(mode) / 60)
            << QString("%1").arg(m_Battery->getRemainingIdleTime(mode) / 60);
    }
    return values;
}

void 
BatteryBusinessLogic::utiliseLED (
        bool activate, 
        const QString &pattern)
{
    if (activate)
        m_Led->activate (pattern);
    else
        m_Led->deactivate (pattern);
}

void 
BatteryBusinessLogic::setPSMThreshold(
        const QString &threshold)
{
    qDebug() << "BatteryBusinessLogic::setPSMthreshold(" << threshold << ")";
    m_SystemUIGConf->setValue(
            SystemUIGConf::BatteryPSMThresholdKey, 
            QVariant(m_PSMThresholds.indexOf(threshold)));
    checkPSMThreshold();
}

QVariant 
BatteryBusinessLogic::GConfItemValue(
        SystemUIGConf::GConfKey key)
{
    return m_SystemUIGConf->value (key);
}

QStringList 
BatteryBusinessLogic::PSMThresholdValues ()
{
    //TODO: replace hardcoded values with real ones when they are available
    QStringList values;
    for (int i = 0; i < 5; ++i)
        values << m_PSMThresholds.at(i);

    return values;
}

QString 
BatteryBusinessLogic::PSMThresholdValue()
{
    return m_PSMThresholds.at(m_SystemUIGConf->value(
                SystemUIGConf::BatteryPSMThresholdKey).toInt());
}

void 
BatteryBusinessLogic::batteryStatus ()
{
    SYS_DEBUG ("");
    switch (m_Battery->getState()) {
        case QmBattery::StateCharging:
            emit batteryCharging (animationRate(m_Battery->getChargerType()));
            break;

        case QmBattery::StateNotCharging:
        case QmBattery::StateChargingFailed:
            emit batteryNotCharging();
            break;
        default:
            break;
    }
}

int
BatteryBusinessLogic::animationRate (
        Maemo::QmBattery::ChargerType type)
{
    int rate = -1;
    SYS_DEBUG ("");
    switch (type) {
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

/*!
 * \param on true for entering PSM mode, false to leave it
 */
void
BatteryBusinessLogic::setPSMState (
        bool     on)
{
    if (on) {
        if (!m_DeviceMode->setPSMState(QmDeviceMode::PSMStateOn)) {
            SYS_WARNING ("Entering power save mode failed.");
        }
    } else {
        if (!m_DeviceMode->setPSMState(QmDeviceMode::PSMStateOff)) {
            SYS_WARNING ("Leaving power save mode failed.");
        }
    }
}
