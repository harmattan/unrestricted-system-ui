#ifndef NETWORKWIDGET_H
#define NETWORKWIDGET_H

#include "dcpwidget.h" 

class DuiButton;
class DuiComboBox;
class DuiContainer;
class DuiLinearLayoutPolicy;
class NetworkDBusInterface;


class NetworkWidget : public DcpWidget
{
    Q_OBJECT
		
public:
    NetworkWidget(QGraphicsWidget *parent = 0);
    virtual ~NetworkWidget();

protected:
    void initWidget();

private slots:

private: //methods

private: //attributes        
    DuiButton *phoneNetworkButton;
    DuiButton *enableRoamingButton;
    DuiButton *enableRoamingUpdatesButton;
    DuiButton *dataCounterButton;
    DuiComboBox *networkModeComboBox;
    DuiComboBox *networkSelectionComboBox;
    DuiContainer *roamingLayoutWidgetContainer;
    DuiContainer *networkWidgetContainer;
    DuiContainer *availableNetworksContainer;
    DuiContainer *networkLayoutWidgetContainer;
    DuiLinearLayoutPolicy *availableNetworksLayoutPolicy;
    NetworkDBusInterface *networkIf;
};
#endif // NETWORKWIDGET_H
