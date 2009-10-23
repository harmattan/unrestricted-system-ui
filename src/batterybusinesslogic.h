#ifndef BATTERYBUSINESSLOGIC_H
#define BATTERYBUSINESSLOGIC_H

#include "notifier.h"
#include "systemuigconf.h"
//#include <qmsystem/qmbattery.h>
#include "batterystub.h"
//#include <qmsystem/qmdevicemode.h> //replaced with stub class
#include "devicemodestub.h"

#include <QObject>
#include <QStringList>

using namespace Maemo;

class BatteryBusinessLogic : public QObject
{
    Q_OBJECT
public:
    BatteryBusinessLogic(SystemUIGConf *systemUIGConf);
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
    void PSMAutoValueChanged(bool);
    void remainingTimeValuesChanged(QStringList);

public slots:
    void initBattery();

private: //attributes
    SystemUIGConf *systemUIGConf;
    QmBattery *battery;
    QmDeviceMode *deviceMode;    
    Notifier *uiNotif;        
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

