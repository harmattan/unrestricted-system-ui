#include "networkdbusinterface.h"
#include <QtDBus>
#include <QDebug>
#include <QList>
#include <QVariant>


NetworkDBusInterface::NetworkDBusInterface()
{
    dbusIf = new QDBusInterface("com.nokia.systemui", "/", 
                                "com.nokia.systemui.Notifier",
                                QDBusConnection::sessionBus());
}

NetworkDBusInterface::~NetworkDBusInterface()
{
    delete dbusIf;
    dbusIf = NULL;
}

void NetworkDBusInterface::showNotification(const QString &text)
{    
    QList<QVariant> list;
    dbusIf->call(QDBus::NoBlock, QString("showErrorNotification"), text);
}
