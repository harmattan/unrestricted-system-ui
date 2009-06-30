#ifndef PINCODEQUERY_H
#define PINCODEQUERY_H

#include <DuiApplicationPage>
#include <DuiLayout>
#include "notifier.h"

class DuiButton;
class DuiLabel;
class DuiTextEdit;
class DuiGridLayoutPolicy;

class PinCodeQuery : public DuiApplicationPage
{
    Q_OBJECT
public:

    enum UIState {
        UIPINState,
        UIPIN2AttemptsLeftState,
        UIPIN1AttemptLeftState,
        UIPUKState,
        UIEnterNewPINState,
        UIReEnterNewPINState,        
        UIUnlockState //kytkykauppa
    };

    PinCodeQuery(Notifier *notifier);
    virtual ~PinCodeQuery();
    virtual void createContent();
    void changeUIState(UIState uiState);

signals:
    void codeEntered(PinCodeQuery::UIState uiState, QString code);

private slots:
    void buttonReleased();
    void orientationChanged(const Dui::Orientation &orientation);

private:
    void createWidgetItems();
    void createNumpad();
    void checkEntry();

private:
    DuiButton *emergencyCallButton;
    DuiButton *enterButton;
    DuiButton *cancelButton;
    DuiButton *backspaceButton;
    DuiLabel *headerLabel;
    DuiTextEdit *entryTextEdit;
    DuiLayout *numpadLayout;
    DuiGridLayoutPolicy *portraitPolicy;
    DuiGridLayoutPolicy *landscapePolicy;
    Notifier *notifier;
    bool checkDigitCount;
    UIState uiState;

};

#endif // PINCODEQUERY_H
