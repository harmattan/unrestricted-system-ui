#include "timecontainer.h"

#include <DuiGridLayoutPolicy>
#include <DuiImageWidget>
#include <DuiLabel>
#include <DuiLayout>

TimeContainer::TimeContainer(
		const QString  &text, 
		DuiImageWidget *image, 
		DuiWidget      *parent) :
	DuiContainer(parent),
        image(image),
        textLabel(NULL),
        timeLabel(NULL)
{
    timeLabel = new DuiLabel ("N/A");
    timeLabel->setObjectName ("batteryTimeLabel");
    textLabel = new DuiLabel (text);
    textLabel->setObjectName ("batteryLabel");
    setHeaderVisible (false);
    setLayout ();
}


void 
TimeContainer::setText (
		const QString &text)
{
    textLabel->setText (text);
}

void TimeContainer::updateTimeLabel (const QString &value)
{
    //% "%a hours %b minutes"
    const QString TimeValueText = qtTrId ("qtn_ener_remtime");

    if (timeLabel == NULL)
        return;

    if (value.toInt() == 0) {// a text, not minute value
        timeLabel->setText(value);
        return;
    }
    int minutes = value.toInt();

    const QString minutesPrefix =
        TimeValueText.section("%b", 1, 1).trimmed();
    const QString hoursPrefix =
        (TimeValueText.section("%b", 0, 0)).section("%a", 1, 1).trimmed();

    QString time;

    if (minutes < 60)
        time = QString("%1 %2").arg(minutes).arg(minutesPrefix);
    else {
        QVariant minsVar = minutes % 60;
        minsVar = (minsVar.toInt() == 0) ? "00" : minsVar;
        time = QString("%1:%2 %3").arg(minutes / 60).arg(minsVar.toString()).arg(hoursPrefix);
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
    layoutPolicy->addItem(image, 0, 0, 2, 1);
    layoutPolicy->addItem(textLabel, 0, 1, 1, 1);
    layoutPolicy->addItem(timeLabel, 1, 1, 1, 1);
}

