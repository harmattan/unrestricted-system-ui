#ifndef LOCKSCREENBUSINESSLOGIC_H
#define LOCKSCREENBUSINESSLOGIC_H

#include "qmsystem/qmlocks.h"
//#include "qmsystem/qmdisplaystate.h" // stub used
//#include "displaystatestub.h"
#include "eventeater.h"

#include <QObject>

class LockScreenUI;
class QDBusInterface;

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
    void sleepModeOff();
    void unlockScreen();
    void disable(bool disable);

private slots:    
    //void displayStateChanged(Maemo::QmDisplayState::DisplayState state);
    void updateMissedEventAmounts(int calls, int messages, int emails, int chatMessages);

private: //methods
    void toggleScreenLock(bool toggle);
    void toggleSleepMode(bool toggle);
    //void toggleDisplayStateListener(bool toggle);

private: //attributes
    EventEater *eventEater;
    bool screenLock;
    bool sleepMode;
    bool isDisabled;
    QmLocks *touchPadLocker;
    //QmDisplayState *display; //this should be handled by the displaybusinesslogic
    LockScreenUI *lockUI;
    QDBusInterface *dbusIf;
};

#endif // LOCKSCREENBUSINESSLOGIC_H
