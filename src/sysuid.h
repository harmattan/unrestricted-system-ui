#ifndef SYSUID_H
#define SYSUID_H

#include <QObject>
#include <QPointer>

class SystemUIGConf;
class PinCodeQueryBusinessLogic;
class BatteryBusinessLogic;
class BatteryBusinessLogicAdaptor;
class DisplayBusinessLogic;
class DisplayBusinessLogicAdaptor;
class PhoneNetworkBusinessLogic;
class PhoneNetworkBusinessLogicAdaptor;
class LockScreenBusinessLogic;
class ShutdownBusinessLogic;
class EventHandler;
class ProfileBusinessLogic;
class ProfileBusinessLogicAdaptor;
class Notifier;

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
    BatteryBusinessLogic *batteryLogic;
    BatteryBusinessLogicAdaptor *batteryLogicAdaptor;
    DisplayBusinessLogic *displayLogic;
    DisplayBusinessLogicAdaptor *displayLogicAdaptor;
    PhoneNetworkBusinessLogic *networkLogic;
    PhoneNetworkBusinessLogicAdaptor *networkLogicAdaptor;
    ProfileBusinessLogic* profileLogic;
    ProfileBusinessLogicAdaptor* profileLogicAdaptor;
    LockScreenBusinessLogic *lockScreenLogic;
    ShutdownBusinessLogic *shutdownLogic;
    EventHandler *eventHandler;
};

#endif // SYSUID_H
