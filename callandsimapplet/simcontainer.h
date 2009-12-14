#ifndef SIMCONTAINER_H
#define SIMCONTAINER_H

#include "dcpcallandsim.h"

#include <cellular-qt/SIM>

#include <DuiContainer>

#include <QString>

class DuiLabel;
class DuiButton;
class DuiGridLayoutPolicy;
class QDBusInterface;

using namespace Cellular;

class SimContainer : public DuiContainer
{
    Q_OBJECT

public:
    SimContainer(DuiWidget* parent = 0);
    virtual ~SimContainer();

signals:
    void pinRequestChanged(bool);
    void pinChangeRequested();

public slots:
    void setPinRequest(bool enabled);

private slots:
    void buttonToggled(bool checked);
    void changePinClicked();

    void pinQueryState(SIMSecurity::PINQuery state);

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
    QDBusInterface* dbusPinIf;
};

#endif // SIMCONTAINER_H
