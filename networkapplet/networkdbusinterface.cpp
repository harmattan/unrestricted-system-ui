#include "networkdbusinterface.h"
#include <QtDBus>
#include <QDebug>
#include <QList>
#include <QVariant>


NetworkDBusInterface::NetworkDBusInterface()
{
    dbusIf = new QDBusInterface("com.nokia.systemui", "/", 
                                "com.nokia.systemui.network",
                                QDBusConnection::sessionBus());

    connect(dbusIf, SIGNAL(networkModeValuesReceived(int, QStringList)), this, SIGNAL(networkModeValuesReceived(int, QStringList)));
    connect(dbusIf, SIGNAL(networkSelectionValuesReceived(int, QStringList)), this, SIGNAL(networkSelectionValuesReceived(int, QStringList)));
    connect(dbusIf, SIGNAL(availableNetworksReceived(int, QStringList)), this, SIGNAL(availableNetworksReceived(int, QStringList)));    
}

NetworkDBusInterface::~NetworkDBusInterface()
{
    delete dbusIf;
    dbusIf = NULL;
}

void NetworkDBusInterface::phoneNetworkValueRequired()
{
    qDebug() << "NetworkDBusInterface::phoneNetworkValueRequired()";    
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("phoneNetworkValue"), list, this, SIGNAL(phoneNetworkValueReceived(bool)), SLOT(DBusMessagingFailure()));
}

void NetworkDBusInterface::roamingValueRequired()
{
    qDebug() << "NetworkDBusInterface::roamingValueRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("roamingValue"), list, this, SIGNAL(roamingValueReceived(bool)), SLOT(DBusMessagingFailure()));
}

void NetworkDBusInterface::roamingUpdatesValueRequired()
{
    qDebug() << "NetworkDBusInterface::roamingUpdatesValueRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("roamingUpdatesValue"), list, this, SIGNAL(roamingUpdatesValueReceived(bool)), SLOT(DBusMessagingFailure()));
}

void NetworkDBusInterface::networkModeValuesRequired()
{
    qDebug() << "NetworkDBusInterface::networkModeValuesRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("networkModeValues"), list, this, SLOT(querySent()), SLOT(DBusMessagingFailure()));
}

void NetworkDBusInterface::networkSelectionValuesRequired()
{
    qDebug() << "NetworkDBusInterface::networkSelectionValuesRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("networkSelectionValues"), list, this, SLOT(querySent()), SLOT(DBusMessagingFailure()));
}

void NetworkDBusInterface::availableNetworksRequired()
{
    qDebug() << "NetworkDBusInterface::availableNetworksRequired()";
    QList<QVariant> list;
    dbusIf->callWithCallback(QString("availableNetworks"), list, this, SLOT(querySent()), SLOT(DBusMessagingFailure()));
}

void NetworkDBusInterface::setPhoneNetworkValue(bool value)
{
    qDebug() << "NetworkDBusInterface::setPhoneNetworkValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setPhoneNetworkValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void NetworkDBusInterface::setRoamingValue(bool value)
{
    qDebug() << "NetworkDBusInterface::setRoamingValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setRoamingValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void NetworkDBusInterface::setRoamingUpdatesValue(bool value)
{
    qDebug() << "NetworkDBusInterface::setRoamingUpdatesValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setRoamingUpdatesValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
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
    dbusIf->callWithCallback(QString("setNetworkSelectionValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void NetworkDBusInterface::setSelectedNetworkValue(const QString &value)
{
    qDebug() << "NetworkDBusInterface::setSelectedNetworkValue(" << value << ")";
    QList<QVariant> list;
    list << QVariant(value);
    dbusIf->callWithCallback(QString("setSelectedNetworkValue"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void NetworkDBusInterface::networkAppletClosing()
{
    qDebug() << "NetworkDBusInterface::networkAppletClosing()";
    QList<QVariant> list;    
    dbusIf->callWithCallback(QString("networkAppletClosing"), list, this, SLOT(valueSet()), SLOT(DBusMessagingFailure()));
}

void NetworkDBusInterface::valueSet()
{
    qDebug() << "DisplayDBusInterface::valueSet()";
}

void NetworkDBusInterface::querySent()
{
    qDebug() << "DisplayDBusInterface::querySent()";
}

void NetworkDBusInterface::DBusMessagingFailure()
{
    qDebug() << "DisplayDBusInterface::DBusMessagingFailure()";
}
