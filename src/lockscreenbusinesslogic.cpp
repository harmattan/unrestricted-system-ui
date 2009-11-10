#include <DuiApplicationWindow>
#include <DuiApplication>
#include <QDBusInterface>
#include <QDebug>

#include "lockscreenui.h"
#include "lockscreenbusinesslogic.h"

LockScreenBusinessLogic::LockScreenBusinessLogic(QObject* parent) :
        QObject(parent),
        display(NULL),
        lockUI(NULL)
{
    qDebug() << Q_FUNC_INFO;

    display = new QmDisplayState(this);
    qDebug() << "lockUI = new LockScreenUI()";
    lockUI = new LockScreenUI();
    qDebug() << "lockUI =" << (void*)lockUI;

    connect(display, SIGNAL(displayStateChanged(Maemo::QmDisplayState::DisplayState)),
            this, SLOT(displayStateChanged(Maemo::QmDisplayState::DisplayState)));
    connect(lockUI, SIGNAL(unlocked()), this, SLOT(unlockScreen()));


    /*
    dbusIf = new QDBusInterface("org.maemo.dui.NotificationManager", "/systemui",
                                "org.maemo.dui.NotificationManager.MissedEvents",
                                QDBusConnection::sessionBus(), this);
    connect(dbusIf, SIGNAL(missedEventAmountsChanged(int, int, int, int)),
            this, SLOT(updateMissedEventAmounts(int, int, int, int)));
    dbusIf->call(QDBus::NoBlock, QString("missedEventAmountsRequired"));
    */
}

LockScreenBusinessLogic::~LockScreenBusinessLogic()
{
    //delete eventEater;
    //eventEater = NULL;
    delete lockUI;
}

void LockScreenBusinessLogic::shortPowerKeyPressOccured()
{
    qDebug() << Q_FUNC_INFO;
    switch (display->get()) {
    case Maemo::QmDisplayState::Off:
        toggleScreenLockUI(true); //make sure the UI is on
        toggleKeyPadLock(false);
        display->set(Maemo::QmDisplayState::On);
        break;
    case Maemo::QmDisplayState::On:
    case Maemo::QmDisplayState::Dimmed:
        toggleKeyPadLock(true);
        display->set(Maemo::QmDisplayState::Off);
        toggleScreenLockUI(true);
        break;
    }
}

void LockScreenBusinessLogic::displayStateChanged(Maemo::QmDisplayState::DisplayState state)
{
    qDebug() << Q_FUNC_INFO << state;
    switch (state) {
    case Maemo::QmDisplayState::Off:
        toggleKeyPadLock(true); //lock the keypad
        toggleScreenLockUI(true); //show UI
        break;
    case Maemo::QmDisplayState::On:
        toggleKeyPadLock(false); //unlock the keypad
        break;
    default:
        break;
    }
}

void LockScreenBusinessLogic::unlockScreen()
{
    qDebug() << Q_FUNC_INFO;
    toggleScreenLockUI(false); //turn off the UI
    toggleKeyPadLock(false); //unlock the keypad
}

void LockScreenBusinessLogic::updateMissedEventAmounts(int a, int b, int c, int d)
{
    qDebug() << "LockScreenBusinessLogic::updateMissedEventAmounts("
    << a << ", " << b << ", " << c << ", " << d << ")";
    lockUI->updateMissedEventAmounts(a, b, c, d);
}

void LockScreenBusinessLogic::toggleKeyPadLock(bool toggle)
{
    qDebug() << Q_FUNC_INFO << toggle;
    QmLocks::State newState = (toggle ? QmLocks::Locked : QmLocks::Unlocked);
    QmLocks touchPadLocker;
    touchPadLocker.setState(QmLocks::TouchAndKeyboard, newState);
}

void LockScreenBusinessLogic::toggleScreenLockUI(bool toggle)
{
    qDebug() << Q_FUNC_INFO << toggle;
    if (toggle) {
        DuiApplication::instance()->applicationWindow()->show();
        lockUI->appear();
    } else {
        DuiApplication::instance()->applicationWindow()->hide();
    }
}


