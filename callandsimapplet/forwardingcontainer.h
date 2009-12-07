#ifndef FORWARDINGCONTAINER_H
#define FORWARDINGCONTAINER_H

#include "dcpcallandsim.h"

#include <DuiContainer>

class ForwardingWidget;

class ForwardingContainer : public DuiContainer
{
    Q_OBJECT

public:
    ForwardingContainer();
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
    ForwardingWidget* fwdAll;
    ForwardingWidget* fwdBusy;
    ForwardingWidget* fwdNotAnswered;
    ForwardingWidget* fwdOutOfReach;
};

#endif // FORWARDINGCONTAINER_H
