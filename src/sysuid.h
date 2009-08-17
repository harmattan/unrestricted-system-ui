#ifndef SYSUID_H
#define SYSUID_H

#include <QObject>

#include "pincodequerybusinesslogic.h"
#include "batterybusinesslogic.h"
#include "lockscreenbusinesslogic.h"
#include "eventhandler.h"

class Sysuid : public QObject
{
    Q_OBJECT
public:
    Sysuid();
    virtual ~Sysuid();

private:
    PinCodeQueryBusinessLogic *pinCodeQueryLogic;
    BatteryBusinessLogic *batteryLogic;
    LockScreenBusinessLogic *lockScreenLogic;
    EventHandler *eventHandler;

};

#endif // SYSUID_H
