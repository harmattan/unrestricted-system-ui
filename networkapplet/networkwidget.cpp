#include "networkwidget.h"
#include "networktranslation.h"
#include "networkcontainer.h"
#include "roamingcontainer.h"
#include "networkdbusinterface.h"
#include "dcpnetwork.h"

#include <DuiButton>
#include <DuiContainer>
#include <DuiDialog>
#include <DuiGridLayoutPolicy>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiList>
#include <DuiStylableWidget>
#include <DuiWidgetController>
#include <QDebug>

/* TODO:
   1) Show data counter dialog
   2) Update the list so that the selected operator is indicated somehow
   3) Update the list so that when selection fails teh list is updated accordingly
*/

NetworkWidget::NetworkWidget(QGraphicsWidget *parent) :
        DcpWidget(parent),
        phoneNetworkButton(NULL),        
        dataCounterButton(NULL),
        contentLayoutPolicy(NULL),        
        networkSelected(false),
        networkContainer(NULL),
        roamingContainer(NULL),
        networkIf(NULL)
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
        networkContainer->setDefaultSelection();        
        dlg->accept();
    }
    else
        dlg->reject();

    dlg->deleteLater();
    dlg = NULL;        
}

void NetworkWidget::initWidget()
{
    phoneNetworkButton = new DuiButton(this);
    dataCounterButton = new DuiButton(DcpNetwork::DataCounterText, this);
    networkContainer = new NetworkContainer(this); 
    roamingContainer = new RoamingContainer(this);
    networkIf = new NetworkDBusInterface();       

    // widgets
    phoneNetworkButton->setCheckable(true);
    phoneNetworkButton->setObjectName("basicNetworkButton");    
    dataCounterButton->setObjectName("dataCounterButton");

    // main container
    DuiLayout *contentLayout = new DuiLayout();
    contentLayoutPolicy = new DuiGridLayoutPolicy(contentLayout);
    contentLayoutPolicy->addItemAtPosition(new DuiLabel(DcpNetwork::PhoneNetworkText, this), 0, 0);
    contentLayoutPolicy->addItemAtPosition(phoneNetworkButton, 0, 1);    
    contentLayoutPolicy->addItemAtPosition(roamingContainer, 1, 0, 1, 2);
    contentLayoutPolicy->addItemAtPosition(dataCounterButton, 2, 0, 1, 2);
    contentLayoutPolicy->setSpacing(10);   
    DuiContainer *mainContainer = new DuiContainer(this);
    mainContainer->centralWidget()->setLayout(contentLayout);

    // connect the value receive signals
    connect(networkIf, SIGNAL(phoneNetworkValueReceived(bool)), this, SLOT(initPhoneNetworkButton(bool)));
    connect(networkIf, SIGNAL(roamingValueReceived(bool)), roamingContainer, SLOT(initRoamingButton(bool)));
    connect(networkIf, SIGNAL(roamingUpdatesValueReceived(bool)), roamingContainer, SLOT(initRoamingUpdatesButton(bool)));
    connect(networkIf, SIGNAL(networkSelected(bool)), this, SLOT(toggleNetworkSelected(bool)));
    connect(networkIf, SIGNAL(networkModeValuesReceived(int, QStringList)), networkContainer, SLOT(initModeComboBox(int, QStringList)));
    connect(networkIf, SIGNAL(networkSelectionValuesReceived(int, int, QStringList)), networkContainer, SLOT(initSelectionComboBox(int, int, QStringList)));
    connect(networkIf, SIGNAL(availableNetworksReceived(int, QStringList, bool)), networkContainer, SLOT(toggleAvailableNetworks(int, QStringList, bool)));
    connect(networkIf, SIGNAL(networkIconValueReceived(QString)), this, SLOT(updateNetworkIcon(QString)));

    // catch user actions
    connect(phoneNetworkButton, SIGNAL(toggled(bool)), this, SLOT(toggleNetworkSettings(bool)));
    connect(phoneNetworkButton, SIGNAL(toggled(bool)), networkIf, SLOT(setPhoneNetworkValue(bool)));    
    connect(dataCounterButton, SIGNAL(pressed()), this, SLOT(dataCounterButtonPressed()));
    connect(networkContainer, SIGNAL(networkModeChanged(QString)), networkIf, SLOT(setNetworkModeValue(QString)));
    connect(networkContainer, SIGNAL(networkSelectionChanged(QString)), networkIf, SLOT(setNetworkSelectionValue(QString)));
    connect(networkContainer, SIGNAL(availableNetworkSelected(QString)), networkIf, SLOT(setSelectedNetworkValue(QString)));

     // send value requests over dbus
    networkIf->phoneNetworkValueRequired();
    networkIf->roamingValueRequired();
    networkIf->roamingUpdatesValueRequired();    
    networkIf->networkModeValuesRequired();
    networkIf->networkSelectionValuesRequired();
    networkIf->networkIconValueRequired();

    // mainLayout
    DuiLayout *mainLayout = new DuiLayout();
    DuiLinearLayoutPolicy *mainLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    mainLayoutPolicy->addItem(mainContainer);
    this->setLayout(mainLayout);
}

void NetworkWidget::initPhoneNetworkButton(bool toggle)
{
    phoneNetworkButton->setChecked(toggle);
    toggleNetworkSettings(toggle);
}

void NetworkWidget::toggleNetworkSettings(bool toggle)
{
    qDebug() << "NetworkWidget::toggleNetworkSettings(" << toggle << ")";    

    if(toggle)
        contentLayoutPolicy->addItemAtPosition(networkContainer, 3, 0, 1, 2);
    else
        contentLayoutPolicy->removeItem(networkContainer);

}

void NetworkWidget::dataCounterButtonPressed()
{   
    qDebug() << "Show dialog";
}

void NetworkWidget::toggleNetworkSelected(bool toggle)
{
    qDebug() << "NetworkWidget::toggleNetworkSelected(" << toggle << ")";

    //TODO update the DuiList

    networkSelected = toggle;
}

void NetworkWidget::updateNetworkIcon(const QString &value)
{
    phoneNetworkButton->setIconID(value);
}
