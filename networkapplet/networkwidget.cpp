#include "networkwidget.h"
#include "networktranslation.h"
#include "networkcontainer.h"
#include "networkdbusinterface.h"
#include "dcpnetwork.h"

#include <DuiButton>
#include <DuiContainer>
#include <DuiDialog>
#include <DuiGridLayoutPolicy>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiStylableWidget>
#include <DuiWidgetController>
#include <QDebug>

NetworkWidget::NetworkWidget(QGraphicsWidget *parent) :
        DcpWidget(parent),
        phoneNetworkButton(new DuiButton(this)),
        roamingButton(new DuiButton(this)),
        roamingUpdatesButton(new DuiButton(this)),
        dataCounterButton(new DuiButton(DcpNetwork::DataCounterText, this)),
        contentLayoutPolicy(NULL),
        roamingUpdatesLabel(new DuiLabel(DcpNetwork::RoamingUpdatesText, this)),
        networkSelected(false),
        networkContainer(new NetworkContainer(this)),
        networkIf(new NetworkDBusInterface())
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
    // widgets
    phoneNetworkButton->setCheckable(true);
    phoneNetworkButton->setObjectName("basicNetworkButton");
    roamingButton->setCheckable(true);
    roamingButton->setObjectName("basicNetworkButton");
    roamingUpdatesButton->setCheckable(true);
    roamingUpdatesButton->setObjectName("basicNetworkButton");
    dataCounterButton->setObjectName("dataCounterButton");

    // main container
    DuiLayout *contentLayout = new DuiLayout();
    contentLayoutPolicy = new DuiGridLayoutPolicy(contentLayout);
    contentLayoutPolicy->addItemAtPosition(new DuiLabel(DcpNetwork::PhoneNetworkText, this), 0, 0);
    contentLayoutPolicy->addItemAtPosition(phoneNetworkButton, 0, 1);
    contentLayoutPolicy->addItemAtPosition(new DuiLabel(DcpNetwork::RoamingText, this), 1, 0);
    contentLayoutPolicy->addItemAtPosition(roamingButton, 1, 1);
    contentLayoutPolicy->addItemAtPosition(dataCounterButton, 3, 0, 1, 2);    
    contentLayoutPolicy->setSpacing(10);   
    DuiContainer *mainContainer = new DuiContainer(this);
    mainContainer->centralWidget()->setLayout(contentLayout);

    // connect the value receive signals
    connect(networkIf, SIGNAL(phoneNetworkValueReceived(bool)), this, SLOT(initPhoneNetworkButton(bool)));
    connect(networkIf, SIGNAL(roamingValueReceived(bool)), this, SLOT(initRoamingButton(bool)));
    connect(networkIf, SIGNAL(roamingUpdatesValueReceived(bool)), this, SLOT(initRoamingUpdatesButton(bool)));
    connect(networkIf, SIGNAL(networkSelected(bool)), this, SLOT(toggleNetworkSelected(bool)));
    connect(networkIf, SIGNAL(networkModeValuesReceived(int, QStringList)), networkContainer, SLOT(initModeComboBox(int, QStringList)));
    connect(networkIf, SIGNAL(networkSelectionValuesReceived(int, int, QStringList)), networkContainer, SLOT(initSelectionComboBox(int, int, QStringList)));
    connect(networkIf, SIGNAL(availableNetworksReceived(int, QStringList, bool)), networkContainer, SLOT(toggleAvailableNetworks(int, QStringList, bool)));

    // catch user actions
    connect(phoneNetworkButton, SIGNAL(toggled(bool)), this, SLOT(toggleNetworkSettings(bool)));
    connect(phoneNetworkButton, SIGNAL(toggled(bool)), networkIf, SLOT(setPhoneNetworkValue(bool)));
    connect(roamingButton, SIGNAL(toggled(bool)), this, SLOT(toggleRoamingUpdatesButton(bool)));
    connect(roamingButton, SIGNAL(toggled(bool)), networkIf, SLOT(setRoamingValue(bool)));
    connect(roamingUpdatesButton, SIGNAL(toggled(bool)), networkIf, SLOT(setRoamingUpdatesValue(bool)));
    connect(dataCounterButton, SIGNAL(pressed()), this, SLOT(dataCounterButtonPressed()));
    connect(networkContainer, SIGNAL(networkModeChanged(QString)), networkIf, SLOT(setNetworkModeValue(QString)));
    connect(networkContainer, SIGNAL(networkSelectionChanged(QString)), networkIf, SLOT(setNetworkModeValue(QString)));
    connect(networkContainer, SIGNAL(availableNetworkSelected(QString)), networkIf, SLOT(setSelectedNetworkValue(QString)));

     // send value requests over dbus
    networkIf->phoneNetworkValueRequired();
    networkIf->roamingValueRequired();
    networkIf->roamingUpdatesValueRequired();    
    networkIf->networkModeValuesRequired();
    networkIf->networkSelectionValuesRequired();

    // mainLayout
    DuiLayout *mainLayout = new DuiLayout(this);
    DuiLinearLayoutPolicy *mainLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    mainLayoutPolicy->addItem(mainContainer);
    this->setLayout(mainLayout);
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

void NetworkWidget::toggleNetworkSettings(bool toggle)
{
    qDebug() << "NetworkWidget::toggleNetworkSettings(" << toggle << ")";    

    if(toggle)
        contentLayoutPolicy->addItemAtPosition(networkContainer, 4, 0, 1, 2);
    else
        contentLayoutPolicy->removeItem(networkContainer);

}

void NetworkWidget::toggleRoamingUpdatesButton(bool toggle)
{
    if(toggle) {
        contentLayoutPolicy->addItemAtPosition(roamingUpdatesLabel, 2, 0);
        contentLayoutPolicy->addItemAtPosition(roamingUpdatesButton, 2, 1);
    }
    else {
        contentLayoutPolicy->removeItem(roamingUpdatesLabel);
        contentLayoutPolicy->removeItem(roamingUpdatesButton);
    }
}



void NetworkWidget::dataCounterButtonPressed()
{   
    qDebug() << "Show dialog";
}




void NetworkWidget::toggleNetworkSelected(bool toggle)
{
    qDebug() << "NetworkWidget::toggleNetworkSelected(" << toggle << ")";
    networkSelected = toggle;
}
