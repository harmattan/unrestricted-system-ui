#include "batterywidget.h"
#include "batteryimage.h"
#include "batterytranslation.h"
#include "batterydbusinterface.h"
#include "dcpbattery.h"
#include "talktimecontainer.h"
#include "standbytimecontainer.h"
#include "slidercontainer.h"

#include <QGraphicsLinearLayout>
#include <QDebug>

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
2) The layout should be finalized
3) Battery header not yet defined (logical name)

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

bool BatteryWidget::back()
{
    return true; // back is handled by main window by default
}

void BatteryWidget::initWidget()
{       
    // proxy for dbus interface on remote object
    batteryIf = new BatteryDBusInterface();

    // talkTimeContainer        
    talkTimeContainer = new TalkTimeContainer();

    // standByTimeContainer
    standByTimeContainer = new StandByTimeContainer();

    // PSMButton
    PSMButton = new DuiButton();
    connect(PSMButton, SIGNAL(released()), this, SLOT(PSMButtonReleased()));

    // sliderContainer
    sliderContainer = new SliderContainer();
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

    DuiContainer *mainContainer = new DuiContainer();
    mainContainer->centralWidget()->setLayout(orientationLayout);

    // connect the value receive signals
    connect(batteryIf, SIGNAL(remainingTimeValuesReceived(QStringList)), this, SLOT(remainingTimeValuesReceived(QStringList)));  
    connect(batteryIf, SIGNAL(batteryCharging(int)), talkTimeContainer->batteryImage(), SLOT(startCharging(int)));
    connect(batteryIf, SIGNAL(batteryNotCharging()), talkTimeContainer->batteryImage(), SLOT(stopCharging()));
    connect(batteryIf, SIGNAL(batteryNotCharging()), batteryIf, SLOT(batteryBarValueRequired()));
    connect(batteryIf, SIGNAL(batteryBarValueReceived(int)), talkTimeContainer->batteryImage(), SLOT(updateBatteryLevel(int)));
    connect(batteryIf, SIGNAL(PSMValueReceived(bool)), this, SLOT(updatePSMButton(bool)));
    connect(batteryIf, SIGNAL(PSMAutoValueReceived(bool)), sliderContainer, SLOT(initPSMAutoButton(bool)));
    connect(batteryIf, SIGNAL(PSMAutoDisabled()), sliderContainer, SLOT(PSMAutoDisabled()));
    connect(batteryIf, SIGNAL(PSMThresholdValuesReceived(QStringList)), sliderContainer, SLOT(initSlider(QStringList)));
    connect(batteryIf, SIGNAL(PSMThresholdValuesReceived(QStringList)), batteryIf, SLOT(PSMThresholdValueRequired()));
    connect(batteryIf, SIGNAL(PSMThresholdValueReceived(QString)), sliderContainer, SLOT(updateSlider(QString)));

    // send value requests over dbus
    batteryIf->remainingTimeValuesRequired();
    batteryIf->batteryBarValueRequired();
    batteryIf->batteryChargingStateRequired();
    batteryIf->PSMValueRequired();
    batteryIf->PSMAutoValueRequired();
    batteryIf->PSMThresholdValuesRequired();

    // mainLayout
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Vertical);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addItem(mainContainer);
    setLayout(mainLayout);

}

void BatteryWidget::PSMButtonReleased()
{    
    bool toggle = ( PSMButton->text() == DcpBattery::PSMActivateText ? true : false);
    batteryIf->setPSMValue(toggle);
}

void BatteryWidget::updatePSMButton(bool toggle)
{
    QString text = ( toggle ? DcpBattery::PSMDeactivateText : DcpBattery::PSMActivateText );
    PSMButton->setText(text); 
}

void BatteryWidget::remainingTimeValuesReceived(const QStringList &timeValues)
{
    qDebug() << "BatteryWidget::remainingTimeValuesReceived(" << timeValues.at(0) << ", " << timeValues.at(1) << ")";    
    talkTimeContainer->updateTimeLabel(timeValues.at(0));
    standByTimeContainer->updateTimeLabel(timeValues.at(1));   
}
