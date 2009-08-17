#include "eventhandler.h"

EventHandler::EventHandler(LockScreenBusinessLogic *lockScreenLogic)
{
    this->lockScreenLogic = lockScreenLogic;

    keys = new QmKeys();

    //Monitor hardware key presses
    connect(keys, SIGNAL(keyPressed(QmKeys::Key key, QmKeys::HowPressed how)),
            this, SLOT(keyPressed(QmKeys::Key key, QmKeys::HowPressed how)));
}

EventHandler::~EventHandler()
{
}

void EventHandler::keyPressed(QmKeys::Keys key, QmKeys::HowPressed how)
{
    if( lockScreenLogic == NULL)
        return;

    lockScreenLogic->stopMonitroringIdleTime();

    switch(key) {
        case QmKeys::PowerKey:
            switch(how) {
                case QmKeys::ShortPress:
                    shortPowerKeyPressOccured();
                break;
                case QmKeys::LongPress:
                break;
            }
        break;
        case QmKeys::HomeKey:
        break;
        case QmKeys::CameraShutterKey:
        break;
        default:
        break;
    }

    lockScreenLogic->startMonitroringIdleTime();
}

void EventHandler::shortPowerKeyPressOccured()
{
    if(lockScreenLogic->screenLockOn()) {
        if(lockScreenLogic->sleepModeOn())
           lockScreenLogic->toggleSleepMode(false);
        else
            lockScreenLogic->toggleSleepMode(true);
    }
    else {
        lockScreenLogic->toggleScreenLock(true);
        lockScreenLogic->toggleSleepMode(true);
    }

}
