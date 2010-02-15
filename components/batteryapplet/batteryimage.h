/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef BATTERYIMAGE_H
#define BATTERYIMAGE_H

#include <DuiImageWidget>

class QTimer;

class BatteryImage : public DuiImageWidget
{
    Q_OBJECT

public:
    BatteryImage (QGraphicsItem *parent = 0);
    ~BatteryImage ();

public slots:
    void updateBatteryLevel (int level);
    void startCharging (int rate);
    void stopCharging ();

private slots:
    void updateImage (bool charging = true);

private: //attributes
    QTimer      *timer;
    int          batteryLevel;
    QStringList  batteryImages;
    QStringList  batteryChargingImages;
};

#endif // BATTERYIMAGE_H
