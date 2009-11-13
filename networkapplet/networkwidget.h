#ifndef NETWORKWIDGET_H
#define NETWORKWIDGET_H

#include "dcpwidget.h" 

class DuiButton;
class DuiComboBox;
class DuiContainer;
class DuiGridLayoutPolicy;
class DuiLabel;
class DuiLayout;
class DuiLinearLayoutPolicy;
class DuiStylableWidget;
class DuiWidgetController;
class NetworkContainer;
class RoamingContainer;
class NetworkBusinessLogic;

#include <QDebug>

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

private: //methods
    void initPhoneNetworkButton(bool toggle);
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
