#include "callandsim.h"

using namespace DcpCallAndSim;

// Delay before a call is forwarded
const int FORWARDING_DELAY(20);

CallAndSim::CallAndSim(QObject* parent) :
        QObject(parent)
{
    callForwarding = new CallForwarding(this);
    callWaiting = new CallWaiting(this);
    simSecurity = new SIMSecurity(this);
//    networkRegistration = new NetworkRegistration(this);

    connect(callWaiting, SIGNAL(waitingActivateComplete(CallWaiting::WaitingError)),
            this, SLOT(waitingActivateComplete(CallWaiting::WaitingError)));
    connect(callWaiting, SIGNAL(waitingCheckComplete(bool, CallWaiting::WaitingError)),
            this, SLOT(waitingCheckComplete(bool, CallWaiting::WaitingError)));
    connect(callWaiting, SIGNAL(waitingCancelComplete(CallWaiting::WaitingError)),
            this, SLOT(waitingCancelComplete(CallWaiting::WaitingError)));

    connect(callForwarding, SIGNAL(divertActivateComplete(CallForwarding::DivertError)),
            this, SLOT(divertActivateComplete(CallForwarding::DivertError)));
    connect(callForwarding, SIGNAL(divertCheckComplete(bool, QString, CallForwarding::DivertError)),
            this, SLOT(divertCheckComplete(bool, QString, CallForwarding::DivertError)));
    connect(callForwarding, SIGNAL(divertCancelComplete(CallForwarding::DivertError)),
            this, SLOT(divertCancelComplete(CallForwarding::DivertError)));

//    connect(networkRegistration, SIGNAL(statusChanged(int)), this, SLOT(networkStatusChanged(int)));

    // TODO: CallerId

    dbusPinIf = new QDBusInterface("com.nokia.systemui.pin",
                                   "/com/nokia/systemui/pin",
                                   "com.nokia.systemui.pin.PinCodeQuery",
                                   QDBusConnection::systemBus(),
                                   this);
    /*
        connect(dbusPinIf, SIGNAL(PinQueryStateCompleted(SIMSecurity::PINQuery)),
                this, SLOT(pinQueryStateComplete(SIMSecurity::PINQuery, SIMError)));
        connect(dbusPinIf, SIGNAL(PinQueryEnabled(SIMSecurity::PINQuery)),
                this, SLOT(pinQueryStateComplete(SIMSecurity::PINQuery, SIMError)));
    */
}

void CallAndSim::setCallerIdSending(int value)
{
    qDebug() << Q_FUNC_INFO << value;
    // TODO:
}

void CallAndSim::setCallWaiting(bool enabled)
{
    qDebug() << Q_FUNC_INFO << enabled;

    if (enabled) {
        callWaiting->waitingActivate();
    } else {
        callWaiting->waitingCancel();
    }
}

void CallAndSim::setCallForwarding(bool enabled, QString number)
{
    qDebug() << Q_FUNC_INFO << enabled << number;

    if (enabled) {
        callForwarding->divertActivate(CallForwarding::Unconditional, number, FORWARDING_DELAY);
    } else {
        callForwarding->divertCancel(CallForwarding::CancelAll);
    }
}

void CallAndSim::setPinRequest(bool enabled)
{
    qDebug() << Q_FUNC_INFO << enabled;

    QList<QVariant> list;
    list << QVariant(enabled);
    dbusPinIf->call(QString("EnablePinQuery"), QVariant(enabled));
}

void CallAndSim::changePinCode()
{
    qDebug() << Q_FUNC_INFO;

    dbusPinIf->call(QDBus::NoBlock, QString("ChangePinCode"));
}

void CallAndSim::requestData(DcpCallAndSim::Data data)
{
    qDebug() << Q_FUNC_INFO << data;

    switch (data) {
    case CallerIdSendingData:
        // TODO
        break;
    case CallWaitingData:
        callWaiting->waitingCheck();
        break;
    case CallForwardingData:
        callForwarding->divertCheck(CallForwarding::Unconditional);
        break;
    case PinRequestData:
        dbusPinIf->call(QDBus::NoBlock, QString("PinQueryState"), QString("PIN"));
        break;
    case AllData:
        callWaiting->waitingCheck();
        callForwarding->divertCheck(CallForwarding::Unconditional);
        dbusPinIf->call(QDBus::NoBlock, QString("PinQueryState"), QString("PIN"));
        // TODO: CallerId sending
        // TODO: Queuing needed also?
        break;
    }
}

void CallAndSim::waitingActivateComplete(CallWaiting::WaitingError error)
{
    qDebug() << Q_FUNC_INFO << error;

    if (error != CallWaiting::NoError) {
        // TODO: display error note
        emit requestFailed(DcpCallAndSim::CallWaitingData);
    }
}

void CallAndSim::waitingCancelComplete(CallWaiting::WaitingError error)
{
    qDebug() << Q_FUNC_INFO << error;

    if (error != CallWaiting::NoError) {
        // TODO: display error note
        emit requestFailed(DcpCallAndSim::CallWaitingData);
    }

    emit callWaitingComplete(false);
}

void CallAndSim::waitingCheckComplete(bool active, CallWaiting::WaitingError error)
{
    qDebug() << Q_FUNC_INFO << active << error;

    if (error != CallWaiting::NoError) {
        // TODO: display error note
        emit requestFailed(DcpCallAndSim::CallWaitingData);
        return;
    }

    emit callWaitingComplete(active);
}

void CallAndSim::divertActivateComplete(CallForwarding::DivertError error)
{
    qDebug() << Q_FUNC_INFO << error;

    if (error != CallForwarding::NoError) {
        // TODO: display error note
        emit requestFailed(DcpCallAndSim::CallForwardingData);
    }
}

void CallAndSim::divertCancelComplete(CallForwarding::DivertError error)
{
    qDebug() << Q_FUNC_INFO << error;

    if (error != CallForwarding::NoError) {
        // TODO: display error note
        emit requestFailed(DcpCallAndSim::CallForwardingData);
    }
}

void CallAndSim::divertCheckComplete(bool active, QString number, CallForwarding::DivertError error)
{
    qDebug() << Q_FUNC_INFO << active << number << error;

    if (error != CallForwarding::NoError) {
        // TODO: display error note
        emit requestFailed(DcpCallAndSim::CallForwardingData);
        return;
    }

    emit callForwardingComplete(active, number);
}

void CallAndSim::pinQueryStateComplete(SIMSecurity::PINQuery state, SIMError error)
{
    qDebug() << Q_FUNC_INFO << state << error;

    pinQueryEnabled(error == SIMErrorNone ? state : SIMSecurity::UnknownState);
}

void CallAndSim::pinQueryEnabled(SIMSecurity::PINQuery queryState)
{
    qDebug() << Q_FUNC_INFO << queryState;

    if (queryState != SIMSecurity::UnknownState) {
        emit pinRequestComplete(queryState == SIMSecurity::Enabled ? true : false);
    } else {
        emit requestFailed(DcpCallAndSim::PinRequestData);
    }
}

void CallAndSim::networkStatusChanges(int status)
{
    (status);
    // TODO
}

void CallAndSim::DBusMessagingFailure()
{
    qDebug() << Q_FUNC_INFO;

    emit requestFailed(DcpCallAndSim::PinRequestData);
}
