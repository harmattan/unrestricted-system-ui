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
    connect(networkIf, SIGNAL(availableNetworksReceived(QStringList)), this, SLOT(toggleAvailableNetworks(QStringList)));

    /*
     * phoneNetworkLayoutWidget
     */    
    phoneNetworkButton = new DuiButton(this);
    phoneNetworkButton->setObjectName("basicNetworkButton");
    //networkIf->phoneNetworkValueRequired();

    DuiLayout *phoneNetworkLayout = new DuiLayout();
    DuiLinearLayoutPolicy *phoneNetworkLayoutPolicy = new DuiLinearLayoutPolicy(phoneNetworkLayout, Qt::Horizontal);
    phoneNetworkLayoutPolicy->addItem(new DuiLabel(DcpNetwork::PhoneNetworkText), Qt::AlignLeft);
    phoneNetworkLayoutPolicy->addItem(phoneNetworkButton, Qt::AlignRight);

    DuiStylableWidget *phoneNetworkLayoutWidget = new DuiStylableWidget(this);
    //phoneNetworkLayoutWidget->setObjectName("networkLayoutWidget");
    phoneNetworkLayoutWidget->setLayout(phoneNetworkLayout);

    /*
     * roamingLayoutWidgetContainer
     */        
    enableRoamingButton = new DuiButton(this);
    enableRoamingButton->setObjectName("basicNetworkButton");
    //networkIf->enableRoamingValueRequired();
    enableRoamingUpdatesButton = new DuiButton(this);
    enableRoamingUpdatesButton->setObjectName("basicNetworkButton");
    //networkIf->enableRoamingUpdatesValueRequired();

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
    DuiStylableWidget *roamingRightLayoutWidget = new DuiStylableWidget(this);
    roamingRightLayoutWidget->setLayout(roamingRightLayout);
    roamingRightLayoutWidget->setObjectName("networkLayoutWidget2");

    DuiLayout *roamingLayout = new DuiLayout();
    DuiLinearLayoutPolicy *roamingLayoutPolicy = new DuiLinearLayoutPolicy(roamingLayout, Qt::Horizontal);
    roamingLayoutPolicy->addItem(roamingLeftLayoutWidget, Qt::AlignLeft);
    roamingLayoutPolicy->addItem(roamingRightLayoutWidget, Qt::AlignLeft);
    DuiStylableWidget *roamingLayoutWidget = new DuiStylableWidget(this);
    roamingLayoutWidget->setLayout(roamingLayout);

    roamingLayoutWidgetContainer = new DuiContainer(this);
    roamingLayoutWidgetContainer->setTitle(DcpNetwork::RoamingText);
    roamingLayoutWidgetContainer->setCentralWidget(roamingLayoutWidget);
    roamingLayoutWidgetContainer->setObjectName("networkLayoutWidgetContainer");

    /*
     * dataCounterButton
     */
    dataCounterButton = new DuiButton(DcpNetwork::DataCounterText, this);
    dataCounterButton->setObjectName("dataCounterButton");

    /*
     * availableNetworksContainer

    availableNetworkButtons = new DuiButtonGroup(this);

    DuiLayout *availableNetworksLayout = new DuiLayout();
    availableNetworksLayoutPolicy = new DuiLinearLayoutPolicy(availableNetworksLayout, Qt::Vertical);
    DuiStylableWidget *availableNetworksLayoutWidget = new DuiStylableWidget(this);
    availableNetworksLayoutWidget->setLayout(availableNetworksLayout);
    //availableNetworksLayoutWidget->setObjectName("networkLayoutWidget");
    availableNetworksContainer = new DuiContainer(this);
    availableNetworksContainer->setCentralWidget(availableNetworksLayoutWidget);
    //roamingLayoutWidgetContainer->setObjectName("networkLayoutWidgetContainer");
*/
    /*
     * networkLayoutWidgetContainer
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
    availableNetworksLabel = new DuiLabel(DcpNetwork::AvailableNetworksText, this);
    noAvailableNetworksLabel = new DuiLabel(DcpNetwork::NoAvailableNetworksText, this);

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
   // networkLayoutWidget->setObjectName("networkLayoutWidget1");

    networkLayoutWidgetContainer = new DuiContainer(this);
    networkLayoutWidgetContainer->setCentralWidget(networkLayoutWidget);
    networkLayoutWidgetContainer->setTitle(DcpNetwork::NetworkText);
    //networkLayoutWidgetContainer->setObjectName("networkLayoutWidgetContainer");

    /* 
     * contentLayoutWidgetContainer
     */        
    DuiLayout *contentLayout = new DuiLayout();
    DuiLinearLayoutPolicy *contentLayoutPolicy = new DuiLinearLayoutPolicy(contentLayout, Qt::Vertical);
    contentLayoutPolicy->addItem(phoneNetworkLayoutWidget);
    contentLayoutPolicy->addItem(roamingLayoutWidgetContainer);
    contentLayoutPolicy->addItem(dataCounterButton);
    contentLayoutPolicy->addItem(networkLayoutWidgetContainer);
    contentLayoutPolicy->setSpacing(10);

    DuiStylableWidget *contentLayoutWidget = new DuiStylableWidget(this);
    contentLayoutWidget->setLayout(contentLayout);
    contentLayoutWidget->setObjectName("networkLayoutWidget1");

    DuiContainer *contentLayoutWidgetContainer = new DuiContainer(this);
    contentLayoutWidgetContainer->setCentralWidget(contentLayoutWidget);
    //contentLayoutWidgetContainer->setObjectName("networkLayoutWidgetContainer");

    /*
     * mainLayout
     */
    DuiLayout *mainLayout = new DuiLayout(this);

    DuiLinearLayoutPolicy *landscapeLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    landscapeLayoutPolicy->addItem(contentLayoutWidgetContainer);
    mainLayout->setLandscapePolicy(landscapeLayoutPolicy);

    DuiLinearLayoutPolicy *portraitLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    portraitLayoutPolicy->addItem(contentLayoutWidgetContainer);
    mainLayout->setPortraitPolicy(portraitLayoutPolicy);

    // catch user actions
    signalMapper = new QSignalMapper(this);    
    connect(phoneNetworkButton, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(enableRoamingButton, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(enableRoamingUpdatesButton, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(dataCounterButton, SIGNAL(pressed()), signalMapper, SLOT(map()));
    connect(networkModeComboBox, SIGNAL(currentIndexChanged(QString)), networkIf, SLOT(setNetworkModeValue(QString)));
    connect(networkSelectionComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(networkSelectionValueChanged(QString)));

    signalMapper->setMapping(phoneNetworkButton, DcpNetwork::PhoneNetworkText);
    signalMapper->setMapping(enableRoamingButton, DcpNetwork::EnableRoamingText);
    signalMapper->setMapping(enableRoamingUpdatesButton, DcpNetwork::EnableRoamingUpdatesText);
    signalMapper->setMapping(dataCounterButton, DcpNetwork::DataCounterText);
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(buttonPressed(QString)));


    this->setLayout(mainLayout);    
}

void NetworkWidget::buttonPressed(const QString &text)
{
    if(text == DcpNetwork::PhoneNetworkText)
        networkIf->setPhoneNetworkValue(phoneNetworkButton->isChecked());
    else if(text == DcpNetwork::EnableRoamingText)
        networkIf->setEnableRoamingValue(enableRoamingButton->isChecked());
    else if(text == DcpNetwork::EnableRoamingUpdatesText)
        networkIf->setEnableRoamingUpdatesValue(enableRoamingUpdatesButton->isChecked());
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
            else
                networkLayoutPolicy->addItem(noAvailableNetworksLabel, Qt::AlignLeft);
        }       
    }
    else {
        if(availableNetworksButtonGroup->buttons().size() > 0) {
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
        else
            networkLayoutPolicy->removeItem(noAvailableNetworksLabel);
    }

}

void NetworkWidget::availableNetworksButtonPressed(DuiButton *button)
{
    networkIf->setNetworkSelectionValue(button->text());
}
