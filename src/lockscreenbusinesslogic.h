#ifndef LOCKSCREENBUSINESSLOGIC_H
#define LOCKSCREENBUSINESSLOGIC_H

#include "qmsystem/qmlocks.h"
//#include "qmsystem/qmdisplaystate.h" // stub used
#include "displaystatestub.h"
#include "eventeater.h"

#include <QObject>

class LockScreenUI;
class QDBusInterface;

using namespace Maemo;

class LockScreenBusinessLogic : public QObject
{
    Q_OBJECT

public:
    LockScreenBusinessLogic(QObject* parent = 0);
    virtual ~LockScreenBusinessLogic();    

public slots:
    void shortPowerKeyPressOccured();

private slots:
    void displayStateChanged(Maemo::QmDisplayState::DisplayState state);
    void unlockScreen();
    void updateMissedEventAmounts(int calls, int messages, int emails, int chatMessages);

private: //methods
    void toggleKeyPadLock(bool toggle);
    void toggleScreenLockUI(bool toggle);    

private: //attributes
    QmDisplayState *display;
    EventEater *eventEater;    
    LockScreenUI *lockUI;
    QDBusInterface *dbusIf;
};

#endif // LOCKSCREENBUSINESSLOGIC_H
