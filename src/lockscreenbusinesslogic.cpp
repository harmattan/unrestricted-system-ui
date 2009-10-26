#include <DuiApplicationWindow>
#include <DuiApplication>
#include <QDBusInterface>
#include <QDebug>

#include "lockscreenui.h"
#include "lockscreenbusinesslogic.h"
#include "displaystatestub.h"

LockScreenBusinessLogic::LockScreenBusinessLogic() :        
        phase(LockScreenBusinessLogic::Off)
{    
}

LockScreenBusinessLogic::~LockScreenBusinessLogic()
{ 
    delete eventEater;
    eventEater = NULL;
    delete lockUI;
    lockUI = NULL;
}

LockScreenBusinessLogic::ScreenLockPhase LockScreenBusinessLogic::screenLockPhase()
{
    return phase;
}\

void LockScreenBusinessLogic::shortPowerKeyPressOccured()
{    
    qDebug() << "LockScreenBusinessLogic::shortPowerKeyPressOccured()";
    switch(phase) {
        case LockScreenBusinessLogic::Off:        
            //Screenlock is off. Put asleep.
            phase = LockScreenBusinessLogic::Sleep;
            toggleKeyPadLock(true); //lock the keypad
            emit toggleDisplay(false); //order display off            
            break;
        case LockScreenBusinessLogic::Sleep:
            //Screenlock is in sleep mode. Wake up.
            phase = LockScreenBusinessLogic::On;            
            toggleScreenLockUI(true); //turn on the UI
            emit toggleDisplay(true); //order display on
            break;
        case LockScreenBusinessLogic::On:
            //Screenlock is on. Put asleep.
            phase = LockScreenBusinessLogic::Sleep;
            toggleScreenLockUI(false); //turn off the UI
            emit toggleDisplay(false); //order display off
            break;
    }
    emit screenLockPhaseChanged(phase);
}

void LockScreenBusinessLogic::displayOff()
{
    qDebug() << "LockScreenBusinessLogic::displayOff()";
    switch(phase) {
        case LockScreenBusinessLogic::Off:
            //Screenlock is off. Put asleep.
            phase = LockScreenBusinessLogic::Sleep;
            toggleKeyPadLock(true); //lock the keypad            
            break;
        case LockScreenBusinessLogic::Sleep:
            break;
        case LockScreenBusinessLogic::On:
            //Screenlock is on. Put asleep.
            phase = LockScreenBusinessLogic::Sleep;
            toggleScreenLockUI(false); //turn off the UI            
            break;
    }
    emit screenLockPhaseChanged(phase);
}

void LockScreenBusinessLogic::displayOn()
{
    qDebug() << "LockScreenBusinessLogic::displayOn()";
    // This can happen e.g. when charger is connected

    switch(phase) {
        case LockScreenBusinessLogic::Off:
        case LockScreenBusinessLogic::On:
            break;
        case LockScreenBusinessLogic::Sleep:
            //Screenlock is in sleep mode. Wake up.
            phase = LockScreenBusinessLogic::On;
            toggleScreenLockUI(true); //turn on the UI
            break;
    }
    emit screenLockPhaseChanged(phase);
}

void LockScreenBusinessLogic::toggleKeyPadLock(bool toggle)
{
    qDebug() << "LockScreenBusinessLogic::toggleKeyPadLock(" << toggle << ")";
    QmLocks::State newState = (toggle ? QmLocks::Locked : QmLocks::Unlocked);
    QmLocks touchPadLocker;
    touchPadLocker.setState(QmLocks::TouchAndKeyboard, newState);
}

void LockScreenBusinessLogic::toggleScreenLockUI(bool toggle)
{
    qDebug() << "LockScreenBusinessLogic::toggleScreenLockUI(" << toggle << ")";
    if(toggle) {
        DuiApplication::instance()->applicationWindow()->show();
        lockUI->appear();
    }
    else
        DuiApplication::instance()->applicationWindow()->hide();
}

void LockScreenBusinessLogic::unlockScreen()
{
    qDebug() << "LockScreenBusinessLogic::unlockScreen()";
    phase = LockScreenBusinessLogic::Off;
    toggleScreenLockUI(false); //turn off the UI
    toggleKeyPadLock(false); //unlock the keypad    
    emit screenLockPhaseChanged(phase);
}

void LockScreenBusinessLogic::updateMissedEventAmounts(int calls, int messages, int emails, int chatMessages)
{
    qDebug() << "LockScreenBusinessLogic::updateMissedEventAmounts(" << calls << ", " << messages << ", " << emails << ", " << chatMessages << ")";
    lockUI->updateMissedEventAmounts(calls, messages, emails, chatMessages);
}
