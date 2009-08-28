#ifndef BATTERYWIDGET_H
#define BATTERYWIDGET_H

#include "dcpwidget.h" 

class DuiLabel;
class DuiButton;
class DuiSlider;
class BatteryGConf;

class BatteryWidget : public DcpWidget
{
    Q_OBJECT
		
public:
    BatteryWidget(QGraphicsWidget *parent = 0);
    virtual ~BatteryWidget();

protected:
    void initWidget();

public slots:    
    void BatteryPSMToggleChanged(QString key, bool value);

private slots:
    void sliderValueChanged(int newValue);

private: //methods
    void initSlider();
    void initBatteryGConf();
    QString timeInString(int time, QString pattern);
    void updateButton(DuiButton *button, bool toggle);    

private: //attributes
    DuiLabel *talkTimeLabel;
    DuiLabel *standByTimeLabel;
    DuiLabel *sliderLabel;//temp duiLabel (as long as the duiSlider doesn't support thumbLabel)
    DuiButton *PSMButton;
    DuiButton *disablePSMButton;   
    DuiSlider *slider;
    QList<int> sliderValues;
    BatteryGConf *batteryGConf;

};
#endif // BATTERYWIDGET_H
