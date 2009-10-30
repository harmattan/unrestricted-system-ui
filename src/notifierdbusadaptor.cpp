#include <QDebug>
#include <QDBusConnection>

#include "notifierdbusadaptor.h"
#include "sysuid.h"

NotifierDBusAdaptor::NotifierDBusAdaptor() :
        QDBusAbstractAdaptor(Sysuid::sysuid())
{
}

NotifierDBusAdaptor::~NotifierDBusAdaptor()
{
}

QString NotifierDBusAdaptor::dbusInterfaceName()
{
    return QString("com.nokia.systemui.Notifier");
}

void NotifierDBusAdaptor::pinQueryCancel()
{
    emit pinQueryCanceled();
}

void NotifierDBusAdaptor::simLockRetry()
{
    emit doSimLockRetry();
}

void NotifierDBusAdaptor::showErrorNotification(QString msg)
{
    emit showNotification(msg, NotificationType::error);
}

void NotifierDBusAdaptor::showWarningNotification(QString msg)
{
    emit showNotification(msg, NotificationType::warning);
}

void NotifierDBusAdaptor::showInfoNotification(QString msg)
{
    emit showNotification(msg, NotificationType::info);
}
