#ifndef BATTERYWIDGET_H
#define BATTERYWIDGET_H

#include "dcpwidget.h"

class DuiButton;
class BatteryDBusInterface;
class BatteryImage;
class SliderContainer;
class TimeContainer;

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

private:
    BatteryDBusInterface    *batteryIf;
    BatteryImage            *batteryImage;
    DuiButton               *PSMButton;
    bool                     PSMButtonToggle;
    SliderContainer         *sliderContainer;
    TimeContainer           *standByTimeContainer;
    TimeContainer           *talkTimeContainer;

    void    retranslateUi ();

};
#endif // BATTERYWIDGET_H
