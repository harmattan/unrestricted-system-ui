#include "lockscreenbusinesslogic.h"


LockScreenBusinessLogic::LockScreenBusinessLogic() : QObject()
{
    eventEater = new EventEater();

    screenLock = false,
    sleepMode = false;

    touchPadLocker = new QmLocks();
    display = new QmDisplayState();

    toggleDisplayStateListener(true);

    toggleSleepMode(false);    
}

LockScreenBusinessLogic::~LockScreenBusinessLogic()
{
    delete touchPadLocker;
    touchPadLocker = NULL;
    delete display;
    display = NULL;
    delete eventEater;
    eventEater = NULL;
}

void LockScreenBusinessLogic::shortPowerKeyPressOccured()
{    
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

void LockScreenBusinessLogic::toggleScreenLock(bool toggle)
{
    if(!toggle)
        emit lockScreenOff();

    screenLock = toggle;
}

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

void LockScreenBusinessLogic::toggleSleepMode(bool toggle)
{
    if(toggle) {
        //we turn on the sleep mode and lock the touchpad + turn off the display
        touchPadLocker->setState(QmLocks::TouchAndKeyboard, QmLocks::Locked);

        if(display->get() != QmDisplayState::Off) {
            //we don't need to listen display signals when we change the state by ourself
            toggleDisplayStateListener(false);
            display->set(QmDisplayState::Off);
            toggleDisplayStateListener(true);
        }
    }
    else  {
        //we turn off the the sleep mode and unlock the touchpad + turn on the display
        touchPadLocker->setState(QmLocks::TouchAndKeyboard, QmLocks::Unlocked);

        if(display->get() != QmDisplayState::On) {
            //we don't need to listen display signals when we change the state by ourself
            toggleDisplayStateListener(false);
            display->set(QmDisplayState::On);
            toggleDisplayStateListener(true);
        }
    }

    sleepMode = toggle;
}

void LockScreenBusinessLogic::toggleDisplayStateListener(bool toggle)
{
    if(toggle)
        connect(display, SIGNAL(displayStateChanged(Maemo::QmDisplayState::DisplayState)),
            this, SLOT(displayStateChanged(Maemo::QmDisplayState::DisplayState)));
    else
        disconnect(display, SIGNAL(displayStateChanged(Maemo::QmDisplayState::DisplayState)),
            this, SLOT(displayStateChanged(Maemo::QmDisplayState::DisplayState)));
}
