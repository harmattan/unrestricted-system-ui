#ifndef BATTERYIMAGE_H
#define BATTERYIMAGE_H

#include <DuiImageWidget>

class QTimer;

class BatteryImage : public DuiImageWidget
{
    Q_OBJECT

public:
    BatteryImage(QGraphicsItem *parent = 0);
    virtual ~BatteryImage();

public slots:
    void updateBatteryLevel(int level);
    void startCharging(int rate);
    void stopCharging();

private slots:
    void updateImage(bool charging = true);

private: //attributes
    QTimer *timer;
    int batteryLevel;
    QStringList batteryImages;
    QStringList batteryChargingImages;
};

#endif // BATTERYIMAGE_H
