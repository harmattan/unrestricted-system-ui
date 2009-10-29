#include "pincodequerydbusadaptor.h"
#include "sysuid.h"

PinCodeQueryDBusAdaptor::PinCodeQueryDBusAdaptor() :
        QDBusAbstractAdaptor(Sysuid::sysuid())
{
}

PinCodeQueryDBusAdaptor::~PinCodeQueryDBusAdaptor()
{
}

QString PinCodeQueryDBusAdaptor::dbusInterfaceName()
{
    return QString("com.nokia.systemui.PinCodeQuery");
}

void  PinCodeQueryDBusAdaptor::changePinCode()
{
    emit changePinCodeRequested();
}

void  PinCodeQueryDBusAdaptor::enablePinQuery(bool enable)
{
    emit enablePinQueryRequested(enable);
}

void PinCodeQueryDBusAdaptor::pinQueryEnabledResponse(SIMSecurity::PINQuery queryState)
{
    emit pinQueryEnabled(queryState);
}

bool PinCodeQueryDBusAdaptor::launchPinQuery(SIMSecurity::PINType pinType)
{
    // STUB: actual functionality missing
    Q_UNUSED(pinType);
    emit pinQueryDone(SIM::UnknownStatus, false);
    return false;
}
