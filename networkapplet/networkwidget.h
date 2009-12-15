#ifndef NETWORKWIDGET_H
#define NETWORKWIDGET_H

#include "dcpwidget.h"
#include "roamingcontainer.h"

class DuiButton;
class DuiGridLayoutPolicy;
class NetworkBusinessLogic;
class NetworkContainer;

class NetworkWidget : public DcpWidget
{
    Q_OBJECT    
		
public:
    NetworkWidget(QGraphicsWidget *parent = 0);
    virtual ~NetworkWidget();
    bool back();

protected:
    void initWidget();

private slots:
    void updateNetworkIcon(const QString &value);
    void toggleNetworkSettings(bool toggle);
    void dataCounterButtonPressed();
    void togglePhoneNetworkButton(bool toggle);
    void roamingStateChanged(int state);
    void roamingStateChanged(RoamingContainer::RoamingState state);

private: //methods    
    void changeSelection();    

private: //attributes
    NetworkBusinessLogic *logic;
    DuiButton *phoneNetworkButton;    
    DuiButton *dataCounterButton;    
    DuiGridLayoutPolicy *contentLayoutPolicy;    
    bool networkSelected;
    NetworkContainer *networkContainer;
    RoamingContainer *roamingContainer;    

};
#endif // NETWORKWIDGET_H
