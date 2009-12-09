#ifndef CALLCONTAINER_H
#define CALLCONTAINER_H

#include "dcpcallandsim.h"

#include <cellular-qt/CallWaiting>

#include <DuiContainer>

#include <QString>

class DuiLabel;
class DuiButton;
class DuiComboBox;
class DuiLinearLayoutPolicy;
class DuiTextEdit;

using namespace Cellular;

class CallContainer : public DuiContainer
{
    Q_OBJECT

public:
    CallContainer(DuiWidget* parent = 0);
    virtual ~CallContainer();

public slots:
//    void setCallerIdSending(int value);
//    void setSendCallerId(int value);

private slots:
    void waitingActivateComplete(CallWaiting::WaitingError);
    void waitingCancelComplete(CallWaiting::WaitingError);
    void waitingCheckComplete(bool active, CallWaiting::WaitingError);

    void sendCallerIdSelected(int index);
    void callWaitingToggled(bool checked);

private:
    DuiLabel* createLabel(const QString& text);
    QGraphicsWidget* createCheckBox(const QString& text, DuiButton*& button);

private:
    CallWaiting* callWaiting;

    DuiComboBox* sendCallerIdComboBox;
    DuiButton* callWaitingButton;
    DuiLinearLayoutPolicy* lp;
    DuiLinearLayoutPolicy* pp;
};

#endif // CALLCONTAINER_H
