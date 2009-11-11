#include "network.h"
#include "networkdbusinterface.h"

#include <DuiButton>
#include <DuiContainer>
#include <DuiGridLayoutPolicy>
#include <DuiImage>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiLocale>
#include <DuiStatusIndicatorMenuPluginInterface>
#include <DuiTheme>

#include <QDebug>

const QString cssDir = "/usr/share/duistatusindicatormenu/themes/style/";

Network::Network(DuiStatusIndicatorMenuInterface &statusIndicatorMenu, QGraphicsItem *parent) :
        DuiWidget(parent),
        networkIf(NULL),
        statusIndicatorMenu(statusIndicatorMenu),
        toggleButton(NULL),
        signalStrengthImage(NULL),
        networkLabel(NULL),
        operatorLabel(NULL)
{
    DuiTheme::loadCSS(cssDir + "networkplugin.css");

    networkIf = new NetworkDBusInterface();

    // init widgets
    networkLabel = new DuiLabel(trid("qtn_cell_phone_network", "Phone network"));
    networkLabel->setObjectName("networkLabel");
    toggleButton = new DuiButton();
    toggleButton->setCheckable(true);
    toggleButton->setObjectName("toggleButton");
    signalStrengthImage = new DuiImage();
    operatorLabel = new DuiLabel("Elisa");
    operatorLabel->setObjectName("operatorLabel");
    toggleWidgetsVisibility(true);

    // insert widgets
    DuiLayout *layout = new DuiLayout();
    DuiGridLayoutPolicy *layoutPolicy = new DuiGridLayoutPolicy(layout);
    layoutPolicy->addItemAtPosition(networkLabel, 0, 0, 1, 2);
    layoutPolicy->addItemAtPosition(toggleButton, 0, 2, 2, 1);
    layoutPolicy->addItemAtPosition(signalStrengthImage, 1, 0, 1, 1);    
    layoutPolicy->setColumnStretchFactor(1, 1);
    layoutPolicy->addItemAtPosition(operatorLabel, 1, 1, 1, 1);
    DuiContainer *container = new DuiContainer();
    container->centralWidget()->setLayout(layout);

    // connect networkIf responses
    connect(networkIf, SIGNAL(phoneNetworkValueReceived(bool)), this, SLOT(updateButton(bool)));
    connect(networkIf, SIGNAL(networkIconValueReceived(QString)), this, SLOT(updateButtonIcon(QString)));
    connect(networkIf, SIGNAL(currentOperatorValueReceived(QString)), operatorLabel, SLOT(setText(QString)));        
    connect(networkIf, SIGNAL(signalStrengthIconValueReceived(QString)), signalStrengthImage, SLOT(setImage(QString)));

    // request the values for the widgets
    networkIf->phoneNetworkValueRequired();
    networkIf->networkIconValueRequired();
    networkIf->currentOperatorValueRequired();        
    networkIf->signalStrengthIconValueRequired();

    // mainLayout    
    DuiLayout *mainLayout = new DuiLayout();
    DuiLinearLayoutPolicy *mainLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    mainLayoutPolicy->addItem(container);
    this->setLayout(mainLayout);
}

Network::~Network()
{
    delete networkIf;
    networkIf = NULL;
}

void Network::updateButton(bool toggle)
{
    toggleButton->setChecked(toggle);    
    toggleWidgetsVisibility(toggle);
}

void Network::updateButtonImage(const QString &icon)
{
    toggleButton->setIconID(icon);
}

void Network::toggleWidgetsVisibility(bool toggle)
{
    signalStrengthImage->setVisible(toggle);
    operatorLabel->setVisible(toggle);
}
