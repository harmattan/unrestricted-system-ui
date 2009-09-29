#include "phonenetworkbusinesslogic.h"

#include <DuiLocale>
#include <QStringList>

PhoneNetworkBusinessLogic::PhoneNetworkBusinessLogic(SystemUIGConf *systemUIGConf) :
        systemUIGConf(systemUIGConf)
{
}

PhoneNetworkBusinessLogic::~PhoneNetworkBusinessLogic()
{
}

void PhoneNetworkBusinessLogic::setNetworkMode(const QString &value)
{
    systemUIGConf->setValue(SystemUIGConf::NetworkModeKey, QVariant(value));
}

void PhoneNetworkBusinessLogic::setNetworkSelection(const QString &value)
{
    systemUIGConf->setValue(SystemUIGConf::NetworkSelectionKey, QVariant(value));
}

void PhoneNetworkBusinessLogic::toggleNetwork(bool toggle)
{
    //TODO: use API to set the value for real
}

void PhoneNetworkBusinessLogic::toggleRoaming(bool toggle)
{
    //TODO: use API to set the value for real or is there some gconf value to change?
    systemUIGConf->setValue(SystemUIGConf::NetworkRoamingKey, QVariant(toggle));
}

void PhoneNetworkBusinessLogic::toggleRoamingUpdates(bool toggle)
{
    //TODO: use API to set the value for real or is there some gconf value to change?
    systemUIGConf->setValue(SystemUIGConf::NetworkRoamingUpdatesKey, QVariant(toggle));
}

void PhoneNetworkBusinessLogic::selectNetwork(QString value)
{
    //TODO: use API to set the network for real
    systemUIGConf->setValue(SystemUIGConf::NetworkSelectedNetworkKey, QVariant(value));
}

QStringList PhoneNetworkBusinessLogic::networkModes()
{
    QStringList networkModes;
    networkModes << trid("qtn_cell_network_dual", "Dual") << trid("qtn_cell_network_3g", "3G") << trid("qtn_cell_network_gsm", "GSM");
    return networkModes;
}

QStringList PhoneNetworkBusinessLogic::availableNetworks()
{
    //TODO: Change the hard coded values
    QStringList networks;
    networks << QString("Radiolinja") << QString("Tele") << QString("Telia");
    return networks;
}

QVariant PhoneNetworkBusinessLogic::GConfItemValue(SystemUIGConf::GConfKey key)
{
    return systemUIGConf->value(key);
}
