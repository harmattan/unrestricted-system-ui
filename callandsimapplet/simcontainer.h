#ifndef SIMCONTAINER_H
#define SIMCONTAINER_H

#include <DuiContainer>
#include <QString>

class DuiLabel;
class DuiButton;

class SimContainer : public DuiContainer
{
public:
    SimContainer(DuiWidget *parent);

private:
    void setLayout();

private:
    DuiLabel* pinRequestLabel;
    DuiButton* pinRequestButton;
    DuiButton* changePinButton;
};

#endif // SIMCONTAINER_H
