#include "batterywidget.h"
#include "batterytranslation.h"
#include "batterydbusinterface.h"
#include "dcpbattery.h"
#include "talktimecontainer.h"
#include "standbytimecontainer.h"
#include "slidercontainer.h"

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

/*

TODO list:

1) what is the correct interval for updating the battery image when charging? Is there a difference between
   USB and normal charging?
2) the battery images should be logical names instead of hard coded values
3) There should be 8 normal and 8 charging battery images instead of 6
4) The values in the slider should be percentage (QmBattery API provides)
5) It should be confirmed that the PSMAuto button should be toggled off every time the PSM is switched manually
6) The layout should be finalized
7) It should be confirmed, how the remaining time labels should behave when the battery is charging.
   Currently QmBattery API return -1 when charging.
8) It should be confirmed that 10 seconds is correct interval to update the remaining time labels

*/

BatteryWidget::BatteryWidget(QGraphicsWidget *parent) :
        DcpWidget(parent),
        talkTimeContainer(NULL),
        standByTimeContainer(NULL),
        sliderContainer(NULL),
        batteryIf(NULL),
        PSMButton(NULL)
{
    setReferer(DcpBattery::None);
    initWidget();
}

BatteryWidget::~BatteryWidget()
{    
}

void BatteryWidget::initWidget()
{       
    // proxy for dbus interface on remote object
    batteryIf = new BatteryDBusInterface();

    // talkTimeContainer        
    talkTimeContainer = new TalkTimeContainer(this);

    // standByTimeContainer
    standByTimeContainer = new StandByTimeContainer(this);

    // PSMButton
    PSMButton = new DuiButton(this);
    connect(PSMButton, SIGNAL(released()), this, SLOT(PSMButtonReleased()));

    // sliderContainer
    sliderContainer = new SliderContainer(this);   
    connect(sliderContainer, SIGNAL(PSMAutoToggled(bool)), batteryIf, SLOT(setPSMAutoValue(bool)));
    connect(sliderContainer, SIGNAL(PSMThresholdValueChanged(QString)), batteryIf, SLOT(setPSMThresholdValue(QString)));

    // mainContainer
    DuiLayout *orientationLayout = new DuiLayout();

    DuiGridLayoutPolicy *landscapeLayoutPolicy = new DuiGridLayoutPolicy(orientationLayout);
    landscapeLayoutPolicy->addItemAtPosition(talkTimeContainer, 0, 0);
    landscapeLayoutPolicy->addItemAtPosition(standByTimeContainer, 0, 1);
    landscapeLayoutPolicy->addItemAtPosition(PSMButton, 1, 0, 1, 2);
    landscapeLayoutPolicy->addItemAtPosition(sliderContainer, 2, 0, 1, 2);
    landscapeLayoutPolicy->setSpacing(20);
    orientationLayout->setLandscapePolicy(landscapeLayoutPolicy);

    DuiLinearLayoutPolicy *portraitLayoutPolicy = new DuiLinearLayoutPolicy(orientationLayout, Qt::Vertical);
    portraitLayoutPolicy->addItem(talkTimeContainer, Qt::AlignLeft);
    portraitLayoutPolicy->addItem(standByTimeContainer, Qt::AlignLeft);
    portraitLayoutPolicy->addItem(PSMButton, Qt::AlignCenter);
    portraitLayoutPolicy->addItem(sliderContainer, Qt::AlignLeft);
    portraitLayoutPolicy->setSpacing(20);
    orientationLayout->setPortraitPolicy(portraitLayoutPolicy);

    DuiContainer *mainContainer = new DuiContainer(this);
    mainContainer->centralWidget()->setLayout(orientationLayout);

    // connect the value receive signals
    connect(batteryIf, SIGNAL(remainingTimeValuesReceived(QStringList)), this, SLOT(remainingTimeValuesReceived(QStringList)));  
    connect(batteryIf, SIGNAL(batteryCharging()), talkTimeContainer, SLOT(startCharging()));
    connect(batteryIf, SIGNAL(batteryNotCharging()), talkTimeContainer, SLOT(stopCharging()));
    connect(batteryIf, SIGNAL(batteryNotCharging()), batteryIf, SLOT(batteryLevelValueRequired()));
    connect(batteryIf, SIGNAL(batteryLevelValueReceived(int)), talkTimeContainer, SLOT(updateBattery(int)));
    connect(batteryIf, SIGNAL(PSMValueReceived(QString)), this, SLOT(updatePSMButton(QString)));
    connect(batteryIf, SIGNAL(PSMAutoValueReceived(bool)), sliderContainer, SLOT(initPSMAutoButton(bool)));
    connect(batteryIf, SIGNAL(PSMThresholdValuesReceived(QStringList)), sliderContainer, SLOT(initSlider(QStringList)));
    connect(batteryIf, SIGNAL(PSMThresholdValuesReceived(QStringList)), batteryIf, SLOT(PSMThresholdValueRequired()));
    connect(batteryIf, SIGNAL(PSMThresholdValueReceived(QString)), sliderContainer, SLOT(updateSlider(QString)));

    // send value requests over dbus
    batteryIf->remainingTimeValuesRequired();
    batteryIf->batteryLevelValueRequired();
    batteryIf->batteryChargingStateRequired();
    batteryIf->PSMValueRequired();
    batteryIf->PSMAutoValueRequired();
    batteryIf->PSMThresholdValuesRequired();

    // mainLayout
    DuiLayout *mainLayout = new DuiLayout(this);
    DuiLinearLayoutPolicy *mainLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    mainLayoutPolicy->addItem(mainContainer);
    this->setLayout(mainLayout);
}

void BatteryWidget::PSMButtonReleased()
{
    PSMButton->setEnabled(false);
    batteryIf->setPSMValue(PSMButton->text());
}

void BatteryWidget::updatePSMButton(const QString &value)
{    
    PSMButton->setText(value);
    PSMButton->setEnabled(true);
}

void BatteryWidget::remainingTimeValuesReceived(const QStringList &timeValues)
{
    qDebug() << "BatteryWidget::updateLabels(" << timeValues.at(0) << ", " << timeValues.at(1) << ")";
    talkTimeContainer->updateTimeLabel(timeValues.at(0).toInt());
    standByTimeContainer->updateTimeLabel(timeValues.at(1).toInt());

    QTimer::singleShot(10000, batteryIf, SLOT(remainingTimeValuesRequired()));
}
