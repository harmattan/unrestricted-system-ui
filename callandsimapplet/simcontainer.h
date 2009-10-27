#ifndef SIMCONTAINER_H
#define SIMCONTAINER_H

#include "dcpcallandsim.h"

#include <DuiContainer>

#include <QString>

class DuiLabel;
class DuiButton;

class SimContainer : public DuiContainer
{
    Q_OBJECT

public:
    SimContainer(DuiWidget *parent);

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

private:
    DuiLabel* pinRequestLabel;
    DuiButton* pinRequestButton;
    DuiButton* changePinButton;
};

#endif // SIMCONTAINER_H
