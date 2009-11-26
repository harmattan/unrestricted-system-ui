#include "networkbusinesslogic.h"
#include "networktranslation.h"

#include <DuiNotification>
#include <QDebug>


/*
    TODO:
    1) What to do when toggling "Enable phone network"-feature?
    2) What to do when toggling "Enable roaming updates"-feature?
    3) When user tries to toggle phone network on and there is no SIM card inserted, a note should be displayed

*/

NetworkBusinessLogic::NetworkBusinessLogic(QObject* parent) :
        QObject(parent),        
        networkRegistration(NULL),
        radioAccess(NULL),
        networkOperator(NULL),        
        technology(NULL)
{
    qDebug() << Q_FUNC_INFO;

    //network registration
    networkRegistration = new NetworkRegistration();
    radioAccess = new RadioAccess();

    // network operator
    networkOperator = new NetworkOperator();
    connect(networkOperator, SIGNAL(nameChanged(QString)), this, SIGNAL(networkOperatorChanged(QString)));

    // network technology
    technology = new NetworkTechnology(radioAccess);    
    connect(technology, SIGNAL(technologyChanged(NetworkTechnology::Technology)),
            this, SLOT(technologyChanged(NetworkTechnology::Technology)));

    // init mode and selection values
    modes.insert(RadioAccess::DualMode, DcpNetwork::DualText);
    modes.insert(RadioAccess::GSMMode, DcpNetwork::GSMText);
    modes.insert(RadioAccess::UMTSMode, DcpNetwork::ThreeGText);
    selectionValues.insert(NetworkRegistration::Automatic, DcpNetwork::AutomaticText);
    selectionValues.insert(NetworkRegistration::Manual, DcpNetwork::ManualText);

    tempNetworkToggle = true;
}

NetworkBusinessLogic::~NetworkBusinessLogic()
{
    qDebug() << Q_FUNC_INFO;
    delete networkRegistration;
    networkRegistration = NULL,
    delete radioAccess;
    radioAccess = NULL;
    delete networkOperator;
    networkOperator = NULL;
    delete technology;
    technology = NULL;    
}

bool NetworkBusinessLogic::networkEnabled()
{
    qDebug() << Q_FUNC_INFO;
    //TODO: use API to get the value for real
    return tempNetworkToggle;
}

bool NetworkBusinessLogic::roamingEnabled()
{
    qDebug() << Q_FUNC_INFO;
    return tempRoamingToggle;
    //return systemUIGConf->value(SystemUIGConf::NetworkRoamingKey, QVariant(false)).toBool();
}

bool NetworkBusinessLogic::roamingUpdatesEnabled()
{
    qDebug() << Q_FUNC_INFO;
    return tempRoamingUpdatesToggle;
    //return systemUIGConf->value(SystemUIGConf::NetworkRoamingUpdatesKey, QVariant(false)).toBool();
}

QStringList NetworkBusinessLogic::networkModes()
{
    qDebug() << Q_FUNC_INFO;
    return modes.values();
}

QString NetworkBusinessLogic::selectedNetworkMode()
{
    qDebug() << Q_FUNC_INFO;
    return (radioAccess->mode() == RadioAccess::UnknownMode ? QString("") : modes.value(radioAccess->mode()));
}

QStringList NetworkBusinessLogic::networkSelectionValues()
{
    qDebug() << Q_FUNC_INFO;
    return selectionValues.values();
}

QString NetworkBusinessLogic::selectedNetworkSelectionValue()
{
    qDebug() << Q_FUNC_INFO;
    NetworkRegistration::Mode mode = networkRegistration->mode();
    QString modeStr;
    switch(mode) {
        case NetworkRegistration::Automatic:
            modeStr = selectionValues.value(mode);
            break;
        case NetworkRegistration::Manual:
            modeStr = selectionValues.value(mode);
            queryAvailableOperators();
            break;
        default:
            break;
    }
    return modeStr;
}

QString NetworkBusinessLogic::defaultNetworkSelectionValue()
{
    qDebug() << Q_FUNC_INFO;
    return selectionValues.value(NetworkRegistration::Automatic);
}

void NetworkBusinessLogic::toggleNetwork(bool toggle)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(toggle);
    //TODO: use API to set the value for real
    tempNetworkToggle = toggle;
    if(!tempNetworkToggle)
        emit networkIconChanged(QString("icon-s-network-0"));


    //DcpNetwork::NoSIMText

}

void NetworkBusinessLogic::setNetworkMode(const QString &value)
{
    qDebug() << Q_FUNC_INFO;
    radioAccess->setMode(modes.key(value));
}

void NetworkBusinessLogic::setNetworkSelectionValue(const QString &value)
{
    qDebug() << Q_FUNC_INFO;
    if(selectionValues.key(value) == NetworkRegistration::Manual)
        queryAvailableOperators();
    else { //Automatic
        networkRegistration->selectOperator();
        this->operators.clear(); //remove old operators
        emit availableNetworkOperators(-1, QStringList(), false);        
    }
}

void NetworkBusinessLogic::selectOperator(int index)
{    

    qDebug() << Q_FUNC_INFO;    
    QMapIterator<QString, QStringList> iter(operators);    
    while (iter.hasNext()) {
        iter.next();        
        if(--index < 0)
            break;
    }

    connect(networkRegistration, SIGNAL(selectionCompleted(bool, const QString &)),
        this, SLOT(selectOperatorCompleted(bool, const QString &)));    
    networkRegistration->selectOperator(iter.value().at(0), iter.value().at(1));    
}

void NetworkBusinessLogic::selectOperatorCompleted(bool success, const QString &reason)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(reason);
    disconnect(networkRegistration, SIGNAL(selectionCompleted(bool, const QString &)),
               this, SLOT(selectOperatorCompleted(bool, const QString &)));   

    if(!success)
        DuiNotification("", "", DcpNetwork::NoAccessText);    
}

void NetworkBusinessLogic::toggleRoaming(bool toggle)
{
    qDebug() << Q_FUNC_INFO;
    //TODO: change the temp
    tempRoamingToggle = toggle;
    if(!toggle) {
        tempRoamingUpdatesToggle = false;
        emit roamingUpdatesValueChanged(false);        
    }
}

void NetworkBusinessLogic::toggleRoamingUpdates(bool toggle)
{
    qDebug() << Q_FUNC_INFO;
    //TODO: change the temp
    tempRoamingUpdatesToggle = toggle;
}

void NetworkBusinessLogic::queryAvailableOperators()
{
    qDebug() << Q_FUNC_INFO;
    connect(networkRegistration, SIGNAL(availableOperators(bool, const QList<AvailableOperator*> &, const QString &)),
        this, SLOT(availableOperatorsReceived(bool, const QList<AvailableOperator*> &, const QString &)));
    networkRegistration->queryAvailableOperators();
}

void NetworkBusinessLogic::availableOperatorsReceived(bool success, const QList<AvailableOperator*> &operators, const QString &reason)
{
    qDebug() << Q_FUNC_INFO;
    Q_UNUSED(reason);    
    disconnect(networkRegistration, SIGNAL(availableOperators(bool, const QList<AvailableOperator*> &, const QString &)),
            this, SLOT(availableOperatorsReceived(bool, const QList<AvailableOperator*> &, const QString &)));

    if(!success || operators.size() == 0) {
        emit availableNetworkOperators(-1, QStringList(), true);
        return;
    }    

    QString selectedOperatorMnc;
    QString selectedOperatorMcc;
    this->operators.clear(); //just to be sure

    for(int i=0; i<operators.size(); ++i) {
        if(operators.at(i)->availability() != AvailableOperator::NotAvailable)
            this->operators.insert(operators.at(i)->name(), QStringList() << operators.at(i)->mnc() << operators.at(i)->mcc());
        if(operators.at(i)->availability() == AvailableOperator::Current) {
            selectedOperatorMnc = operators.at(i)->mnc();
            selectedOperatorMcc = operators.at(i)->mcc();
        }
    }

    QStringList operatorNames;
    int selectedOperatorIndex = -1;
    // operators (QMultiMap) is sorted by it's keys (operator names)

    if(selectedOperatorMnc.isEmpty()) // no selected operator
        operatorNames = this->operators.keys();
    else {        
        QMapIterator<QString, QStringList> iter(this->operators);        
        while (iter.hasNext()) {
            iter.next();
            operatorNames << iter.key();
            if(iter.value().at(0) == selectedOperatorMnc && iter.value().at(1) == selectedOperatorMcc)
                selectedOperatorIndex = operatorNames.size() - 1;
        }
    }        

    emit availableNetworkOperators(selectedOperatorIndex, operatorNames, true);
}

void NetworkBusinessLogic::technologyChanged(NetworkTechnology::Technology technology)
{
    qDebug() << Q_FUNC_INFO;
    emit networkIconChanged(mapTechnologyToIcon(technology));
}

QString NetworkBusinessLogic::networkIcon()
{
    qDebug() << Q_FUNC_INFO;
    return mapTechnologyToIcon(technology->currentTechnology());
}

QString NetworkBusinessLogic::currentOperator()
{
    qDebug() << Q_FUNC_INFO;
    return networkOperator->name();
}

QString NetworkBusinessLogic::mapTechnologyToIcon(NetworkTechnology::Technology technology)
{
    qDebug() << Q_FUNC_INFO;
    QString icon;
    switch(technology) {
        case NetworkTechnology::TwoG:
            icon = QString("icon-s-gsm");
            break;
        case NetworkTechnology::TwoPointFiveG:
            icon = QString("icon-s-25g");
            break;
        case NetworkTechnology::ThreeG:
            icon = QString("icon-s-3g");
            break;
        case NetworkTechnology::ThreePointFiveG:
            icon = QString("icon-s-35g");
            break;
        default:
            icon = QString("icon-s-network-0");
            break;
    }
    return icon;   
}

bool NetworkBusinessLogic::manualSelectionRequired()
{
    qDebug() << Q_FUNC_INFO;
    return (networkRegistration->mode() == NetworkRegistration::Manual ? true : false);
}
