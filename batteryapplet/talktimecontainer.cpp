#include "talktimecontainer.h"
#include "batterytranslation.h"
#include "batteryimage.h"

#include <DuiLabel>
#include <DuiGridLayoutPolicy>
#include <QDebug>

TalkTimeContainer::TalkTimeContainer(DuiWidget *parent) :
        TimeContainer(parent),
        image(NULL)
{
    image = new BatteryImage();
    setLayout();
}

TalkTimeContainer::~TalkTimeContainer()
{
    delete image;
    image = NULL;
}

BatteryImage * TalkTimeContainer::batteryImage()
{
    return image;
}

void TalkTimeContainer::setLayout()
{
    DuiLayout *layout = new DuiLayout();
    DuiGridLayoutPolicy *layoutPolicy = new DuiGridLayoutPolicy(layout);    
    layoutPolicy->addItemAtPosition(image, 0, 0, 2, 1, Qt::AlignLeft);
    layoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::TalkTimeText, this), 0, 1);
    timeLabel = new DuiLabel();
    layoutPolicy->addItemAtPosition(timeLabel, 1, 1);
    layoutPolicy->setColumnFixedWidth(0, 50);
    centralWidget()->setLayout(layout);
}
