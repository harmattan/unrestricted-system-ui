#ifndef BATTERYWIDGET_H
#define BATTERYWIDGET_H

#include "dcpwidget.h" 
#include "batterygconf.h"

class DuiLabel;
class DuiButton;
class DuiSlider;

class BatteryWidget : public DcpWidget
{
    Q_OBJECT
		
public:
    BatteryWidget(QGraphicsWidget *parent = 0);
    virtual ~BatteryWidget();

protected:
    void initWidget();

private slots:
    void sliderValueChanged(int newValue);
    void gConfValueChanged(BatteryGConf::GConfKey key, QVariant value);

private: //methods
    void initSlider();    
    QString minutesInString(int time, QString pattern);
    void updateButton(DuiButton *button, bool toggle);    

private: //attributes
    DuiLabel *talkTimeLabel;
    DuiLabel *standByTimeLabel;
    DuiLabel *sliderLabel;//temp duiLabel (as long as the duiSlider doesn't support thumbLabel)
    DuiButton *PSMButton;
    DuiButton *disablePSMButton;   
    DuiSlider *slider;
    QList<QVariant> sliderValues;
    BatteryGConf *batteryGConf;

};
#endif // BATTERYWIDGET_H
