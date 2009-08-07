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

private: //attributes
    Notifier *uiNotif;
    QmBattery *battery;
    QmDeviceMode *deviceMode;
    bool powerSaveMode;

private: //methos
    void checkRemainingTime();
    void checkChargingState();

private slots:
    void batteryLevelChanged(QmBattery::Level level);
    void batteryStateChanged(QmBattery::State state);

};


#endif // BATTERYBUSINESSLOGIC_H

