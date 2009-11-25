#include "phonenetworkcontainer.h"
#include "networksignalstrengthicon.h"

#include <DuiButton>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLocale>
#include <DuiGridLayoutPolicy>

PhoneNetworkContainer::PhoneNetworkContainer(DuiWidget *parent) :
        DuiContainer(parent),
        toggleButton(NULL),
        operatorLabel(NULL),
        signalStrengthIcon(NULL),
        networkOperator(NULL)
{
    networkOperator = new NetworkOperator();
    toggleButton = new DuiButton();
    toggleButton->setCheckable(true);
    toggleButton->setChecked(/* TODO: check state from correct API */true);
    toggleButton->setObjectName("connectivityButton");
    connect(toggleButton, SIGNAL(toggled(bool)), this, SIGNAL(toggled(bool)));    
    signalStrengthIcon = new NetworkSignalStrengthIcon();
    operatorLabel = new DuiLabel();
    operatorLabel->setObjectName("connectivityLabel2");
    operatorLabel->setText(networkOperator->name());
    connect(networkOperator, SIGNAL(nameChanged(QString)), operatorLabel, SLOT(setText(QString)));

    setLayout();
}

PhoneNetworkContainer::~PhoneNetworkContainer()
{
    delete networkOperator;
    networkOperator = NULL;
}

bool PhoneNetworkContainer::phoneNetworkOn()
{
    return toggleButton->isChecked();
}

void PhoneNetworkContainer::togglePhoneNetwork(bool toggle)
{ 
    toggleButton->setChecked(toggle);
}

void PhoneNetworkContainer::setLayout()
{
    DuiLayout *layout = new DuiLayout();
    DuiGridLayoutPolicy *layoutPolicy = new DuiGridLayoutPolicy(layout);
    DuiLabel *headerLabel = new DuiLabel(trid("qtn_cell_phone_network", "Phone network"));
    headerLabel->setObjectName("connectivityLabel1");
    layoutPolicy->addItemAtPosition(headerLabel, 0, 0, 1, 2);
    layoutPolicy->addItemAtPosition(signalStrengthIcon, 1, 0, 1, 1);
    layoutPolicy->addItemAtPosition(operatorLabel, 1, 1, 1, 1);
    layoutPolicy->addItemAtPosition(toggleButton, 0, 2, 2, 1);
    layoutPolicy->setColumnStretchFactor(1, 1);
    centralWidget()->setLayout(layout);
}

