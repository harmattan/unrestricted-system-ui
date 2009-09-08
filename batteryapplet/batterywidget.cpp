#include "batterywidget.h"
#include "batterytranslation.h"
#include "dcpbattery.h"
#include "dcpspaceritem.h"

#include <QDebug>
#include <QTimer>
#include <QColor>

#include <DuiButton>
#include <DuiLayout>
#include <DuiGridLayoutPolicy>
#include <DuiLinearLayoutPolicy>
#include <DuiLabel>
#include <DuiSlider>
#include <DuiImage>
#include <DuiSceneManager>
#include <DuiStylableWidget>

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

    int fullWidth = DuiSceneManager::instance()->visibleSceneSize().width();

    /*
     * talkTimeLayoutWidget
     */    
    DuiLayout *talkTimeLayout = new DuiLayout(0);
    DuiGridLayoutPolicy *talkTimeLayoutPolicy = new DuiGridLayoutPolicy(talkTimeLayout);    
    initImage();
    talkTimeLayoutPolicy->addItemAtPosition(image, 0, 0, 2, 1, Qt::AlignLeft);
    talkTimeLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::TalkTimeText), 0, 1);
    talkTimeLabel = new DuiLabel();
    talkTimeLayoutPolicy->addItemAtPosition(talkTimeLabel, 1, 1);    

    DuiStylableWidget *talkTimeLayoutWidget = new DuiStylableWidget();
    talkTimeLayoutWidget->setObjectName("layoutWidget1");
    talkTimeLayoutWidget->setLayout(talkTimeLayout);

    /*
     * standByTimeLayoutWidget
     */
    DuiLayout *standByTimeLayout = new DuiLayout(0);
    DuiLinearLayoutPolicy *standByTimeLayoutPolicy = new DuiLinearLayoutPolicy(standByTimeLayout, Qt::Vertical);
    standByTimeLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::StandByTimeText), 0);
    standByTimeLabel = new DuiLabel();
    standByTimeLayoutPolicy->addItemAtPosition(standByTimeLabel, 1);

    DuiStylableWidget *standByTimeLayoutWidget = new DuiStylableWidget();
    standByTimeLayoutWidget->setObjectName("layoutWidget1");
    standByTimeLayoutWidget->setLayout(standByTimeLayout);

    /*
     * PSMLayoutWidget
     */
    DuiLayout *PSMLayout = new DuiLayout(0);
    DuiGridLayoutPolicy *PSMLayoutPolicy = new DuiGridLayoutPolicy(PSMLayout);
    PSMLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::PSMText), 0, 0);
    PSMLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::NBCText), 1, 0);
    PSMButton = new DuiButton();
    PSMButton->setCheckable(true);    
    PSMButton->setObjectName("PSMButton");
    updateButton(PSMButton, batteryGConf->value(BatteryGConf::PSMToggleKey).toBool());
    PSMLayoutPolicy->addItemAtPosition(PSMButton, 0, 1, 2, 1, Qt::AlignRight);

    DuiStylableWidget *PSMLayoutWidget = new DuiStylableWidget();
    PSMLayoutWidget->setObjectName("layoutWidget1");
    PSMLayoutWidget->setLayout(PSMLayout);

    /*
     * upperLayout
     * (includes talkTimeLayoutWidget, standByTimeLayoutWidget and PSMLayoutWidget)
     */
    DuiLayout *upperLayout = new DuiLayout(0);    
    DuiGridLayoutPolicy *upperLayoutPolicy = new DuiGridLayoutPolicy(upperLayout);
    upperLayoutPolicy->addItemAtPosition(talkTimeLayoutWidget, 0, 0);    
    upperLayoutPolicy->addItemAtPosition(standByTimeLayoutWidget, 0, 2);
    upperLayoutPolicy->addItemAtPosition(PSMLayoutWidget, 2, 0);

    //adjust the size parameters of upperLayout
    talkTimeLayoutPolicy->setColumnFixedWidth(0, fullWidth/15); //battery image        
    upperLayoutPolicy->setColumnFixedWidth(1, 5); //empty column        
    upperLayoutPolicy->setRowFixedHeight(1, 5); //empty row        

    /*
     * sliderLayoutWidget
     */
    DuiLayout *sliderLayout = new DuiLayout(0);
    DuiGridLayoutPolicy *sliderLayoutPolicy = new DuiGridLayoutPolicy(sliderLayout);
    sliderLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::AutoPSMText), 0, 0);
    sliderLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::AutoPSMDescText), 1, 0);
    initSlider();
    sliderLayoutPolicy->addItemAtPosition(slider, 2, 0);    

    DuiStylableWidget *sliderLayoutWidget = new DuiStylableWidget();
    sliderLayoutWidget->setObjectName("layoutWidget2");
    sliderLayoutWidget->setLayout(sliderLayout);

    /*
     * disablePSMLayoutWidget
     */
    DuiLayout *disablePSMLayout = new DuiLayout(0);
    DuiGridLayoutPolicy *disablePSMLayoutPolicy = new DuiGridLayoutPolicy(disablePSMLayout);
    disablePSMButton = new DuiButton();
    disablePSMButton->setCheckable(true);    
    disablePSMButton->setObjectName("disablePSMButton");
    updateButton(disablePSMButton, batteryGConf->value(BatteryGConf::PSMDisabledKey).toBool());
    disablePSMLayoutPolicy->addItemAtPosition(disablePSMButton, 0, 0);
    disablePSMLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::DisablePSMText), 0, 1);
    disablePSMLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::AutoPSMAdv1Text), 1, 0, 1, 2);
    disablePSMLayoutPolicy->addItemAtPosition(new DuiLabel(QString("- " + DcpBattery::AutoPSMAdv2Text)), 2, 0, 1, 2);
    disablePSMLayoutPolicy->addItemAtPosition(new DuiLabel(QString("- " + DcpBattery::AutoPSMAdv3Text)), 3, 0, 1, 2);

    DuiStylableWidget *disablePSMLayoutWidget = new DuiStylableWidget();
    disablePSMLayoutWidget->setObjectName("layoutWidget3");
    disablePSMLayoutWidget->setLayout(disablePSMLayout);

    /*
     * lowerLayout
     * (includes sliderLayoutWidget and disablePSMLayoutWidget)
     */    
    DuiLayout *lowerLayout = new DuiLayout(0);
    DuiLinearLayoutPolicy *lowerLayoutPolicy = new DuiLinearLayoutPolicy(lowerLayout, Qt::Vertical);
    lowerLayoutPolicy->addItemAtPosition(sliderLayoutWidget, 0);    
    lowerLayoutPolicy->addItemAtPosition(disablePSMLayoutWidget, 2);
    lowerLayoutPolicy->setSpacing(20);    

    /* 
     * MainLayout
     * (includes lowerLayout and upperLayout)
     */
    //TODO create portraitLayoutPolicy and listen the orientation change signals
    DuiLayout *mainLayout = new DuiLayout(this);
    DuiLinearLayoutPolicy *landscapeLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);    
    landscapeLayoutPolicy->addItemAtPosition(upperLayout, 0);    
    landscapeLayoutPolicy->addItemAtPosition(lowerLayout, 2);
    landscapeLayoutPolicy->setSpacing(20);

    // catch user actions
    connect(PSMButton, SIGNAL(pressed()), this, SLOT(buttonPressed()));
    connect(disablePSMButton, SIGNAL(pressed()), this, SLOT(buttonPressed()));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    
    // catch system actions
    connect(batteryGConf, SIGNAL(valueChanged(BatteryGConf::GConfKey, QVariant)),
            this, SLOT(gConfValueChanged(BatteryGConf::GConfKey, QVariant)));

    //disable the PSM widgets if PSM is disabled
    togglePSMWidgets(!batteryGConf->value(BatteryGConf::PSMDisabledKey).toBool());

    //indicate that the talkTimeLabel and standByTime labels need to be updated
    indicateNeedForUpdatingLabels();

    this->setLayout(mainLayout);
}

void BatteryWidget::initImage()
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
    QString path("/scratchbox/users/jan/targets/harmattan_i386/usr/share/themes/base/dui/libdui/svg/");
    batteryImages << QString("%1icon-s-battery-100.svg").arg(path)
            << QString("%1icon-s-battery-80.svg").arg(path)
            << QString("%1icon-s-battery-60.svg").arg(path)
            << QString("%1icon-s-battery-40.svg").arg(path)
            << QString("%1icon-s-battery-20.svg").arg(path)
            << QString("%1icon-s-battery-0.svg").arg(path);

    batteryChargingImages << QString("%1icon-s-battery-100.svg").arg(path)
            << QString("%1icon-s-battery-80.svg").arg(path)
            << QString("%1icon-s-battery-60.svg").arg(path)
            << QString("%1icon-s-battery-40.svg").arg(path)
            << QString("%1icon-s-battery-20.svg").arg(path)
            << QString("%1icon-s-battery-0.svg").arg(path);

    image = new DuiImage();

    if(batteryGConf->value(BatteryGConf::ChargingKey).toBool()) //is charging
        QTimer::singleShot(200, this, SLOT(updateImage()));
    else
        updateImage(false, batteryGConf->value(BatteryGConf::BatteryLevelKey).toInt());
}

void BatteryWidget::initSlider()
{
    slider = new DuiSlider(0, "continuous");

    sliderValues = batteryGConf->value(BatteryGConf::PSMThresholdValuesKey).toList();
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
        case BatteryGConf::RemainingTimesKey:
            updateLabels(value.toList());
            break;        
        case BatteryGConf::BatteryLevelKey:
            updateImage(batteryGConf->value(BatteryGConf::ChargingKey).toBool(), value.toInt());
            break;
        case BatteryGConf::ChargingKey:            
            updateImage(value.toBool(), batteryGConf->value(BatteryGConf::BatteryLevelKey).toInt());
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
    button->setChecked(toggle);
}

void BatteryWidget::updateSlider(const QString &text)
{    
    slider->setThumbLabel(text);    
}

void BatteryWidget::updateLabels(const QList<QVariant> &values)
{
    qDebug() << "BatteryWidget::updateLabels()";
    if(values.at(0).toInt() == -1) // self-set
        return;

    talkTimeLabel->setText(minutesInString(values.at(0).toInt(), DcpBattery::TalkTimeValueText));
    standByTimeLabel->setText(minutesInString(values.at(1).toInt(), DcpBattery::StandByTimeValueText));

    indicateNeedForUpdatingLabels();
}

void BatteryWidget::indicateNeedForUpdatingLabels()
{    
    QList<QVariant> newValues;
    newValues << QVariant(-1) << QVariant(-1);
    batteryGConf->setValue(BatteryGConf::RemainingTimesKey, newValues);
}

void BatteryWidget::updateImage(bool charging, int level)
{    
    static int chargingImageIndex = 5;   

    if(charging) {        
        if(chargingImageIndex < 0)
            chargingImageIndex = 5;
        QTimer::singleShot(200, this, SLOT(updateImage()));
        image->setImage(QImage(batteryChargingImages.at(chargingImageIndex--)));
    }
    else {        
        if(level > 0)
            image->setImage(QImage(batteryImages.at(level)));            
        chargingImageIndex = 5;
    }    
}
