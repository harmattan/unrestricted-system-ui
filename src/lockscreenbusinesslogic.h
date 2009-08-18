#ifndef LOCKSCREENBUSINESSLOGIC_H
#define LOCKSCREENBUSINESSLOGIC_H

#include "qmsystem/qmlocks.h"
#include "qmsystem/qmdisplaystate.h"
#include "eventeater.h"

#include <QObject>


using namespace Maemo;

class LockScreenBusinessLogic : public QObject
{
    Q_OBJECT

public:
    LockScreenBusinessLogic();
    virtual ~LockScreenBusinessLogic();

signals:
    void lockScreenOff();

public slots:
    void shortPowerKeyPressOccured();

private slots:
    void displayStateChanged(Maemo::QmDisplayState::DisplayState state);

private: //methods
    void toggleScreenLock(bool toggle);
    void toggleSleepMode(bool toggle);
    void toggleDisplayStateListener(bool toggle);

private: //attributes
    EventEater *eventEater;
    bool screenLock;
    bool sleepMode;
    QmLocks *touchPadLocker;
    QmDisplayState *display;    
};

#endif // LOCKSCREENBUSINESSLOGIC_H
