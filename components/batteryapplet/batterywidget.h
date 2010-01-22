/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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
    BatteryWidget (QGraphicsWidget *parent = 0);
    ~BatteryWidget ();
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
    // true means we are believed to be in power save mode
    bool                     PSMButtonToggle;
    SliderContainer         *sliderContainer;
    TimeContainer           *standByTimeContainer;
    TimeContainer           *talkTimeContainer;

    void    retranslateUi ();

};
#endif // BATTERYWIDGET_H
