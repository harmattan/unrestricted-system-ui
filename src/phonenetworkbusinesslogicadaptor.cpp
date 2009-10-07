#include "phonenetworkbusinesslogicadaptor.h"
#include "phonenetworkbusinesslogic.h"

#include <QDebug>

PhoneNetworkBusinessLogicAdaptor::PhoneNetworkBusinessLogicAdaptor(QObject *obj, PhoneNetworkBusinessLogic *networkLogic)
    : QDBusAbstractAdaptor(obj), networkLogic(networkLogic)
{    
    connect(networkLogic, SIGNAL(networkModeValuesAvailable(int, QStringList)), this, SIGNAL(networkModeValuesReceived(int, QStringList)));
    connect(networkLogic, SIGNAL(networkSelectionValuesAvailable(int, int, QStringList)), this, SIGNAL(networkSelectionValuesReceived(int, int, QStringList)));
    connect(networkLogic, SIGNAL(availableNetworksAvailable(int, QStringList, bool)), this, SIGNAL(availableNetworksReceived(int, QStringList, bool)));
    connect(networkLogic, SIGNAL(networkSelected(bool)), this, SIGNAL(networkSelected(bool)));
}

void PhoneNetworkBusinessLogicAdaptor::setPhoneNetworkValue(bool value)
{
    networkLogic->toggleNetwork(value);
}

void PhoneNetworkBusinessLogicAdaptor::setRoamingValue(bool value)
{
    networkLogic->toggleRoaming(value);
}

void PhoneNetworkBusinessLogicAdaptor::setRoamingUpdatesValue(bool value)
{
    networkLogic->toggleRoamingUpdates(value);
}

void PhoneNetworkBusinessLogicAdaptor::setNetworkModeValue(const QString &value)
{
    networkLogic->setNetworkMode(value);
}

void PhoneNetworkBusinessLogicAdaptor::setNetworkSelectionValue(const QString &value)
{
    networkLogic->setNetworkSelection(value);
}

void PhoneNetworkBusinessLogicAdaptor::setSelectedNetworkValue(const QString &value)
{
    networkLogic->selectNetwork(value);
}

void PhoneNetworkBusinessLogicAdaptor::networkAppletClosing()
{
    networkLogic->networkAppletClosing();
}

void PhoneNetworkBusinessLogicAdaptor::networkModeValues()
{
    return networkLogic->queryNetworkModes();
}

void PhoneNetworkBusinessLogicAdaptor::networkSelectionValues()
{
    return networkLogic->queryNetworkSelectionValues();
}

bool PhoneNetworkBusinessLogicAdaptor::phoneNetworkValue()
{
    return networkLogic->phoneNetworkEnabled();
}

bool PhoneNetworkBusinessLogicAdaptor::roamingValue()
{
    return networkLogic->roamingEnabled();
}

bool PhoneNetworkBusinessLogicAdaptor::roamingUpdatesValue()
{
    return networkLogic->roamingUpdatesEnabled();
}
