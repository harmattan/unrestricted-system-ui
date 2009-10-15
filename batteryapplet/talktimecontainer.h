#ifndef TALKTIMECONTAINER_H
#define TALKTIMECONTAINER_H

#include "timecontainer.h"

class DuiImage;
class DuiLabel;
class QTimer;

class TalkTimeContainer : public TimeContainer
{
    Q_OBJECT
public:
    TalkTimeContainer(DuiWidget *parent);
    virtual ~TalkTimeContainer();

public slots:
    void updateBattery(int level);
    void startCharging();
    void stopCharging();

private slots:
    void updateImage(bool charging = true);

private: //methods
    void setLayout();
    void initImage();

private: //attributes
    DuiImage *image;    
    QStringList batteryImages;
    QStringList batteryChargingImages;
    int batteryLevel;
    QTimer *timer;

};

#endif // TALKTIMECONTAINER_H
