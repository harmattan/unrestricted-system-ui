#ifndef TALKTIMECONTAINER_H
#define TALKTIMECONTAINER_H

#include "timecontainer.h"

class DuiImage;
class DuiLabel;
class QTimer;

class TalkTimeContainer : public TimeContainer
{    
public:
    TalkTimeContainer(DuiWidget *parent);
    virtual ~TalkTimeContainer();

public slots:
    void updateBattery(int level);
    void startCharging();
    void stopCharging();

private: //methods
    void setLayout();
    void initImage();
    void updateImage(bool charging = true);

private: //attributes
    DuiImage *image;    
    QStringList batteryImages;
    QStringList batteryChargingImages;
    int batteryLevel;
    QTimer *timer;

};

#endif // TALKTIMECONTAINER_H
