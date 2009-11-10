#include "phonenetworkbusinesslogic.h"

#include <DuiLocale>
#include <QDebug>

/*
    TODO:
    1) What to do when toggling "Enable phone network"-feature?
    2) What to do when toggling "Enable roaming updates"-feature?
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
    const QString NoAccessText = trid("qtn_cell_error_no_access", "No access");
}

PhoneNetworkTechnology::PhoneNetworkTechnology(RadioAccess *radioAccess, QObject *parent) :
        QObject(parent),
        radioAccess(radioAccess),
        networkCell(NULL),
        timer(NULL)
{
    /* Technology changes depend to the RadioAccess's techonology and NetworkCell's services.
       The state changes of both of those have to be taken into account before we indicate any
       changes to end-user. There will be 500 ms interval which makes it possible that both the
       signals are catched before acting. */

    timer = new QTimer(this);
    timer->setInterval(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkTechnology()));
    connect(radioAccess, SIGNAL(technologyChanged(int)), timer, SLOT(start()));
    networkCell = new NetworkCell();        
    connect(networkCell, SIGNAL(servicesChanged(NetworkCell::Services)), timer, SLOT(start()));    
    updateTechnology();
}

PhoneNetworkTechnology::~PhoneNetworkTechnology()
{ 
    delete networkCell;
    networkCell = NULL;
}

void PhoneNetworkTechnology::checkTechnology()
{
    PhoneNetworkTechnology::Technology previousTechnology = currentTechnology();
    updateTechnology();
    if(previousTechnology != currentTechnology())
        emit technologyChanged(technology);
}

PhoneNetworkTechnology::Technology PhoneNetworkTechnology::currentTechnology()
{
    return technology;
}

void PhoneNetworkTechnology::updateTechnology()
{
    technology = PhoneNetworkTechnology::None;
    switch(radioAccess->technology()) {
        case RadioAccess::GSM:
            technology = PhoneNetworkTechnology::TwoG;
            if(networkCell->services() == NetworkCell::EGPRSSupport)
                technology = PhoneNetworkTechnology::TwoPointFiveG;
            break;
        case RadioAccess::UMTS:
            technology = PhoneNetworkTechnology::ThreeG;
            if(networkCell->services() == NetworkCell::HSDPASupport && radioAccess->state() == RadioAccess::AllocatedHSDPA)
                technology = PhoneNetworkTechnology::ThreePointFiveG;
            break;
        default:
            break;
    }
}

PhoneNetworkBusinessLogic::PhoneNetworkBusinessLogic(SystemUIGConf *systemUIGConf, QObject* parent) :
        QObject(parent),
        systemUIGConf(systemUIGConf),
        networkRegistration(NULL),
        radioAccess(NULL),
        technology(NULL)
{
    networkRegistration = new NetworkRegistration();
    radioAccess = new RadioAccess();    
    technology = new PhoneNetworkTechnology(radioAccess);
    connect(technology, SIGNAL(technologyChanged(PhoneNetworkTechnology::Technology)),
            this, SLOT(technologyChanged(PhoneNetworkTechnology::Technology)));
    networkModes.insert(RadioAccess::DualMode, DualText);
    networkModes.insert(RadioAccess::GSMMode, GSMText);
    networkModes.insert(RadioAccess::UMTSMode, ThreeGText);    
    networkSelectionValues.insert(NetworkRegistration::Automatic, AutomaticText);
    networkSelectionValues.insert(NetworkRegistration::Manual, ManualText);

    temp = true;
}

PhoneNetworkBusinessLogic::~PhoneNetworkBusinessLogic()
{
    delete networkRegistration;
    networkRegistration = NULL,
    delete radioAccess;
    radioAccess = NULL;
}

bool PhoneNetworkBusinessLogic::phoneNetworkEnabled()
{
    //TODO: use API to get the value for real
    return temp;
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
    radioAccess->setMode(networkModes.key(value));
}

void PhoneNetworkBusinessLogic::setNetworkSelection(const QString &value)
{    
    qDebug() << "PhoneNetworkBusinessLogic::setNetworkSelection(" << value << ")";    

    if(networkSelectionValues.key(value) == NetworkRegistration::Manual) {
        emit networkSelected(false);
        queryAvailableNetworks();
    }
    else { //Automatic        
        networkRegistration->selectOperator();
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
            connect(networkRegistration, SIGNAL(selectionCompleted(bool, const QString &)),
                    this, SLOT(selectNetworkCompleted(bool, const QString &)));
            networkRegistration->selectOperator(i.value().at(0), i.value().at(1));
            break;
        }
    }
}

void PhoneNetworkBusinessLogic::selectNetworkCompleted(bool success, const QString &reason)
{
    Q_UNUSED(reason);
    disconnect(networkRegistration, SIGNAL(selectionCompleted(bool, const QString &)),
               this, SLOT(selectNetworkCompleted(bool, const QString &)));

    if(success)
        emit networkSelected(true);            
    else {
        emit showNotification(NoAccessText);
        emit networkSelected(false);
    }        
}

void PhoneNetworkBusinessLogic::toggleNetwork(bool toggle)
{
    Q_UNUSED(toggle);
    //TODO: use API to set the value for real
    temp = toggle;
    if(!temp)
        emit networkIconChanged(QString("icon-s-network-0"));
}

void PhoneNetworkBusinessLogic::toggleRoaming(bool toggle)
{    
    systemUIGConf->setValue(SystemUIGConf::NetworkRoamingKey, QVariant(toggle));
    if(!toggle) {
        emit roamingUpdatesValueChanged(false);
        systemUIGConf->setValue(SystemUIGConf::NetworkRoamingUpdatesKey, QVariant(false));
    }
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

    int index = (radioAccess->mode() == RadioAccess::UnknownMode ? -1 : modes.indexOf(networkModes.value(radioAccess->mode())));
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

    int defaultIndex = values.indexOf(networkSelectionValues.value(NetworkRegistration::Automatic)); ///automatic is default
    int currentIndex = (networkRegistration->mode() == NetworkRegistration::UnknownMode ?
                        -1 : values.indexOf(networkSelectionValues.value(networkRegistration->mode())));
    emit networkSelectionValuesAvailable(defaultIndex, currentIndex, values);

    if(networkRegistration->mode() == NetworkRegistration::Manual) {
        emit networkSelected(false);
        queryAvailableNetworks();
    }
    else
        emit networkSelected(true);    
}

void PhoneNetworkBusinessLogic::queryAvailableNetworks()
{
    qDebug() << "PhoneNetworkBusinessLogic::queryAvailableNetworks()";
    connect(networkRegistration, SIGNAL(availableOperators(bool, const QList<AvailableOperator*> &, const QString &)),
        this, SLOT(availableNetworksReceived(bool, const QList<AvailableOperator*> &, const QString &)));
    networkRegistration->queryAvailableOperators();
}

void PhoneNetworkBusinessLogic::availableNetworksReceived(bool success, const QList<AvailableOperator*> &operators, const QString &reason)
{
    Q_UNUSED(reason);
    qDebug() << "PhoneNetworkBusinessLogic::availableNetworksReceived(" << operators.size() << ")";
    disconnect(networkRegistration, SIGNAL(availableOperators(bool, const QList<AvailableOperator*> &, const QString &)),
            this, SLOT(availableNetworksReceived(bool, const QList<AvailableOperator*> &, const QString &)));

    if(!success || operators.size() == 0) {
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
    emit availableNetworksAvailable(selectedNetwork, networks, true);
}

void PhoneNetworkBusinessLogic::technologyChanged(PhoneNetworkTechnology::Technology technology)
{
    emit networkIconChanged(mapTechnologyToIcon(technology));
}

QString PhoneNetworkBusinessLogic::networkIcon()
{
    return mapTechnologyToIcon(technology->currentTechnology());
}

QString PhoneNetworkBusinessLogic::mapTechnologyToIcon(PhoneNetworkTechnology::Technology technology)
{
    QString icon;
    switch(technology) {
        case PhoneNetworkTechnology::TwoG:
            icon = QString("icon-s-gsm");
            break;
        case PhoneNetworkTechnology::TwoPointFiveG:
            icon = QString("icon-s-25g");
            break;
        case PhoneNetworkTechnology::ThreeG:
            icon = QString("icon-s-3g");
            break;
        case PhoneNetworkTechnology::ThreePointFiveG:
            icon = QString("icon-s-35g");
            break;
        default:
            icon = QString("icon-s-network-0");
            break;
    }
    return icon;   
}

void PhoneNetworkBusinessLogic::networkAppletClosing()
{
    this->operators.clear();
}

QVariant PhoneNetworkBusinessLogic::GConfItemValue(SystemUIGConf::GConfKey key)
{
    return systemUIGConf->value(key);
}
