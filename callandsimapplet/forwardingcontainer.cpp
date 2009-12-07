#include "forwardingcontainer.h"

#include <DuiLayout>

ForwardingContainer::ForwardingContainer()
{
    /*
    // layout & policies
    DuiLayout* layout = new DuiLayout();
    lp = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    layout->setLandscapePolicy(lp); // ownership transferred
    //    pp = new DuiGridLayoutPolicy(layout);
    //    layout->setPortraitPolicy(pp); // ownership transferred


    // dummy placeholder
    dummyWidget = new QGraphicsWidget;
    dummyWidget->setContentsMargins(0, 0, 0, 0);
    dummyWidget->setPreferredWidth(1);

    //    lp->addItemAtPosition(callFwdWidget, 1, 0, Qt::AlignLeft | Qt::AlignVCenter);
    //    lp->addItemAtPosition(dummyWidget, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);

    //    pp->addItemAtPosition(callFwdWidget, 2, 0, Qt::AlignCenter);
    //    pp->addItemAtPosition(dummyWidget, 3, 0, Qt::AlignCenter);

    //    connect(callFwdButton, SIGNAL(toggled(bool)), this, SLOT(callForwardingToggled(bool)));
    //    connect(numberEdit, SIGNAL(lostFocus(Qt::FocusReason)), this, SLOT(numberChanged()));
    // landscape policy
    lp->setSpacing(5);

    // portrait policy
    //    pp->setSpacing(5);

    // connect signals

    // layout
    centralWidget()->setLayout(layout);
    */
}
/*
ForwardingContainer::~ForwardingContainer()
{
    //delete proxy widgets
    delete dummyWidget;
    dummyWidget = NULL;
    delete fwdNumberWidget;
    fwdNumberWidget = NULL;
}

void CallContainer::toggleFwdNumberWidget(bool toggle)
{
    if(toggle) {
        lp->removeItem(dummyWidget);
        lp->addItemAtPosition(fwdNumberWidget, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
        pp->removeItem(dummyWidget);
        pp->addItemAtPosition(fwdNumberWidget, 3, 0, Qt::AlignCenter);
    } else {
        lp->removeItem(fwdNumberWidget);
        lp->addItemAtPosition(dummyWidget, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
        pp->removeItem(fwdNumberWidget);
        pp->addItemAtPosition(dummyWidget, 3, 0, Qt::AlignCenter);
    }
}

void CallContainer::setCallForwarding(bool enabled, QString number)
{
    qDebug() << Q_FUNC_INFO << enabled << number;

    if (number.compare(numberEdit->text()) != 0) {
        numberEdit->setText(number);
    }

    if (callFwdButton->isChecked() != enabled) {
        callFwdButton->setChecked(enabled);
    } else {
        toggleFwdNumberWidget(enabled);
    }
}

void CallContainer::requestFailed(DcpCallAndSim::Data data)
{
    qDebug() << Q_FUNC_INFO << data;

    switch (data) {
    case DcpCallAndSim::CallForwardingData:
        toggleFwdNumberWidget(false);

        callFwdButton->blockSignals(true);
        callFwdButton->setChecked(false);
        callFwdButton->blockSignals(false);
        break;
    default:
        break;
    }
}

void CallContainer::callForwardingToggled(bool checked)
{
    qDebug() << Q_FUNC_INFO << checked;

    toggleFwdNumberWidget(checked);

    emit callForwardingChanged(checked, numberEdit->text());
}

void CallContainer::numberChanged()
{
    QString number(numberEdit->text());

    qDebug() << Q_FUNC_INFO << callFwdButton->isChecked() << number;

    emit callForwardingChanged(callFwdButton->isChecked(), number);
}
*/
