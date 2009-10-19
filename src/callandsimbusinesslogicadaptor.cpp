#include "callandsimbusinesslogicadaptor.h"
#include "callandsimbusinesslogic.h"

CallAndSimBusinessLogicAdaptor::CallAndSimBusinessLogicAdaptor(QObject* obj, CallAndSimBusinessLogic* logic) :
    QDBusAbstractAdaptor(obj),
    logic(logic)
{
    connect(logic, SIGNAL(callerIdSending(int)), this, SIGNAL(callerIdSending(int)));
    connect(logic, SIGNAL(callWaiting(bool)),    this, SIGNAL(callWaiting(bool)));
    connect(logic, SIGNAL(callForwarding(bool)), this, SIGNAL(callForwarding(bool)));
    connect(logic, SIGNAL(forwardTo(QString)),   this, SIGNAL(forwardTo(QString)));
    connect(logic, SIGNAL(pinRequest(bool)),     this, SIGNAL(pinRequest(bool)));
}

CallAndSimBusinessLogicAdaptor::~CallAndSimBusinessLogicAdaptor()
{
}

void CallAndSimBusinessLogicAdaptor::getCallerIdSending()
{
    logic->getCallerIdSending();
}

void CallAndSimBusinessLogicAdaptor::getCallWaiting()
{
    logic->getCallWaiting();
}

void CallAndSimBusinessLogicAdaptor::getCallForwarding()
{
    logic->getCallForwarding();
}

void CallAndSimBusinessLogicAdaptor::getForwardTo()
{
    logic->getForwardTo();
}

void CallAndSimBusinessLogicAdaptor::getPinRequest()
{
    logic->getPinRequest();
}

void CallAndSimBusinessLogicAdaptor::setCallerIdSending(int value)
{
    logic->setCallerIdSending(value);
}

void CallAndSimBusinessLogicAdaptor::setCallWaiting(bool enabled)
{
    logic->setCallWaiting(enabled);
}

void CallAndSimBusinessLogicAdaptor::setCallForwarding(bool enabled)
{
    logic->setCallForwarding(enabled);
}

void CallAndSimBusinessLogicAdaptor::setForwardTo(const QString& number)
{
    logic->setForwardTo(number);
}

void CallAndSimBusinessLogicAdaptor::setPinRequest(bool enabled)
{
    logic->setPinRequest(enabled);
}
