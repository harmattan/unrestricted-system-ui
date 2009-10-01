#include "networkwidget.h"
#include "networktranslation.h"
#include "networkdbusinterface.h"
#include "dcpnetwork.h"

#include <QDebug>
#include <QTimer>

#include <DuiButton>
#include <DuiComboBox>
#include <DuiContainer>
#include <DuiGridLayoutPolicy>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiStylableWidget>
#include <DuiSceneManager>
#include <DuiButtonGroup>
#include <QSignalMapper>
#include <QSizePolicy>

NetworkWidget::NetworkWidget(QGraphicsWidget *parent)
	    :DcpWidget(parent)
{
    setReferer(DcpNetwork::None);
    initWidget();
}

NetworkWidget::~NetworkWidget()
{    
}

void NetworkWidget::initWidget()
{    
    //DuiSceneManager::instance()->setOrientationAngle((Dui::OrientationAngle) 90);

    //create dbus if
    networkIf = new NetworkDBusInterface();

    availableNetworksButtonGroup = new DuiButtonGroup(this);

    // catch network If actions    
    connect(networkIf, SIGNAL(phoneNetworkValueReceived(bool)), this, SLOT(initPhoneNetworkButton(bool)));
    connect(networkIf, SIGNAL(roamingValueReceived(bool)), this, SLOT(initRoamingButton(bool)));
    connect(networkIf, SIGNAL(roamingUpdatesValueReceived(bool)), this, SLOT(initRoamingUpdatesButton(bool)));
    connect(networkIf, SIGNAL(networkModeValuesReceived(int, QStringList)), this, SLOT(initNetworkModeComboBox(int, QStringList)));
    connect(networkIf, SIGNAL(networkSelectionValuesReceived(int, QStringList)), this, SLOT(initNetworkSelectionComboBox(int, QStringList)));
    connect(networkIf, SIGNAL(availableNetworksReceived(int, QStringList)), this, SLOT(toggleAvailableNetworks(int, QStringList)));    

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
    roamingUpdatesButton = new DuiButton(this);
    roamingUpdatesButton->setObjectName("basicNetworkButton");
    roamingUpdatesButton->setCheckable(true);

    roamingButton = new DuiButton(this);
    roamingButton->setObjectName("basicNetworkButton");
    roamingButton->setCheckable(true);

    DuiLayout *roamingLeftLayout = new DuiLayout();
    DuiLinearLayoutPolicy *roamingLeftLayoutPolicy = new DuiLinearLayoutPolicy(roamingLeftLayout, Qt::Horizontal);
    roamingLeftLayoutPolicy->addItem(new DuiLabel(DcpNetwork::RoamingButtonText), Qt::AlignLeft);
    roamingLeftLayoutPolicy->addItem(roamingButton, Qt::AlignRight);
    DuiStylableWidget *roamingLeftLayoutWidget = new DuiStylableWidget(this);
    roamingLeftLayoutWidget->setLayout(roamingLeftLayout);
    roamingLeftLayoutWidget->setObjectName("networkLayoutWidget2");

    DuiLayout *roamingRightLayout = new DuiLayout();
    DuiLinearLayoutPolicy *roamingRightLayoutPolicy = new DuiLinearLayoutPolicy(roamingRightLayout, Qt::Horizontal);
    roamingRightLayoutPolicy->addItem(new DuiLabel(DcpNetwork::RoamingUpdatesButtonText), Qt::AlignLeft);
    roamingRightLayoutPolicy->addItem(roamingUpdatesButton, Qt::AlignRight);
    roamingRightLayoutWidget = new DuiStylableWidget(this);
    roamingRightLayoutWidget->setLayout(roamingRightLayout);
    roamingRightLayoutWidget->setObjectName("networkLayoutWidget2");

    DuiLayout *roamingLayout = new DuiLayout();
    roamingLandscapeLayoutPolicy = new DuiLinearLayoutPolicy(roamingLayout, Qt::Horizontal);
    roamingLandscapeLayoutPolicy->addItem(roamingLeftLayoutWidget, Qt::AlignLeft);
    roamingLandscapeLayoutPolicy->addItem(roamingRightLayoutWidget, Qt::AlignLeft);
    roamingPortraitLayoutPolicy = new DuiLinearLayoutPolicy(roamingLayout, Qt::Vertical);
    roamingPortraitLayoutPolicy->addItem(roamingLeftLayoutWidget, Qt::AlignLeft);
    roamingPortraitLayoutPolicy->addItem(roamingRightLayoutWidget, Qt::AlignLeft);
    roamingLayout->setLandscapePolicy(roamingLandscapeLayoutPolicy);
    roamingLayout->setPortraitPolicy(roamingPortraitLayoutPolicy);

    networkIf->roamingUpdatesValueRequired();
    networkIf->roamingValueRequired();

    DuiStylableWidget *roamingLayoutWidget = new DuiStylableWidget(this);
    roamingLayoutWidget->setLayout(roamingLayout);

    roamingContainer = new DuiContainer(this);
    roamingContainer->setTitle(DcpNetwork::RoamingText);
    roamingContainer->setCentralWidget(roamingLayoutWidget);
    roamingContainer->setExpand(true);
    roamingContainer->setObjectName("networkContainer");

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

    DuiStylableWidget *networkLayoutWidget = new DuiStylableWidget(this);
    networkLayoutWidget->setLayout(networkLayout);
    //networkLayoutWidget->setObjectName("networkLayoutWidget1");

    networkContainer = new DuiContainer(this);
    networkContainer->setCentralWidget(networkLayoutWidget);
    networkContainer->setTitle(DcpNetwork::NetworkText);
    networkContainer->setExpand(false);
    //networkContainer->setObjectName("networkContainer");

    /*
     * contentContainer
     */
    DuiLayout *contentLayout = new DuiLayout();

    contentLayoutPolicy = new DuiGridLayoutPolicy(contentLayout);
    contentLayoutPolicy->addItemAtPosition(phoneNetworkLabel, 0, 0);
    contentLayoutPolicy->addItemAtPosition(phoneNetworkButton, 0, 1);    
    contentItems << roamingContainer << dataCounterButton << networkContainer;

    DuiStylableWidget *contentWidget = new DuiStylableWidget(this);
    contentWidget->setLayout(contentLayout);
    DuiContainer *contentContainer = new DuiContainer(this);
    contentContainer->setCentralWidget(contentWidget);

    networkIf->phoneNetworkValueRequired();

    /*
     * mainLayout
     */
    DuiLayout *mainLayout = new DuiLayout(this);
    DuiLinearLayoutPolicy *mainLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Horizontal);
    mainLayoutPolicy->addItem(contentContainer);

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

void NetworkWidget::initPhoneNetworkButton(bool toggle)
{
    phoneNetworkButton->setChecked(toggle);
    toggleSettings(toggle);
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

void NetworkWidget::initNetworkSelectionComboBox(int selected, const QStringList &values)
{    
    networkSelectionComboBox->addItems(values);
    if (selected != -1) {
        disconnect(networkSelectionComboBox, SIGNAL(currentIndexChanged(QString)), networkIf, SLOT(setNetworkSelectionValue(QString)));
        networkSelectionComboBox->setCurrentIndex(selected);
        connect(networkSelectionComboBox, SIGNAL(currentIndexChanged(QString)), networkIf, SLOT(setNetworkSelectionValue(QString)));
    }
}

void NetworkWidget::toggleSettings(bool toggle)
{
    qDebug() << "NetworkWidget::toggleSettings(" << toggle << ")";
    if(toggle) {
        if(contentLayoutPolicy->indexOf(contentItems.at(0)) == -1) {
            for(int i=0; i<contentItems.size(); ++i)
                contentLayoutPolicy->addItemAtPosition(contentItems.at(i), i+1, 0, 1, 2);            
            contentLayoutPolicy->setSpacing(10);
        }
    }
    else {
        if(contentLayoutPolicy->indexOf(contentItems.at(0)) != -1) {
            for(int i=0; i<contentItems.size(); ++i)
                contentLayoutPolicy->removeItem(contentItems.at(i));
            contentLayoutPolicy->setSpacing(0);
        }
    }
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
        toggleSettings(checked);
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
    if(toggle) {
        if(availableNetworksButtonGroup->buttons().size() == 0) { //no buttons on the screen
            if(networks.size() > 0) { //some networks available
                if(availableNetworksLabel == NULL)
                    availableNetworksLabel = new DuiLabel(DcpNetwork::AvailableNetworksText, this);
                networkLayoutPolicy->addItem(availableNetworksLabel, Qt::AlignLeft);
                for(int i=0; i<networks.size(); ++i) {
                    DuiButton *button = new DuiButton(networks.at(i), this);
                    button->setCheckable(true);
                    if(i==selected)
                        button->setChecked(true);
                    availableNetworksButtonGroup->addButton(button);
                    networkLayoutPolicy->addItem(button, Qt::AlignLeft);
                }
                connect(availableNetworksButtonGroup, SIGNAL(buttonPressed(DuiButton*)),
                        this, SLOT(availableNetworksButtonPressed(DuiButton*)));
            }
            else {
                if(noAvailableNetworksLabel == NULL)
                    noAvailableNetworksLabel = new DuiLabel(DcpNetwork::NoAvailableNetworksText, this);
                networkLayoutPolicy->addItem(noAvailableNetworksLabel, Qt::AlignLeft);
            }
        }       
    }
    else {
        if(availableNetworksButtonGroup->buttons().size() > 0) {
            if(availableNetworksLabel == NULL)
                availableNetworksLabel = new DuiLabel(DcpNetwork::AvailableNetworksText, this);
            networkLayoutPolicy->removeItem(availableNetworksLabel);
            QList<DuiButton *> buttons = availableNetworksButtonGroup->buttons();
            while(!buttons.isEmpty()) {
                DuiButton *button = buttons.takeLast();
                availableNetworksButtonGroup->removeButton(button);
                networkLayoutPolicy->removeItem(button);
                delete button;
                button = NULL;
            }            
        }
        else {
            if(noAvailableNetworksLabel == NULL)
                noAvailableNetworksLabel = new DuiLabel(DcpNetwork::NoAvailableNetworksText, this);
            networkLayoutPolicy->removeItem(noAvailableNetworksLabel);
        }
    }

}

void NetworkWidget::availableNetworksButtonPressed(DuiButton *button)
{
    networkIf->setNetworkSelectionValue(button->text());
}
