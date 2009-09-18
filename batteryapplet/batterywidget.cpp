#include "batterywidget.h"
#include "batterytranslation.h"
#include "batterydbusinterface.h"
#include "dcpbattery.h"
#include "dcpspaceritem.h"

#include <QDebug>
#include <QTimer>


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
}

void BatteryWidget::initWidget()
{
    //create dbus if
    batteryIf = new BatteryDBusInterface();

    // catch battery If actions
    connect(batteryIf, SIGNAL(batteryCharging()), this, SLOT(startUpdatingChargingImage()));
    connect(batteryIf, SIGNAL(batteryNotCharging()), this, SLOT(stopUpdatingChargingImage()));
    connect(batteryIf, SIGNAL(batteryLevelValueChanged(int)), this, SLOT(batteryLevelValueChanged(int)));
    connect(batteryIf, SIGNAL(PSMToggleValueChanged(bool)), this, SLOT(updatePSMToggleButton(bool)));
    connect(batteryIf, SIGNAL(PSMDisabledValueChanged(bool)), this, SLOT(updatePSMDisableButton(bool)));
    connect(batteryIf, SIGNAL(PSMThresholdValuesAvailable(QStringList)), this, SLOT(initSlider(QStringList)));
    connect(batteryIf, SIGNAL(PSMThresholdValueAvailable(QString)), this, SLOT(updateSlider(QString)));
    connect(batteryIf, SIGNAL(remainingTimeValuesAvailable(QStringList)), this, SLOT(updateLabels(QStringList)));
    connect(batteryIf, SIGNAL(batteryLevelValueAvailable(int)), this, SLOT(updateNotChargingImage(int)));

    int fullWidth = DuiSceneManager::instance()->visibleSceneSize().width();
    updateChargingImageTimer = NULL;

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
    PSMButton->setObjectName("PSMToggleButton");    
    batteryIf->PSMToggleValueRequired();
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
    slider = new DuiSlider(0, "continuous");
    batteryIf->PSMThresholdValuesRequired();
    sliderLayoutPolicy->addItemAtPosition(slider, 2, 0);    

    DuiStylableWidget *sliderLayoutWidget = new DuiStylableWidget();
    sliderLayoutWidget->setObjectName("layoutWidget2");
    sliderLayoutWidget->setLayout(sliderLayout);

    /*
     * disablePSMLayoutWidget
     */
    DuiLayout *disablePSMLayout = new DuiLayout(0);
    DuiGridLayoutPolicy *disablePSMLayoutPolicy = new DuiGridLayoutPolicy(disablePSMLayout);
    PSMDisableButton = new DuiButton();    
    PSMDisableButton->setCheckable(true);
    PSMDisableButton->setObjectName("PSMDisableButton");    
    batteryIf->PSMDisabledValueRequired();
    disablePSMLayoutPolicy->addItemAtPosition(PSMDisableButton, 0, 0);
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
    connect(PSMDisableButton, SIGNAL(pressed()), this, SLOT(buttonPressed()));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
        
    //update labels
    batteryIf->remainingTimeValuesRequired();

    //TODO: Before setting, be sure that UI is initialized

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
    QString path("/usr/share/themes/base/dui/libdui/svg/");
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

    batteryIf->BatteryChargingStateRequired();    
}

void BatteryWidget::initSlider(QStringList values)
{    
    sliderValues = QStringList(values);
    //sliderValues << values.at(0) << values.at(1) << values.at(2); 
    slider->setRange(0,sliderValues.size()-1);    
    slider->setOrientation(Qt::Horizontal);

    batteryIf->PSMThresholdValueRequired();
}

void BatteryWidget::sliderValueChanged(int value)
{  
    qDebug() << "BatteryWidget::sliderValueChanged(" << value << ")";
    batteryIf->setPSMThresholdValue(sliderValues.at(value));    
    updateSlider(sliderValues.at(value));
}

void BatteryWidget::buttonPressed()
{    
    qDebug() << "BatteryWidget::buttonPressed()";
    DuiButton *duiButton = static_cast<DuiButton*>(this->sender());

    if(duiButton->objectName() == "PSMToggleButton")
        batteryIf->setPSMToggleValue(duiButton->isChecked());
    else if(duiButton->objectName() == "PSMDisableButton") {        
        batteryIf->setPSMDisabledValue(duiButton->isChecked());
        if(duiButton->isChecked())
            batteryIf->setPSMToggleValue(false);
        togglePSMWidgets(!duiButton->isChecked());
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
    slider->setEnabled(enable);
}

void BatteryWidget::updatePSMToggleButton(bool toggle)
{
    updateButton(toggle, PSMButton);
}

void BatteryWidget::updatePSMDisableButton(bool toggle)
{    
    updateButton(toggle, PSMDisableButton);
    if(toggle) //PSM disabled
        togglePSMWidgets(false);
}

void BatteryWidget::updateButton(bool toggle, DuiButton *button)
{    
    button->setChecked(toggle);
}

void BatteryWidget::updateSlider(const QString &value)
{    
    if( slider != NULL) {       
        slider->setValue(sliderValues.indexOf(value)); //in case this is the first call, we need to set the value
        slider->setThumbLabel(QString("%1%").arg(value));
    }
}

void BatteryWidget::updateLabels(const QStringList &timeValues)
{
    qDebug() << "BatteryWidget::updateLabels(const QStringList &timeValues)";

    talkTimeLabel->setText(minutesInString(timeValues.at(0).toInt(), DcpBattery::TalkTimeValueText));
    standByTimeLabel->setText(minutesInString(timeValues.at(1).toInt(), DcpBattery::StandByTimeValueText));

    QTimer::singleShot(10000, batteryIf, SLOT(remainingTimeValuesRequired()));
}

void BatteryWidget::updateNotChargingImage(int level)
{
    updateImage(false, level);
}

void BatteryWidget::updateImage(bool charging, int level)
{    
    static int chargingImageIndex = 5;   

    if(charging) {        
        if(chargingImageIndex < 0)
            chargingImageIndex = 5;
        image->setImage(QImage(batteryChargingImages.at(chargingImageIndex--)));
    }
    else {        
        if(level > 0)
            image->setImage(QImage(batteryImages.at(level)));
        chargingImageIndex = 5;
    }    
}

void BatteryWidget::startUpdatingChargingImage()
{    
    if(updateChargingImageTimer == NULL) {
        qDebug() << "startUpdatingChargingImage()";
        updateChargingImageTimer = new QTimer(this);
        connect(updateChargingImageTimer, SIGNAL(timeout()), this, SLOT(updateImage()));
        updateChargingImageTimer->setInterval(400);
        updateChargingImageTimer->start();
    }
}

void BatteryWidget::stopUpdatingChargingImage()
{    
    if(updateChargingImageTimer != NULL) {
        qDebug() << "stopUpdatingChargingImage()";
        updateChargingImageTimer->stop();
        delete updateChargingImageTimer;
        updateChargingImageTimer = NULL;        
    }
    batteryIf->batteryLevelValueRequired();
    updateImage(false, (batteryIf->call("batteryLevelValue", BatteryDBusInterface::DBusDataTypeInt)).toInt());
}

void BatteryWidget::batteryLevelValueChanged(int value)
{
    if(updateChargingImageTimer == NULL)
        updateImage(false, value);
}
