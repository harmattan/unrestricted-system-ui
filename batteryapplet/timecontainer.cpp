#include "timecontainer.h"
#include "batterytranslation.h"

#include <DuiLabel>

TimeContainer::TimeContainer(DuiWidget *parent) :
        DuiContainer(parent),        
        timeLabel(NULL)
{    
}

TimeContainer::~TimeContainer()
{
}

void TimeContainer::updateTimeLabel(int minutes)
{
    if(timeLabel == NULL)
        return;

    QString minutesPrefix = DcpBattery::TimeValueText.section("%b", 1, 1).trimmed();
    QString hoursPrefix = (DcpBattery::TimeValueText.section("%b", 0, 0)).section("%a", 1, 1).trimmed();
    QString time;

    if(minutes < 60)
        time = QString("%1%2").arg(minutes).arg(minutesPrefix);
    else {
        QVariant minsVar = minutes%60;
        minsVar = (minsVar.toInt() == 0) ? "00" : minsVar;
        time = QString("%1:%2%3").arg(minutes/60).arg(minsVar.toString()).arg(hoursPrefix);
    }
    timeLabel->setText(time);
}
