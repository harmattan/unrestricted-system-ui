#include "networkwidget.h"
#include "networkbusinesslogic.h"
#include "networktranslation.h"
#include "networkcontainer.h"
#include "dcpnetwork.h"

#include <QDebug>
#include <QGraphicsLinearLayout>

#include <DuiButton>
#include <DuiContainer>
#include <DuiDialog>
#include <DuiGridLayoutPolicy>
#include <DuiLabel>
#include <DuiLayout>

/* TODO:
   1) Show data counter dialog
   3) Update the list so that the selected operator is indicated somehow
   4) Update the list so that when selection fails teh list is updated accordingly
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
    toggleNetworkSettings(true); // just to make sure it will be deleted
}

bool NetworkWidget::back()
{
    //check that the user has selected a network before closing the window
    if (logic->manualSelectionRequired()) {
        qDebug() << "No selection made!\n\n\n";
        changeSelection();
        return false;
    }

    if (referer() > -1) { // emit the changewidget signal if there is a referer set
        emit changeWidget(referer());
        return false;
    } else
        return true; // back is handled by main window by default
}

void NetworkWidget::changeSelection()
{
    DuiDialog* dlg = new DuiDialog();
    dlg->setTitle(DcpNetwork::NoNetworkSelectedText);
    DuiButton* changeButton = dlg->addButton(DcpNetwork::ChangeText);
    dlg->exec();

    if (dlg->clickedButton() == changeButton) {
        networkContainer->setDefaultSelection(logic->defaultNetworkSelectionValue());
        dlg->accept();
    } else
        dlg->reject();

    dlg->deleteLater();
    dlg = NULL;
}

void NetworkWidget::initWidget()
{
    logic = new NetworkBusinessLogic(this);
    connect(logic, SIGNAL(roamingStateChanged(int)), this, SLOT(roamingStateChanged(int)));

    // widgets
    DuiLabel *headerLabel = new DuiLabel(DcpNetwork::PhoneNetworkText);
    headerLabel->setObjectName("networkLabel");
    phoneNetworkButton = new DuiButton();
    phoneNetworkButton->setCheckable(true);
    phoneNetworkButton->setObjectName("networkToggleButton");
    dataCounterButton = new DuiButton(DcpNetwork::DataCounterText);
    dataCounterButton->setObjectName("dataCounterButton");
    networkContainer = new NetworkContainer();
    roamingContainer = new RoamingContainer((RoamingContainer::RoamingState)logic->roamingState(), logic->roamingUpdatesState());

    // main container
    DuiLayout *contentLayout = new DuiLayout();
    contentLayoutPolicy = new DuiGridLayoutPolicy(contentLayout);
    contentLayoutPolicy->addItemAtPosition(headerLabel, 0, 0);
    contentLayoutPolicy->addItemAtPosition(phoneNetworkButton, 0, 1);
    contentLayoutPolicy->addItemAtPosition(roamingContainer, 1, 0, 1, 2);
    contentLayoutPolicy->addItemAtPosition(dataCounterButton, 2, 0, 1, 2, Qt::AlignHCenter);
    contentLayoutPolicy->setSpacing(10);
    DuiContainer *mainContainer = new DuiContainer();
    mainContainer->centralWidget()->setLayout(contentLayout);

    // set values
    networkContainer->initModeComboBox(logic->selectedNetworkMode(), logic->networkModes());
    networkContainer->initSelectionComboBox(logic->selectedNetworkSelectionValue(), logic->networkSelectionValues());
    togglePhoneNetworkButton(logic->networkState());
    updateNetworkIcon(logic->networkIcon());

    // connect the value receive signals
    connect(logic, SIGNAL(availableNetworkOperators(int, QStringList)), networkContainer, SLOT(showAvailableOperators(int, QStringList)));
    connect(logic, SIGNAL(autoSelectNetworkOperator()), networkContainer, SLOT(hideAvailableOperators()));
    connect(logic, SIGNAL(networkIconChanged(QString)), this, SLOT(updateNetworkIcon(QString)));
    connect(logic, SIGNAL(roamingStateChanged(int)), this, SLOT(roamingStateChanged(int)));
    connect(logic, SIGNAL(roamingUpdatesStateChanged(bool)), roamingContainer, SLOT(toggleRoamingUpdates(bool)));
    connect(logic, SIGNAL(networkStateChanged(bool)), this, SLOT(togglePhoneNetworkButton(bool)));
    connect(logic, SIGNAL(operatorSelectionFailed()), networkContainer, SLOT(operatorSelectionFailed()));
    connect(logic, SIGNAL(searchingOperators(bool)), networkContainer, SLOT(setProgressIndicatorVisible(bool)));

    // catch user actions
    connect(phoneNetworkButton, SIGNAL(toggled(bool)), this, SLOT(toggleNetworkSettings(bool)));
    connect(phoneNetworkButton, SIGNAL(toggled(bool)), logic, SLOT(toggleNetwork(bool)));
    connect(dataCounterButton, SIGNAL(pressed()), this, SLOT(dataCounterButtonPressed()));
    connect(networkContainer, SIGNAL(networkModeChanged(QString)), logic, SLOT(setNetworkMode(QString)));
    connect(networkContainer, SIGNAL(networkSelectionChanged(QString)), logic, SLOT(setNetworkSelectionValue(QString)));
    connect(networkContainer, SIGNAL(availableOperatorSelected(int)), logic, SLOT(selectOperator(int)));
    connect(roamingContainer, SIGNAL(roamingStateChanged(RoamingContainer::RoamingState)), this, SLOT(roamingStateChanged(RoamingContainer::RoamingState)));
    connect(roamingContainer, SIGNAL(roamingUpdatesToggled(bool)), logic, SLOT(changeRoamingUpdatesState(bool)));

    // mainLayout
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout();
    mainLayout->addItem(mainContainer);
    this->setLayout(mainLayout);
}

void NetworkWidget::togglePhoneNetworkButton(bool toggle)
{
    phoneNetworkButton->setChecked(toggle);
    toggleNetworkSettings(toggle);
}

void NetworkWidget::toggleNetworkSettings(bool toggle)
{
    qDebug() << "NetworkWidget::toggleNetworkSettings(" << toggle << ")";

    if (toggle) {
        if (contentLayoutPolicy->itemAt(3, 0) != networkContainer) {
            contentLayoutPolicy->addItemAtPosition(networkContainer, 3, 0, 1, 2);
        }
    } else {
        contentLayoutPolicy->removeItem(networkContainer);
    }
}

void NetworkWidget::updateNetworkIcon(const QString &value)
{
    phoneNetworkButton->setIconID(value);
}

void NetworkWidget::dataCounterButtonPressed()
{
    qDebug() << "Show dialog";
}

void NetworkWidget::roamingStateChanged(int state)
{
    roamingContainer->changeRoamingState((RoamingContainer::RoamingState)state);
}

void NetworkWidget::roamingStateChanged(RoamingContainer::RoamingState state)
{
    logic->changeRoamingState((int)state);
}
