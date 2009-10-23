#ifndef CALLCONTAINER_H
#define CALLCONTAINER_H

#include "dcpcallandsim.h"

#include <DuiContainer>

#include <QString>

class DuiLabel;
class DuiButton;
class DuiComboBox;
class DuiTextEdit;
class QGraphicsLinearLayout;

class CallContainer : public DuiContainer
{
    Q_OBJECT

public:
    CallContainer(DuiWidget *parent);

signals:
    void sendCallerIdChanged(int);
    void callWaitingChanged(bool);
    void callForwardingChanged(bool, const QString);

public slots:
    void setSendCallerId(int value);
    void setCallWaiting(bool enabled);
    void setCallForwarding(bool enabled, QString number);
    void requestFailed(DcpCallAndSim::Data data);

private slots:
    void sendCallerIdSelected(int index);
    void callWaitingToggled(bool checked);
    void callForwardingToggled(bool checked);
    void numberChanged();

private:
    DuiLabel* createLabel(const QString& text);
    QGraphicsLinearLayout* createCheckBox(const QString& text, DuiButton*& button);

private:
//    DuiLabel* sendCallerIdLabel;
    DuiComboBox* sendCallerIdComboBox;
//    DuiLabel* callWaitingLabel;
    DuiButton* callWaitingButton;
//    DuiLabel* callForwardingLabel;
    DuiButton* callFwdButton;
    DuiLabel* numberLabel;
    DuiTextEdit* numberEdit;
    DuiButton* pickerButton;
};

#endif // CALLCONTAINER_H
