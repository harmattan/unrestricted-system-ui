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
    connect(networkIf, SIGNAL(enableRoamingValueReceived(bool)), this, SLOT(initEnableRoamingButton(bool)));
    connect(networkIf, SIGNAL(enableRoamingUpdatesValueReceived(bool)), this, SLOT(initEnableRoamingUpdatesButton(bool)));
    connect(networkIf, SIGNAL(availableNetworksReceived(QStringList)), this, SLOT(toggleAvailableNetworks(QStringList)));

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
    enableRoamingUpdatesButton = new DuiButton(this);
    enableRoamingUpdatesButton->setObjectName("basicNetworkButton");
    enableRoamingUpdatesButton->setCheckable(true);    

    enableRoamingButton = new DuiButton(this);
    enableRoamingButton->setObjectName("basicNetworkButton");
    enableRoamingButton->setCheckable(true);

    DuiLayout *roamingLeftLayout = new DuiLayout();
    DuiLinearLayoutPolicy *roamingLeftLayoutPolicy = new DuiLinearLayoutPolicy(roamingLeftLayout, Qt::Horizontal);
    roamingLeftLayoutPolicy->addItem(new DuiLabel(DcpNetwork::EnableRoamingText), Qt::AlignLeft);
    roamingLeftLayoutPolicy->addItem(enableRoamingButton, Qt::AlignRight);
    DuiStylableWidget *roamingLeftLayoutWidget = new DuiStylableWidget(this);
    roamingLeftLayoutWidget->setLayout(roamingLeftLayout);
    roamingLeftLayoutWidget->setObjectName("networkLayoutWidget2");

    DuiLayout *roamingRightLayout = new DuiLayout();
    DuiLinearLayoutPolicy *roamingRightLayoutPolicy = new DuiLinearLayoutPolicy(roamingRightLayout, Qt::Horizontal);
    roamingRightLayoutPolicy->addItem(new DuiLabel(DcpNetwork::EnableRoamingUpdatesText), Qt::AlignLeft);
    roamingRightLayoutPolicy->addItem(enableRoamingUpdatesButton, Qt::AlignRight);
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

    networkIf->enableRoamingUpdatesValueRequired();
    networkIf->enableRoamingValueRequired();

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
    QStringList networkModeValues;
    networkModeValues << DcpNetwork::DualText << DcpNetwork::ThreeGText << DcpNetwork::GSMText;
    networkModeComboBox->addItems(networkModeValues);    
    //networkIf->networkModeValueRequired();
    networkSelectionComboBox = new DuiComboBox(this);
    QStringList networkSelectionValues;
    networkSelectionValues << DcpNetwork::AutomaticText << DcpNetwork::ManualText;
    networkSelectionComboBox->addItems(networkSelectionValues);    
    //networkIf->networkSelectionValueRequired();
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
    connect(enableRoamingButton, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(enableRoamingUpdatesButton, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(dataCounterButton, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(networkModeComboBox, SIGNAL(currentIndexChanged(QString)), networkIf, SLOT(setNetworkModeValue(QString)));
    connect(networkSelectionComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(networkSelectionValueChanged(QString)));
    connect(roamingContainer, SIGNAL(headerClicked()), roamingContainer, SLOT(toggleExpand()));
    connect(networkContainer, SIGNAL(headerClicked()), networkContainer, SLOT(toggleExpand()));

    signalMapper->setMapping(phoneNetworkButton, DcpNetwork::PhoneNetworkText);
    signalMapper->setMapping(enableRoamingButton, DcpNetwork::EnableRoamingText);
    signalMapper->setMapping(enableRoamingUpdatesButton, DcpNetwork::EnableRoamingUpdatesText);
    signalMapper->setMapping(dataCounterButton, DcpNetwork::DataCounterText);
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(buttonPressed(QString)));
    //DuiSceneManager::instance()->setOrientationAngle((Dui::OrientationAngle) 90);

    this->setLayout(mainLayout);    
}

void NetworkWidget::toggleSettings(bool toggle)
{
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

void NetworkWidget::toggleEnableRoamingUpdates(bool toggle)
{
    if(toggle) {        
        if(roamingLandscapeLayoutPolicy->indexOf(roamingRightLayoutWidget) == -1) {
            roamingLandscapeLayoutPolicy->addItem(roamingRightLayoutWidget, Qt::AlignLeft);
            roamingPortraitLayoutPolicy->addItem(roamingRightLayoutWidget, Qt::AlignLeft);
        }
    }
    else {       
        if(enableRoamingUpdatesButton->isChecked()) {
            enableRoamingUpdatesButton->setChecked(false);
            networkIf->setEnableRoamingUpdatesValue(false);
        }        
        if(roamingLandscapeLayoutPolicy->indexOf(roamingRightLayoutWidget) != -1) {
            roamingLandscapeLayoutPolicy->removeItem(roamingRightLayoutWidget);
            roamingPortraitLayoutPolicy->removeItem(roamingRightLayoutWidget);            
        }        
    }
}

void NetworkWidget::initRoamingRightLayoutWidget()
{
    qDebug() << "INIT\n\n\n";
    if(roamingRightLayoutWidget == NULL) {

    }
}

void NetworkWidget::initPhoneNetworkButton(bool toggle)
{
    phoneNetworkButton->setChecked(toggle);
    toggleSettings(toggle);
}

void NetworkWidget::initEnableRoamingButton(bool toggle)
{
    enableRoamingButton->setChecked(toggle);
    toggleEnableRoamingUpdates(toggle);
}

void NetworkWidget::initEnableRoamingUpdatesButton(bool toggle)
{    
    enableRoamingUpdatesButton->setChecked(toggle);
}

void NetworkWidget::buttonPressed(const QString &text)
{   
    //NOTE: DuiButton->isChecked() method returns the state before the press at this point    

    if(text == DcpNetwork::PhoneNetworkText) {
        bool checked = !phoneNetworkButton->isChecked();
        networkIf->setPhoneNetworkValue(checked);
        toggleSettings(checked);
    }

    else if(text == DcpNetwork::EnableRoamingText) {
        bool checked = !enableRoamingButton->isChecked();

        networkIf->setEnableRoamingValue(checked);
        toggleEnableRoamingUpdates(checked);        
    }

    else if(text == DcpNetwork::EnableRoamingUpdatesText)
        networkIf->setEnableRoamingUpdatesValue(!enableRoamingUpdatesButton->isChecked());

    else if(text == DcpNetwork::DataCounterText)
        qDebug() << "Show dialog";
}

void NetworkWidget::networkSelectionValueChanged(const QString &value)
{
    if(value == DcpNetwork::ManualText) {
        networkIf->availableNetworksRequired();
    }
    else {
        toggleAvailableNetworks(QStringList(), false);
    }
    //networkIf->setNetworkModeValue(value);
}

void NetworkWidget::toggleAvailableNetworks(const QStringList &networks, bool toggle)
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
                    if(i==0) { //set the first one by default
                        button->setChecked(true);
                        networkIf->setNetworkSelectionValue(button->text());
                    }
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
