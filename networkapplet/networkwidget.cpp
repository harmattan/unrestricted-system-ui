#include "networkwidget.h"
#include "networktranslation.h"
#include "networkdbusinterface.h"
#include "dcpnetwork.h"

#include <DuiButton>
#include <DuiComboBox>
#include <DuiContainer>
#include <DuiDialog>
#include <DuiGridLayoutPolicy>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiList>
#include <DuiStylableWidget>
#include <DuiSceneManager>
#include <DuiPopupList>
#include <DuiWidgetController>
#include <DuiWidgetFactory>
#include <DuiWidgetListModel>
#include <QDebug>
#include <QModelIndex>
#include <QSignalMapper>
#include <QSizePolicy>
#include <QStringListModel>

NetworkWidget::NetworkWidget(QGraphicsWidget *parent)
	    :DcpWidget(parent)
{
    setReferer(DcpNetwork::None);    
    initWidget();
}

NetworkWidget::~NetworkWidget()
{
     networkIf->networkAppletClosing();
     delete networkIf;
     networkIf = NULL;
}

bool NetworkWidget::back()
{
    //check that the user has selected a network before closing the window
    if(!networkSelected) {
        qDebug() << "No selection made!\n\n\n";
        changeSelection();
        return false;
    }

    if (referer() > -1) { // emit the changewidget signal if there is a referer set
        emit changeWidget(referer());
        return false;
    }
    else
        return true; // back is handled by main window by default    
}

void NetworkWidget::changeSelection()
{
    DuiDialog* dlg = new DuiDialog();
    dlg->setTitle(DcpNetwork::NoNetworkSelectedText);
    DuiButton* changeButton = dlg->addButton(DcpNetwork::ChangeText);    
    dlg->exec();

    if(dlg->clickedButton() == changeButton) {        
        networkSelectionComboBox->setCurrentIndex(networkSelectionComboBoxDefaultIndex);
        dlg->accept();
    }
    else
        dlg->reject();

    dlg->deleteLater();
    dlg = NULL;        
}

void NetworkWidget::initWidget()
{
    //DuiSceneManager::instance()->setOrientationAngle((Dui::OrientationAngle) 90);

    //create dbus if
    networkIf = new NetworkDBusInterface();

    availableNetworksList = NULL;
    networkSelected = false;

    // catch network If actions    
    connect(networkIf, SIGNAL(phoneNetworkValueReceived(bool)), this, SLOT(initPhoneNetworkButton(bool)));
    connect(networkIf, SIGNAL(roamingValueReceived(bool)), this, SLOT(initRoamingButton(bool)));
    connect(networkIf, SIGNAL(roamingUpdatesValueReceived(bool)), this, SLOT(initRoamingUpdatesButton(bool)));
    connect(networkIf, SIGNAL(networkModeValuesReceived(int, QStringList)), this, SLOT(initNetworkModeComboBox(int, QStringList)));
    connect(networkIf, SIGNAL(networkSelectionValuesReceived(int, int, QStringList)), this, SLOT(initNetworkSelectionComboBox(int, int, QStringList)));
    connect(networkIf, SIGNAL(availableNetworksReceived(int, QStringList, bool)), this, SLOT(toggleAvailableNetworks(int, QStringList, bool)));
    connect(networkIf, SIGNAL(networkSelected(bool)), this, SLOT(toggleNetworkSelected(bool)));

    /*
     * phoneNetworkLabel and phoneNetworkButton
     */
    DuiLabel *phoneNetworkLabel = new DuiLabel(DcpNetwork::PhoneNetworkText);    
    phoneNetworkButton = new DuiButton(this);
    phoneNetworkButton->setCheckable(true);
    phoneNetworkButton->setObjectName("basicNetworkButton");    

    /*
     * roamingContainer
     */
    roamingButton = new DuiButton(this);
    roamingButton->setObjectName("basicNetworkButton");
    roamingButton->setCheckable(true);
    roamingUpdatesButton = new DuiButton(this);
    roamingUpdatesButton->setObjectName("basicNetworkButton");
    roamingUpdatesButton->setCheckable(true);        

    widgets << new DuiLabel(DcpNetwork::RoamingButtonText) << roamingButton;
    alignments.insert(roamingButton, Qt::AlignRight);
    DuiStylableWidget *roamingLeftLayoutWidget = createStylableWidget(Qt::Horizontal, QString("networkLayoutWidget2"));

    clearWidgetLists();
    widgets << new DuiLabel(DcpNetwork::RoamingUpdatesButtonText) << roamingUpdatesButton;
    alignments.insert(roamingUpdatesButton, Qt::AlignRight);
    roamingRightLayoutWidget = createStylableWidget(Qt::Horizontal, QString(""));

    DuiLayout *roamingLayout = new DuiLayout();
    clearWidgetLists();
    widgets << roamingLeftLayoutWidget << roamingRightLayoutWidget;
    roamingLandscapeLayoutPolicy = createLinearLayoutPolicy(roamingLayout, Qt::Horizontal);
    roamingLayout->setLandscapePolicy(roamingLandscapeLayoutPolicy);
    roamingPortraitLayoutPolicy = createLinearLayoutPolicy(roamingLayout, Qt::Vertical);
    roamingLayout->setPortraitPolicy(roamingPortraitLayoutPolicy);

    networkIf->roamingUpdatesValueRequired();
    networkIf->roamingValueRequired();

    roamingContainer = createContainer(roamingLayout, DcpNetwork::RoamingText, true);

    /*
     * dataCounterButton
     */
    dataCounterButton = new DuiButton(DcpNetwork::DataCounterText, this);
    dataCounterButton->setObjectName("dataCounterButton");

    /*
     * networkContainer
     */
    networkModeComboBox = new DuiComboBox(this);    
    networkIf->networkModeValuesRequired();    
    networkSelectionComboBox = new DuiComboBox(this);    
    networkIf->networkSelectionValuesRequired();    
    availableNetworksLabel = NULL; //init to null to prevent from showing behind the other widgets
    noAvailableNetworksLabel = NULL; //init to null to prevent from showing behind the other widgets

    DuiLayout *networkComboBoxLayout = new DuiLayout();    
    DuiGridLayoutPolicy *networkComboBoxLayoutPolicy = new DuiGridLayoutPolicy(networkComboBoxLayout);    
    networkComboBoxLayoutPolicy->addItemAtPosition(new DuiLabel(DcpNetwork::NetworkModeText), 0, 0);
    networkComboBoxLayoutPolicy->addItemAtPosition(new DuiLabel(DcpNetwork::NetworkSelectionText), 0, 1);
    networkComboBoxLayoutPolicy->addItemAtPosition(networkModeComboBox, 1, 0);
    networkComboBoxLayoutPolicy->addItemAtPosition(networkSelectionComboBox, 1, 1);    

    DuiLayout *networkLayout = new DuiLayout();
    networkLayoutPolicy = new DuiLinearLayoutPolicy(networkLayout, Qt::Vertical);
    networkLayoutPolicy->addItem(networkComboBoxLayout, Qt::AlignLeft);

    networkContainer = createContainer(networkLayout, DcpNetwork::NetworkText, false);

    /*
     * contentContainer
     */
    DuiLayout *contentLayout = new DuiLayout();

    contentLayoutPolicy = new DuiGridLayoutPolicy(contentLayout);
    contentLayoutPolicy->addItemAtPosition(phoneNetworkLabel, 0, 0);
    contentLayoutPolicy->addItemAtPosition(phoneNetworkButton, 0, 1);
    contentLayoutPolicy->addItemAtPosition(roamingContainer, 1, 0, 1, 2);
    contentLayoutPolicy->addItemAtPosition(dataCounterButton, 2, 0, 1, 2);
    contentLayoutPolicy->setSpacing(10);

    DuiContainer *contentContainer = createContainer(contentLayout);    

    networkIf->phoneNetworkValueRequired();

    /*
     * mainLayout
     */
    DuiLayout *mainLayout = new DuiLayout(this);
    clearWidgetLists();
    widgets << contentContainer;
    DuiLinearLayoutPolicy *mainLayoutPolicy = createLinearLayoutPolicy(mainLayout, Qt::Horizontal);
    Q_UNUSED(mainLayoutPolicy);

    // catch user actions
    signalMapper = new QSignalMapper(this);    
    connect(phoneNetworkButton, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(roamingButton, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(roamingUpdatesButton, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(dataCounterButton, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(networkModeComboBox, SIGNAL(currentIndexChanged(QString)), networkIf, SLOT(setNetworkModeValue(QString)));
    connect(networkSelectionComboBox, SIGNAL(currentIndexChanged(QString)), networkIf, SLOT(setNetworkSelectionValue(QString)));
    connect(roamingContainer, SIGNAL(headerClicked()), roamingContainer, SLOT(toggleExpand()));
    connect(networkContainer, SIGNAL(headerClicked()), networkContainer, SLOT(toggleExpand()));

    signalMapper->setMapping(phoneNetworkButton, DcpNetwork::PhoneNetworkText);
    signalMapper->setMapping(roamingButton, DcpNetwork::RoamingButtonText);
    signalMapper->setMapping(roamingUpdatesButton, DcpNetwork::RoamingUpdatesButtonText);
    signalMapper->setMapping(dataCounterButton, DcpNetwork::DataCounterText);
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(buttonPressed(QString)));
    //DuiSceneManager::instance()->setOrientationAngle((Dui::OrientationAngle) 90);

    this->setLayout(mainLayout);    
}

void NetworkWidget::clearWidgetLists()
{
    widgets.clear();
    alignments.clear();
}

DuiStylableWidget* NetworkWidget::createStylableWidget(Qt::Orientation policyOrientation, const QString &widgetObjectName, int policySpacing)
{
    DuiLayout *layout = new DuiLayout();
    DuiLinearLayoutPolicy *layoutPolicy = createLinearLayoutPolicy(layout, policyOrientation, policySpacing);
    Q_UNUSED(layoutPolicy);
    DuiStylableWidget *stylableWidget = new DuiStylableWidget(this);
    stylableWidget->setLayout(layout);
    stylableWidget->setObjectName(widgetObjectName);
    return stylableWidget;
}

DuiLinearLayoutPolicy* NetworkWidget::createLinearLayoutPolicy(DuiLayout *layout, Qt::Orientation policyOrientation, int policySpacing)
{
    DuiLinearLayoutPolicy *layoutPolicy = new DuiLinearLayoutPolicy(layout, policyOrientation);
    for(int i=0; i<widgets.size(); ++i)
        layoutPolicy->addItem(widgets.at(i), (alignments.value(widgets.at(i)) == 0 ? Qt::AlignLeft : alignments.value(widgets.at(i))));
    layoutPolicy->setSpacing(policySpacing);
    return layoutPolicy;
}

DuiContainer* NetworkWidget::createContainer(DuiLayout *layout, const QString &title, bool expandable)
{
    DuiStylableWidget *stylableWidget = new DuiStylableWidget(this);
    stylableWidget->setLayout(layout);
    DuiContainer *container = new DuiContainer(this);
    container->setCentralWidget(stylableWidget);
    if(!title.isEmpty())
        container->setTitle(title);
    container->setExpand(expandable);
    return container;
}


void NetworkWidget::initPhoneNetworkButton(bool toggle)
{
    phoneNetworkButton->setChecked(toggle);
    toggleNetworkSettings(toggle);
}

void NetworkWidget::initRoamingButton(bool toggle)
{
    roamingButton->setChecked(toggle);
    toggleRoamingUpdatesButton(toggle);
}

void NetworkWidget::initRoamingUpdatesButton(bool toggle)
{
    roamingUpdatesButton->setChecked(toggle);
}

void NetworkWidget::initNetworkModeComboBox(int selected, const QStringList &values)
{    
    networkModeComboBox->addItems(values);
    if (selected != -1) {
        disconnect(networkModeComboBox, SIGNAL(currentIndexChanged(QString)), networkIf, SLOT(setNetworkModeValue(QString)));
        networkModeComboBox->setCurrentIndex(selected);
        connect(networkModeComboBox, SIGNAL(currentIndexChanged(QString)), networkIf, SLOT(setNetworkModeValue(QString)));
    }
}

void NetworkWidget::initNetworkSelectionComboBox(int defaultIndex, int selected, const QStringList &values)
{
    networkSelectionComboBoxDefaultIndex = defaultIndex;
    networkSelectionComboBox->addItems(values);
    if (selected != -1) {
        disconnect(networkSelectionComboBox, SIGNAL(currentIndexChanged(QString)), networkIf, SLOT(setNetworkSelectionValue(QString)));
        networkSelectionComboBox->setCurrentIndex(selected);
        connect(networkSelectionComboBox, SIGNAL(currentIndexChanged(QString)), networkIf, SLOT(setNetworkSelectionValue(QString)));
    }
}

void NetworkWidget::toggleNetworkSettings(bool toggle)
{
    qDebug() << "NetworkWidget::toggleNetworkSettings(" << toggle << ")";
    if(toggle)
        contentLayoutPolicy->addItemAtPosition(networkContainer, 3, 0, 1, 2);
    else
        contentLayoutPolicy->removeItem(networkContainer);

    DuiPopupList *popuplist = new DuiPopupList();
    QStringListModel *model = new QStringListModel(this);
    QStringList stringList;
    stringList << "Item 1" << "Item 2";
    model->setStringList(stringList);
    popuplist->setItemModel(model);
    contentLayoutPolicy->addItemAtPosition(popuplist, 4, 0, 1, 2);
}

void NetworkWidget::toggleRoamingUpdatesButton(bool toggle)
{
    if(toggle) {        
        if(roamingLandscapeLayoutPolicy->indexOf(roamingRightLayoutWidget) == -1) {
            roamingLandscapeLayoutPolicy->addItem(roamingRightLayoutWidget, Qt::AlignLeft);
            roamingPortraitLayoutPolicy->addItem(roamingRightLayoutWidget, Qt::AlignLeft);
        }
    }
    else {       
        if(roamingUpdatesButton->isChecked()) {
            roamingUpdatesButton->setChecked(false);
            networkIf->setRoamingUpdatesValue(false);
        }        
        if(roamingLandscapeLayoutPolicy->indexOf(roamingRightLayoutWidget) != -1) {
            roamingLandscapeLayoutPolicy->removeItem(roamingRightLayoutWidget);
            roamingPortraitLayoutPolicy->removeItem(roamingRightLayoutWidget);            
        }        
    }
}



void NetworkWidget::buttonPressed(const QString &text)
{   
    //NOTE: DuiButton->isChecked() method returns the state before the press at this point    

    if(text == DcpNetwork::PhoneNetworkText) {
        bool checked = !phoneNetworkButton->isChecked();
        networkIf->setPhoneNetworkValue(checked);
        toggleNetworkSettings(checked);
    }

    else if(text == DcpNetwork::RoamingButtonText) {
        bool checked = !roamingButton->isChecked();

        networkIf->setRoamingValue(checked);
        toggleRoamingUpdatesButton(checked);
    }

    else if(text == DcpNetwork::RoamingUpdatesButtonText)
        networkIf->setRoamingUpdatesValue(!roamingUpdatesButton->isChecked());

    else if(text == DcpNetwork::DataCounterText)
        qDebug() << "Show dialog";
}

void NetworkWidget::toggleAvailableNetworks(int selected, const QStringList &networks, bool toggle)
{
    qDebug() << "NetworkWidget::toggleAvailableNetworks(" << selected << ", " << networks << ", " << toggle << ")";
    if(toggle) {
        if(availableNetworksList == NULL) {
            if(networks.size() > 0) { //some networks available

                //create the header label
                if(availableNetworksLabel == NULL)
                    availableNetworksLabel = new DuiLabel(DcpNetwork::AvailableNetworksText, this);
                networkLayoutPolicy->addItem(availableNetworksLabel, Qt::AlignLeft);

                //create the list of available networks
                availableNetworksList = new DuiList(this);
                availableNetworksList->setObjectName("availableNetworksList");
                availableNetworksList->enableItemSelection(true);
                availableNetworksListModel = new DuiWidgetListModel;
                for(int i=0; i<networks.size(); ++i) {
                    DuiLabel *label = new DuiLabel(networks.at(i));
                    availableNetworksListModel->appendWidget(label);
                }
                availableNetworksList->setItemModel(availableNetworksListModel);
                if(selected != -1)
                    availableNetworksList->selectItem(availableNetworksListModel->index(selected));
                networkLayoutPolicy->addItem(availableNetworksList, Qt::AlignLeft);

                connect(availableNetworksList, SIGNAL(itemClicked(const QModelIndex &)),
                        this, SLOT(availableNetworkSelected(const QModelIndex &)));
            }
            else {
                if(noAvailableNetworksLabel == NULL)
                    noAvailableNetworksLabel = new DuiLabel(DcpNetwork::NoAvailableNetworksText, this);
                networkLayoutPolicy->addItem(noAvailableNetworksLabel, Qt::AlignLeft);                
            }
        }       
    }
    else {
        if(availableNetworksList != NULL) {
            networkLayoutPolicy->removeItem(availableNetworksList);            
            delete availableNetworksList;
            availableNetworksList = NULL;      
            delete availableNetworksListModel;
            availableNetworksListModel = NULL;
        }
        if(availableNetworksLabel != NULL) {
            networkLayoutPolicy->removeItem(availableNetworksLabel);
            delete availableNetworksLabel;
            availableNetworksLabel = NULL;
        }
        if(noAvailableNetworksLabel != NULL) {
            networkLayoutPolicy->removeItem(noAvailableNetworksLabel);
            delete noAvailableNetworksLabel;
            noAvailableNetworksLabel = NULL;
        }
    }
}

void NetworkWidget::availableNetworkSelected(const QModelIndex &index)
{
    qDebug() << "NetworkWidget::availableNetworkSelected(" << index << ")";
    availableNetworksList->selectItem(index);
    DuiWidgetFactory* factory = DuiWidgetFactory::instance();
    DuiLabel* label = static_cast<DuiLabel*>(factory->create(availableNetworksListModel, index.row()));    
    networkIf->setNetworkSelectionValue(label->text());
}

void NetworkWidget::toggleNetworkSelected(bool toggle)
{
    qDebug() << "NetworkWidget::toggleNetworkSelected(" << toggle << ")";
    networkSelected = toggle;
}
