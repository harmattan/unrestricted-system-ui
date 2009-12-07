#include "forwardingcontainer.h"
#include "forwardingwidget.h"

#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiLocale>

ForwardingContainer::ForwardingContainer(DuiWidget* parent) :
        DuiContainer(trid("qtn_cell_call_forwarding", "Call forwardign"), parent)
{
    DuiLayout* layout = new DuiLayout;
    lp = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    lp->setSpacing(5);

    fwdAll         = new ForwardingWidget(trid("qtn_cell_forw_unconditional", "All voice calls"),
                                          QString("dbusfunc"));
    fwdBusy        = new ForwardingWidget(trid("qtn_cell_forw_busy", "If busy"),
                                          QString("dbusfunc"));
    fwdNotAnswered = new ForwardingWidget(trid("qtn_cell_forw_no_reply", "If not answered"),
                                          QString("dbusfunc"));
    fwdOutOfReach  = new ForwardingWidget(trid("qtn_cell_forw_not_reachable", "If out of reach"),
                                          QString("dbusfunc"));

    // dummy widgets are used to hide others when unconditional forwarding is active
    dummyBusy = newDummy();
    dummyNotAnswered = newDummy();
    dummyOutOfReach = newDummy();

    lp->addItem(fwdAll);
    setWidgetVisibilities();

    // connect signals

    // layout
    centralWidget()->setLayout(layout);
}

ForwardingContainer::~ForwardingContainer()
{
    if (lp->indexOf(dummyBusy) < 0) {
        delete dummyBusy;
        delete dummyNotAnswered;
        delete dummyOutOfReach;
    } else {
        delete fwdBusy;
        delete fwdNotAnswered;
        delete fwdOutOfReach;
    }
}

void ForwardingContainer::setWidgetVisibilities()
{
    if (fwdAll->isEnabled()) {
        // if unconditional forwarding is enabled, hide other widgets
        switchWidget(dummyBusy, fwdBusy);
        switchWidget(dummyNotAnswered, fwdNotAnswered);
        switchWidget(dummyOutOfReach, fwdOutOfReach);
    } else {
        switchWidget(fwdBusy, dummyBusy);
        switchWidget(fwdNotAnswered, dummyNotAnswered);
        switchWidget(fwdOutOfReach, dummyOutOfReach);
    }
}

void ForwardingContainer::switchWidget(QGraphicsWidget* show, QGraphicsWidget* hide)
{
    lp->removeItem(hide);

    if (lp->indexOf(show) < 0) {
        lp->addItem(show);
    }
}

QGraphicsWidget* ForwardingContainer::newDummy()
{
    QGraphicsWidget* dummy = new QGraphicsWidget;
    dummy->setContentsMargins(0, 0, 0, 0);
    dummy->setPreferredWidth(1);
    return dummy;
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
