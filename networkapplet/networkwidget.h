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
class NetworkDBusInterface;
class NetworkContainer;

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
    void initPhoneNetworkButton(bool toggle);
    void initRoamingButton(bool value);
    void initRoamingUpdatesButton(bool value);
    void initNetworkModeComboBox(int selected, const QStringList &values);
    void initNetworkSelectionComboBox(int defaultIndex, int selected, const QStringList &values);
    void dataCounterButtonPressed();
    void toggleNetworkSelected(bool toggle);
    void toggleNetworkSettings(bool toggle);
    void toggleRoamingUpdatesButton(bool toggle);

private: //methods
    void changeSelection();    

private: //attributes        
    DuiButton *phoneNetworkButton;
    DuiButton *roamingButton;
    DuiButton *roamingUpdatesButton;
    DuiButton *dataCounterButton;    
    DuiGridLayoutPolicy *contentLayoutPolicy;
    DuiLabel *roamingUpdatesLabel;
    bool networkSelected;
    NetworkContainer *networkContainer;
    NetworkDBusInterface *networkIf;

};
#endif // NETWORKWIDGET_H
