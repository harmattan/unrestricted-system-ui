#include "callandsimbusinesslogic.h"

#include <QString>

CallAndSimBusinessLogic::CallAndSimBusinessLogic()
{
}

CallAndSimBusinessLogic::~CallAndSimBusinessLogic()
{
}

void CallAndSimBusinessLogic::getCallerIdSending()
{
    emit callerIdSending(2);
}

void CallAndSimBusinessLogic::getCallWaiting()
{
    emit callWaiting(true);
}

void CallAndSimBusinessLogic::getCallForwarding()
{
    emit callForwarding(true);
}

void CallAndSimBusinessLogic::getForwardTo()
{
    emit forwardTo("+3581234567");
}

void CallAndSimBusinessLogic::getPinRequest()
{
    emit pinRequest(true);
}

void CallAndSimBusinessLogic::setCallerIdSending(int value)
{
}

void CallAndSimBusinessLogic::setCallWaiting(bool enable)
{
}

void CallAndSimBusinessLogic::setCallForwarding(bool enable)
{
}

void CallAndSimBusinessLogic::setForwardTo(const QString& number)
{
}

void CallAndSimBusinessLogic::setPinRequest(bool enable)
{
}
