#include "networkbrief.h"
#include "networkbusinesslogic.h"
#include "dcpwidgettypes.h"

NetworkBrief::NetworkBrief() :
        DcpBrief(),
        logic(NULL)
{
    logic = new NetworkBusinessLogic();

    connect(logic, SIGNAL(networkOperatorChanged(QString)), this, SLOT(updateOperatorLabel(QString)));
    connect(logic, SIGNAL(networkIconChanged(QString)), this, SLOT(updateToggleIcon(QString)));

    //TODO: catch the toggle signal and manipulate logic

    updateToggle(logic->networkEnabled());
    updateToggleIcon(logic->networkIcon());   
    updateOperatorLabel(logic->currentOperator());
    emit valuesChanged();
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

int NetworkBrief::widgetTypeID() const
{    
    return DCPLABEL2BUTTON;
}

void NetworkBrief::updateOperatorLabel(const QString &text)
{
    networkText = text;
}

void NetworkBrief::updateToggle(bool toggle)
{
    networkToggle = toggle;    
}

void NetworkBrief::updateToggleIcon(const QString &icon)
{
    Q_UNUSED(icon);
    // TODO: when DcpBried has support, change the toggle button icon
    //button->setIconID(icon);
    //emit valuesChanged();
}
