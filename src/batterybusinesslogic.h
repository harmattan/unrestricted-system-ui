#ifndef BATTERYBUSINESSLOGIC_H
#define BATTERYBUSINESSLOGIC_H

#include <QDebug>

#ifdef UNIT_TEST
#include "ledstub.h"
#include "batterystub.h"
#include "devicemodestub.h"
#include "displaystatestub.h"
#include "gconfstub.h"
#else
#include <qmsystem/qmled.h>
#include <qmsystem/qmbattery.h>
#include <qmsystem/qmdevicemode.h>
#include <qmsystem/qmdisplaystate.h>
#include "systemuigconf.h"
#endif

#include <QObject>
#include <QStringList>
#include <QTime>

class QTimer;

using namespace Maemo;

class LowBatteryNotifier : public QObject
{
    Q_OBJECT

public:
    LowBatteryNotifier(QObject* parent = 0);
    virtual ~LowBatteryNotifier();

public slots:
    void showLowBatteryNotification();

private slots:
    void displayStateChanged(Maemo::QmDisplayState::DisplayState state);

signals:
    void showNotification(QString);

private:
    QmDisplayState *display;
    QTimer *timer;
    QTime time;
    bool sleep;
    int activeInterval;
    int inactiveInterval;

#ifdef UNIT_TEST
    friend class Ut_LowBatteryNotifier;
#endif // UNIT_TEST


};

class BatteryBusinessLogic : public QObject
{
    Q_OBJECT
public:
    BatteryBusinessLogic(SystemUIGConf *systemUIGConf, QObject* parent = 0);
    virtual ~BatteryBusinessLogic();

    void setPSMThreshold(const QString &threshold);
    void togglePSM(bool toggle);
    void togglePSMAuto(bool toggle);
    int batteryBarValue(int percentage = -1);
    bool PSMValue();
    void batteryStatus();
    QVariant GConfItemValue(SystemUIGConf::GConfKey key);
    QStringList remainingTimeValues();
    QStringList PSMThresholdValues();
    QString PSMThresholdValue();

signals:
    void batteryCharging(int);
    void batteryNotCharging();
    void batteryBarValueChanged(int);
    void PSMValueChanged(bool);
    void PSMAutoDisabled();
    void remainingTimeValuesChanged(QStringList);
    void showNotification(QString);

public slots:
    void initBattery();

private: //attributes
    SystemUIGConf *systemUIGConf;
    QmBattery *battery;
    QmDeviceMode *deviceMode;
    QmLED *led;
    LowBatteryNotifier *lowBatteryNotifier;
    QStringList PSMThresholds;

private: //methods
    void initSystemUIGConfKeys();
    void checkPSMThreshold();
    int animationRate(Maemo::QmBattery::ChargerType type);

private slots:
    void batteryLevelChanged(Maemo::QmBattery::Level level);
    void batteryEnergyLevelChanged(int energyLevel);
    void batteryStatusChanged(Maemo::QmBattery::State state);
    void batteryChargerEvent(Maemo::QmBattery::ChargerType type);
    void devicePSMStateChanged(Maemo::QmDeviceMode::PSMState PSMState);
    void utiliseLED(bool activate, const QString &pattern);

#ifdef UNIT_TEST
    friend class Ut_BatteryBusinessLogic;
#endif // UNIT_TEST

};


#endif // BATTERYBUSINESSLOGIC_H

