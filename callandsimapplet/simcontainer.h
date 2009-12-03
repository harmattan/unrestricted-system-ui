#ifndef SIMCONTAINER_H
#define SIMCONTAINER_H

#include "dcpcallandsim.h"

#include <DuiContainer>

#include <QString>

class DuiLabel;
class DuiButton;
class DuiGridLayoutPolicy;

class SimContainer : public DuiContainer
{
    Q_OBJECT

public:
    SimContainer(DuiWidget *parent);
    virtual ~SimContainer();

signals:
    void pinRequestChanged(bool);
    void pinChangeRequested();

public slots:
    void setPinRequest(bool enabled);
    void requestFailed(DcpCallAndSim::Data data);

private slots:
    void buttonToggled(bool checked);
    void changePinClicked();

private:
    void setLayout();
    void toggleChangePinButtonWidget(bool toggle);

private:
    DuiLabel* pinRequestLabel;
    DuiButton* pinRequestButton;
    DuiButton* changePinButton;
    DuiGridLayoutPolicy* lp;
    DuiGridLayoutPolicy* pp;
    QGraphicsWidget *changePinButtonWidget;
    QGraphicsWidget *dummyWidget;
};

#endif // SIMCONTAINER_H
