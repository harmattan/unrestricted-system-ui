#include "callandsimbusinesslogic.h"

#include <QString>
#include <QDebug>

CallAndSimBusinessLogic::CallAndSimBusinessLogic() :
        i1(1),
        b1(false),
        b2(false),
        b3(false),
        s1("+3581234567")
{
}

CallAndSimBusinessLogic::~CallAndSimBusinessLogic()
{
}

void CallAndSimBusinessLogic::getCallerIdSending()
{
    qDebug() << Q_FUNC_INFO;
    emit callerIdSending(i1);
}

void CallAndSimBusinessLogic::getCallWaiting()
{
    qDebug() << Q_FUNC_INFO;
    emit callWaiting(b1);
}

void CallAndSimBusinessLogic::getCallForwarding()
{
    qDebug() << Q_FUNC_INFO;
    emit callForwarding(b2);
}

void CallAndSimBusinessLogic::getForwardTo()
{
    qDebug() << Q_FUNC_INFO;
    emit forwardTo(s1);
}

void CallAndSimBusinessLogic::getPinRequest()
{
    qDebug() << Q_FUNC_INFO;
    emit pinRequest(b3);
}

void CallAndSimBusinessLogic::setCallerIdSending(int value)
{
    qDebug() << Q_FUNC_INFO << value;
    i1 = value;
}

void CallAndSimBusinessLogic::setCallWaiting(bool enabled)
{
    qDebug() << Q_FUNC_INFO << enabled;
    b1 = enabled;
}

void CallAndSimBusinessLogic::setCallForwarding(bool enabled)
{
    qDebug() << Q_FUNC_INFO << enabled;
    b2 = enabled;
}

void CallAndSimBusinessLogic::setForwardTo(const QString& number)
{
    qDebug() << Q_FUNC_INFO << number;
    s1 = number;
}

void CallAndSimBusinessLogic::setPinRequest(bool enabled)
{
    qDebug() << Q_FUNC_INFO << enabled;
    b3 = enabled;
}
