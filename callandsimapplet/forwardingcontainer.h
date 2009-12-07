#ifndef FORWARDINGCONTAINER_H
#define FORWARDINGCONTAINER_H

#include "dcpcallandsim.h"

#include <DuiContainer>

class ForwardingWidget;
class DuiLinearLayoutPolicy;

class ForwardingContainer : public DuiContainer
{
    Q_OBJECT

public:
    ForwardingContainer(DuiWidget* parent = 0);
    ~ForwardingContainer();
    /*
    signals:
        void callForwardingChanged(bool, const QString);

    public slots:
        void setCallForwarding(bool enabled, QString number);

    private slots:
        void callForwardingToggled(bool checked);
        void numberChanged();

    private:
        void toggleFwdNumberWidget(bool toggle);
    */


private:
    void setWidgetVisibilities();
    void switchWidget(QGraphicsWidget* show, QGraphicsWidget* hide);
    QGraphicsWidget* newDummy();

private:
    ForwardingWidget* fwdAll;
    ForwardingWidget* fwdBusy;
    ForwardingWidget* fwdNotAnswered;
    ForwardingWidget* fwdOutOfReach;
    QGraphicsWidget* dummyBusy;
    QGraphicsWidget* dummyNotAnswered;
    QGraphicsWidget* dummyOutOfReach;
    DuiLinearLayoutPolicy* lp;
};

#endif // FORWARDINGCONTAINER_H
