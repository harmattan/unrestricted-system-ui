#ifndef SYSUID_H
#define SYSUID_H

#include <QObject>

#include "systemuigconf.h"
#include "pincodequerybusinesslogic.h"
#include "batterybusinesslogic.h"
#include "batterybusinesslogicadaptor.h"
#include "displaybusinesslogic.h"
#include "displaybusinesslogicadaptor.h"
#include "lockscreenbusinesslogic.h"
#include "shutdowndialogbusinesslogic.h"
#include "eventhandler.h"

class Sysuid : public QObject
{
    Q_OBJECT
public:
    Sysuid();
    virtual ~Sysuid();

private:
    SystemUIGConf *systemUIGConf;
    PinCodeQueryBusinessLogic *pinCodeQueryLogic;
    BatteryBusinessLogic *batteryLogic;
    BatteryBusinessLogicAdaptor *batteryLogicAdaptor;
    DisplayBusinessLogic *displayLogic;
    DisplayBusinessLogicAdaptor *displayLogicAdaptor;
    LockScreenBusinessLogic *lockScreenLogic;
    ShutdownDialogBusinessLogic *shutdownLogic;
    EventHandler *eventHandler;

};

#endif // SYSUID_H
