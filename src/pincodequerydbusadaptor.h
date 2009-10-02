#ifndef PINCODEQUERYDBUSADAPTOR_H
#define PINCODEQUERYDBUSADAPTOR_H

#include <QDBusAbstractAdaptor>

/**
  * DBus adaptor class for PIN code query
  */

class PinCodeQueryDBusAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.PinCodeQuery")

public:
    PinCodeQueryDBusAdaptor();
    virtual ~PinCodeQueryDBusAdaptor();
    static QString dbusInterfaceName();

signals:
    void changePinCodeRequested();

public slots:
    Q_NOREPLY void changePinCode();
};

#endif // PINCODEQUERYDBUSADAPTOR_H
