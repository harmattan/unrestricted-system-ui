#include "batterywidget.h"
#include "batterytranslation.h"
#include "batterydbusinterface.h"
#include "dcpbattery.h"
//#include "dcpspaceritem.h"

#include <QDebug>
#include <QTimer>


#include <DuiButton>
#include <DuiContainer>
#include <DuiGridLayoutPolicy>
#include <DuiImage>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiSceneManager>
#include <DuiSlider>
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

    batteryLevel = 0;
    updateChargingImageTimer = NULL;

    // catch battery If actions
    connect(batteryIf, SIGNAL(batteryCharging()), this, SLOT(startUpdatingChargingImage()));
    connect(batteryIf, SIGNAL(batteryNotCharging()), this, SLOT(stopUpdatingChargingImage()));
    connect(batteryIf, SIGNAL(batteryLevelValueReceived(int)), this, SLOT(batteryLevelValueChanged(int)));
    connect(batteryIf, SIGNAL(PSMValueReceived(bool)), this, SLOT(updatePSMButton(bool)));
    connect(batteryIf, SIGNAL(PSMAutoValueReceived(bool)), this, SLOT(updatePSMAutoButton(bool)));
    connect(batteryIf, SIGNAL(PSMThresholdValuesReceived(QStringList)), this, SLOT(initSlider(QStringList)));
    connect(batteryIf, SIGNAL(PSMThresholdValueReceived(QString)), this, SLOT(updateSlider(QString)));
    connect(batteryIf, SIGNAL(remainingTimeValuesReceived(QStringList)), this, SLOT(updateLabels(QStringList)));       

    /*
     * talkTimeContainer
     */    
    DuiLayout *talkTimeLayout = new DuiLayout(0);
    DuiGridLayoutPolicy *talkTimeLayoutPolicy = new DuiGridLayoutPolicy(talkTimeLayout);    
    initImage();    
    talkTimeLayoutPolicy->addItemAtPosition(image, 0, 0, 2, 1, Qt::AlignLeft);
    talkTimeLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::TalkTimeText), 0, 1);
    talkTimeLabel = new DuiLabel();
    talkTimeLayoutPolicy->addItemAtPosition(talkTimeLabel, 1, 1);
    talkTimeLayoutPolicy->setColumnFixedWidth(0, DuiSceneManager::instance()->visibleSceneSize().width()/15); //battery image

    DuiStylableWidget *talkTimeLayoutWidget = new DuiStylableWidget();
    //talkTimeLayoutWidget->setObjectName("batteryLayoutWidget1");
    talkTimeLayoutWidget->setLayout(talkTimeLayout);

    DuiContainer *talkTimeContainer = new DuiContainer(this);
    talkTimeContainer->setCentralWidget(talkTimeLayoutWidget);

    //update battery icon
    batteryIf->batteryLevelValueRequired();
    batteryIf->batteryChargingStateRequired();

    /*
     * standByTimeContainer
     */
    DuiLayout *standByTimeLayout = new DuiLayout(0);
    DuiLinearLayoutPolicy *standByTimeLayoutPolicy = new DuiLinearLayoutPolicy(standByTimeLayout, Qt::Vertical);
    standByTimeLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::StandByTimeText), 0);
    standByTimeLabel = new DuiLabel();
    standByTimeLayoutPolicy->addItemAtPosition(standByTimeLabel, 1);

    DuiStylableWidget *standByTimeLayoutWidget = new DuiStylableWidget();
    //standByTimeLayoutWidget->setObjectName("batteryLayoutWidget1");
    standByTimeLayoutWidget->setLayout(standByTimeLayout);

    DuiContainer *standByTimeContainer = new DuiContainer(this);
    standByTimeContainer->setCentralWidget(standByTimeLayoutWidget);

    //update labels
    batteryIf->remainingTimeValuesRequired();

    /*
     * PSMButton
     */
    PSMButton = new DuiButton(this);
    //PSMButton->setObjectName("PSMButton");
    batteryIf->PSMValueRequired();

    /*
     * sliderContainer
     */
    PSMAutoButton = new DuiButton();
    PSMAutoButton->setCheckable(true);
    PSMAutoButton->setObjectName("PSMAutoButton");
    slider = new DuiSlider(0, "continuous");    

    DuiLayout *sliderLayout = new DuiLayout(0);
    sliderLayoutPolicy = new DuiGridLayoutPolicy(sliderLayout);
    sliderLayoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::PSMAutoActivateText), 0, 0);
    sliderLayoutPolicy->addItemAtPosition(PSMAutoButton, 0, 1);
    sliderLayoutPolicy->setRowMaximumHeight(1, 75);

    DuiStylableWidget *sliderLayoutWidget = new DuiStylableWidget();
    //sliderLayoutWidget->setObjectName("batteryLayoutWidget2");
    sliderLayoutWidget->setLayout(sliderLayout);

    DuiContainer *sliderContainer = new DuiContainer(this);
    sliderContainer->setCentralWidget(sliderLayoutWidget);

    //init widgets    
    batteryIf->PSMThresholdValuesRequired();
    batteryIf->PSMAutoValueRequired();

    /*
     * orientationLayout
     */
    DuiLayout *orientationLayout = new DuiLayout();

    DuiGridLayoutPolicy *landscapeLayoutPolicy = new DuiGridLayoutPolicy(orientationLayout);
    DuiLinearLayoutPolicy *portraitLayoutPolicy = new DuiLinearLayoutPolicy(orientationLayout, Qt::Vertical);

    landscapeLayoutPolicy->addItemAtPosition(talkTimeContainer, 0, 0);
    landscapeLayoutPolicy->addItemAtPosition(standByTimeContainer, 0, 1);
    landscapeLayoutPolicy->addItemAtPosition(PSMButton, 1, 0, 1, 2);
    landscapeLayoutPolicy->addItemAtPosition(sliderContainer, 2, 0, 1, 2);
    landscapeLayoutPolicy->setSpacing(20);

    portraitLayoutPolicy->addItemAtPosition(talkTimeContainer, 0, Qt::AlignLeft);
    portraitLayoutPolicy->addItemAtPosition(standByTimeContainer, 1, Qt::AlignLeft);
    portraitLayoutPolicy->addItemAtPosition(PSMButton, 2, Qt::AlignCenter);
    portraitLayoutPolicy->addItemAtPosition(sliderContainer, 3, Qt::AlignLeft);
    portraitLayoutPolicy->setSpacing(20);

    orientationLayout->setLandscapePolicy(landscapeLayoutPolicy);
    orientationLayout->setPortraitPolicy(portraitLayoutPolicy);

    /* 
     * mainLayout
     */    
    DuiStylableWidget *mainWidget = new DuiStylableWidget(this);
    mainWidget->setLayout(orientationLayout);
    DuiContainer *mainContainer = new DuiContainer(this);
    mainContainer->setCentralWidget(mainWidget);
    DuiLayout *mainLayout = new DuiLayout(this);
    DuiLinearLayoutPolicy *mainLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Horizontal);
    mainLayoutPolicy->addItem(mainContainer);

    // catch user actions
    connect(PSMButton, SIGNAL(released()), this, SLOT(PSMButtonPressed()));
    connect(PSMAutoButton, SIGNAL(released()), this, SLOT(PSMAutoButtonPressed()));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
            
    //DuiSceneManager::instance()->setOrientationAngle((Dui::OrientationAngle) 90);

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
    batteryImages << QString("%1icon-s-battery-0.svg").arg(path)
            << QString("%1icon-s-battery-20.svg").arg(path)
            << QString("%1icon-s-battery-40.svg").arg(path)
            << QString("%1icon-s-battery-60.svg").arg(path)
            << QString("%1icon-s-battery-80.svg").arg(path)
            << QString("%1icon-s-battery-100.svg").arg(path);

    batteryChargingImages << QString("%1icon-s-battery-0.svg").arg(path)
            << QString("%1icon-s-battery-20.svg").arg(path)
            << QString("%1icon-s-battery-40.svg").arg(path)
            << QString("%1icon-s-battery-60.svg").arg(path)
            << QString("%1icon-s-battery-80.svg").arg(path)
            << QString("%1icon-s-battery-100.svg").arg(path);

    image = new DuiImage();   
}

void BatteryWidget::initSlider(const QStringList &values)
{    
    sliderValues = QStringList(values);    
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

void BatteryWidget::PSMButtonPressed()
{
    PSMButton->setEnabled(false);
    batteryIf->setPSMValue((PSMButton->text() == DcpBattery::PSMActivateText) ? true : false);
}

void BatteryWidget::PSMAutoButtonPressed()
{        
    //NOTE: DuiButton->isChecked() method returns the state before the press at this point
    bool checked = !PSMAutoButton->isChecked();
    batteryIf->setPSMAutoValue(checked);
    toggleSliderVisibility(checked);
}

QString BatteryWidget::minutesInString(int mins, const QString &pattern)
{    
    QString minutes = pattern.section("%b", 1, 1).trimmed();
    QString hours = (pattern.section("%b", 0, 0)).section("%a", 1, 1).trimmed();

    if(mins < 60)
        return QString("%1%2").arg(mins).arg(minutes);
    else {
        QVariant minsVar = mins%60;
        minsVar = (minsVar.toInt() == 0) ? "00" : minsVar;
        return QString("%1:%2%3").arg(mins/60).arg(minsVar.toString()).arg(hours);
    }    
}

void BatteryWidget::toggleSliderVisibility(bool toggle)
{    
    if(toggle) {        
        if(sliderLayoutPolicy->itemAt(1, 0) != slider)
            sliderLayoutPolicy->addItemAtPosition(slider, 1, 0, 1, 2);            
    }
    else {        
        if(sliderLayoutPolicy->itemAt(1, 0) == slider)
            sliderLayoutPolicy->removeItem(slider);        
    }
}

void BatteryWidget::updatePSMButton(bool toggle)
{    
    PSMButton->setText(toggle ? DcpBattery::PSMDeactivateText : DcpBattery::PSMActivateText);
    PSMButton->setEnabled(true);
}

void BatteryWidget::updatePSMAutoButton(bool toggle)
{    
    PSMAutoButton->setChecked(toggle);
    toggleSliderVisibility(toggle);
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

    talkTimeLabel->setText(minutesInString(timeValues.at(0).toInt(), DcpBattery::TimeValueText));
    standByTimeLabel->setText(minutesInString(timeValues.at(1).toInt(), DcpBattery::TimeValueText));

    QTimer::singleShot(10000, batteryIf, SLOT(remainingTimeValuesRequired()));
}

void BatteryWidget::updateImage(bool charging)
{    
    static int chargingImageIndex = batteryLevel;
    if(charging) {        
        if(chargingImageIndex > 5)
            chargingImageIndex = batteryLevel;
        image->setImage(QImage(batteryChargingImages.at(chargingImageIndex++)));
    }
    else {                
        image->setImage(QImage(batteryImages.at(batteryLevel)));
        chargingImageIndex = batteryLevel;
    }    
}

void BatteryWidget::startUpdatingChargingImage()
{
    qDebug() << "BatteryWidget::startUpdatingChargingImage()";
    if(updateChargingImageTimer == NULL) {        
        updateChargingImageTimer = new QTimer(this);
        connect(updateChargingImageTimer, SIGNAL(timeout()), this, SLOT(updateImage()));
        updateChargingImageTimer->setInterval(400);
        updateChargingImageTimer->start();
    }
}

void BatteryWidget::stopUpdatingChargingImage()
{
    qDebug() << "BatteryWidget::stopUpdatingChargingImage()";
    if(updateChargingImageTimer != NULL) {        
        updateChargingImageTimer->stop();
        delete updateChargingImageTimer;
        updateChargingImageTimer = NULL;        
    }
    batteryIf->batteryLevelValueRequired();    
}

void BatteryWidget::batteryLevelValueChanged(int value)
{    
    batteryLevel = value;
    if(updateChargingImageTimer == NULL)
        updateImage(false);
}
