#include "batterywidget.h"
#include "batterytranslation.h"
#include "dcpbattery.h"
#include "dcpspaceritem.h"

#include <DuiButton>
#include <DuiLayout>
#include <DuiGridLayoutPolicy>
#include <DuiLinearLayoutPolicy>
#include <DuiLabel>
#include <DuiSlider>

BatteryWidget::BatteryWidget(QGraphicsWidget *parent)
	    :DcpWidget(parent)
{
    setReferer(DcpBattery::None);
    initWidget();
}

BatteryWidget::~BatteryWidget()
{
    delete batteryGConf;
    batteryGConf = NULL;
}

void BatteryWidget::initWidget()
{
    //create gconf if
    batteryGConf = new BatteryGConf();
    connect(batteryGConf, SIGNAL(valueChanged(BatteryGConf::GConfKey, QVariant)),
            this, SLOT(gConfValueChanged(BatteryGConf::GConfKey, QVariant)));

    //testing purposes
    batteryGConf->setValue(BatteryGConf::RemainingTalkTimeKey, QVariant(140));
    batteryGConf->setValue(BatteryGConf::RemainingStandByTimeKey, QVariant(300));
    batteryGConf->setValue(BatteryGConf::PSMThresholdKey, QVariant(20));
    QList<QVariant> values;
    values << QVariant(10) << QVariant(20) << QVariant(30) << QVariant(60) << QVariant(90) << QVariant(120) << QVariant(150) << QVariant(180) << QVariant(210);
    batteryGConf->setValue(BatteryGConf::PSMThresholdValuesKey, values);
    batteryGConf->setValue(BatteryGConf::PSMToggleKey, QVariant(true));

    //create talkTimeLayout
    DuiLayout *talkTimeLayout = new DuiLayout(0);
    DuiGridLayoutPolicy *talkTimeLayoutPolicy = new DuiGridLayoutPolicy(talkTimeLayout);
    //TODO add the charging icon to position 0, 0, 1, 2
    talkTimeLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::TalkTimeText), 0, 1);
    talkTimeLabel = new DuiLabel(minutesInString(batteryGConf->value(BatteryGConf::RemainingTalkTimeKey).toInt(), DcpBattery::TalkTimeValueText));
    talkTimeLayoutPolicy->addItemAtPosition(talkTimeLabel, 1, 1);

    //create standByTimeLayout
    DuiLayout *standByTimeLayout = new DuiLayout(0);
    DuiLinearLayoutPolicy *standByTimeLayoutPolicy = new DuiLinearLayoutPolicy(standByTimeLayout, Qt::Vertical);
    standByTimeLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::StandByTimeText), 0);
    standByTimeLabel = new DuiLabel(minutesInString(batteryGConf->value(BatteryGConf::RemainingStandByTimeKey).toInt(), DcpBattery::StandByTimeValueText));
    standByTimeLayoutPolicy->addItemAtPosition(standByTimeLabel, 1);

    //create PSMLayout
    DuiLayout *PSMLayout = new DuiLayout(0);
    DuiGridLayoutPolicy *PSMLayoutPolicy = new DuiGridLayoutPolicy(PSMLayout);
    PSMLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::PSMText), 0, 0);
    PSMLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::NBCText), 1, 0);
    PSMButton = new DuiButton();
    PSMButton->setCheckable(true);
    updateButton(PSMButton, batteryGConf->value(BatteryGConf::PSMToggleKey).toBool());
    PSMLayoutPolicy->addItemAtPosition(PSMButton, 0, 1, 2, 1);   

    //create upperLayout and put talkTimeLayot, standByTimeLayout and PSMLayout into it
    DuiLayout *upperLayout = new DuiLayout(0);
    DuiGridLayoutPolicy *upperLayoutPolicy = new DuiGridLayoutPolicy(upperLayout);
    upperLayoutPolicy->addItemAtPosition(talkTimeLayout, 0, 0);
    upperLayoutPolicy->addItemAtPosition(standByTimeLayout, 0, 1);
    upperLayoutPolicy->addItemAtPosition(PSMLayout, 1, 0);

    //create disablePSMLayout
    DuiLayout *disablePSMLayout = new DuiLayout(0);
    DuiLinearLayoutPolicy *disablePSMLayoutPolicy = new DuiLinearLayoutPolicy(disablePSMLayout, Qt::Horizontal);
    disablePSMButton = new DuiButton();
    disablePSMButton->setCheckable(true);
    updateButton(disablePSMButton, batteryGConf->value(BatteryGConf::PSMDisabledKey).toBool());
    disablePSMLayoutPolicy->addItemAtPosition(disablePSMButton, 0);
    disablePSMLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::DisablePSMText), 1);

    //create lowerLayout and put disablePSMLayout into it
    DuiLayout *lowerLayout = new DuiLayout(0);
    DuiLinearLayoutPolicy *lowerLayoutPolicy = new DuiLinearLayoutPolicy(lowerLayout, Qt::Vertical);
    lowerLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::AutoPSMText), 0);
    lowerLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::AutoPSMDescText), 1);
    lowerLayoutPolicy->addItemAtPosition(disablePSMLayout, 2);
    
    //temp duiLabel (as long as the duiSlider doesn't support thumbLabel)
    sliderLabel = new DuiLabel(minutesInString(batteryGConf->value(BatteryGConf::PSMThresholdKey).toInt(), DcpBattery::PSMThresholdValueText));
    lowerLayoutPolicy->addItemAtPosition(sliderLabel, 3);

    initSlider();    
    lowerLayoutPolicy->addItemAtPosition(slider, 4);
    lowerLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::AutoPSMAdv1Text), 5);
    lowerLayoutPolicy->addItemAtPosition(new DuiLabel(QString("- " + DcpBattery::AutoPSMAdv2Text)), 6);
    lowerLayoutPolicy->addItemAtPosition(new DuiLabel(QString("- " + DcpBattery::AutoPSMAdv3Text)), 7);

    /*
    landscapePolicy->addItemAtPosition(
                    new DcpSpacerItem(this, widgetWidth, 10, QSizePolicy::Expanding, 
                    QSizePolicy::Expanding), 0, Qt::AlignLeft);
    mainLayoutPolicy->addItemAtPosition(centralLayout, 1, Qt::AlignHCenter);
    mainLayoutPolicy->addItemAtPosition(
                    new DcpSpacerItem(this, widgetWidth, 10, QSizePolicy::Expanding,
                    QSizePolicy::Expanding), 2, Qt::AlignRight);
                    */
    //create mainLayout
    DuiLayout *mainLayout = new DuiLayout(this);
    DuiLinearLayoutPolicy *landscapeLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    //TODO create portraitLayoutPolicy and listen the orientation change signals    
    landscapeLayoutPolicy->addItemAtPosition(upperLayout, 0);
    landscapeLayoutPolicy->addItemAtPosition(lowerLayout, 1);


    this->setLayout(mainLayout);    
}

void BatteryWidget::initSlider()
{
    slider = new DuiSlider();

    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));

    sliderValues = (batteryGConf->value(BatteryGConf::PSMThresholdValuesKey)).toList();
    slider->setRange(0,sliderValues.size()-1);
    slider->setValue(sliderValues.indexOf(batteryGConf->value(BatteryGConf::PSMThresholdKey).toInt()));
    slider->setThumbLabel(minutesInString(batteryGConf->value(BatteryGConf::PSMThresholdKey).toInt(), DcpBattery::PSMThresholdValueText));
    slider->setThumbLabelVisible(true);
    slider->setOrientation(Qt::Horizontal);
}

void BatteryWidget::sliderValueChanged(int newValue)
{
    slider->setThumbLabel(minutesInString(sliderValues.at(newValue).toInt(), DcpBattery::PSMThresholdValueText));

    //temp duiLabel (as long as the duiSlider doesn't support thumbLabel)
    sliderLabel->setText(minutesInString(sliderValues.at(newValue).toInt(), DcpBattery::PSMThresholdValueText));

}
void BatteryWidget::gConfValueChanged(BatteryGConf::GConfKey key, QVariant value)        
{
    switch(key) {
        default:
        break;
    }
}

QString BatteryWidget::minutesInString(int time, QString pattern)
{
    //removing possible extra "!! " from in front
    QString patternCut = pattern.right(pattern.length() - pattern.indexOf("%1"));
    //saving possible extra "!! " from in front
    QString prefix = pattern.left(pattern.indexOf("%1"));

    QStringList list = patternCut.split("%1", QString::SkipEmptyParts);

    if(time < 60)
        return QString("%1%2%3").arg(prefix).arg(time).arg(list.at(0).trimmed());
    else {
        QVariant minutes = time%60;
        if(minutes.toInt() == 0)
            minutes = "00";
        return QString("%1%2:%3%4").arg(prefix).arg(time/60).arg(minutes.toString()).arg(list.at(1).trimmed());
    }
}


void BatteryWidget::updateButton(DuiButton *button, bool toggle)
{
    if(toggle)
        button->setText("ON");
    else
        button->setText("OFF");
    button->setChecked(toggle);
}
