#ifndef BATTERYBUSINESSLOGIC_H
#define BATTERYBUSINESSLOGIC_H

#include <QObject>
#include <qmsystem/qmbattery.h>
#include <qmsystem/qmdevicemode.h>

#include "notifier.h"

using namespace Maemo;

class BatteryBusinessLogic : public QObject
{
    Q_OBJECT
public:
    BatteryBusinessLogic();
    virtual ~BatteryBusinessLogic();

    // This method should be called also when the device is returned from sleep mode
    void checkBattery();

private: //attributes
    Notifier *uiNotif;
    QmBattery *battery;
    QmDeviceMode *deviceMode;
    bool powerSaveMode;

private: //methos
    void checkRemainingTime(); //TEMP
    void checkChargingState();
    void checkBatteryLevel();

private slots:
    void batteryLevelChanged(QmBattery::Level level);
    void batteryStateChanged(QmBattery::State state);
    void remainingTalkTimeChanged(int secondsLeft);
    void activatePSM();

};


#endif // BATTERYBUSINESSLOGIC_H

