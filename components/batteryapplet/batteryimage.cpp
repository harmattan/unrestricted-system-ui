/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "batteryimage.h"

#include <QTimer>
#include <QDebug>

BatteryImage::BatteryImage(QGraphicsItem *parent) :
        DuiImageWidget(parent),
        timer(NULL),
        batteryLevel(0)
{
    /*
     * We have to show something even if we get no signals from DBus. FIXME:
     * maybe this is not the right image, but it is the only one works now.
     */
    setImage ("icon-m-battery");

    batteryImages << 
	    QString ("icon-m-battery-verylow") << 
	    QString ("icon-m-battery-low") << 
	    QString ("icon-m-battery-13") << 
	    QString ("icon-m-battery-25") << 
	    QString ("icon-m-battery-38") << 
	    QString ("icon-m-battery-50") << 
	    QString ("icon-m-battery-62") << 
	    QString ("icon-m-battery-75") << 
	    QString ("icon-m-battery-88") << 
	    QString ("icon-m-battery-100");

    batteryChargingImages << 
	    QString("") << 
	    QString("") << 
	    QString("icon-s-battery-13") << 
	    QString("icon-s-battery-25") << 
	    QString("icon-s-battery-38") << 
	    QString("icon-s-battery-50") << 
	    QString("icon-s-battery-62") << 
	    QString("icon-s-battery-75") << 
	    QString("icon-s-battery-88") << 
	    QString("icon-s-battery-100");
}

BatteryImage::~BatteryImage()
{
    delete timer;
    timer = NULL;
}

void 
BatteryImage::updateBatteryLevel(int level)
{
    batteryLevel = level;
    if (timer == NULL)
        updateImage (false);
}

void 
BatteryImage::updateImage (
		bool charging)
{
    static int chargingImageIndex = batteryLevel;
    if (charging) {
        if (chargingImageIndex >= batteryChargingImages.size())
            chargingImageIndex = (batteryLevel > 1 ? batteryLevel : 2);
        qDebug() << "Charging index: " << chargingImageIndex;
        setImage(batteryChargingImages.at(chargingImageIndex++));
    } else {
        qDebug() << "Normal index: " << batteryLevel;
        setImage(batteryImages.at(batteryLevel));
        chargingImageIndex = batteryLevel;
    }
}

void
BatteryImage::startCharging (
		int rate)
{
    if (rate < 0) //USB 100mA
        return;

    if (timer == NULL) {
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateImage()));
    }

    timer->setInterval(rate);
    timer->start();
}

void
BatteryImage::stopCharging()
{
    if (timer != NULL) {
        timer->stop();
        delete timer;
        timer = NULL;
    }
}
