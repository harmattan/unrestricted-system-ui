#include "batterywidget.h"
#include "batterytranslation.h"
#include "dcpbattery.h"
#include "dcpspaceritem.h"

#include <QDebug>

#include <DuiButton>
#include <DuiLayout>
#include <DuiGridLayoutPolicy>
#include <DuiLinearLayoutPolicy>
#include <DuiLabel>
#include <DuiSlider>
#include <DuiImage>

BatteryWidget::BatteryWidget(QGraphicsWidget *parent)
	    :DcpWidget(parent)
{
    setReferer(DcpBattery::None);
    initWidget();
}

BatteryWidget::~BatteryWidget()
{
    //indicate the batteryBusinessLogic that battery system setting is no more used
    batteryGConf->setValue(BatteryGConf::BatterySystemSettingInUseKey, QVariant(false));

    delete batteryGConf;
    batteryGConf = NULL;
}

void BatteryWidget::initWidget()
{
    //create gconf if
    batteryGConf = new BatteryGConf();

    //indicate the batteryBusinessLogic that battery system setting is used
    batteryGConf->setValue(BatteryGConf::BatterySystemSettingInUseKey, QVariant(true));

    //create talkTimeLayout
    DuiLayout *talkTimeLayout = new DuiLayout(0);
    DuiGridLayoutPolicy *talkTimeLayoutPolicy = new DuiGridLayoutPolicy(talkTimeLayout);
    //TODO add the charging icon to position 0, 0, 1, 2
    //initChargingIcon();
    chLabel = new DuiLabel(batteryGConf->value(BatteryGConf::ChargingKey).toBool() ? "Charging" : "Not Charging");//temp
    talkTimeLayoutPolicy->addItemAtPosition(chLabel, 0, 0, 2, 1);
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
    PSMButton->setObjectName("PSMButton");
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
    disablePSMButton->setObjectName("disablePSMButton");
    updateButton(disablePSMButton, batteryGConf->value(BatteryGConf::PSMDisabledKey).toBool());    
    disablePSMLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::DisablePSMText), 0);
    disablePSMLayoutPolicy->addItemAtPosition(disablePSMButton, 1);

    //create lowerLayout and put disablePSMLayout into it
    DuiLayout *lowerLayout = new DuiLayout(0);
    DuiLinearLayoutPolicy *lowerLayoutPolicy = new DuiLinearLayoutPolicy(lowerLayout, Qt::Vertical);
    lowerLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::AutoPSMText), 0);
    lowerLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::AutoPSMDescText), 1);
    initSlider();
    lowerLayoutPolicy->addItemAtPosition(slider, 2);
    lowerLayoutPolicy->addItemAtPosition(disablePSMLayout, 3);
    lowerLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::AutoPSMAdv1Text), 4);
    lowerLayoutPolicy->addItemAtPosition(new DuiLabel(QString("- " + DcpBattery::AutoPSMAdv2Text)), 5);
    lowerLayoutPolicy->addItemAtPosition(new DuiLabel(QString("- " + DcpBattery::AutoPSMAdv3Text)), 6);

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

    // catch user actions
    connect(PSMButton, SIGNAL(pressed()), this, SLOT(buttonPressed()));
    connect(disablePSMButton, SIGNAL(pressed()), this, SLOT(buttonPressed()));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    
    // catch system actions
    connect(batteryGConf, SIGNAL(valueChanged(BatteryGConf::GConfKey, QVariant)),
            this, SLOT(gConfValueChanged(BatteryGConf::GConfKey, QVariant)));

    //testing purposes
    /*
    batteryGConf->setValue(BatteryGConf::RemainingTalkTimeKey, QVariant(140));
    batteryGConf->setValue(BatteryGConf::RemainingStandByTimeKey, QVariant(300));
    batteryGConf->setValue(BatteryGConf::PSMThresholdKey, QVariant(20));
    QList<QVariant> values;
    values << QVariant(10) << QVariant(20) << QVariant(30) << QVariant(60) << QVariant(90) << QVariant(120) << QVariant(150) << QVariant(180) << QVariant(210);
    batteryGConf->setValue(BatteryGConf::PSMThresholdValuesKey, values);
    //batteryGConf->setValue(BatteryGConf::PSMToggleKey, QVariant(true));
    */

    //disable the PSM widgets if PSM is disabled
    togglePSMWidgets(!batteryGConf->value(BatteryGConf::PSMDisabledKey).toBool());

    this->setLayout(mainLayout);
}
/*
void BatterWidget::initChargingIcon()
{
    chargingImages << "dui_ener_m_verylow"
    if(batteryGConf->value(BatteryGConf::ChargingKey).toBool()) { //is charging
        imageUpdateTimer = new QTimer(0);
        connect(imageUpdateTimer, SIGNAL(timeout()), this, SLOT(updateChargingImage()));
        backspaceTimer->start(200);
    }
    else {
        updateImage(



}
*/
void BatteryWidget::initSlider()
{
    slider = new DuiSlider(0, "continuous");

    sliderValues = (batteryGConf->value(BatteryGConf::PSMThresholdValuesKey)).toList();
    slider->setRange(0,sliderValues.size()-1);
    slider->setValue(sliderValues.indexOf(batteryGConf->value(BatteryGConf::PSMThresholdKey).toInt()));
    slider->setThumbLabel(QString("%1%").arg(batteryGConf->value(BatteryGConf::PSMThresholdKey).toInt()));
    slider->setThumbLabelVisible(true);
    slider->setOrientation(Qt::Horizontal);
}

void BatteryWidget::sliderValueChanged(int newValue)
{  
    batteryGConf->setValue(BatteryGConf::PSMThresholdKey, QVariant(sliderValues.at(newValue)));
}

void BatteryWidget::buttonPressed()
{    
    DuiButton *duiButton = static_cast<DuiButton*>(this->sender());
    BatteryGConf::GConfKey key;
    QVariant value;

    if(duiButton->objectName() == "PSMButton") {
        key = BatteryGConf::PSMToggleKey;
        value = batteryGConf->value(key).toBool() ? QVariant(false) : QVariant(true);
    }
    else if(duiButton->objectName() == "disablePSMButton") {
        key = BatteryGConf::PSMDisabledKey;
        value = batteryGConf->value(key).toBool() ? QVariant(false) : QVariant(true);
    }
    batteryGConf->setValue(key, value);
}

void BatteryWidget::gConfValueChanged(BatteryGConf::GConfKey key, QVariant value)
{    
    switch(key) {
        case BatteryGConf::PSMToggleKey:
            updateButton(PSMButton, value.toBool());
            break;
        case BatteryGConf::PSMDisabledKey:
            updateButton(disablePSMButton, value.toBool());
            togglePSMWidgets(!value.toBool());
            break;
        case BatteryGConf::PSMThresholdKey:
            updateSlider(QString("%1%").arg(value.toInt()));
            break;
        case BatteryGConf::RemainingTalkTimeKey:
            updateLabel(talkTimeLabel, minutesInString(value.toInt(), DcpBattery::TalkTimeText));
            break;
        case BatteryGConf::RemainingStandByTimeKey:
            updateLabel(standByTimeLabel, minutesInString(value.toInt(), DcpBattery::StandByTimeText));
            break;
        case BatteryGConf::BatteryLevelKey:
            updateBatteryIcon(batteryGConf->value(BatteryGConf::ChargingKey).toBool(), value.toInt());
            break;
        case BatteryGConf::ChargingKey:
            chLabel->setText(value.toBool() ? "Charging" : "Not Charging");//temp
            updateBatteryIcon(value.toBool(), batteryGConf->value(BatteryGConf::BatteryLevelKey).toInt());
            break;
        default:
            break;
    }   
}

QString BatteryWidget::minutesInString(int mins, QString pattern)
{       
    //removing possible extra "!! " from in front
    QString patternCut = pattern.right(pattern.length() - pattern.indexOf("%1"));
    //saving possible extra "!! " from in front
    QString prefix = pattern.left(pattern.indexOf("%1"));

    QStringList list = patternCut.split("%1", QString::SkipEmptyParts);

    if(mins < 60)
        return QString("%1%2%3").arg(prefix).arg(mins).arg(list.at(0).trimmed());
    else {
        QVariant minutes = mins%60;
        if(minutes.toInt() == 0)
            minutes = "00";
        return QString("%1%2:%3%4").arg(prefix).arg(mins/60).arg(minutes.toString()).arg(list.at(1).trimmed());
    }
}

void BatteryWidget::togglePSMWidgets(bool enable)
{
    PSMButton->setEnabled(enable);
    updateButton(PSMButton, false);
    slider->setEnabled(enable);
}

void BatteryWidget::updateButton(DuiButton *button, bool toggle)
{    
    if(toggle)
        button->setText("ON"); //This should be localized
    else
        button->setText("OFF"); //This should be localized

    button->setChecked(toggle);    

}

void BatteryWidget::updateSlider(const QString &text)
{    
    slider->setThumbLabel(text);    
}

void BatteryWidget::updateLabel(DuiLabel *label, const QString &text)
{
    label->setText(text);
}

void BatteryWidget::updateBatteryIcon(bool charging, int level)
{
}

