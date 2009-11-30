#ifndef BATTERYWIDGET_H
#define BATTERYWIDGET_H

#include "dcpwidget.h" 

class DuiButton;
class BatteryDBusInterface;
class SliderContainer;
class StandByTimeContainer;
class TalkTimeContainer;

class BatteryWidget : public DcpWidget
{
    Q_OBJECT
		
public:
    BatteryWidget(QGraphicsWidget *parent = 0);
    virtual ~BatteryWidget();
    bool back();

protected:
    void initWidget();

private slots:
    void PSMButtonReleased();
    void updateNotChargingImage(int level);
    void updatePSMButton(bool toggle);
    void remainingTimeValuesReceived(const QStringList &timeValues);

private: //attributes
    TalkTimeContainer *talkTimeContainer;
    StandByTimeContainer *standByTimeContainer;
    SliderContainer *sliderContainer;
    BatteryDBusInterface *batteryIf;
    DuiButton *PSMButton;    

};
#endif // BATTERYWIDGET_H
