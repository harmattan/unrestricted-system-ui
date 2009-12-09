#include "forwardingcontainer.h"
#include "forwardingwidget.h"

#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiNotification>
#include <DuiLocale>

#include <QDebug>

ForwardingContainer::ForwardingContainer(DuiWidget* parent) :
        DuiContainer(trid("qtn_cell_call_forwarding", "Call forwardign"), parent)
{
    DuiLayout* layout = new DuiLayout;
    policy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setSpacing(5);

    fwdAll          = new ForwardingWidget(trid("qtn_cell_forw_unconditional", "All voice calls"));
    fwdBusy         = new ForwardingWidget(trid("qtn_cell_forw_busy", "If busy"));
    fwdNoReply      = new ForwardingWidget(trid("qtn_cell_forw_no_reply", "If not answered"));
    fwdNotReachable = new ForwardingWidget(trid("qtn_cell_forw_not_reachable", "If out of reach"));

    // dummy widgets are used to hide others when unconditional forwarding is active
    dummyBusy         = newDummy();
    dummyNoReply      = newDummy();
    dummyNotReachable = newDummy();

    policy->addItem(fwdAll);
    // hideCFWidgets adds the other widgets to layout
    hideCFWidgets(fwdAll->isEnabled());

    callForwarding = new CallForwarding(this);

    connect(callForwarding, SIGNAL(divertCheckComplete(bool, QString, CallForwarding::DivertError)),
            this, SLOT(divertCheckComplete(bool, QString, CallForwarding::DivertError)));
    connect(callForwarding, SIGNAL(divertCancelComplete(CallForwarding::DivertError)),
            this, SLOT(divertCancelComplete(CallForwarding::DivertError)));

    // connect signals
    connect(fwdAll, SIGNAL(enabled(bool)), this, SLOT(hideCFWidgets(bool)));

    connect(fwdAll, SIGNAL(buttonClicked()), this, SLOT(selectNumberAll()));
    connect(fwdBusy, SIGNAL(buttonClicked()), this, SLOT(selectNumberBusy()));
    connect(fwdNoReply, SIGNAL(buttonClicked()), this, SLOT(selectNumberNoReply()));
    connect(fwdNotReachable, SIGNAL(buttonClicked()), this, SLOT(selectNumberNotReachable()));

    // query state of each divert
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

void ForwardingContainer::selectNumberAll()
{
    DuiNotification notification("", "", "Number picker not implemented yet");
}

void ForwardingContainer::selectNumberBusy()
{
    DuiNotification notification("", "", "Number picker not implemented yet");
}

void ForwardingContainer::selectNumberNoReply()
{
    DuiNotification notification("", "", "Number picker not implemented yet");
}

void ForwardingContainer::selectNumberNotReachable()
{
    DuiNotification notification("", "", "Number picker not implemented yet");
}

// called when a divert has been set to update the UI
void ForwardingContainer::divertSet(CallForwarding::DivertType type, bool active, QString number, CallForwarding::DivertError error)
{
    qDebug() << Q_FUNC_INFO << type << active << number << error;
    ForwardingWidget* widget = widgetForType(type);
    if (widget) {
        if (error == CallForwarding::NoError) {
            widget->update(active, number);
        } else {
            widget->update(false, "");
        }
    }
}

// hides/shows conditional forwarding widgets (used when unconditional forwarding is enabled)
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

// called when divertCancel completes
void ForwardingContainer::divertCancelComplete(CallForwarding::DivertError error)
{
    qDebug() << Q_FUNC_INFO;
    FwdAction action = actionQueue.dequeue();

    if (error != CallForwarding::NoError) {
        qDebug() << "error:" << error;
        DuiNotification notification("", "", "divertCancel failed");
    }

    ForwardingWidget* widget = widgetForType(action.type);
    if (widget != NULL) {
        widget->update(false, "");
    }

    processQueue();
}

// called when divertCheck completes
void ForwardingContainer::divertCheckComplete(bool active, QString number, CallForwarding::DivertError error)
{
    qDebug() << Q_FUNC_INFO << active << number << error;
    FwdAction action = actionQueue.dequeue();

    if (error != CallForwarding::NoError) {
        DuiNotification notification("", "", "divertCheck failed");
    }

    ForwardingWidget* widget = widgetForType(action.type);
    if (widget != NULL) {
        widget->update(active, number);
    }

    processQueue();
}

// checks a divert state
// if the queue parameter is set to true, leaves the request to the queue without processing it
void ForwardingContainer::divertCheck(CallForwarding::DivertType type, bool queue)
{
    qDebug() << Q_FUNC_INFO << type << queue;
    actionQueue.enqueue(FwdAction(type, FwdAction::Check));

    if (!queue) {
        processQueue();
    }
}

// cancels a divert
// if the queue parameter is set to true, leaves the request to the queue without processing it
void ForwardingContainer::divertCancel(CallForwarding::DivertType type, bool queue)
{
    qDebug() << Q_FUNC_INFO << type << queue;
    actionQueue.enqueue(FwdAction(type, FwdAction::Cancel));

    if (!queue) {
        processQueue();
    }
}

// starts processing the action queue, next items are processed as responses are received
// prevents multiple calls to Cellular API
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

// returns ForwardingWidget pointer for wanted divert type
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

// replaces widget with another one in a layout
void ForwardingContainer::switchWidget(QGraphicsWidget* show, QGraphicsWidget* hide)
{
    if (policy->indexOf(hide) >= 0) {
        policy->removeItem(hide);
    }

    if (policy->indexOf(show) < 0) {
        policy->addItem(show);
    }
}

// creates a new dummy widget for hiding others
QGraphicsWidget* ForwardingContainer::newDummy()
{
    QGraphicsWidget* dummy = new QGraphicsWidget;
    dummy->setContentsMargins(0, 0, 0, 0);
    dummy->setPreferredWidth(1);
    return dummy;
}
