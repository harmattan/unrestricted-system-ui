#include "phonenetworkbusinesslogic.h"

#include <DuiLocale>
#include <QDebug>

namespace {      
    const QString DualText = trid("qtn_cell_network_dual", "Dual");    
    const QString GSMText = trid("qtn_cell_network_gsm", "GSM");
    const QString ThreeGText = trid("qtn_cell_network_3g", "3G");
    const QString AutomaticText = trid("qtn_cell_automatic", "Automatic");
    const QString ManualText = trid("qtn_cell_manual", "Manual");
}

PhoneNetworkBusinessLogic::PhoneNetworkBusinessLogic(SystemUIGConf *systemUIGConf) :
        systemUIGConf(systemUIGConf), networkRegistration(NULL)
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
    systemUIGConf->value(SystemUIGConf::NetworkRoamingKey, QVariant(false));
}

bool PhoneNetworkBusinessLogic::roamingUpdatesEnabled()
{
    systemUIGConf->value(SystemUIGConf::NetworkRoamingUpdatesKey, QVariant(false));
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
        operators.clear(); //remove old operators
        emit availableNetworksAvailable(-1, QStringList(), false);
        emit networkSelected(true);
    }
}

void PhoneNetworkBusinessLogic::selectNetwork(const QString &value)
{    
    for(int i=0; i<operators.size(); ++i) {
        if(operators.at(i)->name() == value) {
            if(networkRegistration == NULL)
                networkRegistration = new NetworkRegistration();
            connect(networkRegistration, SIGNAL(selectionCompleted(bool, const QString &)),
                    this, SLOT(selectNetworkCompleted(bool, const QString &)));
            networkRegistration->selectOperator(operators.at(i)->mnc(), operators.at(i)->mcc());
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
    //TODO: use API to set the value for real or is there some gconf value to change?
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
    if(networkRegistration != NULL) {
        delete networkRegistration;
        networkRegistration = NULL;
    }
    if(!success) {
        //TODO: show note based on the reason        
        emit availableNetworksAvailable(-1, QStringList(), true);
        return;
    }

    QStringList networks;
    int selectedNetwork = -1;

    for(int i=0; i<operators.size(); ++i) {
        if(operators.at(i)->availability() != AvailableOperator::NotAvailable) {
            networks << operators.at(i)->name();
            this->operators << operators.at(i);
        }
        if(operators.at(i)->availability() == AvailableOperator::Current) {
            selectedNetwork = i;
            emit networkSelected(true);
        }
    }
    emit availableNetworksAvailable(selectedNetwork, networks, true);
}

void PhoneNetworkBusinessLogic::networkAppletClosing()
{
    operators.clear();
}

QVariant PhoneNetworkBusinessLogic::GConfItemValue(SystemUIGConf::GConfKey key)
{
    return systemUIGConf->value(key);
}
