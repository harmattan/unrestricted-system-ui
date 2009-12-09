#ifndef FORWARDINGCONTAINER_H
#define FORWARDINGCONTAINER_H

#include "dcpcallandsim.h"

#include <cellular-qt/CallForwarding>
#include <DuiContainer>
#include <QQueue>

class ForwardingWidget;
class DuiLinearLayoutPolicy;

using namespace Cellular;

class FwdAction
{
public:
    enum Action {
        Check = 0,
        Cancel
    };

    FwdAction(CallForwarding::DivertType type, Action action) : type(type), action(action) {}
    FwdAction(const FwdAction &other) : type(other.type), action(other.action) {}
    FwdAction &operator=(const FwdAction &other) {
        type = other.type;
        action = other.action;
        return *this;
    }

    CallForwarding::DivertType type;
    Action action;
};

class ForwardingContainer : public DuiContainer
{
    Q_OBJECT

public:
    ForwardingContainer(DuiWidget* parent = 0);
    ~ForwardingContainer();

public slots:
    void selectNumberAll();
    void selectNumberBusy();
    void selectNumberNoReply();
    void selectNumberNotReachable();
    void divertSet(CallForwarding::DivertType type, bool active, QString number, CallForwarding::DivertError error);

private slots:
    void hideCFWidgets(bool show);

    void divertCancelComplete(CallForwarding::DivertError error);
    void divertCheckComplete(bool active, QString number, CallForwarding::DivertError error);

private:
    void divertCheck(CallForwarding::DivertType type, bool queue = false);
    void divertCancel(CallForwarding::DivertType type, bool queue = false);
    void processQueue();

    ForwardingWidget* widgetForType(CallForwarding::DivertType type);
    void switchWidget(QGraphicsWidget* show, QGraphicsWidget* hide);
    QGraphicsWidget* newDummy();

private:
    CallForwarding* callForwarding;
    QQueue<FwdAction> actionQueue;
    ForwardingWidget* fwdAll;
    ForwardingWidget* fwdBusy;
    ForwardingWidget* fwdNoReply;
    ForwardingWidget* fwdNotReachable;
    QGraphicsWidget* dummyBusy;
    QGraphicsWidget* dummyNoReply;
    QGraphicsWidget* dummyNotReachable;
    DuiLinearLayoutPolicy* policy;
};

#endif // FORWARDINGCONTAINER_H
