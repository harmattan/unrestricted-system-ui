#ifndef BATTERYBUSINESSLOGIC_H
#define BATTERYBUSINESSLOGIC_H

#include <QObject>
#include <QmBattery>

#include "notifier.h"
//get rid of the path -- this doesn't work yet
//#include "/targets/harmattan_i386/usr/include/qmsystem/qmbattery.h"

class BatteryBusinessLogic : public QObject
{
    Q_OBJECT
public:
    BatteryBusinessLogic();
    virtual ~BatteryBusinessLogic();

private: //attributes
    Notifier *uiNotif;
    QmBattery *battery;

private: //methos
    void checkRemainingTime();
    void checkChargingState();
    void setPowerSaveMode(bool toggle);    

private slots:
    void batteryLevelChanged(QmBattery::Level level);
    void chargerStateChanged(QmBattery::State state);

};


#endif // BATTERYBUSINESSLOGIC_H
