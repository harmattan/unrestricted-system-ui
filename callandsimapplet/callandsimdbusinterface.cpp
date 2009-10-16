#include "callandsimdbusinterface.h"

#include <QDBusInterface>

CallAndSimDBusInterface::CallAndSimDBusInterface()
{
    dbusIf = new QDBusInterface("com.nokia.systemui", "/",
                                "com.nokia.systemui.callandsim",
                                QDBusConnection::sessionBus());
}

CallAndSimDBusInterface::~CallAndSimDBusInterface()
{
    delete dbusIf;
    dbusIf = NULL;
}

