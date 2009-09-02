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

signals:
    void charging();

private: //attributes
    Notifier *uiNotif;
    QmBattery *battery;
    QmDeviceMode *deviceMode;
    BatteryGConf *batteryGConf;
    bool powerSaveMode;
    QTimer *timer;
    QHash<QmBattery::Level, int> batteryLevels;

private: //methods
    void initBatteryGConfKeys();
    void checkChargingState();
    void checkBatteryLevel();
    void togglePSM(bool toggle);
    void updateTimes();
    void checkPSMThreshold(Maemo::QmBattery::Level level);
    void pollTimes(bool toggle);

public slots:
    void checkBattery();

private slots:
    void batteryLevelChanged(Maemo::QmBattery::Level level);
    void batteryStateChanged(Maemo::QmBattery::State state);    
    void activatePSM();
    void gConfValueChanged(BatteryGConf::GConfKey key, QVariant value);

};


#endif // BATTERYBUSINESSLOGIC_H

