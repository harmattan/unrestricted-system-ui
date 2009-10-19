#include "callandsimbusinesslogicadaptor.h"
#include "callandsimbusinesslogic.h"

#include <QDebug>

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
    qDebug() << Q_FUNC_INFO;
    logic->getCallerIdSending();
}

void CallAndSimBusinessLogicAdaptor::getCallWaiting()
{
    qDebug() << Q_FUNC_INFO;
    logic->getCallWaiting();
}

void CallAndSimBusinessLogicAdaptor::getCallForwarding()
{
    qDebug() << Q_FUNC_INFO;
    logic->getCallForwarding();
}

void CallAndSimBusinessLogicAdaptor::getForwardTo()
{
    qDebug() << Q_FUNC_INFO;
    logic->getForwardTo();
}

void CallAndSimBusinessLogicAdaptor::getPinRequest()
{
    qDebug() << Q_FUNC_INFO;
    logic->getPinRequest();
}

void CallAndSimBusinessLogicAdaptor::setCallerIdSending(int value)
{
    qDebug() << Q_FUNC_INFO;
    logic->setCallerIdSending(value);
}

void CallAndSimBusinessLogicAdaptor::setCallWaiting(bool enabled)
{
    qDebug() << Q_FUNC_INFO;
    logic->setCallWaiting(enabled);
}

void CallAndSimBusinessLogicAdaptor::setCallForwarding(bool enabled)
{
    qDebug() << Q_FUNC_INFO;
    logic->setCallForwarding(enabled);
}

void CallAndSimBusinessLogicAdaptor::setForwardTo(const QString& number)
{
    qDebug() << Q_FUNC_INFO;
    logic->setForwardTo(number);
}

void CallAndSimBusinessLogicAdaptor::setPinRequest(bool enabled)
{
    qDebug() << Q_FUNC_INFO;
    logic->setPinRequest(enabled);
}
