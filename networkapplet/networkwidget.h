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

#include <QDebug>

class NetworkWidget : public DcpWidget
{
    Q_OBJECT
		
public:
    NetworkWidget(QGraphicsWidget *parent = 0);
    virtual ~NetworkWidget();

protected:
    void initWidget();

private slots:        
    void initPhoneNetworkButton(bool toggle);
    void initRoamingButton(bool value);
    void initRoamingUpdatesButton(bool value);
    void initNetworkModeComboBox(int selected, const QStringList &values);
    void initNetworkSelectionComboBox(int selected, const QStringList &values);
    void buttonPressed(const QString &text);        
    void toggleAvailableNetworks(int selected, const QStringList &networks, bool toggle = true);
    void availableNetworksButtonPressed(DuiButton* button);

private: //methods
    void toggleSettings(bool toggle);
    void toggleRoamingUpdatesButton(bool toggle);

private: //attributes        
    DuiButton *phoneNetworkButton;
    DuiButton *roamingButton;
    DuiButton *roamingUpdatesButton;
    DuiButton *dataCounterButton;
    DuiButtonGroup *availableNetworksButtonGroup;
    DuiComboBox *networkModeComboBox;
    DuiComboBox *networkSelectionComboBox;
    DuiContainer *roamingContainer;
    DuiContainer *networkContainer;
    DuiLabel *noAvailableNetworksLabel;
    DuiLabel *availableNetworksLabel;
    DuiLinearLayoutPolicy *networkLayoutPolicy;
    DuiLinearLayoutPolicy *roamingLandscapeLayoutPolicy;
    DuiLinearLayoutPolicy *roamingPortraitLayoutPolicy;
    DuiGridLayoutPolicy *contentLayoutPolicy;
    DuiStylableWidget *roamingRightLayoutWidget;
    NetworkDBusInterface *networkIf;    
    QSignalMapper *signalMapper;
    QList<QGraphicsLayoutItem*> contentItems;
};
#endif // NETWORKWIDGET_H
