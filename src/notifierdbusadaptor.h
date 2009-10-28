#ifndef NOTIFIERDBUSADAPTOR_H
#define NOTIFIERDBUSADAPTOR_H

#include "notifier.h"

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
    void showNotification(QString msg, Notifier::NotificationType type);

public slots:
    Q_NOREPLY void pinQueryCancel();
    Q_NOREPLY void simLockRetry();

    Q_NOREPLY void showErrorNotification(QString msg);
    Q_NOREPLY void showWarningNotification(QString msg);
    Q_NOREPLY void showInfoNotification(QString msg);
};

#endif // NOTIFIERDBUSADAPTOR_H
