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
class DuiList;
class DuiStylableWidget;
class DuiWidgetController;
class DuiWidgetListModel;
class NetworkDBusInterface;
class QModelIndex;
class QSignalMapper;

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
    void buttonPressed(const QString &text);        
    void toggleAvailableNetworks(int selected, const QStringList &networks, bool toggle);
    void availableNetworkSelected(const QModelIndex &index);
    void toggleNetworkSelected(bool toggle);

private: //methods
    void changeSelection();
    void clearWidgetLists();
    DuiLinearLayoutPolicy* createLinearLayoutPolicy(DuiLayout *layout, Qt::Orientation policyOrientation, int policySpacing = 0);
    DuiStylableWidget* createStylableWidget(Qt::Orientation policyOrientation, const QString &widgetObjectName = "", int policySpacing = 0);    
    DuiContainer* createContainer(DuiLayout *layout, const QString &title = "", bool expandable = false);

    void toggleNetworkSettings(bool toggle);
    void toggleRoamingUpdatesButton(bool toggle);

private: //attributes        
    DuiButton *phoneNetworkButton;
    DuiButton *roamingButton;
    DuiButton *roamingUpdatesButton;
    DuiButton *dataCounterButton;    
    DuiComboBox *networkModeComboBox;
    DuiComboBox *networkSelectionComboBox;
    DuiContainer *roamingContainer;
    DuiContainer *networkContainer;
    DuiGridLayoutPolicy *contentLayoutPolicy;
    DuiLabel *noAvailableNetworksLabel;
    DuiLabel *availableNetworksLabel;
    DuiLinearLayoutPolicy *networkLayoutPolicy;
    DuiLinearLayoutPolicy *roamingLandscapeLayoutPolicy;
    DuiLinearLayoutPolicy *roamingPortraitLayoutPolicy;
    DuiList *availableNetworksList;
    DuiStylableWidget *roamingRightLayoutWidget;
    DuiWidgetListModel *availableNetworksListModel;
    NetworkDBusInterface *networkIf;    
    QSignalMapper *signalMapper;    
    QList<DuiWidgetController *> widgets;
    QMap<DuiWidgetController *, Qt::Alignment> alignments;    
    int networkSelectionComboBoxDefaultIndex;
    bool networkSelected;
};
#endif // NETWORKWIDGET_H
