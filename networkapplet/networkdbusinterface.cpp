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

void NetworkDBusInterface::availableNetworksRequired()
{
    qDebug() << "NetworkDBusInterface::availableNetworksRequired()";
    //QList<QVariant> list;
    //dbusIf->callWithCallback(QString("availableNetworks"), list, this, SIGNAL(availableNetworksReceived(QStringList)), SLOT(DBusMessagingFailure()));
    QStringList test;
    test << "jake" << "kake" << "make";
    emit availableNetworksReceived(test);
}

void NetworkDBusInterface::setPhoneNetworkValue(bool value)
{
    qDebug() << "NetworkDBusInterface::setPhoneNetworkValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setPhoneNetworkValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void NetworkDBusInterface::setEnableRoamingValue(bool value)
{
    qDebug() << "NetworkDBusInterface::setEnableRoamingValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setEnableRoamingValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void NetworkDBusInterface::setEnableRoamingUpdatesValue(bool value)
{
    qDebug() << "NetworkDBusInterface::setEnableRoamingUpdatesValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setEnableRoamingUpdatesValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void NetworkDBusInterface::setNetworkModeValue(const QString &value)
{
    qDebug() << "NetworkDBusInterface::setNetworkModeValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setNetworkModeValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void NetworkDBusInterface::setNetworkSelectionValue(const QString &value)
{
    qDebug() << "NetworkDBusInterface::setNetworkSelectionValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    //dbusIf->callWithCallback(QString("setNetworkSelectionValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void NetworkDBusInterface::valueSet()
{
    qDebug() << "DisplayDBusInterface::valueSet()";
}

void NetworkDBusInterface::DBusMessagingFailure()
{
    qDebug() << "DisplayDBusInterface::DBusMessagingFailure()";
}
