#ifndef SYSUID_H
#define SYSUID_H

#include <QObject>
#include <QPointer>

class SystemUIGConf;
class PinCodeQueryBusinessLogic;
class BatteryBusinessLogic;
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
    static Sysuid* sysuid();
    static QString dbusService();
    static QString dbusPath();

private:
    SystemUIGConf *systemUIGConf;
    PinCodeQueryBusinessLogic *pinCodeQueryLogic;
    BatteryBusinessLogic *batteryLogic;    
    LockScreenBusinessLogic *lockScreenLogic;
    ShutdownBusinessLogic *shutdownLogic;
    EventHandler *eventHandler;
    Notifier *notifier;   
    static Sysuid* _sysuid;
};

#endif // SYSUID_H
