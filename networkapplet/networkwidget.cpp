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
    //create dbus if
    //networkIf = new NetworkDBusInterface();

    // catch network If actions
    //connect(networkIf, SIGNAL(someValueReceived(QString)), this, SLOT(jake(QString)));

    /*
     * phoneNetworkLayoutWidget
     */    
    phoneNetworkButton = new DuiButton();
    phoneNetworkButton->setObjectName("basicNetworkButton");
    //networkIf->phoneNetworkValueRequired();

    DuiLayout *phoneNetworkLayout = new DuiLayout();
    DuiLinearLayoutPolicy *phoneNetworkLayoutPolicy = new DuiLinearLayoutPolicy(phoneNetworkLayout, Qt::Horizontal);
    phoneNetworkLayoutPolicy->addItem(new DuiLabel(DcpNetwork::PhoneNetworkText), Qt::AlignLeft);
    phoneNetworkLayoutPolicy->addItem(phoneNetworkButton, Qt::AlignRight);

    DuiStylableWidget *phoneNetworkLayoutWidget = new DuiStylableWidget();
    phoneNetworkLayoutWidget->setObjectName("networkLayoutWidget1");
    phoneNetworkLayoutWidget->setLayout(phoneNetworkLayout);    

    /*
     * roamingLayoutWidgetContainer
     */        
    enableRoamingButton = new DuiButton();
    enableRoamingButton->setObjectName("basicNetworkButton");
    //networkIf->enableRoamingValueRequired();
    enableRoamingUpdatesButton = new DuiButton();
    enableRoamingButton->setObjectName("basicNetworkButton");
    //networkIf->enableRoamingUpdatesValueRequired();

    DuiLayout *roamingLayout = new DuiLayout();
    DuiLinearLayoutPolicy *roamingLayoutPolicy = new DuiLinearLayoutPolicy(roamingLayout, Qt::Horizontal);
    roamingLayoutPolicy->addItem(new DuiLabel(DcpNetwork::EnableRoamingText), Qt::AlignLeft);
    roamingLayoutPolicy->addItem(enableRoamingButton, Qt::AlignRight);
    roamingLayoutPolicy->addItem(new DuiLabel(DcpNetwork::EnableRoamingUpdatesText), Qt::AlignLeft);
    roamingLayoutPolicy->addItem(enableRoamingUpdatesButton, Qt::AlignRight);

    DuiStylableWidget *roamingLayoutWidget = new DuiStylableWidget();
    roamingLayoutWidget->setObjectName("networkLayoutWidget2");
    roamingLayoutWidget->setLayout(roamingLayout);

    roamingLayoutWidgetContainer = new DuiContainer();
    roamingLayoutWidgetContainer->setTitle(DcpNetwork::RoamingText);
    roamingLayoutWidgetContainer->setCentralWidget(roamingLayoutWidget);

    /*
     * dataCounterLayoutWidget
     */
    dataCounterButton = new DuiButton(DcpNetwork::DataCounterText);
    dataCounterButton->setObjectName("dataCounterButton");

    DuiLayout *dataCounterLayout = new DuiLayout();
    DuiLinearLayoutPolicy *dataCounterLayoutPolicy = new DuiLinearLayoutPolicy(dataCounterLayout, Qt::Horizontal);
    dataCounterLayoutPolicy->addItem(dataCounterButton, Qt::AlignLeft);

    DuiStylableWidget *dataCounterLayoutWidget = new DuiStylableWidget();
    dataCounterLayoutWidget->setObjectName("networkLayoutWidget3");
    dataCounterLayoutWidget->setLayout(dataCounterLayout);

    /*
     * availabaleNetworksWidgetContainer
     */
    DuiLayout *availableNetworksLayout = new DuiLayout();
    availableNetworksLayoutPolicy = new DuiLinearLayoutPolicy(availableNetworksLayout, Qt::Vertical);

    DuiStylableWidget *availableNetworksLayoutWidget = new DuiStylableWidget();
    availableNetworksLayoutWidget->setObjectName("networkLayoutWidget4");
    availableNetworksLayoutWidget->setLayout(availableNetworksLayout);
    //networkIf->availableNetworksRequired();

    availableNetworksContainer = new DuiContainer();
    availableNetworksContainer->setCentralWidget(availableNetworksLayoutWidget);

    /*
     * networkLayoutWidgetContainer
     */
    networkModeComboBox = new DuiComboBox();
    QStringList networkModeValues;
    networkModeValues << DcpNetwork::DualText << DcpNetwork::ThreeGText << DcpNetwork::GSMText;
    //networkIf->networkModeValueRequired();
    networkSelectionComboBox = new DuiComboBox();
    QStringList networkSelectionValues;
    networkSelectionValues << DcpNetwork::AutomaticText << DcpNetwork::ManualText;
    //networkIf->networkSelectionValueRequired();

    DuiLayout *networkComboBoxLayout = new DuiLayout();
    DuiGridLayoutPolicy *networkComboBoxLayoutPolicy = new DuiGridLayoutPolicy(networkComboBoxLayout);
    networkComboBoxLayoutPolicy->addItemAtPosition(new DuiLabel(DcpNetwork::NetworkModeText), 0, 0);
    networkComboBoxLayoutPolicy->addItemAtPosition(new DuiLabel(DcpNetwork::NetworkSelectionText), 0, 1);
    networkComboBoxLayoutPolicy->addItemAtPosition(networkModeComboBox, 1, 0);
    networkComboBoxLayoutPolicy->addItemAtPosition(networkSelectionComboBox, 1, 1);

    DuiLayout *networkLayout = new DuiLayout();
    DuiLinearLayoutPolicy *networkLayoutPolicy = new DuiLinearLayoutPolicy(networkLayout, Qt::Vertical);
    networkLayoutPolicy->addItem(networkComboBoxLayout, Qt::AlignLeft);
    networkLayoutPolicy->addItem(new DuiLabel(DcpNetwork::AvailableNetworksText), Qt::AlignLeft);
    networkLayoutPolicy->addItem(availableNetworksContainer, Qt::AlignLeft);

    DuiStylableWidget *networkLayoutWidget = new DuiStylableWidget();
    networkLayoutWidget->setObjectName("networkLayoutWidget5");
    networkLayoutWidget->setLayout(networkLayout);

    networkLayoutWidgetContainer = new DuiContainer();
    networkLayoutWidgetContainer->setCentralWidget(networkLayoutWidget);

    /* 
     * MainLayout     
     */        
    DuiLayout *landscapeLayout = new DuiLayout();
    DuiLinearLayoutPolicy *landscapeLayoutPolicy = new DuiLinearLayoutPolicy(landscapeLayout, Qt::Vertical);
    landscapeLayoutPolicy->addItem(phoneNetworkLayoutWidget);
    landscapeLayoutPolicy->addItem(roamingLayoutWidgetContainer);
    landscapeLayoutPolicy->addItem(dataCounterLayoutWidget);
    landscapeLayoutPolicy->addItem(networkLayoutWidgetContainer);
    landscapeLayoutPolicy->setSpacing(20);
    DuiStylableWidget *landscapeLayoutWidget = new DuiStylableWidget();
    landscapeLayoutWidget->setObjectName("networkLayoutWidget6");
    landscapeLayoutWidget->setLayout(landscapeLayout);

    DuiLayout *mainLayout = new DuiLayout(this);
    DuiLinearLayoutPolicy *mainLandscapeLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    mainLandscapeLayoutPolicy->addItem(landscapeLayoutWidget);

    mainLayout->setLandscapePolicy(mainLandscapeLayoutPolicy);


    DuiLinearLayoutPolicy *portraitLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    mainLayout->setPortraitPolicy(portraitLayoutPolicy);

    // catch user actions
    //connect(brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(brightnessSliderValueChanged(int)));    
    this->setLayout(mainLayout);    
}
