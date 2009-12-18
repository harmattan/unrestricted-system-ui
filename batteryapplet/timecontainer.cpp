#include "timecontainer.h"
#include "batterytranslation.h"

#include <DuiGridLayoutPolicy>
#include <DuiImage>
#include <DuiLabel>
#include <DuiLayout>

TimeContainer::TimeContainer(const QString &text, DuiImage *image, DuiWidget *parent) :
        DuiContainer(parent),
        image(image),
        textLabel(NULL),
        timeLabel(NULL)
{
    timeLabel = new DuiLabel();
    timeLabel->setObjectName("batteryTimeLabel");
    textLabel = new DuiLabel(text);
    textLabel->setObjectName("batteryLabel");
    setHeaderVisible(false);
    setLayout();
}

TimeContainer::~TimeContainer()
{
}

void TimeContainer::updateTimeLabel(const QString &value)
{
    if (timeLabel == NULL)
        return;

    if (value.toInt() == 0) {// a text, not minute value
        timeLabel->setText(value);
        return;
    }
    int minutes = value.toInt();

    const QString minutesPrefix = DcpBattery::TimeValueText.section("%b", 1, 1).trimmed();
    const QString hoursPrefix = (DcpBattery::TimeValueText.section("%b", 0, 0)).section("%a", 1, 1).trimmed();
    QString time;

    if (minutes < 60)
        time = QString("%1%2").arg(minutes).arg(minutesPrefix);
    else {
        QVariant minsVar = minutes % 60;
        minsVar = (minsVar.toInt() == 0) ? "00" : minsVar;
        time = QString("%1:%2%3").arg(minutes / 60).arg(minsVar.toString()).arg(hoursPrefix);
    }
    timeLabel->setText(time);
}

void TimeContainer::setLayout()
{
    // set the layout
    DuiLayout *layout = new DuiLayout();
    DuiGridLayoutPolicy *layoutPolicy = new DuiGridLayoutPolicy(layout);
    centralWidget()->setLayout(layout);

    // add the widgets
    layoutPolicy->addItemAtPosition(image, 0, 0, 2, 1);
    layoutPolicy->addItemAtPosition(textLabel, 0, 1, 1, 1);
    layoutPolicy->addItemAtPosition(timeLabel, 1, 1, 1, 1);
}
