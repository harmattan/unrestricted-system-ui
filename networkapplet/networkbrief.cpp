#include "networkbrief.h"
#include "dcpwidgettypes.h"
#include "networkdbusinterface.h"

NetworkBrief::NetworkBrief()
{
    networkIf = new NetworkDBusInterface();    
    connect(networkIf, SIGNAL(currentOperatorValueReceived(QString)), this, SLOT(changeText(QString)));
    connect(networkIf, SIGNAL(phoneNetworkValueReceived(bool)), this, SLOT(changeToggle(bool)));
    connect(networkIf, SIGNAL(networkIconValueChanged(QString)), this, SLOT(changeIcon(QString)));
    networkToggle = false;
    networkIf->phoneNetworkValueRequired();
    networkIf->networkIconValueRequired();
}

NetworkBrief::~NetworkBrief()
{
    delete networkIf;
    networkIf = NULL;
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

void NetworkBrief::changeText(const QString &text)
{
    networkText = text;
    emit valuesChanged();
}

void NetworkBrief::changeToggle(bool toggle)
{
    networkToggle = toggle;
    if(networkToggle)
        networkIf->currentOperatorValueRequired();
    emit valuesChanged();
}

void NetworkBrief::changeIcon(const QString &icon)
{
    Q_UNUSED(icon);
    // TODO: when DcpBried has support, change the toggle button icon
    //button->setIconID(icon);
    //emit valuesChanged();
}
