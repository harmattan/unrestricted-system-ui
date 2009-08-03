#ifndef  PINCODEQUERYUI_H
#define  PINCODEQUERYUI_H

#include <DuiApplicationPage>
#include <DuiLayout>
#include "notifier.h"

class DuiButton;
class DuiLabel;
class DuiTextEdit;
class DuiGridLayoutPolicy;

class  PinCodeQueryUI : public DuiApplicationPage
{
    Q_OBJECT
public:

     PinCodeQueryUI();
    virtual ~ PinCodeQueryUI();
    virtual void createContent();



    DuiButton *getEmergencyBtn();
    DuiButton *getCancelBtn();
    DuiButton *getEnterBtn();
    DuiTextEdit *getCodeEntry();

    void setHeader(QString);

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

};

#endif //  PINCODEQUERYUI_H
