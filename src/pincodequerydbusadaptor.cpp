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
    if(!busSystem.registerService("com.nokia.systemui.pin")) {
        qDebug() << Q_FUNC_INFO << "failed to register busSystem service";
    }else if(!busSystem.registerObject("/", logic)) {
        qDebug() << Q_FUNC_INFO << "failed to register busSystem objects";
    }
}

PinCodeQueryDBusAdaptor::~PinCodeQueryDBusAdaptor()
{
}

QString PinCodeQueryDBusAdaptor::dbusInterfaceName()
{
    return QString("com.nokia.systemui.pin.PinCodeQuery");
}

void  PinCodeQueryDBusAdaptor::changePinCode()
{
    logic->changePinCode();
}

void  PinCodeQueryDBusAdaptor::enablePinQuery(bool enable)
{
    logic->enablePinQueryRequested(enable);
}

void PinCodeQueryDBusAdaptor::pinQueryEnabledResponse(SIMSecurity::PINQuery queryState)
{
    emit pinQueryEnabled(queryState);
}

bool PinCodeQueryDBusAdaptor::launchPinQuery(int pinType)
{
    return logic->launchPinQuery((SIMSecurity::PINType)pinType);
}

void PinCodeQueryDBusAdaptor::pinQueryDoneResponse(SIM::SIMStatus currentSimStatus, bool queryOk)
{
    qDebug() << Q_FUNC_INFO << "ok:" << queryOk;

    QDBusMessage msg = QDBusMessage::createSignal("/", dbusInterfaceName(), "pinQueryDone");
    msg << QVariant(currentSimStatus) << QVariant(queryOk);
    QDBusConnection::systemBus().send(msg);
}

