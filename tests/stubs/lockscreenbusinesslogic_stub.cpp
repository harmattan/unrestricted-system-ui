/***********************************************************************
 * AUTHOR: David Kedves <dkedves@blumsoft.eu>
 *   FILE: .//lockscreenbusinesslogic.cpp
 *   DATE: Tue May 25 12:39:05 2010
 *  DESCR: Generated stub for LockScreenBusinessLogic 
 ***********************************************************************/
#include "lockscreenbusinesslogic_stub.h"

/***********************************************************************
 *  Method: LockScreenBusinessLogic::~LockScreenBusinessLogic
 *  Params: 
 * Effects: 
 ***********************************************************************/
LockScreenBusinessLogic::LockScreenBusinessLogic(QObject *parent) :
    lockUIvisible (false),
    eaterUIvisible (false)
{
	Q_UNUSED(parent);
}

/***********************************************************************
 *  Method: LockScreenBusinessLogic::~LockScreenBusinessLogic
 *  Params: 
 * Effects: 
 ***********************************************************************/
LockScreenBusinessLogic::~LockScreenBusinessLogic()
{
}


/***********************************************************************
 *  Method: LockScreenBusinessLogic::toggleScreenLockUI
 *  Params: bool toggle
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
LockScreenBusinessLogic::toggleScreenLockUI(bool toggle)
{
    lockUIvisible = toggle;
    if (! toggle)
        eaterUIvisible = false;
}


/***********************************************************************
 *  Method: LockScreenBusinessLogic::toggleEventEater
 *  Params: bool toggle
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
LockScreenBusinessLogic::toggleEventEater(bool toggle)
{
    eaterUIvisible = toggle;
    if (! toggle)
        lockUIvisible = false;
}


/***********************************************************************
 *  Method: LockScreenBusinessLogic::stopTimer
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
LockScreenBusinessLogic::stopTimer()
{
}


/***********************************************************************
 *  Method: LockScreenBusinessLogic::hidefromTaskBar
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
LockScreenBusinessLogic::hidefromTaskBar()
{
}


/***********************************************************************
 *  Method: LockScreenBusinessLogic::unlockScreen
 *  Params: 
 * Returns: void
 * Effects: 
 ***********************************************************************/
void
LockScreenBusinessLogic::unlockScreen()
{
    toggleScreenLockUI (false);
}


