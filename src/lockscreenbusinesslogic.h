#ifndef LOCKSCREENBUSINESSLOGIC_H
#define LOCKSCREENBUSINESSLOGIC_H

#include "qmsystem/qmlocks.h"
#include "qmsystem/qmdisplaystate.h"
#include <QObject>
#include <QTimer>

using namespace Maemo;

class LockScreenBusinessLogic : public QObject
{
    Q_OBJECT
public:
    LockScreenBusinessLogic();
    virtual ~LockScreenBusinessLogic();

    bool screenLockOn();
    void toggleScreenLock(bool toggle);
    bool sleepModeOn();
    void toggleSleepMode(bool toggle);
    void stopMonitroringIdleTime();
    void startMonitroringIdleTime();

signals:
    void lockScreenOff();

private slots:
    void timeout();

private: //attributes
    bool screenLock;
    bool sleepMode;
    QmLocks *touchPadLocker;
    QmDisplayState *display;
    QTimer *timer;

};

#endif // LOCKSCREENBUSINESSLOGIC_H
