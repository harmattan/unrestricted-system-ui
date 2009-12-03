#ifndef PHONENETWORKCONTAINER_H
#define PHONENETWORKCONTAINER_H

#include "networktechnology.h"

#include <DuiContainer>
#include <NetworkOperator>
#include <RadioAccess>

class DuiButton;
class DuiLabel;
class NetworkSignalStrengthIcon;

using namespace Cellular;

class PhoneNetworkContainer : public DuiContainer
{
    Q_OBJECT

public:
    PhoneNetworkContainer(DuiWidget *parent = 0);
    virtual ~PhoneNetworkContainer();

    bool phoneNetworkOn();
    void togglePhoneNetwork(bool toggle);    

signals:    
    void toggled(bool);

private slots:
    void updateButtonIcon(NetworkTechnology::Technology technology);

private:
    void setLayout();
    QString mapTechnologyToIcon(NetworkTechnology::Technology technology);

private:
    DuiButton *toggleButton;
    DuiLabel *operatorLabel;
    NetworkSignalStrengthIcon *signalStrengthIcon;
    NetworkOperator *networkOperator;
    RadioAccess *radioAccess;
    NetworkTechnology *networkTechnology;

};

#endif // PHONENETWORKCONTAINER_H
