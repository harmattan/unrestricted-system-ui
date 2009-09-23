#ifndef NOTIFIERDBUSADAPTOR_H
#define NOTIFIERDBUSADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <QPointer>

/**
  * DBus adaptor class for Notifier
  */
class NotifierDBusAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.nokia.systemui.Notifier")

public:
    NotifierDBusAdaptor();
    virtual ~NotifierDBusAdaptor();
    static QString dbusInterfaceName();

signals:
    void pinQueryCanceled();
    void doSimLockRetry();

public slots:
    Q_NOREPLY void pinQueryCancel();
    Q_NOREPLY void simLockRetry();

};

#endif // NOTIFIERDBUSADAPTOR_H
