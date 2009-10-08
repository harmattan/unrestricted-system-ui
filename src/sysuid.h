#ifndef SYSUID_H
#define SYSUID_H

#include <QObject>
#include <QPointer>

#include "systemuigconf.h"
#include "pincodequerybusinesslogic.h"
//#include "batterybusinesslogic.h"
//#include "batterybusinesslogicadaptor.h"
#include "displaybusinesslogic.h"
#include "displaybusinesslogicadaptor.h"
#include "phonenetworkbusinesslogic.h"
#include "phonenetworkbusinesslogicadaptor.h"
#include "lockscreenbusinesslogic.h"
#include "lockscreenbusinesslogicadaptor.h"
#include "shutdownbusinesslogic.h"
#include "eventhandler.h"
#include "notifier.h"

class Sysuid : public QObject
{
    Q_OBJECT

public:
    Sysuid();
    virtual ~Sysuid();
    static QPointer<QObject> dbusObject();
    static QString dbusService();
    static QString dbusPath();
    static QPointer<Notifier> notifier();

private:
    SystemUIGConf *systemUIGConf;
    PinCodeQueryBusinessLogic *pinCodeQueryLogic;
    //BatteryBusinessLogic *batteryLogic;
    //BatteryBusinessLogicAdaptor *batteryLogicAdaptor;
    DisplayBusinessLogic *displayLogic;
    DisplayBusinessLogicAdaptor *displayLogicAdaptor;
    PhoneNetworkBusinessLogic *networkLogic;
    PhoneNetworkBusinessLogicAdaptor *networkLogicAdaptor;
    LockScreenBusinessLogic *lockScreenLogic;
    LockScreenBusinessLogicAdaptor *lockScreenLogicAdaptor;
    ShutdownBusinessLogic *shutdownLogic;
    EventHandler *eventHandler;
};

#endif // SYSUID_H
