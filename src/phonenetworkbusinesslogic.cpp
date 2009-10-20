#include "phonenetworkbusinesslogic.h"

#include <DuiLocale>
#include <QDebug>

/*
    TODO:
    1) What to do when toggling "Enable phone network"-feature?
    2) "Enable roaming updates"-feature. Is it wanted after all? Remove if not.
    3) When requesting available networks, an error can occur. Do we show note in this case?
    4) When selecting a network, an error can occur. Do we show note in this case?
    5) Can we trust that the network has unique name?

*/

namespace {      
    const QString DualText = trid("qtn_cell_network_dual", "Dual");    
    const QString GSMText = trid("qtn_cell_network_gsm", "GSM");
    const QString ThreeGText = trid("qtn_cell_network_3g", "3G");
    const QString AutomaticText = trid("qtn_cell_automatic", "Automatic");
    const QString ManualText = trid("qtn_cell_manual", "Manual");
}

PhoneNetworkBusinessLogic::PhoneNetworkBusinessLogic(SystemUIGConf *systemUIGConf) :
        systemUIGConf(systemUIGConf),
        networkRegistration(NULL)
{
    networkModes.insert(RadioAccess::DualMode, DualText);
    networkModes.insert(RadioAccess::GSMMode, GSMText);
    networkModes.insert(RadioAccess::UMTSMode, ThreeGText);    
    networkSelectionValues.insert(NetworkRegistration::Automatic, AutomaticText);
    networkSelectionValues.insert(NetworkRegistration::Manual, ManualText);
}

PhoneNetworkBusinessLogic::~PhoneNetworkBusinessLogic()
{
}

bool PhoneNetworkBusinessLogic::phoneNetworkEnabled()
{
    //TODO: use API to get the value for real
    return true;
}

bool PhoneNetworkBusinessLogic::roamingEnabled()
{
    return systemUIGConf->value(SystemUIGConf::NetworkRoamingKey, QVariant(false)).toBool();
}

bool PhoneNetworkBusinessLogic::roamingUpdatesEnabled()
{
    return systemUIGConf->value(SystemUIGConf::NetworkRoamingUpdatesKey, QVariant(false)).toBool();
}

void PhoneNetworkBusinessLogic::setNetworkMode(const QString &value)
{
    qDebug() << "PhoneNetworkBusinessLogic::setNetworkMode(" << value << ")";
    RadioAccess ra;
    ra.setMode(networkModes.key(value));    
}

void PhoneNetworkBusinessLogic::setNetworkSelection(const QString &value)
{    
    qDebug() << "PhoneNetworkBusinessLogic::setNetworkSelection(" << value << ")";    

    if(networkSelectionValues.key(value) == NetworkRegistration::Manual) {
        emit networkSelected(false);
        queryAvailableNetworks();
    }
    else { //Automatic
        NetworkRegistration nr;
        nr.selectOperator();
        this->operators.clear(); //remove old operators
        emit availableNetworksAvailable(-1, QStringList(), false);
        emit networkSelected(true);
    }
}

void PhoneNetworkBusinessLogic::selectNetwork(const QString &value)
{    
    QHashIterator<QString, QStringList> i(operators);
    while (i.hasNext()) {
        i.next();
        if(value == i.key()) {
            if(networkRegistration == NULL)
                networkRegistration = new NetworkRegistration();
            connect(networkRegistration, SIGNAL(selectionCompleted(bool, const QString &)),
                    this, SLOT(selectNetworkCompleted(bool, const QString &)));
            networkRegistration->selectOperator(i.value().at(0), i.value().at(1));
            break;
        }
    }
}

void PhoneNetworkBusinessLogic::selectNetworkCompleted(bool success, const QString &reason)
{
    if(networkRegistration != NULL) {
        delete networkRegistration;
        networkRegistration = NULL;
    }
    if(success) {
        qDebug() << "Selection completed succesfully";
        emit networkSelected(true);
        //TODO: show a note
    }
    else {
        qDebug() << "Selection failed, reason: " << reason;
        emit networkSelected(false);
        //TODO: show a note
    }
}

void PhoneNetworkBusinessLogic::toggleNetwork(bool toggle)
{
    //TODO: use API to set the value for real
}

void PhoneNetworkBusinessLogic::toggleRoaming(bool toggle)
{    
    systemUIGConf->setValue(SystemUIGConf::NetworkRoamingKey, QVariant(toggle));
}

void PhoneNetworkBusinessLogic::toggleRoamingUpdates(bool toggle)
{
    //TODO: use API to set the value for real or is there some gconf value to change?
    systemUIGConf->setValue(SystemUIGConf::NetworkRoamingUpdatesKey, QVariant(toggle));
}

void PhoneNetworkBusinessLogic::queryNetworkModes()
{    
    QStringList modes;
    QHash<RadioAccess::Mode, QString>::iterator i;
    for (i = networkModes.begin(); i != networkModes.end(); ++i)
        modes << i.value();    

    RadioAccess ra;        
    int index = (ra.mode() == RadioAccess::UnknownMode ? -1 : modes.indexOf(networkModes.value(ra.mode())));
    qDebug() << "PhoneNetworkBusinessLogic::queryNetworkModes( " << modes.size() << ")";
    qDebug() << "\n\n\nJAKE " << ra.mode() << "\n\n\n";
    emit networkModeValuesAvailable(index, modes);
}

void PhoneNetworkBusinessLogic::queryNetworkSelectionValues()
{
    QStringList values;
    QHash<NetworkRegistration::Mode, QString>::iterator i;
    for (i = networkSelectionValues.begin(); i != networkSelectionValues.end(); ++i)
        values << i.value();
    values.sort();

    NetworkRegistration nr;
    int defaultIndex = values.indexOf(networkSelectionValues.value(NetworkRegistration::Automatic)); ///automatic is default
    int currentIndex = (nr.mode() == NetworkRegistration::UnknownMode ? -1 : values.indexOf(networkSelectionValues.value(nr.mode())));
    emit networkSelectionValuesAvailable(defaultIndex, currentIndex, values);

    if(nr.mode() == NetworkRegistration::Manual) {
        emit networkSelected(false);
        queryAvailableNetworks();
    }
    else
        emit networkSelected(true);    
}

void PhoneNetworkBusinessLogic::queryAvailableNetworks()
{
    qDebug() << "PhoneNetworkBusinessLogic::queryAvailableNetworks()";
    if(networkRegistration == NULL) {
        networkRegistration = new NetworkRegistration();
        connect(networkRegistration, SIGNAL(availableOperators(bool, const QList<AvailableOperator*> &, const QString &)),
            this, SLOT(availableNetworksReceived(bool, const QList<AvailableOperator*> &, const QString &)));        
        networkRegistration->queryAvailableOperators();
    }

    /*
    QStringList networks;
    networks << QString("Radiolinja") << QString("Tele") << QString("Telia");
    emit availableNetworksAvailable(0, networks);    
    */
}

void PhoneNetworkBusinessLogic::availableNetworksReceived(bool success, const QList<AvailableOperator*> &operators, const QString &reason)
{
    qDebug() << "PhoneNetworkBusinessLogic::availableNetworksReceived(" << operators.size() << ")";    
    if(!success) {
        //TODO: show note based on the reason        
        emit availableNetworksAvailable(-1, QStringList(), true);
        return;
    }

    QStringList networks;
    int selectedNetwork = -1;
    this->operators.clear(); //just to be sure

    for(int i=0; i<operators.size(); ++i) {
        if(operators.at(i)->availability() != AvailableOperator::NotAvailable) {
            networks << operators.at(i)->name();
            this->operators.insert(operators.at(i)->name(), QStringList() << operators.at(i)->mnc() << operators.at(i)->mcc());
        }
        if(operators.at(i)->availability() == AvailableOperator::Current) {
            selectedNetwork = i;
            emit networkSelected(true);
        }
    }
    if(networkRegistration != NULL) {
        delete networkRegistration;
        networkRegistration = NULL;
    }

    emit availableNetworksAvailable(selectedNetwork, networks, true);
}

void PhoneNetworkBusinessLogic::networkAppletClosing()
{
    this->operators.clear();
}

QVariant PhoneNetworkBusinessLogic::GConfItemValue(SystemUIGConf::GConfKey key)
{
    return systemUIGConf->value(key);
}
