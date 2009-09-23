#include "networkdbusinterface.h"
#include <QtDBus>
#include <QDebug>

NetworkDBusInterface::NetworkDBusInterface()
{
    dbusIf = new QDBusInterface("com.nokia.systemui", "/", 
                                "com.nokia.systemui.network",
				QDBusConnection::sessionBus());
}

NetworkDBusInterface::~NetworkDBusInterface()
{
    delete dbusIf;
    dbusIf = NULL;
}

void NetworkDBusInterface::someValueRequired()
{
    qDebug() << "NetworkDBusInterface::someValueRequired()";
    QList<QVariant> list;    
    dbusIf->callWithCallback(QString("someValue"), list, this, SIGNAL(someValueReceived(QStringList)), SLOT(DBusMessagingFailure()));
}

void NetworkDBusInterface::setSomeValue(const QString &value)
{
    qDebug() << "DisplayDBusInterface::setSomeValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setSomeValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void NetworkDBusInterface::valueSet()
{
    qDebug() << "DisplayDBusInterface::valueSet()";
}

void NetworkDBusInterface::DBusMessagingFailure()
{
    qDebug() << "DisplayDBusInterface::DBusMessagingFailure()";
}
