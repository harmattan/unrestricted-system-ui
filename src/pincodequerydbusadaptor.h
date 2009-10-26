#ifndef PINCODEQUERYDBUSADAPTOR_H
#define PINCODEQUERYDBUSADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <SIM>

/**
  * DBus adaptor class for PIN code query
  */

using namespace Cellular;

class PinCodeQueryDBusAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.PinCodeQuery")

public:
    PinCodeQueryDBusAdaptor();
    virtual ~PinCodeQueryDBusAdaptor();
    static QString dbusInterfaceName();
    void pinQueryEnabledResponse(SIMSecurity::PINQuery queryState);

signals:
    void changePinCodeRequested();
    void enablePinQueryRequested(bool enabled);
    void pinQueryEnabled(SIMSecurity::PINQuery queryState);

public slots:
    Q_NOREPLY void changePinCode();
    Q_NOREPLY void enablePinQuery(bool enable);

};

#endif // PINCODEQUERYDBUSADAPTOR_H
