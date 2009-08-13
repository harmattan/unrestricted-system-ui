#include "lockscreenbusinesslogic.h"

//these are temp values
namespace {
    int idleTimeToDim = 10000; //10 seconds
    int idleTimeToSleepFromDim = 10000; //10seconds
    int idleTimeToSleepFromUnlockScreen = 2000; // 2 seconds
}

LockScreenBusinessLogic::LockScreenBusinessLogic()
{
    screenLock = false,
    sleepMode = false;

    touchPadLocker = new QmLocks();
    display = new QmDisplayState();

    QTimer *t = new QTimer();
    connect(t, SIGNAL(timeout()), this, SLOT(timeout()));
    t->setSingleShot(false);
    t->start(2000);
}

LockScreenBusinessLogic::~LockScreenBusinessLogic()
{
    delete touchPadLocker;
    touchPadLocker = NULL;
    delete display;
    display = NULL;
}

bool LockScreenBusinessLogic::screenLockOn()
{
    return screenLock;
}

void LockScreenBusinessLogic::toggleScreenLock(bool toggle)
{
    screenLock = toggle;
}

bool LockScreenBusinessLogic::sleepModeOn()
{
    return sleepMode;
}

void LockScreenBusinessLogic::timeout()
{
    if(screenLock)
        toggleSleepMode(true);
    else {
        if(display->get() == QmDisplayState::Dimmed) {
            toggleScreenLock(true);
            toggleSleepMode(true);
        }
        else
            display->set(QmDisplayState::Dimmed);
    }
}

void LockScreenBusinessLogic::stopMonitroringIdleTime()
{    
    timer->stop();
}

void LockScreenBusinessLogic::startMonitroringIdleTime()
{
    if(screenLock)
        timer->start(idleTimeToSleepFromUnlockScreen);
    else
        timer->start(idleTimeToDim);       
}

void LockScreenBusinessLogic::toggleSleepMode(bool toggle)
{
    sleepMode = toggle;

    if(sleepMode) {
        //if we are now in sleep mode, we lock the touchpad and turn off the display        
        touchPadLocker->setState(QmLocks::TouchAndKeyboard, QmLocks::Locked);
        display->set(QmDisplayState::Off);
        timer->stop(); //no need to monitor idle time
    }
    else  {
        //if not, we unlock the touchpad and turn on the display
        touchPadLocker->setState(QmLocks::TouchAndKeyboard, QmLocks::Unlocked);
        display->set(QmDisplayState::On);        
        timer->start(idleTimeToSleepFromUnlockScreen); //monitoring idle time and go back to sleep if nothing happens
    }
}
