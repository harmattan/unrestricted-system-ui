#include "forwardingcontainer.h"
#include "forwardingwidget.h"

#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiLocale>

#include <QDebug>

// Delay before a call is forwarded
const int FORWARDING_DELAY(20);

ForwardingContainer::ForwardingContainer(DuiWidget* parent) :
        DuiContainer(trid("qtn_cell_call_forwarding", "Call forwardign"), parent)
{
    DuiLayout* layout = new DuiLayout;
    lp = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    lp->setSpacing(5);

    fwdAll          = new ForwardingWidget(trid("qtn_cell_forw_unconditional", "All voice calls"),
                                           QString("dbusfunc"));
    fwdBusy         = new ForwardingWidget(trid("qtn_cell_forw_busy", "If busy"),
                                           QString("dbusfunc"));
    fwdNoReply      = new ForwardingWidget(trid("qtn_cell_forw_no_reply", "If not answered"),
                                           QString("dbusfunc"));
    fwdNotReachable = new ForwardingWidget(trid("qtn_cell_forw_not_reachable", "If out of reach"),
                                           QString("dbusfunc"));

    // dummy widgets are used to hide others when unconditional forwarding is active
    dummyBusy         = newDummy();
    dummyNoReply      = newDummy();
    dummyNotReachable = newDummy();

    lp->addItem(fwdAll);
    // showCFWidgets adds the other widgets to layout
    hideCFWidgets(fwdAll->isEnabled());

    callForwarding = new CallForwarding(this);

    connect(callForwarding, SIGNAL(divertCheckComplete(bool, QString, CallForwarding::DivertError)),
            this, SLOT(divertCheckComplete(bool, QString, CallForwarding::DivertError)));
    connect(callForwarding, SIGNAL(divertCancelComplete(CallForwarding::DivertError)),
            this, SLOT(divertCancelComplete(CallForwarding::DivertError)));

    // connect signals
    connect(fwdAll, SIGNAL(enabled(bool)), this, SLOT(hideCFWidgets(bool)));

    divertCheck(CallForwarding::NotReachable, true);
    divertCheck(CallForwarding::NoReply, true);
    divertCheck(CallForwarding::Busy, true);
    divertCheck(CallForwarding::Unconditional);

    // layout
    centralWidget()->setLayout(layout);
}

ForwardingContainer::~ForwardingContainer()
{
    // set showCFWidgets to false, removing dummy widgets from the layout
    hideCFWidgets(false);
    delete dummyBusy;
    delete dummyNoReply;
    delete dummyNotReachable;
}

void ForwardingContainer::hideCFWidgets(bool hide)
{
    if (hide) {
        // replace widgets with invisible dummies
        switchWidget(dummyBusy, fwdBusy);
        switchWidget(dummyNoReply, fwdNoReply);
        switchWidget(dummyNotReachable, fwdNotReachable);
    } else {
        switchWidget(fwdBusy, dummyBusy);
        switchWidget(fwdNoReply, dummyNoReply);
        switchWidget(fwdNotReachable, dummyNotReachable);
    }
}

/*void ForwardingContainer::divertActivateComplete(CallForwarding::DivertError error)
{
    Q_UNUSED(error);
    // not needed?
}*/

void ForwardingContainer::divertCancelComplete(CallForwarding::DivertError error)
{
    qDebug() << Q_FUNC_INFO;
    FwdAction action = actionQueue.dequeue();

    if (error != CallForwarding::NoError) {
        // TODO: Error message
        qDebug() << "error:" << error;
    }

    ForwardingWidget* widget = widgetForType(action.type);
    if (widget != NULL) {
        widget->update(false, "");
    }

    processQueue();
}

void ForwardingContainer::divertCheckComplete(bool active, QString number, CallForwarding::DivertError error)
{
    qDebug() << Q_FUNC_INFO;
    FwdAction action = actionQueue.dequeue();

    if (error != CallForwarding::NoError) {
        // TODO: Error message
        qDebug() << "error:" << error;
    }

    ForwardingWidget* widget = widgetForType(action.type);
    if (widget != NULL) {
        widget->update(active, number);
    }

    processQueue();
}

void ForwardingContainer::divertCheck(CallForwarding::DivertType type, bool queue)
{
    qDebug() << Q_FUNC_INFO << type << queue;
    actionQueue.enqueue(FwdAction(type, FwdAction::Check));

    if (!queue) {
        processQueue();
    }
}

void ForwardingContainer::divertCancel(CallForwarding::DivertType type, bool queue)
{
    qDebug() << Q_FUNC_INFO << type << queue;
    actionQueue.enqueue(FwdAction(type, FwdAction::Cancel));

    if (!queue) {
        processQueue();
    }
}


void ForwardingContainer::processQueue()
{
    qDebug() << Q_FUNC_INFO << "count:" << actionQueue.count();
    if (actionQueue.isEmpty()) {
        return;
    }

    FwdAction action = actionQueue.head();

    switch (action.action) {
    case FwdAction::Check:
        qDebug() << "divertCheck" << action.type;
        callForwarding->divertCheck(action.type);
        break;
    case FwdAction::Cancel:
        qDebug() << "divertCancel" << action.type;
        callForwarding->divertCancel(action.type);
        break;
    default:
        actionQueue.dequeue();
    }
}

ForwardingWidget* ForwardingContainer::widgetForType(CallForwarding::DivertType type)
{
    switch (type) {
    case CallForwarding::Unconditional:
        return fwdAll;
        break;
    case CallForwarding::Busy:
        return fwdBusy;
        break;
    case CallForwarding::NoReply:
        return fwdNoReply;
        break;
    case CallForwarding::NotReachable:
        return fwdNotReachable;
        break;
    default:
        return NULL;
    }
}

void ForwardingContainer::switchWidget(QGraphicsWidget* show, QGraphicsWidget* hide)
{
    qDebug() << Q_FUNC_INFO;
    if (lp->indexOf(hide) >= 0) {
        lp->removeItem(hide);
    }

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
