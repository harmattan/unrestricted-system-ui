#include <DuiApplicationWindow>
#include <DuiApplication>
#include <QDBusInterface>
#include <QDebug>

#include "lockscreenui.h"
#include "lockscreenbusinesslogic.h"

LockScreenBusinessLogic::LockScreenBusinessLogic() : QObject()
{
    eventEater = new EventEater();

    screenLock = false,
    sleepMode = false;
    isDisabled = false;

    touchPadLocker = new QmLocks();
    //display = new QmDisplayState(); //this should be handled by the displaybusinesslogic

    //toggleDisplayStateListener(true); //this should be handled by the displaybusinesslogic

    toggleSleepMode(false);

    lockUI = new LockScreenUI;
    connect(lockUI, SIGNAL(unlocked()), this, SLOT(unlockScreen()));

    dbusIf = new QDBusInterface("org.maemo.dui.NotificationManager", "/systemui",
                                "org.maemo.dui.NotificationManager.MissedEvents",
                                QDBusConnection::sessionBus());
    connect(dbusIf, SIGNAL(missedEventAmountsChanged(int, int, int, int)),
            this, SLOT(updateMissedEventAmounts(int, int, int, int)));
    dbusIf->call(QDBus::NoBlock, QString("missedEventAmountsRequired"));

}

LockScreenBusinessLogic::~LockScreenBusinessLogic()
{
    delete touchPadLocker;
    touchPadLocker = NULL;
    //delete display; //this should be handled by the displaybusinesslogic
    //display = NULL; //this should be handled by the displaybusinesslogic
    delete eventEater;
    eventEater = NULL;
    if (lockUI) {
        delete lockUI;
        lockUI = NULL;
    }
}

void LockScreenBusinessLogic::shortPowerKeyPressOccured()
{
    qDebug() << Q_FUNC_INFO << "isDisabled:" << isDisabled << "screenLock:" << screenLock << "sleepMode:" << sleepMode;

    if(!isDisabled) {
        if(screenLock) {
            if(sleepMode)
                toggleSleepMode(false);
            else
                toggleSleepMode(true);
        }
        else {
            toggleScreenLock(true);
            toggleSleepMode(true);
        }
    }
}

void LockScreenBusinessLogic::toggleScreenLock(bool toggle)
{
    qDebug() << Q_FUNC_INFO << toggle;

    if(!toggle) {
        emit lockScreenOff();
        DuiApplication::instance()->applicationWindow()->hide();
    }
    else {        
        DuiApplication::instance()->applicationWindow()->show();
        lockUI->appear();
    }

    screenLock = toggle;
}

//this should be handled by the displaybusinesslogic
/*
void LockScreenBusinessLogic::displayStateChanged(Maemo::QmDisplayState::DisplayState state)
{
    switch(state) {
        case QmDisplayState::Off:
            //idle timer turns off the display, so we will turn on sleep mode and screen lock
            toggleSleepMode(true);
            toggleScreenLock(true);
        break;
        case QmDisplayState::Dimmed:
            //we turn on the eventeater
            eventEater->toggle(true);
        break;
        case QmDisplayState::On:
            //we trun off the event eater
            eventEater->toggle(false);
        break;
    }

}
*/
void LockScreenBusinessLogic::sleepModeOff()
{
    if(sleepMode)
        toggleSleepMode(false);
}

void LockScreenBusinessLogic::unlockScreen()
{
    toggleScreenLock(false);
}


void LockScreenBusinessLogic::toggleSleepMode(bool toggle)
{
    if(toggle) {
        //we turn on the sleep mode and lock the touchpad + turn off the display
        touchPadLocker->setState(QmLocks::TouchAndKeyboard, QmLocks::Locked);

        /*
        if(display->get() != QmDisplayState::Off) {
            //we don't need to listen display signals when we change the state by ourself
            toggleDisplayStateListener(false);
            display->set(QmDisplayState::Off);
            toggleDisplayStateListener(true);
        }
        */
    }
    else {
        //we turn off the sleep mode and unlock the touchpad + turn on the display
        touchPadLocker->setState(QmLocks::TouchAndKeyboard, QmLocks::Unlocked);
        /*
        if(display->get() != QmDisplayState::On) {
            //we don't need to listen display signals when we change the state by ourself
            toggleDisplayStateListener(false);
            display->set(QmDisplayState::On);
            toggleDisplayStateListener(true);
        }
        */
    }
    sleepMode = toggle;
}

//this should be handled by the displaybusinesslogic
/*
void LockScreenBusinessLogic::toggleDisplayStateListener(bool toggle)
{
    if(toggle)
        connect(display, SIGNAL(displayStateChanged(Maemo::QmDisplayState::DisplayState)),
            this, SLOT(displayStateChanged(Maemo::QmDisplayState::DisplayState)));
    else
        disconnect(display, SIGNAL(displayStateChanged(Maemo::QmDisplayState::DisplayState)),
            this, SLOT(displayStateChanged(Maemo::QmDisplayState::DisplayState)));
}
*/

void LockScreenBusinessLogic::disable(bool disable)
{    
    isDisabled = disable;
}

void LockScreenBusinessLogic::updateMissedEventAmounts(int calls, int messages, int emails, int chatMessages)
{
    qDebug() << "LockScreenBusinessLogic::updateMissedEventAmounts(" << calls << ", " << messages << ", " << emails << ", " << chatMessages << ")";
    lockUI->updateMissedEventAmounts(calls, messages, emails, chatMessages);
}
