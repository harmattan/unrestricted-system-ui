#include "standbytimecontainer.h"
#include "batterytranslation.h"

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
    layoutPolicy->addItem(new DuiLabel(DcpBattery::StandByTimeText, this), Qt::AlignLeft);
    timeLabel = new DuiLabel();
    layoutPolicy->addItem(timeLabel, Qt::AlignLeft);
    centralWidget()->setLayout(layout);
}
