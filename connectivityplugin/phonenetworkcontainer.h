#ifndef PHONENETWORKCONTAINER_H
#define PHONENETWORKCONTAINER_H

#include <DuiContainer>
#include <NetworkOperator>

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

private:
    void setLayout();

private:
    DuiButton *toggleButton;
    DuiLabel *operatorLabel;
    NetworkSignalStrengthIcon *signalStrengthIcon;
    NetworkOperator *networkOperator;

};

#endif // PHONENETWORKCONTAINER_H
