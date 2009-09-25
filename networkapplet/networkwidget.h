#ifndef NETWORKWIDGET_H
#define NETWORKWIDGET_H

#include "dcpwidget.h" 

class DuiButton;
class DuiButtonGroup;
class DuiComboBox;
class DuiContainer;
class DuiLabel;
class DuiGridLayoutPolicy;
class DuiLinearLayoutPolicy;
class DuiStylableWidget;
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
    void toggleSettings(bool toggle);
    void toggleEnableRoamingUpdates(bool toggle);
    void initPhoneNetworkButton(bool toggle);
    void initEnableRoamingButton(bool value);
    void initEnableRoamingUpdatesButton(bool value);
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
    DuiContainer *networkLayoutWidgetContainer;
    DuiLabel *noAvailableNetworksLabel;
    DuiLabel *availableNetworksLabel;
    DuiLinearLayoutPolicy *networkLayoutPolicy;
    DuiLinearLayoutPolicy *roamingLayoutPolicy;
    DuiGridLayoutPolicy *contentLayoutPolicy;
    DuiStylableWidget *roamingRightLayoutWidget;
    NetworkDBusInterface *networkIf;    
    QSignalMapper *signalMapper;
    QList<QGraphicsLayoutItem*> contentItems;
};
#endif // NETWORKWIDGET_H
