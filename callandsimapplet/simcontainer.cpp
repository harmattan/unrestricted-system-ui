#include "simcontainer.h"
#include "callandsimtranslation.h"

#include <DuiLayout>
#include <DuiGridLayoutPolicy>
#include <DuiButton>
#include <DuiLabel>

#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <QDBusInterface>
#include <QDebug>

SimContainer::SimContainer(DuiWidget *parent) :
        DuiContainer(DcpCallAndSim::SimCardText, parent),
        pinRequestLabel(NULL),
        pinRequestButton(NULL),
        changePinButton(NULL),
        lp(NULL),
        pp(NULL),
        changePinButtonWidget(NULL),
        dummyWidget(NULL)
{
    setLayout();
}

SimContainer::~SimContainer()
{
    // delete proxy widgets
    delete changePinButtonWidget;
    changePinButtonWidget = NULL;
    delete dummyWidget;
    dummyWidget = NULL;
}

void SimContainer::setPinRequest(bool enabled)
{
    qDebug() << Q_FUNC_INFO << enabled;

    if (pinRequestButton->isChecked() != enabled) {
        pinRequestButton->setChecked(enabled);
    } else {
        toggleChangePinButtonWidget(enabled);
    }
}

void SimContainer::requestFailed(DcpCallAndSim::Data data)
{
    qDebug() << Q_FUNC_INFO << data;

    if (data == DcpCallAndSim::PinRequestData) {
        pinRequestButton->blockSignals(true);
        pinRequestButton->setChecked(false);
        pinRequestButton->blockSignals(false);

        changePinButton->blockSignals(true);
        toggleChangePinButtonWidget(false);
        changePinButton->blockSignals(false);
    }
}

void SimContainer::buttonToggled(bool checked)
{
    qDebug() << Q_FUNC_INFO << checked;

    toggleChangePinButtonWidget(checked);
    emit pinRequestChanged(checked);
}

void SimContainer::changePinClicked()
{
    emit pinChangeRequested();
}

void SimContainer::setLayout()
{
    // layout & policies
    DuiLayout* layout = new DuiLayout();
    lp = new DuiGridLayoutPolicy(layout);
    layout->setLandscapePolicy(lp); // ownership transferred
    pp = new DuiGridLayoutPolicy(layout);
    layout->setPortraitPolicy(pp); // ownership transferred

    // pin request widget
    pinRequestLabel = new DuiLabel(DcpCallAndSim::PinCodeRequestText);
    pinRequestLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pinRequestButton = new DuiButton();
    pinRequestButton->setObjectName("checkBoxButton");
    pinRequestButton->setCheckable(true);
    pinRequestButton->setChecked(false);

    QGraphicsWidget *pinRequestWidget = new QGraphicsWidget;
    pinRequestWidget->setPreferredWidth(1);
    QGraphicsLinearLayout* pinRequestLayout = new QGraphicsLinearLayout(Qt::Horizontal, pinRequestWidget);    
    pinRequestLayout->setContentsMargins(0, 0, 0, 0);        
    pinRequestLayout->addItem(pinRequestLabel);
    pinRequestLayout->addItem(pinRequestButton);

    // change pin code button widget
    changePinButtonWidget = new QGraphicsWidget;
    changePinButtonWidget->setPreferredWidth(1);
    QGraphicsLinearLayout* changePinButtonLayout = new QGraphicsLinearLayout(changePinButtonWidget);
    changePinButtonLayout->setContentsMargins(0, 0, 0, 0);
    changePinButton = new DuiButton(DcpCallAndSim::ChangePinText);    
    changePinButtonLayout->addItem(changePinButton);

    // dummy placeholder
    dummyWidget = new QGraphicsWidget;
    dummyWidget->setPreferredWidth(1);

    // landscape policy
    lp->setSpacing(5);    
    lp->addItemAtPosition(pinRequestWidget, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
    lp->addItemAtPosition(dummyWidget, 0, 1, Qt::AlignCenter);

    // portrait policy
    pp->setSpacing(5);    
    pp->addItemAtPosition(pinRequestWidget, 0, 0, Qt::AlignLeft);
    pp->addItemAtPosition(dummyWidget, 1, 0, Qt::AlignCenter);

    // connect signals
    connect(pinRequestButton, SIGNAL(toggled(bool)), this, SLOT(buttonToggled(bool)));
    connect(changePinButton, SIGNAL(clicked()), this, SLOT(changePinClicked()));

    // layout
    centralWidget()->setLayout(layout);
}

void SimContainer::toggleChangePinButtonWidget(bool toggle)
{
    if(toggle) {
        lp->removeItem(dummyWidget);
        lp->addItemAtPosition(changePinButtonWidget, 0, 1, Qt::AlignCenter);
        pp->removeItem(dummyWidget);
        pp->addItemAtPosition(changePinButtonWidget, 1, 0, Qt::AlignCenter);
    } else {
        lp->removeItem(changePinButtonWidget);
        lp->addItemAtPosition(dummyWidget, 0, 1, Qt::AlignCenter);
        pp->removeItem(changePinButtonWidget);
        pp->addItemAtPosition(dummyWidget, 1, 0, Qt::AlignCenter);
    }
}
