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

    enum ScreenLockPhase {
        Off = 0, // screenlock is off
        On, //screenlock is on
        Sleep // screenlock sleep mode is on
    };

    LockScreenBusinessLogic();
    virtual ~LockScreenBusinessLogic();

signals:
    void lockScreenOff();
    void toggleDisplay(bool toggle);

public slots:
    void shortPowerKeyPressOccured();
    void displayOn();
    void displayOff();

private slots:
    void unlockScreen();
    void updateMissedEventAmounts(int calls, int messages, int emails, int chatMessages);

private: //methods
    void toggleKeyPadLock(bool toggle);
    void toggleScreenLockUI(bool toggle);    

private: //attributes
    EventEater *eventEater;    
    ScreenLockPhase phase;
    LockScreenUI *lockUI;
    QDBusInterface *dbusIf;
};

#endif // LOCKSCREENBUSINESSLOGIC_H
