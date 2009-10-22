#include "callandsim.h"

CallAndSim::CallAndSim(QObject* parent) :
        QObject(parent)
{
    callForwarding = new CallForwarding(this);
    callWaiting = new CallWaiting(this);
    simSecurity = new SIMSecurity(this);

    connect(callWaiting, SIGNAL(waitingActivateComplete(CallWaiting::WaitingError)), this, SLOT(waitingActivateComplete(CallWaiting::WaitingError)));
    connect(callWaiting, SIGNAL(waitingCheckComplete(bool, CallWaiting::WaitingError)), this, SLOT(waitingCheckComplete(bool, CallWaiting::WaitingError)));
    connect(callWaiting, SIGNAL(waitingCancelComplete(CallWaiting::WaitingError)), this, SLOT(waitingCancelComplete(CallWaiting::WaitingError)));

    connect(callForwarding, SIGNAL(divertActivateComplete(CallForwarding::DivertError)), this, SLOT(divertActivateComplete(CallForwarding::DivertError)));
    connect(callForwarding, SIGNAL(divertCheckComplete(bool, QString, CallForwarding::DivertError)), this, SLOT(divertCheckComplete(bool, QString, CallForwarding::DivertError)));
    connect(callForwarding, SIGNAL(divertCancelComplete(CallForwarding::DivertError)), this, SLOT(divertCancelComplete(CallForwarding::DivertError)));

    // TODO: PIN, CallerId
}

void CallAndSim::setCallerIdSending(int value)
{
    // TODO:
}

void CallAndSim::setCallWaiting(bool enabled)
{
    if (enabled) {
        callWaiting->waitingActivate();
    }
    else {
        callWaiting->waitingCancel();
    }
}

void CallAndSim::setCallForwarding(bool enabled, QString number)
{
/*
    -Calls are diverted when the device is busy,
    -when the device is not reachable (turned off or out of network) or
    -when the call is not answered in a pre-defined time (20 seconds).
*/
    //callWaiting->divertActivate();
}

void CallAndSim::setPinRequest(bool enabled)
{
    // TODO
}

void CallAndSim::changePinCode()
{
    if (!dbusPinIf) {
        dbusPinIf = new QDBusInterface("com.nokia.systemui",
                                       "/",
                                       "com.nokia.systemui.PinCodeQuery",
                                       QDBusConnection::sessionBus(),
                                       this);
    }

    dbusPinIf->call(QDBus::NoBlock, QString("changePinCode"));
}

void CallAndSim::requestData(CallAndSim::Item item)
{
    switch (item) {
        case ItemCallerIdSending:
            // TODO
            break;
        case ItemCallWaiting:
            callWaiting->waitingCheck();
            break;
        case ItemCallForwarding:
//            callForwarding->forwardingCheck();
            break;
        case ItemPinRequest:
            // TODO
            break;
        case ItemAll:
//            callWaiting->waitingCheck();
//            callForwarding->forwardingCheck();
            break;
    }
}

void CallAndSim::waitingActivateComplete(CallWaiting::WaitingError error)
{
    if (error != CallWaiting::NoError) {
        // TODO: display error note
        emit requestFailed(CallAndSim::ItemCallWaiting);
    }
}

void CallAndSim::waitingCancelComplete(CallWaiting::WaitingError error)
{
    if (error != CallWaiting::NoError) {
        // TODO: display error note
        emit requestFailed(CallAndSim::ItemCallWaiting);
    }

    emit callWaitingComplete(false);
}

void CallAndSim::waitingCheckComplete(bool active, CallWaiting::WaitingError error)
{
    if (error != CallWaiting::NoError) {
        // TODO: display error note
        emit requestFailed(CallAndSim::ItemCallWaiting);
        return;
    }

    emit callWaitingComplete(active);
}

void CallAndSim::divertActivateComplete(CallForwarding::DivertError error)
{
    if (error != CallForwarding::NoError) {
        // TODO: display error note
        emit requestFailed(CallAndSim::ItemCallForwarding);
    }
}

void CallAndSim::divertCancelComplete(CallForwarding::DivertError error)
{
    if (error != CallForwarding::NoError) {
        // TODO: display error note
        emit requestFailed(CallAndSim::ItemCallForwarding);
    }
}

void CallAndSim::divertCheckComplete(bool active, QString number, CallForwarding::DivertError error)
{
    if (error != CallForwarding::NoError) {
        // TODO: display error note
        emit requestFailed(CallAndSim::ItemCallForwarding);
        return;
    }

    emit callForwardingComplete(active, number);
}
