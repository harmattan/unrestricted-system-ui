#ifndef NETWORKWIDGET_H
#define NETWORKWIDGET_H

#include "dcpwidget.h" 

class DuiButton;
class DuiButtonGroup;
class DuiComboBox;
class DuiContainer;
class DuiLabel;
class DuiLinearLayoutPolicy;
class NetworkDBusInterface;
class QSignalMapper;

class NetworkWidget : public DcpWidget
{
    Q_OBJECT
		
public:
    NetworkWidget(QGraphicsWidget *parent = 0);
    virtual ~NetworkWidget();

protected:
    void initWidget();

private slots:
    void buttonPressed(const QString &text);
    void networkSelectionValueChanged(const QString &value);
    void toggleAvailableNetworks(const QStringList &networks, bool toggle = true);
    void availableNetworksButtonPressed(DuiButton* button);

private: //methods

private: //attributes        
    DuiButton *phoneNetworkButton;
    DuiButton *enableRoamingButton;
    DuiButton *enableRoamingUpdatesButton;
    DuiButton *dataCounterButton;
    DuiButtonGroup *availableNetworksButtonGroup;
    DuiComboBox *networkModeComboBox;
    DuiComboBox *networkSelectionComboBox;
    DuiContainer *roamingLayoutWidgetContainer;
    //DuiContainer *availableNetworksContainer;
    DuiContainer *networkLayoutWidgetContainer;
    DuiLabel *noAvailableNetworksLabel;
    DuiLabel *availableNetworksLabel;    
    //DuiLinearLayoutPolicy *availableNetworksLayoutPolicy;
    DuiLinearLayoutPolicy *networkLayoutPolicy;    
    NetworkDBusInterface *networkIf;    
    QSignalMapper *signalMapper;
};
#endif // NETWORKWIDGET_H
