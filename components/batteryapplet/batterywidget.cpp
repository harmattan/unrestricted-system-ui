/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "batterywidget.h"
#include "batteryimage.h"
#include "batterydbusinterface.h"
#include "dcpbattery.h"
#include "slidercontainer.h"
#include "timecontainer.h"

#include <QGraphicsLinearLayout>

#include <DuiButton>
#include <DuiContainer>
#include <DuiGridLayoutPolicy>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>

#define DEBUG 
#include "../debug.h"

/*
TODO list:
1) what is the correct interval for updating the battery image when charging? Is there a difference between
   USB and normal charging?

*/

BatteryWidget::BatteryWidget (QGraphicsWidget *parent) :
        DcpWidget (parent),
        batteryIf (NULL),
        batteryImage (NULL),
        PSMButton (NULL),
        sliderContainer (NULL),
        standByTimeContainer (NULL),
        talkTimeContainer (NULL)
{
    SYS_DEBUG ("Starting in %p", this);
    /*
     * One can assume, that when the applet is started the power save mode is
     * not active. This way we show a value that makes sense even if the DBus
     * interface to the sysuid is not usable.
     */
    PSMButtonToggle = false;
    initWidget();
}

BatteryWidget::~BatteryWidget ()
{
    SYS_DEBUG ("Destroying %p", this);
    if (batteryIf) {
        delete batteryIf;
        batteryIf = NULL;
    }
}

bool BatteryWidget::back ()
{
    return true; // back is handled by main window by default
}

void BatteryWidget::initWidget ()
{
    SYS_DEBUG ("Start");
    // proxy for dbus interface on remote object
    batteryIf = new BatteryDBusInterface;

    // battery image
    batteryImage = new BatteryImage;

    // talkTimeContainer
    //% "Estimated talk time:"
    talkTimeContainer = new TimeContainer (
            qtTrId ("qtn_ener_tt"), batteryImage);

    // standByTimeContainer
    //% "Estimated stand-by time:"
    standByTimeContainer = new TimeContainer (
            qtTrId ("qtn_ener_st"), new DuiImageWidget);
                                  //"qgn_ener_standby" ^

    // PSMButton
    PSMButton = new DuiButton ;
    connect (PSMButton, SIGNAL (released ()), 
             this, SLOT (PSMButtonReleased ()));

    // sliderContainer
    sliderContainer = new SliderContainer;

    connect (sliderContainer, SIGNAL (PSMAutoToggled (bool)),
             batteryIf, SLOT (setPSMAutoValue (bool)));
    connect (sliderContainer, SIGNAL (PSMThresholdValueChanged (QString)),
             batteryIf, SLOT (setPSMThresholdValue (QString)));

    // mainContainer
    DuiLayout *orientationLayout = new DuiLayout;

    DuiGridLayoutPolicy *landscapeLayoutPolicy =
        new DuiGridLayoutPolicy (orientationLayout);
    landscapeLayoutPolicy->addItem (talkTimeContainer, 0, 0);
    landscapeLayoutPolicy->addItem (standByTimeContainer, 0, 1);
    landscapeLayoutPolicy->setColumnStretchFactor (0, 2);
    landscapeLayoutPolicy->setColumnStretchFactor (1, 2);
    landscapeLayoutPolicy->addItem (sliderContainer, 1, 0, 1, 2);
    landscapeLayoutPolicy->addItem (PSMButton, 2, 0, 1, 2);
    landscapeLayoutPolicy->setSpacing (10);
    orientationLayout->setLandscapePolicy (landscapeLayoutPolicy);

    DuiLinearLayoutPolicy *portraitLayoutPolicy =
        new DuiLinearLayoutPolicy (orientationLayout, Qt::Vertical);
    portraitLayoutPolicy->addItem (talkTimeContainer, Qt::AlignLeft);
    portraitLayoutPolicy->addItem (standByTimeContainer, Qt::AlignLeft);
    portraitLayoutPolicy->setStretchFactor (talkTimeContainer, 2);
    portraitLayoutPolicy->setStretchFactor (standByTimeContainer, 2);
    portraitLayoutPolicy->addItem (sliderContainer, Qt::AlignLeft);
    portraitLayoutPolicy->addItem (PSMButton, Qt::AlignCenter);
    portraitLayoutPolicy->setSpacing (10);
    orientationLayout->setPortraitPolicy (portraitLayoutPolicy);

    DuiContainer *mainContainer = new DuiContainer;
    mainContainer->setHeaderVisible (false);
    mainContainer->centralWidget ()->setLayout (orientationLayout);

    // connect the value receive signals
    connect (batteryIf, SIGNAL (remainingTimeValuesReceived (QStringList)),
             this, SLOT (remainingTimeValuesReceived (QStringList)));
    connect (batteryIf, SIGNAL (batteryCharging (int)),
             batteryImage, SLOT (startCharging (int)));
    connect (batteryIf, SIGNAL (batteryNotCharging ()),
             batteryImage, SLOT (stopCharging ()));
    connect (batteryIf, SIGNAL (batteryNotCharging ()),
             batteryIf, SLOT (batteryBarValueRequired ()));
    connect (batteryIf, SIGNAL (batteryBarValueReceived (int)),
             batteryImage, SLOT (updateBatteryLevel (int)));
    connect (batteryIf, SIGNAL (PSMValueReceived (bool)),
             this, SLOT (updatePSMButton (bool)));
    connect (batteryIf, SIGNAL (PSMAutoValueReceived (bool)),
             sliderContainer, SLOT (initPSMAutoButton (bool)));
    connect (batteryIf, SIGNAL (PSMAutoDisabled ()),
             sliderContainer, SLOT (PSMAutoDisabled ()));
    connect (batteryIf, SIGNAL (PSMThresholdValuesReceived (QStringList)),
             sliderContainer, SLOT (initSlider (QStringList)));
    connect (batteryIf, SIGNAL (PSMThresholdValuesReceived (QStringList)),
             batteryIf, SLOT (PSMThresholdValueRequired ()));
    connect (batteryIf, SIGNAL (PSMThresholdValueReceived (QString)),
             sliderContainer, SLOT (updateSlider (QString)));

    // send value requests over dbus
    batteryIf->remainingTimeValuesRequired ();
    batteryIf->batteryBarValueRequired ();
    batteryIf->batteryChargingStateRequired ();
    batteryIf->PSMValueRequired ();
    batteryIf->PSMAutoValueRequired ();
    batteryIf->PSMThresholdValuesRequired ();

    // mainLayout
    QGraphicsLinearLayout *mainLayout =
        new QGraphicsLinearLayout (Qt::Vertical);
    mainLayout->setContentsMargins (0, 0, 0, 0);
    mainLayout->addItem (mainContainer);
    this->setLayout (mainLayout);

    SYS_DEBUG ("End");
}

/*!
 * This function is called when the user clicked on the 'power save mode' button
 * that activates and disactivates the power saving mode. The function will call
 * the battery interface, but the UI will be changed only when the sysuid
 * reports the change.
 */
void 
BatteryWidget::PSMButtonReleased ()
{
    batteryIf->setPSMValue (!PSMButtonToggle);
}

void 
BatteryWidget::updatePSMButton (
        bool toggle)
{
    PSMButtonToggle = toggle;

    SYS_DEBUG ("PSMButtonToggle now is %s", PSMButtonToggle ? "on" : "off");
    if (toggle) {
        //% "Deactivate power save now"
        PSMButton->setText (qtTrId ("qtn_ener_dps"));
    } else {
        //% "Activate power save now"
        PSMButton->setText (qtTrId ("qtn_ener_aps"));
    }
}

void BatteryWidget::remainingTimeValuesReceived(const QStringList &timeValues)
{
    SYS_DEBUG ("timevalues = %s, %s",
               SYS_STR (timeValues.at (0)),
               SYS_STR (timeValues.at (1)));

    talkTimeContainer->updateTimeLabel (timeValues.at (0));
    standByTimeContainer->updateTimeLabel (timeValues.at (1));
}

void 
BatteryWidget::retranslateUi ()
{
    // This call will reload the translated text on PSButton
    updatePSMButton (PSMButtonToggle);

    // This call will retranslate the label (infoText)
    sliderContainer->retranslate ();

    talkTimeContainer->setText(qtTrId ("qtn_ener_tt"));
    standByTimeContainer->setText (qtTrId ("qtn_ener_st"));

    // This call will reload timelabels on timercontainers
    batteryIf->remainingTimeValuesRequired ();
}

