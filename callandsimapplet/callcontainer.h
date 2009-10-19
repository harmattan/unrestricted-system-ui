#ifndef CALLCONTAINER_H
#define CALLCONTAINER_H

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
    void callForwardingChanged(bool);
    void forwardToChanged(const QString&);

public slots:
    void setSendCallerId(int value);
    void setCallWaiting(bool enabled);
    void setCallForwarding(bool enabled);
    void setForwardTo(const QString& number);

private slots:
    void sendCallerIdSelected(int index);
    void callWaitingToggled(bool checked);
    void callForwardingToggled(bool checked);
    void numberChanged();

private:
    void setLayout();

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
