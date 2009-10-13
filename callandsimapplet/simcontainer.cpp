#include "simcontainer.h"
#include "callandsimtranslation.h"

#include <DuiLayout>
#include <DuiGridLayoutPolicy>
#include <DuiButton>
#include <DuiLabel>

#include <QGraphicsLinearLayout>

SimContainer::SimContainer(DuiWidget *parent) :
        DuiContainer(DcpCallAndSim::SimCardText, parent),
        pinRequestLabel(NULL),
        pinRequestButton(NULL),
        changePinButton(NULL)
{
    setLayout();
}

void SimContainer::setLayout()
{
    // layout & policies

    DuiLayout* layout = new DuiLayout();

    DuiGridLayoutPolicy* lp = new DuiGridLayoutPolicy(layout);
    layout->setLandscapePolicy(lp); // ownership transferred

    DuiGridLayoutPolicy* pp = new DuiGridLayoutPolicy(layout);
    layout->setPortraitPolicy(pp); // ownership transferred

    // pin request

    pinRequestLabel = new DuiLabel(DcpCallAndSim::SimCardText);
    pinRequestLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pinRequestLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    pinRequestButton = new DuiButton();
    pinRequestButton->setObjectName("pinRequestButton");
    pinRequestButton->setCheckable(true);

    QGraphicsLinearLayout* pinRequestLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    // TODO: some const value here some day
    pinRequestLayout->setContentsMargins(10, 10, 10, 10);
    pinRequestLayout->addItem(pinRequestLabel);
    pinRequestLayout->addItem(pinRequestButton);

    // change pin code button

    changePinButton = new DuiButton(DcpCallAndSim::ChangePinText);
    changePinButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // landscape policy

    lp->addItemAtPosition(pinRequestLayout, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
    lp->addItemAtPosition(changePinButton, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);
    lp->setSpacing(5);

    // portrait policy

    pp->addItemAtPosition(pinRequestLayout, 0, 0, Qt::AlignLeft);
    pp->addItemAtPosition(changePinButton, 1, 0, Qt::AlignLeft);
    pp->setSpacing(5);

    // layout

    centralWidget()->setLayout(layout);
}
