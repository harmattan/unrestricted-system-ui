#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include "dcpwidget.h"

class DuiButton;
class DisplayBusinessLogic;
class SliderContainer;

class DisplayWidget : public DcpWidget
{
    Q_OBJECT

public:
    DisplayWidget(QGraphicsWidget *parent = 0);
    virtual ~DisplayWidget();

protected:
    void initWidget();

private slots:
    void initBlankInhibitButton(bool toggle);

private: //attributes
    DuiButton *blankInhibitButton;
    DisplayBusinessLogic *logic;
    SliderContainer *brightnessContainer;
    SliderContainer *screenLightsContainer;
};
#endif // DISPLAYWIDGET_H
