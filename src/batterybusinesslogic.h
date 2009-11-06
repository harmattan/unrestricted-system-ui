#ifndef BATTERYBUSINESSLOGIC_H
#define BATTERYBUSINESSLOGIC_H

#include <QDebug>

#include "systemuigconf.h"
//#include <qmsystem/qmbattery.h>
#include "batterystub.h"
//#include <qmsystem/qmdevicemode.h> //replaced with stub class
#include "devicemodestub.h"
//#include <qmsystem/qmdisplaystate.h> //replaced with stub class
#include "displaystatestub.h"

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
    void togglePSM(const QString &value);
    void togglePSMAuto(bool toggle);
    int batteryBarValue(int percentage = -1);
    QString PSMValue();
    void batteryStatus();
    QVariant GConfItemValue(SystemUIGConf::GConfKey key);
    QStringList remainingTimeValues();
    QStringList PSMThresholdValues();
    QString PSMThresholdValue();

signals:
    void batteryCharging(int);
    void batteryNotCharging();
    void batteryBarValueChanged(int);
    void PSMValueChanged(QString);
    void PSMAutoDisabled();
    void remainingTimeValuesChanged(QStringList);
    void showNotification(QString);

public slots:
    void initBattery();    

private: //attributes
    SystemUIGConf *systemUIGConf;    
    QmBattery *battery;
    QmDeviceMode *deviceMode;
    LowBatteryNotifier *lowBatteryNotifier;
    QStringList PSMThresholds;

private: //methods
    void initSystemUIGConfKeys();
    void checkPSMThreshold();
    int animationRate();

private slots:
    void batteryLevelChanged(Maemo::QmBattery::Level level);
    void batteryEnergyLevelChanged(int energyLevel);
    void batteryStatusChanged(Maemo::QmBattery::State state);
    void batteryChargerEvent(Maemo::QmBattery::ChargerEvent event);
    void devicePSMStateChanged(Maemo::QmDeviceMode::PSMState PSMState);    
    void utiliseLED(bool activate, const QString &pattern);

};


#endif // BATTERYBUSINESSLOGIC_H

