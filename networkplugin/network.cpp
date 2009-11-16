#include "network.h"
#include "networkbusinesslogic.h"

#include <DuiApplicationIfProxy>
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
const QString CONTROL_PANEL_SERVICE_NAME = "com.nokia.DuiControlPanel";

Network::Network(DuiStatusIndicatorMenuInterface &statusIndicatorMenu, QGraphicsItem *parent) :
        DuiWidget(parent),
        logic(NULL),
        statusIndicatorMenu(statusIndicatorMenu),        
        toggleButton(NULL),
        signalStrengthImage(NULL),
        networkLabel(NULL),
        operatorLabel(NULL)
{
    qDebug() << Q_FUNC_INFO;

    DuiTheme::loadCSS(cssDir + "networkplugin.css");

    // init widgets    
    networkLabel = new DuiLabel(trid("qtn_cell_phone_network", "Phone network"));
    networkLabel->setObjectName("networkPluginLabel");
    toggleButton = new DuiButton();
    toggleButton->setCheckable(true);
    toggleButton->setObjectName("toggleButton");
    //signalStrengthImage = new DuiImage();
    operatorLabel = new DuiLabel();
    operatorLabel->setObjectName("operatorLabel");
    toggleWidgetsVisibility(false);

    // insert widgets
    DuiLayout *layout = new DuiLayout();
    DuiGridLayoutPolicy *layoutPolicy = new DuiGridLayoutPolicy(layout);
    layoutPolicy->addItemAtPosition(networkLabel, 0, 0, 1, 2);
    layoutPolicy->addItemAtPosition(toggleButton, 0, 2, 2, 1);
    //layoutPolicy->addItemAtPosition(signalStrengthImage, 1, 0, 1, 1);
    layoutPolicy->setColumnStretchFactor(1, 1);
    layoutPolicy->addItemAtPosition(operatorLabel, 1, 1, 1, 1);
    DuiContainer *container = new DuiContainer();
    container->centralWidget()->setLayout(layout);

    logic = new NetworkBusinessLogic();
    connect(logic, SIGNAL(networkOperatorChanged(QString)), operatorLabel, SLOT(setText(QString)));
    connect(logic, SIGNAL(networkIconChanged(QString)), this, SLOT(updateButtonIcon(QString)));
    connect(toggleButton, SIGNAL(toggled(bool)), logic, SLOT(toggleNetwork(bool)));    
    //connect(logic, SIGNAL(signalStrengthIconChanged(QString)), signalStrengthImage, SLOT(setImage(QString)));

    updateButton(logic->networkEnabled());
    updateButtonImage(logic->networkIcon());
    operatorLabel->setText(logic->currentOperator());
    //signalStrengthImage->setImage(logic->signalStrengthIcon());

    // mainLayout    
    DuiLayout *mainLayout = new DuiLayout();
    DuiLinearLayoutPolicy *mainLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    mainLayoutPolicy->addItem(container);
    this->setLayout(mainLayout);    
}

Network::~Network()
{
    qDebug() << Q_FUNC_INFO;
    delete logic;
    logic = NULL;    
}

void Network::updateButton(bool toggle)
{
    qDebug() << Q_FUNC_INFO;
    toggleButton->setChecked(toggle);    
    toggleWidgetsVisibility(toggle);
}

void Network::updateButtonImage(const QString &icon)
{
    qDebug() << Q_FUNC_INFO;
    toggleButton->setIconID(icon);
}

void Network::toggleWidgetsVisibility(bool toggle)
{
    qDebug() << Q_FUNC_INFO;
    //signalStrengthImage->setVisible(toggle);
    operatorLabel->setVisible(toggle);
}

/*
void Network::buttonClicked()
{
    qDebug() << Q_FUNC_INFO;
    DuiApplicationIfProxy duiApplicationIfProxy(CONTROL_PANEL_SERVICE_NAME, this);
    if (duiApplicationIfProxy.connection().isConnected())
        duiApplicationIfProxy.launch();
    else
        qWarning() << "Could not launch" << CONTROL_PANEL_SERVICE_NAME << "- DBus not connected?";
}
*/
