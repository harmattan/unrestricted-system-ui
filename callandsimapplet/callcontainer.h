#ifndef CALLCONTAINER_H
#define CALLCONTAINER_H

#include "dcpcallandsim.h"

#include <DuiContainer>

#include <QString>

class DuiLabel;
class DuiButton;
class DuiComboBox;
class DuiLinearLayoutPolicy;
class DuiTextEdit;

class CallContainer : public DuiContainer
{
    Q_OBJECT

public:
    CallContainer(DuiWidget* parent = 0);
    virtual ~CallContainer();

signals:
    void sendCallerIdChanged(int);
    void callWaitingChanged(bool);

public slots:
    void setSendCallerId(int value);
    void setCallWaiting(bool enabled);
    void requestFailed(DcpCallAndSim::Data data);

private slots:
    void sendCallerIdSelected(int index);
    void callWaitingToggled(bool checked);

private:
    DuiLabel* createLabel(const QString& text);
    QGraphicsWidget* createCheckBox(const QString& text, DuiButton*& button);

private:
//    DuiLabel* sendCallerIdLabel;
    DuiComboBox* sendCallerIdComboBox;
//    DuiLabel* callWaitingLabel;
    DuiButton* callWaitingButton;
    DuiLinearLayoutPolicy* lp;
    DuiLinearLayoutPolicy* pp;
};

#endif // CALLCONTAINER_H
