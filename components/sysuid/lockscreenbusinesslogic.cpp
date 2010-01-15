#include <DuiApplicationWindow>
#include <DuiApplication>
#include <QDBusInterface>
#include <QDebug>
#include <QTime>

#include "lockscreenui.h"
#include "lockscreenbusinesslogic.h"

LockScreenBusinessLogic::LockScreenBusinessLogic(QObject* parent) :
        QObject(parent),
        display(NULL),
        lockUI(NULL)
{
    qDebug() << Q_FUNC_INFO;

    locks = new QmLocks(this);
    display = new QmDisplayState(this);
    lockUI = new LockScreenUI();

    connect(locks, SIGNAL(stateChanged(Maemo::QmLocks::Lock, Maemo::QmLocks::State)),
            this, SLOT(locksChanged(Maemo::QmLocks::Lock, Maemo::QmLocks::State)));
    connect(display, SIGNAL(displayStateChanged(Maemo::QmDisplayState::DisplayState)),
            this, SLOT(displayStateChanged(Maemo::QmDisplayState::DisplayState)));
    connect(lockUI, SIGNAL(unlocked()), this, SLOT(unlockScreen()));

    connect(&timer, SIGNAL(timeout()), lockUI, SLOT(updateDateTime()));
}

LockScreenBusinessLogic::~LockScreenBusinessLogic()
{
    delete lockUI;
}

void LockScreenBusinessLogic::shortPowerKeyPressOccured()
{
    qDebug() << Q_FUNC_INFO;
    /* THIS SHOULD BE DONE BY MCE OF SYSTEMSW
    */
    if (knownLock == QmLocks::Unlocked) {
        locks->setState(QmLocks::TouchAndKeyboard, QmLocks::Locked);
    }
}

void LockScreenBusinessLogic::locksChanged(Maemo::QmLocks::Lock lock, Maemo::QmLocks::State state)
{
    if (lock == QmLocks::Device)
        return;

    knownLock = state;

    if (knownLock == QmLocks::Locked) {
        toggleScreenLockUI(true);
        mayStartTimer();
    } else {
        toggleScreenLockUI(false);
        stopTimer();
    }
}


void LockScreenBusinessLogic::displayStateChanged(Maemo::QmDisplayState::DisplayState state)
{
    knownDisplay = state;

    switch (state) {
    case Maemo::QmDisplayState::Off:
        stopTimer();
        break;
    case Maemo::QmDisplayState::On:
        mayStartTimer();
        break;
    default:
        break;
    }
}

void LockScreenBusinessLogic::unlockScreen()
{
    qDebug() << Q_FUNC_INFO;
    toggleScreenLockUI(false); //turn off the UI
    locks->setState(QmLocks::TouchAndKeyboard, QmLocks::Unlocked);
}

void LockScreenBusinessLogic::updateMissedEventAmounts(int a, int b, int c, int d)
{
    qDebug() << "LockScreenBusinessLogic::updateMissedEventAmounts("
    << a << ", " << b << ", " << c << ", " << d << ")";
    lockUI->updateMissedEventAmounts(a, b, c, d);
}

void LockScreenBusinessLogic::toggleScreenLockUI(bool toggle)
{
    qDebug() << Q_FUNC_INFO << toggle;
    if (toggle) {
        DuiApplication::activeApplicationWindow()->show();
        lockUI->appear();
    } else {
        DuiApplication::activeApplicationWindow()->hide();
    }
}

void LockScreenBusinessLogic::mayStartTimer()
{
    if (knownLock == QmLocks::Locked && knownDisplay != QmDisplayState::Off) {
        // It's better to update the time straight away.
        lockUI->updateDateTime();

        QTime t(QTime::currentTime());
        // TODO: some adjustments of time may be done
        timer.start(1000);
    }
}

void LockScreenBusinessLogic::stopTimer()
{
    timer.stop();
}
