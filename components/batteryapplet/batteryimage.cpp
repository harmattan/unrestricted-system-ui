/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "batteryimage.h"

#include <QTimer>

#define DEBUG
#include "../debug.h"

BatteryImage::BatteryImage (QGraphicsItem *parent) :
        DuiImageWidget (parent),
        timer (NULL),
        batteryLevel (0)
{
    /*
     * We have to show something even if we get no signals from DBus. FIXME:
     * maybe this is not the right image, but it is the only one works now.
     */
#if 0
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
	    QString ("") << 
	    QString ("") << 
	    QString ("icon-s-battery-13") << 
	    QString ("icon-s-battery-25") << 
	    QString ("icon-s-battery-38") << 
	    QString ("icon-s-battery-50") << 
	    QString ("icon-s-battery-62") << 
	    QString ("icon-s-battery-75") << 
	    QString ("icon-s-battery-88") << 
	    QString ("icon-s-battery-100");
#else
    // Seems themes still not contains the proper images ^
    setImage ("icon-s-battery-60");

    batteryImages <<
        QString ("icon-s-battery-0") <<
        QString ("icon-s-battery-20") <<
        QString ("icon-s-battery-40") <<
        QString ("icon-s-battery-60") <<
        QString ("icon-s-battery-80") <<
        QString ("icon-s-battery-100");

    batteryChargingImages = batteryImages;
#endif
}

BatteryImage::~BatteryImage ()
{
    delete timer;
    timer = NULL;
}

void 
BatteryImage::updateBatteryLevel (int level)
{
    SYS_DEBUG ("level = %d", level);

    batteryLevel = level;
    if (timer == NULL)
        updateImage (false);
}

void 
BatteryImage::updateImage (
		bool charging)
{
#if 0
    static int chargingImageIndex = batteryLevel;
    if (charging) {
        if (chargingImageIndex >= batteryChargingImages.size ())
            chargingImageIndex = (batteryLevel > 1 ? batteryLevel : 2);
        SYS_DEBUG ("Charging index: %d", chargingImageIndex);
        setImage (batteryChargingImages.at (chargingImageIndex++));
    } else {
        SYS_DEBUG ("Normal index: %d", batteryLevel);
        setImage (batteryImages.at (batteryLevel));
        chargingImageIndex = batteryLevel;
    }
#else
    SYS_DEBUG ("charging = %s", charging ? "true" : "false");

    static int imageIndex = batteryLevel;

    if (charging)
    {
        imageIndex++;
    }
    else
    {
        imageIndex = batteryLevel;
    }

    if (batteryImages.size () <= imageIndex)
       imageIndex = 0;

    setImage (batteryImages.at (imageIndex)); 
#endif
}

void
BatteryImage::startCharging (
		int rate)
{
    if (rate < 0) //USB 100mA
        return;

    if (timer == NULL) {
        timer = new QTimer (this);
        connect (timer, SIGNAL (timeout ()),
                 this, SLOT (updateImage ()));
    }

    timer->setInterval (rate);
    timer->start ();
}

void
BatteryImage::stopCharging ()
{
    if (timer != NULL) {
        timer->stop ();
        delete timer;
        timer = NULL;
    }
}

