#ifndef SYSUID_H
#define SYSUID_H

#include <QObject>
#include <QPointer>

class SystemUIGConf;
class PinCodeQueryBusinessLogic;
class BatteryBusinessLogic;
class DisplayBusinessLogic;
class PhoneNetworkBusinessLogic;
class LockScreenBusinessLogic;
class ShutdownBusinessLogic;
class EventHandler;
class Notifier;
class CallAndSimBusinessLogic;

class Sysuid : public QObject
{
    Q_OBJECT

public:
    Sysuid();
    virtual ~Sysuid();
    //static QPointer<QObject> dbusObject();
    static Sysuid* dbusObject();
    static QString dbusService();
    static QString dbusPath();
    static QPointer<Notifier> notifier();

private:
    SystemUIGConf *systemUIGConf;
    PinCodeQueryBusinessLogic *pinCodeQueryLogic;
    BatteryBusinessLogic *batteryLogic;
    DisplayBusinessLogic *displayLogic;
    PhoneNetworkBusinessLogic *networkLogic;
    LockScreenBusinessLogic *lockScreenLogic;
    ShutdownBusinessLogic *shutdownLogic;
    EventHandler *eventHandler;

    static Sysuid* sysuid;
};

#endif // SYSUID_H
