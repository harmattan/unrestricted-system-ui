#ifndef SYSUID_H
#define SYSUID_H

#include <QObject>
#include <QPointer>

class SystemUIGConf;
class BatteryBusinessLogic;
class LockScreenBusinessLogic;
class ShutdownBusinessLogic;

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
    SystemUIGConf* systemUIGConf;
    BatteryBusinessLogic* batteryLogic;
    LockScreenBusinessLogic* lockScreenLogic;
    ShutdownBusinessLogic* shutdownLogic;
    static Sysuid* _sysuid;
};

#endif // SYSUID_H
