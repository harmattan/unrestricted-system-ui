#include "callandsimbusinesslogic.h"

#include <QString>
#include <QDebug>

CallAndSimBusinessLogic::CallAndSimBusinessLogic() :
        i1(1),
        b1(false),
        b3(false),
        s1("+3581234567")
{
    callForwarding = new CallForwarding(this);
    callWaiting = new CallWaiting(this);

    connect(callWaiting, SIGNAL(waitingActivateComplete(CallWaiting::WaitingError)), this, SLOT(waitingActivateComplete(CallWaiting::WaitingError)));
    connect(callWaiting, SIGNAL(waitingCheckComplete(bool, CallWaiting::WaitingError)), this, SLOT(waitingCheckComplete(bool, CallWaiting::WaitingError)));
    connect(callWaiting, SIGNAL(waitingCancelComplete(CallWaiting::WaitingError)), this, SLOT(waitingCancelComplete(CallWaiting::WaitingError)));

    connect(callForwarding, SIGNAL(divertActivateComplete(CallForwarding::DivertError)), this, SLOT(divertActivateComplete(CallForwarding::DivertError)));
    connect(callForwarding, SIGNAL(divertCheckComplete(bool, QString, CallForwarding::DivertError)), this, SLOT(divertCheckComplete(bool, QString, CallForwarding::DivertError)));
    connect(callForwarding, SIGNAL(divertCancelComplete(CallForwarding::DivertError)), this, SLOT(divertCancelComplete(CallForwarding::DivertError)));
}

void CallAndSimBusinessLogic::waitingActivateComplete(CallWaiting::WaitingError)
{
    if (error != CallWaiting::NoError) {
        // TODO: Error handling
        return;
    }

    emit callWaiting(false);
}

void CallAndSimBusinessLogic::waitingCancelComplete(CallWaiting::WaitingError)
{
    if (error != CallWaiting::NoError) {
        // TODO: Error handling
        return;
    }

    emit callWaiting(false);
}

void CallAndSimBusinessLogic::waitingCheckComplete(bool active, CallWaiting::WaitingError)
{
    if (error != CallWaiting::NoError) {
        // TODO: Error handling
        return;
    }

    emit callWaiting(active);
}

void CallAndSimBusinessLogic::divertActivateComplete(CallForwarding::DivertError error)
{
}

void CallAndSimBusinessLogic::divertCancelComplete(CallForwarding::DivertError error)
{
}

void CallAndSimBusinessLogic::divertCheckComplete(bool active, QString number, CallForwarding::DivertError error)
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

    callWaiting->waitingCheck();
}

void CallAndSimBusinessLogic::getCallForwarding()
{
    qDebug() << Q_FUNC_INFO;

    callForwarding->callForwardingCheck();
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

    if (enabled) {
        callWaiting->waitingActivate();
    }
    else {
        callWaiting->waitingCancel();
    }
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
