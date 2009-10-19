#include "simcontainer.h"
#include "callandsimtranslation.h"

#include <DuiLayout>
#include <DuiGridLayoutPolicy>
#include <DuiButton>
#include <DuiLabel>

#include <QGraphicsLinearLayout>
#include <QDebug>

SimContainer::SimContainer(DuiWidget *parent) :
        DuiContainer(DcpCallAndSim::SimCardText, parent),
        pinRequestLabel(NULL),
        pinRequestButton(NULL),
        changePinButton(NULL)
{
    setLayout();
}

void SimContainer::setPinRequest(bool enabled)
{
    qDebug() << Q_FUNC_INFO << enabled;

    if (pinRequestButton->isChecked() != enabled) {
        pinRequestButton->setChecked(enabled);
    } else {
        changePinButton->setVisible(enabled);
    }
}

void SimContainer::buttonToggled(bool checked)
{
    qDebug() << Q_FUNC_INFO << checked;

    changePinButton->setVisible(checked);
    emit valueChanged(checked);
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

    pinRequestLabel = new DuiLabel(DcpCallAndSim::PinCodeRequestText);
    pinRequestLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
//    pinRequestLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    pinRequestButton = new DuiButton();
    pinRequestButton->setObjectName("checkBoxButton");
    pinRequestButton->setCheckable(true);
    pinRequestButton->setChecked(false);

    QGraphicsLinearLayout* pinRequestLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    // TODO: some const value here some day
    pinRequestLayout->setContentsMargins(10, 10, 10, 10);
    pinRequestLayout->addItem(pinRequestLabel);
    pinRequestLayout->addItem(pinRequestButton);

    // change pin code button

    changePinButton = new DuiButton(DcpCallAndSim::ChangePinText);
    changePinButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
//    changePinButton->setVisible(true); // TODO: wtf
//    changePinButton->hide();

    // landscape policy

    lp->setSpacing(5);
    lp->setColumnMaximumWidth(0, 390);
    lp->setColumnMaximumWidth(1, 390);
    lp->addItemAtPosition(pinRequestLayout, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
    lp->addItemAtPosition(changePinButton, 0, 1, Qt::AlignCenter);

    // portrait policy

    pp->setSpacing(5);
    pp->setColumnMaximumWidth(0, 480);
    pp->addItemAtPosition(pinRequestLayout, 0, 0, Qt::AlignLeft);
    pp->addItemAtPosition(changePinButton, 1, 0, Qt::AlignCenter);

    // connect signals

    connect(pinRequestButton, SIGNAL(toggled(bool)), this, SLOT(buttonToggled(bool)));

    // layout

    centralWidget()->setLayout(layout);
}
