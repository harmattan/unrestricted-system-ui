#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <dcpwidget.h>

class DuiButton;
class DuiSlider;
class DuiLabel;
class DuiContainer;
class DisplayBusinessLogic;

class DisplayWidget : public DcpWidget
{
    Q_OBJECT

public:
    DisplayWidget(QGraphicsWidget *parent = 0);
    virtual ~DisplayWidget();

protected:
    void initWidget();

private slots:
    void modify_screenlight_handle (int newValue);

private:
    DisplayBusinessLogic   *m_logic;
    QList<int>              m_brightness_vals;
    QList<int>              m_screenlight_vals;
    DuiSlider              *m_brightnessSlider;
    DuiSlider              *m_screenlightSlider;

    void retranslateUi ();

    DuiContainer           *m_brightnessContainer;
    DuiContainer           *m_screenlightContainer;
    DuiLabel               *m_blankinhibitLabel;
    DuiLabel               *m_noteLabel;
    // TODO: infonote, blankinhibitlabel for retranslateui

};

#endif // DISPLAYWIDGET_H

