#ifndef BATTERYBUSINESSLOGIC_H
#define BATTERYBUSINESSLOGIC_H

#include "notifier.h"
#include "../batteryapplet/batterygconf.h"
#include <qmsystem/qmbattery.h>
#include <qmsystem/qmdevicemode.h>

#include <QObject>

class QTimer;


using namespace Maemo;

class BatteryBusinessLogic : public QObject
{
    Q_OBJECT
public:
    BatteryBusinessLogic();
    virtual ~BatteryBusinessLogic();

    bool initGConfSucceeded();

signals:
    void charging();

private: //attributes    
    QmBattery *battery;
    QmDeviceMode *deviceMode;    

    Notifier *uiNotif;
    BatteryGConf *batteryGConf;

    QHash<QmBattery::Level, int> batteryLevels;
    bool updateRemainingTimesBusy;
    bool forceUpdateRemainingTimes;
    bool initGConfSuccess;

private: //methods
    bool initBatteryGConfKeys();
    void checkChargingState();
    void checkBatteryLevel();
    void togglePSM(bool toggle);    
    void checkPSMThreshold(Maemo::QmBattery::Level level);    

public slots:
    void checkBattery();

private slots:
    void batteryLevelChanged(Maemo::QmBattery::Level level);
    void batteryStateChanged(Maemo::QmBattery::State state);    
    void activatePSM();
    void gConfValueChanged(BatteryGConf::GConfKey key, QVariant value);
    void updateRemainingTimes();
    void utiliseLED(bool activate, const QString &pattern);

};


#endif // BATTERYBUSINESSLOGIC_H

