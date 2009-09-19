#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include "dcpwidget.h" 

class DuiSlider;
class DuiButton;
class DisplayDBusInterface;

class DisplayWidget : public DcpWidget
{
    Q_OBJECT
		
public:
    DisplayWidget(QGraphicsWidget *parent = 0);
    virtual ~DisplayWidget();

protected:
    void initWidget();

private slots:
    void initBrightnessSlider(const QStringList &values);
    void initScreenLightsSlider(const QStringList &values);    
    void updateBrightnessSlider(const QString &value);
    void updateScreenLightsSlider(const QString &value);
    void brightnessSliderValueChanged(int value);
    void screenLightsSliderValueChanged(int value);
    void updateScreenLightsButton(bool toggle);
    void screenLightsButtonPressed();

private: //methods
    void initSlider(DuiSlider *slider, const QStringList &values);

private: //attributes        
    DuiSlider *brightnessSlider;
    DuiSlider *screenLightsSlider;
    DuiButton *screenLightsButton;
    QStringList brightnessSliderValues;
    QStringList screenLightsSliderValues;
    DisplayDBusInterface *displayIf;
};
#endif // DISPLAYWIDGET_H
