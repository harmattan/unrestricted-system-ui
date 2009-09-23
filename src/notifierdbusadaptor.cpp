#include <QDebug>
#include <QDBusConnection>

#include "notifierdbusadaptor.h"
#include "notifier.h"
#include "sysuid.h"

NotifierDBusAdaptor::NotifierDBusAdaptor() :
        QDBusAbstractAdaptor(Sysuid::dbusObject())
{
}

NotifierDBusAdaptor::~NotifierDBusAdaptor()
{
}

void NotifierDBusAdaptor::pinQueryCancel()
{
    emit pinQueryCanceled();
}

void NotifierDBusAdaptor::simLockRetry()
{
    emit doSimLockRetry();
}

QString NotifierDBusAdaptor::dbusInterfaceName()
{
    return QString("com.nokia.systemui.Notifier");
}
