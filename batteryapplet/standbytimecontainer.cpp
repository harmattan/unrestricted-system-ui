#include "standbytimecontainer.h"
#include "batterytranslation.h"

#include <DuiImage>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>

StandByTimeContainer::StandByTimeContainer(DuiWidget *parent) :
        TimeContainer(parent)
{
    setLayout();
}

StandByTimeContainer::~StandByTimeContainer()
{
}

void StandByTimeContainer::setLayout()
{
    DuiLayout *layout = new DuiLayout();
    DuiLinearLayoutPolicy *layoutPolicy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    layoutPolicy->addItem(new DuiImage("qgn_ener_standby"), Qt::AlignLeft);
    layoutPolicy->addItem(new DuiLabel(DcpBattery::StandByTimeText), Qt::AlignLeft);
    timeLabel = new DuiLabel();
    layoutPolicy->addItem(timeLabel, Qt::AlignLeft);
    centralWidget()->setLayout(layout);
}
