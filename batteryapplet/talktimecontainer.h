#ifndef TALKTIMECONTAINER_H
#define TALKTIMECONTAINER_H

#include "timecontainer.h"

class BatteryImage;

class TalkTimeContainer : public TimeContainer
{
    Q_OBJECT
public:
    TalkTimeContainer(DuiWidget *parent=0);
    virtual ~TalkTimeContainer();

    BatteryImage * batteryImage();

public slots:
    void updateBattery(int level);
    void startCharging(int rate);
    void stopCharging();

private slots:
    void updateImage(bool charging = true);

private: //methods
    void setLayout();
    void initImage();

private: //attributes
    BatteryImage *image;

};

#endif // TALKTIMECONTAINER_H
