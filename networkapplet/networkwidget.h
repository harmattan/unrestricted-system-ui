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
class RoamingContainer;

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
    void initNetworkModeComboBox(int selected, const QStringList &values);
    void initNetworkSelectionComboBox(int defaultIndex, int selected, const QStringList &values);
    void dataCounterButtonPressed();
    void toggleNetworkSelected(bool toggle);
    void toggleNetworkSettings(bool toggle);
    void updateNetworkIcon(const QString &value);

private: //methods
    void changeSelection();    

private: //attributes        
    DuiButton *phoneNetworkButton;    
    DuiButton *dataCounterButton;    
    DuiGridLayoutPolicy *contentLayoutPolicy;    
    bool networkSelected;
    NetworkContainer *networkContainer;
    RoamingContainer *roamingContainer;
    NetworkDBusInterface *networkIf;

};
#endif // NETWORKWIDGET_H
