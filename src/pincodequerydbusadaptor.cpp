#include "pincodequerydbusadaptor.h"
#include "pincodequerybusinesslogic.h"
#include "sysuid.h"

#include <QDebug>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QVariant>

PinCodeQueryDBusAdaptor::PinCodeQueryDBusAdaptor(PinCodeQueryBusinessLogic* logic) :
        QDBusAbstractAdaptor(logic),
        logic(logic)
{
    QDBusConnection busSystem = QDBusConnection::systemBus();
    if(!busSystem.registerObject(dbusObjectName(), logic)) {
        qDebug() << Q_FUNC_INFO << "failed to register busSystem objects";
    } else if(!busSystem.registerService(dbusServiceName())) {
        qDebug() << Q_FUNC_INFO << "failed to register busSystem service";
    }
}

PinCodeQueryDBusAdaptor::~PinCodeQueryDBusAdaptor()
{
}

void  PinCodeQueryDBusAdaptor::ChangePinCode()
{
    logic->changePinCode();
}

void PinCodeQueryDBusAdaptor::PinQueryState(const QString &pinType)
{
    Q_UNUSED(pinType);
    logic->pinQueryState(SIMSecurity::PIN);
}

void PinCodeQueryDBusAdaptor::pinQueryStateCompletedResponse(SIMSecurity::PINQuery state, SIMError error)
{
    qDebug() << Q_FUNC_INFO;
    QDBusMessage msg = QDBusMessage::createSignal(dbusObjectName(), dbusInterfaceName(), "PinQueryStateCompleted");
    msg << state << error;
    QDBusConnection::systemBus().send(msg);
}

void  PinCodeQueryDBusAdaptor::EnablePinQuery(bool enable)
{
    logic->enablePinQueryRequested(enable);
}

void PinCodeQueryDBusAdaptor::pinQueryEnabledResponse(SIMSecurity::PINQuery queryState)
{
    qDebug() << Q_FUNC_INFO;
    QDBusMessage msg = QDBusMessage::createSignal(dbusObjectName(), dbusInterfaceName(), "PinQueryEnabled");
    msg << queryState;
    QDBusConnection::systemBus().send(msg);
}

bool PinCodeQueryDBusAdaptor::LaunchPinQuery(QString const & pinType)
{
    Q_UNUSED(pinType); // pinType is ignored
    return logic->launchPinQuery(SIMSecurity::PIN);
}

void PinCodeQueryDBusAdaptor::pinQueryDoneResponse(bool queryOk)
{
    qDebug() << Q_FUNC_INFO << "ok:" << queryOk;

    QDBusMessage msg = QDBusMessage::createSignal(dbusObjectName(), dbusInterfaceName(), "pinQueryDone");
    msg << queryOk;
    QDBusConnection::systemBus().send(msg);
}

