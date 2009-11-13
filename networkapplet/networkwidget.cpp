#include "networkwidget.h"
#include "networkbusinesslogic.h"
#include "networktranslation.h"
#include "networkcontainer.h"
#include "roamingcontainer.h"
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
        logic(NULL),
        phoneNetworkButton(NULL),        
        dataCounterButton(NULL),
        contentLayoutPolicy(NULL),        
        networkSelected(false),
        networkContainer(NULL),
        roamingContainer(NULL)
{    
    setReferer(DcpNetwork::None);
    initWidget();
}

NetworkWidget::~NetworkWidget()
{
     delete logic;
     logic = NULL;
}

bool NetworkWidget::back()
{
    //check that the user has selected a network before closing the window
    if(logic->manualSelectionRequired() && !networkContainer->operatorSelected()) {
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
        networkContainer->setDefaultSelection(logic->defaultNetworkSelectionValue());
        dlg->accept();
    }
    else
        dlg->reject();

    dlg->deleteLater();
    dlg = NULL;        
}

void NetworkWidget::initWidget()
{
    logic = new NetworkBusinessLogic(this);

    // widgets
    phoneNetworkButton = new DuiButton(this);
    phoneNetworkButton->setCheckable(true);
    phoneNetworkButton->setObjectName("basicNetworkButton");
    dataCounterButton = new DuiButton(DcpNetwork::DataCounterText, this);
    dataCounterButton->setObjectName("dataCounterButton");
    networkContainer = new NetworkContainer(this); 
    roamingContainer = new RoamingContainer(this);            

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

    // set values
    initPhoneNetworkButton(logic->networkEnabled());
    updateNetworkIcon(logic->networkIcon());
    roamingContainer->initRoamingButton(logic->roamingEnabled());
    roamingContainer->initRoamingUpdatesButton(logic->roamingUpdatesEnabled());
    networkContainer->initModeComboBox(logic->selectedNetworkMode(), logic->networkModes());
    networkContainer->initSelectionComboBox(logic->selectedNetworkSelectionValue(), logic->networkSelectionValues());

    // connect the value receive signals
    connect(logic, SIGNAL(availableNetworkOperators(int, QStringList, bool)), networkContainer, SLOT(toggleAvailableOperators(int, QStringList, bool)));
    connect(logic, SIGNAL(networkIconChanged(QString)), this, SLOT(updateNetworkIcon(QString)));
    connect(logic, SIGNAL(roamingUpdatesValueChanged(bool)), roamingContainer, SLOT(toggleRoamingUpdates(bool)));

    // catch user actions
    connect(phoneNetworkButton, SIGNAL(toggled(bool)), this, SLOT(toggleNetworkSettings(bool)));
    connect(phoneNetworkButton, SIGNAL(toggled(bool)), logic, SLOT(toggleNetwork(bool)));
    connect(dataCounterButton, SIGNAL(pressed()), this, SLOT(dataCounterButtonPressed()));
    connect(networkContainer, SIGNAL(networkModeChanged(QString)), logic, SLOT(setNetworkMode(QString)));
    connect(networkContainer, SIGNAL(networkSelectionChanged(QString)), logic, SLOT(setNetworkSelectionValue(QString)));
    connect(networkContainer, SIGNAL(availableOperatorSelected(int)), logic, SLOT(selectOperator(int)));

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

void NetworkWidget::updateNetworkIcon(const QString &value)
{
    phoneNetworkButton->setIconID(value);
}

void NetworkWidget::dataCounterButtonPressed()
{   
    qDebug() << "Show dialog";
}
