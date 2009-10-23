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
    image = new DuiImage();


    batteryImages << QString("icon-m-battery-verylow") << QString("icon-m-battery-low") << QString("icon-m-battery-13")
            << QString("icon-m-battery-25") << QString("icon-m-battery-38") << QString("icon-m-battery-50")
            << QString("icon-m-battery-62") << QString("icon-m-battery-75") << QString("icon-m-battery-88")
            << QString("icon-m-battery-100");

    batteryChargingImages << QString("") << QString("") << QString("icon-s-battery-13")
            << QString("icon-s-battery-25") << QString("icon-s-battery-38") << QString("icon-s-battery-50")
            << QString("icon-s-battery-62") << QString("icon-s-battery-75") << QString("icon-s-battery-88")
            << QString("icon-s-battery-100");    
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
        if(chargingImageIndex >= batteryChargingImages.size())
            chargingImageIndex = (batteryLevel > 1 ? batteryLevel : 2);        
        image->setImage(batteryChargingImages.at(chargingImageIndex++));
    }
    else {
        image->setImage(batteryImages.at(batteryLevel));
        chargingImageIndex = batteryLevel;
    }
}

void TalkTimeContainer::startCharging(int rate)
{
    qDebug() << "TalkTimeContainer::startCharging(" << rate << ")";
    if(timer == NULL) {
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(updateImage()));
        timer->setInterval(rate);
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
