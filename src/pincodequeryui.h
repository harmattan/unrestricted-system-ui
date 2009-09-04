#ifndef  PINCODEQUERYUI_H
#define  PINCODEQUERYUI_H

#include <DuiApplicationPage>

class DuiButton;
class DuiLabel;
class DuiTextEdit;
class DuiLayout;
class DuiGridLayoutPolicy;
class DuiSceneManager;
class QTimer;
class QStringList;

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
    void buttonPressed();
    void orientationChanged(const Dui::Orientation &orientation);
    void removeText();

private: //methods
    void createWidgetItems();
    void createNumpad();

private: //attributes
    DuiButton *emergencyCallButton;
    DuiButton *enterButton;
    DuiButton *cancelButton;
    DuiButton *backspaceButton;
    DuiLabel *headerLabel;
    DuiTextEdit *entryTextEdit;
    DuiLayout *numpadLayout;
    DuiGridLayoutPolicy *portraitPolicy;
    DuiGridLayoutPolicy *landscapePolicy;
    QTimer *backspaceTimer;
};

#endif //  PINCODEQUERYUI_H
