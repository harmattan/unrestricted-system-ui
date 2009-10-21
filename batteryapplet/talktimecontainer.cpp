#include "talktimecontainer.h"
#include "batterytranslation.h"

#include <DuiImage>
#include <DuiLabel>
#include <DuiLabel>
#include <DuiGridLayoutPolicy>
#include <QDebug>
#include <QTimer>

TalkTimeContainer::TalkTimeContainer(DuiWidget *parent) :
        TimeContainer(parent),
        image(NULL),
        batteryImages(QStringList()),
        batteryChargingImages(QStringList()),
        batteryLevel(0),
        timer(NULL)
{
    setLayout();
}

TalkTimeContainer::~TalkTimeContainer()
{
}

void TalkTimeContainer::setLayout()
{
    DuiLayout *layout = new DuiLayout();
    DuiGridLayoutPolicy *layoutPolicy = new DuiGridLayoutPolicy(layout);
    initImage();
    layoutPolicy->addItemAtPosition(image, 0, 0, 2, 1, Qt::AlignLeft);
    layoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::TalkTimeText, this), 0, 1);
    timeLabel = new DuiLabel();
    layoutPolicy->addItemAtPosition(timeLabel, 1, 1);
    layoutPolicy->setColumnFixedWidth(0, 50); //battery image
    centralWidget()->setLayout(layout);
}

void TalkTimeContainer::initImage()
{
    /* Uncomment when logical names are in use
    batteryImages << BatteryImageFull100
            << BatteryImageFull75
            << BatteryImageFull50
            << BatteryImageFull25
            << BatteryImageLow
            << BatteryImageVeryLow;

    batteryChargingImages << BatteryImageChargingFull100
            << BatteryImageChargingFull75
            << BatteryImageChargingFull50
            << BatteryImageChargingFull25
            << BatteryImageChargingLow
            << BatteryImageChargingVeryLow;
    */


    //Currently the images are hard-coded
    QString path("/usr/share/themes/base/dui/libdui/svg/");
    batteryImages << QString("%1icon-s-battery-0.svg").arg(path)
            << QString("%1icon-s-battery-20.svg").arg(path)
            << QString("%1icon-s-battery-40.svg").arg(path)            
            << QString("%1icon-s-battery-60.svg").arg(path)
            << QString("%1icon-s-battery-80.svg").arg(path)
            << QString("%1icon-s-battery-100.svg").arg(path);

    batteryChargingImages << QString("%1icon-s-battery-0.svg").arg(path)
            << QString("%1icon-s-battery-20.svg").arg(path)
            << QString("%1icon-s-battery-40.svg").arg(path)            
            << QString("%1icon-s-battery-60.svg").arg(path)
            << QString("%1icon-s-battery-80.svg").arg(path)
            << QString("%1icon-s-battery-100.svg").arg(path);

    image = new DuiImage();
}

void TalkTimeContainer::updateBattery(int level)
{
    qDebug() << "TalkTimeContainer::updateBattery(" << level << ")";
    batteryLevel = level;
    if(timer == NULL)
        updateImage(false);
}

void TalkTimeContainer::updateImage(bool charging)
{    
    static int chargingImageIndex = batteryLevel;
    if(charging) {
        if(chargingImageIndex > 5)
            chargingImageIndex = batteryLevel;        
        image->setImage(QImage(batteryChargingImages.at(chargingImageIndex++)));
    }
    else {
        image->setImage(QImage(batteryImages.at(batteryLevel)));
        chargingImageIndex = batteryLevel;
    }
}

void TalkTimeContainer::startCharging()
{
    qDebug() << "TalkTimeContainer::startUpdatingChargingImage()";
    if(timer == NULL) {
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateImage()));
        timer->setInterval(400);
        timer->start();
    }
}

void TalkTimeContainer::stopCharging()
{
    qDebug() << "TalkTimeContainer::stopUpdatingChargingImage()";
    if(timer != NULL) {
        timer->stop();
        delete timer;
        timer = NULL;
    }    
}
