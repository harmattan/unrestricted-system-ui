#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include "dcpwidget.h" 

class DisplayDBusInterface;
class DuiButton;
class DuiContainer;
class DuiLinearLayoutPolicy;
class DuiSlider;
class DuiWidgetController;

class DisplayWidget : public DcpWidget
{
    Q_OBJECT
		
public:
    DisplayWidget(QGraphicsWidget *parent = 0);
    virtual ~DisplayWidget();

protected:
    void initWidget();

private slots:
    void initBrightnessSlider(int index, const QStringList &values);
    void initScreenLightsSlider(int index, const QStringList &values);    
    void initBlankInhibitButton(bool toggle);
    void sliderValueChanged(int index);
    void blankInhibitButtonPressed();

private: //methods
    DuiContainer* createContainer(QList<DuiWidgetController *> widgets, QMap<DuiWidgetController *, Qt::Alignment> alignments,
                                  Qt::Orientation policyOrientation, QString widgetObjectName = "", int policySpacing = 0);

    void addLayoutWidgets(DuiLinearLayoutPolicy *policy, const QList<DuiContainer*> &containers);
    void initSlider(DuiSlider *slider, int index, const QStringList &values);
    void updateSliderThumbLabel(DuiSlider *slider, const QString &value, const QString &pattern = QString(""));

private: //attributes        
    DuiSlider *brightnessSlider;
    DuiSlider *screenLightsSlider;
    DuiButton *blankInhibitButton;
    QStringList brightnessValues;
    QStringList screenLightsValues;
    DisplayDBusInterface *displayIf;    
};
#endif // DISPLAYWIDGET_H
