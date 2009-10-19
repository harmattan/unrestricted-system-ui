#ifndef SIMCONTAINER_H
#define SIMCONTAINER_H

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
    void valueChanged(bool);

public slots:
    void setPinRequest(bool enabled);

private slots:
    void buttonToggled(bool checked);

private:
    void setLayout();

private:
    DuiLabel* pinRequestLabel;
    DuiButton* pinRequestButton;
    DuiButton* changePinButton;
};

#endif // SIMCONTAINER_H
