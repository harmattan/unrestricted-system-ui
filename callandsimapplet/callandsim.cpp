#include "callandsim.h"

using namespace DcpCallAndSim;

CallAndSim::CallAndSim(QObject* parent) :
        QObject(parent)
{
    simSecurity = new SIMSecurity(this);
//    networkRegistration = new NetworkRegistration(this);


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
//        emit requestFailed(DcpCallAndSim::PinRequestData);
    }
}

void CallAndSim::networkStatusChanges(int status)
{
    Q_UNUSED(status);
    // TODO
}

void CallAndSim::DBusMessagingFailure()
{
    qDebug() << Q_FUNC_INFO;

    //emit requestFailed(DcpCallAndSim::PinRequestData);
}
