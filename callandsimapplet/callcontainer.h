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
public:
    CallContainer(DuiWidget *parent);

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
//    DuiLabel* numberLabel;
    DuiTextEdit* numberEdit;
    DuiButton* pickerButton;
};

#endif // CALLCONTAINER_H
