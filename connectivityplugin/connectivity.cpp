#include "connectivity.h"
#include "phonenetworkcontainer.h"
#include "offlinecontainer.h"
#include "systemuigconf.h"

#include <DuiControlPanelIf>
#include <DuiContainer>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiLocale>
#include <DuiNotification>
#include <DuiStatusIndicatorMenuPluginInterface>
#include <DuiTheme>

#include <QGraphicsLinearLayout>
#include <QDebug>


const QString cssDir = "/usr/share/duistatusindicatormenu/themes/style/";

/*
    TODO:
    1) Network API needed to toggle the phone network on/off  
*/

Connectivity::Connectivity(DuiStatusIndicatorMenuInterface &statusIndicatorMenu, QGraphicsItem *parent) :
        DuiWidget(parent),
        statusIndicatorMenu(statusIndicatorMenu),        
        networkContainer(NULL),        
        offlineContainer(NULL),
        bluetoothContainer(NULL),
        layoutPolicy(NULL)
{
    DuiTheme::loadCSS(cssDir + "connectivity.css");

    // offline container
    SystemUIGConf systemUIGConf;
    offlineContainer = new OfflineContainer(systemUIGConf.value(SystemUIGConf::OfflineModeKey, QVariant(false)).toBool());
    connect(offlineContainer, SIGNAL(toggled(bool)), this, SLOT(toggleOfflineMode(bool)));

    // network container
    networkContainer = new PhoneNetworkContainer();
    connect(networkContainer, SIGNAL(headerClicked()), this, SLOT(showNetworkModificationPage()));


    ///////////////////////// PLACE HOLDERS /////////////////////////
    // dummy internet container
    DuiContainer *internetContainer = createDummyContainer(trid("NOT DEFINED", "Internet connection"));

    // dummy bluetooth container
    bluetoothContainer = createDummyContainer(trid("NOT DEFINED", "Bluetooth"));

    // dummy location container
    DuiContainer *locationContainer = createDummyContainer(trid("NOT DEFINED", "Show location"));
     ///////////////////////// PLACE HOLDERS /////////////////////////


    // Create the main container
    DuiContainer *mainContainer = new DuiContainer();
    DuiWidget *widget = new DuiWidget();
    DuiLayout *layout = new DuiLayout();    
    widget->setLayout(layout);
    mainContainer->setTitle(trid("qtn_stat_menu_connectivity", "Connectivity"));
    mainContainer->setCentralWidget(widget);

    // insert widgets
    layoutPolicy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    layoutPolicy->addItem(offlineContainer, Qt::AlignLeft);    
    layoutPolicy->addItem(internetContainer, Qt::AlignLeft);    
    layoutPolicy->addItem(locationContainer, Qt::AlignLeft);
    if(!offlineContainer->offlineMode())
        toggleContainersVisibility(true);

    // Create the main layout
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Vertical);    
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addItem(mainContainer);
    setLayout(mainLayout);

}

Connectivity::~Connectivity()
{    
}

void Connectivity::toggleContainersVisibility(bool toggle)
{
    qDebug() << "Connectivity::toggleContainersVisibility(" << toggle << ")";
    if(toggle) {
        layoutPolicy->insertItem(networkContainer, 1, Qt::AlignLeft);
        layoutPolicy->insertItem(bluetoothContainer, 3, Qt::AlignLeft);
    }
    else {
        layoutPolicy->removeItem(networkContainer);
        layoutPolicy->removeItem(bluetoothContainer);
    }
}

void Connectivity::showNetworkModificationPage()
{
    // instantiate the interface
    DuiControlPanelIf cpIf;
    // check the interface is valid
    if (!cpIf.isValid())
        return;
    cpIf.appletPage("Network");
}

void Connectivity::toggleOfflineMode(bool toggle)
{
    qDebug() << "Connectivity::toggleOfflineMode(" << toggle << ")";

    // store the state of offline mode to gconf
    SystemUIGConf systemUIGConf;
    systemUIGConf.setValue(SystemUIGConf::OfflineModeKey, QVariant(toggle));

    if(toggle) {
        // store the current state of phone network to gconf
        systemUIGConf.setValue(SystemUIGConf::OfflinePhoneNetworkOnKey, QVariant(networkContainer->phoneNetworkOn()));

        // turn off the phone network
        networkContainer->togglePhoneNetwork(false);        

        //show notification
        DuiNotification("", "", trid("qtn_offline_entering", "Closing all connections. Switching to offline mode."));       
    }
    else {
        // restore the previous state of phone network from gconf
        bool networkWasOn = systemUIGConf.value(SystemUIGConf::OfflinePhoneNetworkOnKey, QVariant(false)).toBool();

        // turn on the phone network if needed
        if(networkWasOn)
            networkContainer->togglePhoneNetwork(true);        

        //show notification
        DuiNotification("", "", trid("qtn_offline_exiting", "Restoring all connections. Exiting offline mode."));                
    }

    // if offline mode is switched on, the visibility is switched off and vice versa
    toggleContainersVisibility(!toggle);
}

///////////////////////// TEMP /////////////////////////
DuiContainer* Connectivity::createDummyContainer(const QString &text)
{
    DuiContainer *container = new DuiContainer();
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    DuiLabel *headerLabel = new DuiLabel(text);
    headerLabel->setObjectName("connectivityLabel1");
    layout->addItem(headerLabel);
    DuiLabel *label = new DuiLabel("Placeholder");
    label->setObjectName("connectivityLabel2");
    layout->addItem(label);
    container->centralWidget()->setLayout(layout);
    return container;
}
///////////////////////// PLACE HOLDERS /////////////////////////
