#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include "dcpwidget.h" 

class DuiSlider;
class DuiButton;
class DisplayDBusInterface;
class DuiLinearLayoutPolicy;
class DuiStylableWidget;

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
    void initBlankTimeoutSlider(const QStringList &values);
    void initDimTimeoutSlider(const QStringList &values);
    void initBrightnessSliderValue(const QString &value);
    void initBlankTimeoutSliderValue(const QString &value);
    void initDimTimeoutSliderValue(const QString &value);    
    void brightnessSliderValueChanged(int value);
    void blankTimeoutSliderValueChanged(int value);
    void dimTimeoutSliderValueChanged(int value);
    void initBlankInhibitButtonValue(bool toggle);
    void blankInhibitButtonPressed();

private: //methods
    void addLayoutWidgets(DuiLinearLayoutPolicy *policy, const QList<DuiStylableWidget*> &widgets);
    void initSlider(DuiSlider *slider, const QStringList &values);
    void initSliderValue(DuiSlider *slider, const QStringList &values, const QString &value);
    void updateBrightnessSliderThumbLabel(const QString &value);
    void updateBlankTimeoutSliderThumbLabel(const QString &value);
    void updateDimTimeoutSliderThumbLabel(const QString &value);
    void updateSliderThumbLabel(DuiSlider *slider, const QString &value, const QString &pattern = QString(""));

private: //attributes        
    DuiSlider *brightnessSlider;
    DuiSlider *blankTimeoutSlider;
    DuiSlider *dimTimeoutSlider;
    DuiButton *blankInhibitButton;
    QStringList brightnessSliderValues;
    QStringList blankTimeoutSliderValues;
    QStringList dimTimeoutSliderValues;
    DisplayDBusInterface *displayIf;    
};
#endif // DISPLAYWIDGET_H
