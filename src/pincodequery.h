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

    PinCodeQuery();
    virtual ~PinCodeQuery();
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

#endif // PINCODEQUERY_H
