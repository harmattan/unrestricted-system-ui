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
    // talkTimeContainer        
    talkTimeContainer = new TalkTimeContainer(this);

    // standByTimeContainer
    standByTimeContainer = new StandByTimeContainer(this);

    // PSMButton
    PSMButton = new DuiButton(this);
    connect(PSMButton, SIGNAL(pressed()), this, SLOT(PSMButtonPressed()));

    // sliderContainer
    sliderContainer = new SliderContainer(this);
    connect(sliderContainer, SIGNAL(PSMToggled(bool)), batteryIf, SLOT(setPSMAutoValue(bool)));
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

    // proxy for dbus interface on remote object
    batteryIf = new BatteryDBusInterface();

    // connect the value receive signals
    connect(batteryIf, SIGNAL(remainingTimeValuesReceived(QStringList)), this, SLOT(remainingTimeValuesReceived(QStringList)));  
    connect(batteryIf, SIGNAL(batteryCharging()), talkTimeContainer, SLOT(startCharging()));
    connect(batteryIf, SIGNAL(batteryNotCharging()), talkTimeContainer, SLOT(stopCharging()));
    connect(batteryIf, SIGNAL(batteryNotCharging()), batteryIf, SLOT(batteryLevelValueRequired()));
    connect(batteryIf, SIGNAL(batteryLevelValueReceived(int)), talkTimeContainer, SLOT(updateBattery(int)));
    connect(batteryIf, SIGNAL(PSMValueReceived(bool)), this, SLOT(updatePSMButton(bool)));
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

void BatteryWidget::PSMButtonPressed()
{
    PSMButton->setEnabled(false);
    batteryIf->setPSMValue((PSMButton->text() == DcpBattery::PSMActivateText) ? true : false);
}

void BatteryWidget::updatePSMButton(bool toggle)
{    
    PSMButton->setText(toggle ? DcpBattery::PSMDeactivateText : DcpBattery::PSMActivateText);
    PSMButton->setEnabled(true);
}

void BatteryWidget::remainingTimeValuesReceived(const QStringList &timeValues)
{
    qDebug() << "BatteryWidget::updateLabels(const QStringList &timeValues)";
    talkTimeContainer->updateTimeLabel(timeValues.at(0).toInt());
    standByTimeContainer->updateTimeLabel(timeValues.at(1).toInt());

    QTimer::singleShot(10000, batteryIf, SLOT(remainingTimeValuesRequired()));
}
