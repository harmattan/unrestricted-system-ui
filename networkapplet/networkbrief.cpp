#include "networkbrief.h"
#include "networkbusinesslogic.h"
#include "dcpwidgettypes.h"

#include <QDebug>

NetworkBrief::NetworkBrief() :
        DcpBrief(),
        logic(NULL),
        networkText(QString("")),
        networkToggle(false),
        networkToggleIcon(QString(""))
{

    logic = new NetworkBusinessLogic();    

    connect(logic, SIGNAL(networkStateChanged(bool)), this, SLOT(updateToggle(bool)));
    connect(logic, SIGNAL(networkOperatorChanged(QString)), this, SLOT(updateOperatorLabel(QString)));
    connect(logic, SIGNAL(networkIconChanged(QString)), this, SLOT(updateToggleIcon(QString)));

    updateToggle(logic->networkEnabled());
    updateToggleIcon(logic->networkIcon());
    updateOperatorLabel(logic->currentOperator());
}

NetworkBrief::~NetworkBrief()
{   
    delete logic;
    logic = NULL; 
}

QString NetworkBrief::valueText() const
{
    return networkText;
}

bool NetworkBrief::toggle() const
{
    return networkToggle;
}

QString NetworkBrief::toggleIconId() const
{
    return networkToggleIcon;
}

int NetworkBrief::widgetTypeID() const
{    
    return DCPLABELBUTTON + 100;
}

void NetworkBrief::updateOperatorLabel(const QString &text)
{
    networkText = text;
    emit valuesChanged();
}

void NetworkBrief::updateToggle(bool toggle)
{    
    networkToggle = toggle;
    emit valuesChanged();
}

void NetworkBrief::updateToggleIcon(const QString &icon)
{
    networkToggleIcon = icon;    
    emit valuesChanged();
}

void NetworkBrief::setToggle (bool toggle)
{
    logic->toggleNetwork(toggle);
}

