#ifndef LOCKSCREENBUSINESSLOGIC_H
#define LOCKSCREENBUSINESSLOGIC_H

#include <qmsystem/qmlocks.h>
#include <qmsystem/qmdisplaystate.h>
//#include "eventeater.h"

#include <QObject>
#include <QTimer>

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
    void locksChanged(Maemo::QmLocks::Lock what, Maemo::QmLocks::State how);
    void unlockScreen();

    // from dbus
    void updateMissedEventAmounts(int, int, int, int);

signals:
    void updateTime();

private: //methods
    void toggleScreenLockUI(bool toggle);
    void mayStartTimer();
    void stopTimer();

private: //attributes
    QmDisplayState *display;
    QmLocks *locks;
    LockScreenUI *lockUI;

    QTimer timer;

    QmLocks::State knownLock;
    QmDisplayState::DisplayState knownDisplay;
};

#endif // LOCKSCREENBUSINESSLOGIC_H
