#include "displaywidget.h"
#include "displaybusinesslogic.h"
#include "dcpdisplay.h"

#include <QDebug>

#include <DuiButton>
#include <DuiContainer>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiSlider>
#include <DuiLinearLayoutPolicy>

#define VALUE_IN_HEADER

/* TODO:
    1) Display header not yet defined (logical name)
*/

DisplayWidget::DisplayWidget(QGraphicsWidget *parent) :
        DcpWidget(parent),
        m_logic(NULL)
{
    setReferer(DcpDisplay::None);
    initWidget();
}

DisplayWidget::~DisplayWidget()
{
}

void DisplayWidget::initWidget()
{
    DuiLayout               *mainLayout;
    DuiLinearLayoutPolicy   *policy;

    mainLayout = new DuiLayout (this);
    policy     = new DuiLinearLayoutPolicy (mainLayout, Qt::Vertical);

    policy->setSpacing (10);
    mainLayout->setPolicy (policy);
   
    m_logic = new DisplayBusinessLogic();

    // Brightness
    DuiLayout* brightnessLayout = new DuiLayout;

    DuiLinearLayoutPolicy* brightnessLayoutPolicy =
        new DuiLinearLayoutPolicy (brightnessLayout, Qt::Horizontal);
    brightnessLayoutPolicy->setSpacing (0);
    brightnessLayoutPolicy->setContentsMargins (0, 0, 0, 0);

    m_brightnessSlider = new DuiSlider;
    brightnessLayoutPolicy->addItem (m_brightnessSlider);

    m_brightness_vals = m_logic->brightnessValues ();
    m_brightnessSlider->setRange (0, m_brightness_vals.size () - 1);
    m_brightnessSlider->setValue (m_logic->selectedBrightnessValue ());

// FIXME: now its casues crash on the device :-S <dkedves>
#if 0
    m_brightnessSlider->setMinLabelVisible (true);
    m_brightnessSlider->setMaxLabelVisible (true);
    m_brightnessSlider->setMinLabelIconID ("icon-l-brightness-min");
    m_brightnessSlider->setMaxLabelIconID ("icon-l-brightness-max");
#endif

    connect (m_brightnessSlider, SIGNAL (valueChanged (int)),
             m_logic, SLOT (setBrightnessValue (int)));

    m_brightnessContainer = new DuiContainer;
    //% "Brightness:"
    m_brightnessContainer->setTitle (qtTrId ("qtn_disp_bright"));
    m_brightnessContainer->centralWidget ()->setLayout (brightnessLayout);

    policy->addItem (m_brightnessContainer);

    // Screen light
    DuiLayout* screenlightLayout = new DuiLayout;
    DuiLinearLayoutPolicy* screenlightLayoutPolicy =
        new DuiLinearLayoutPolicy(screenlightLayout, Qt::Vertical);
    screenlightLayoutPolicy->setSpacing(0);
    screenlightLayoutPolicy->setContentsMargins(0, 0, 0, 0);

    m_screenlightSlider = new DuiSlider;
    screenlightLayoutPolicy->addItem (m_screenlightSlider);

    m_screenlight_vals = m_logic->screenLightsValues ();
    m_screenlightSlider->setRange (0, m_screenlight_vals.size () - 1);
    m_screenlightSlider->setValue (m_logic->selectedScreenLightsValue ()); 

#if 0
    m_screenlightSlider->setMinLabelVisible (true);
    m_screenlightSlider->setMaxLabelVisible (true);
    m_screenlightSlider->setMinLabelIconID ("icon-l-screenlight-min?");
    m_screenlightSlider->setMaxLabelIconID ("icon-l-screenlight-max?");
#endif

    connect (m_screenlightSlider, SIGNAL (valueChanged (int)),
             m_logic, SLOT (setScreenLightTimeouts (int)));
    connect (m_screenlightSlider, SIGNAL (valueChanged (int)),
             this, SLOT (modify_screenlight_handle (int)));

    m_screenlightContainer = new DuiContainer;
    //% "Screen lights off after: %1 sec" 
    m_screenlightContainer->setTitle (qtTrId ("qtn_disp_screenoff"));
    m_screenlightContainer->centralWidget ()->setLayout (screenlightLayout);

    modify_screenlight_handle (m_logic->selectedScreenLightsValue ());
#ifndef VALUE_IN_HEADER
    m_screenlightSlider->setHandleLabelVisible (true);
#endif

    policy->addItem (m_screenlightContainer);

    // Blank inhibit
    DuiButton *blankInhibitButton = new DuiButton;
    blankInhibitButton->setCheckable (true);
    blankInhibitButton->setChecked (m_logic->blankInhibitValue());
    blankInhibitButton->setObjectName ("blankInhibitButton");

    DuiLayout *blankInhibitLayout = new DuiLayout;
    DuiLinearLayoutPolicy *blankInhibitLayoutPolicy =
        new DuiLinearLayoutPolicy (blankInhibitLayout, Qt::Horizontal);

    //% "Keep screen lights on during the charging"
    m_blankinhibitLabel = new DuiLabel (qtTrId ("qtn_disp_screenon"));
//  m_blankinhibitLabel->setObjectName ("displayLabel");
//    XXX: ^ this should be black
    blankInhibitLayoutPolicy->addItem (m_blankinhibitLabel, Qt::AlignLeft);
    blankInhibitLayoutPolicy->addItem (blankInhibitButton, Qt::AlignRight);

    DuiContainer *blankInhibitContainer = new DuiContainer;
    blankInhibitContainer->setHeaderVisible (false);
    blankInhibitContainer->centralWidget ()->setLayout (blankInhibitLayout);

    connect (blankInhibitButton, SIGNAL (toggled (bool)),
             m_logic, SLOT (setBlankInhibitValue (bool)));

    // policy->addItem(blankInhibitContainer, Qt::AlignLeft);
    // This inhibit button looks better in screenlights container:
    screenlightLayoutPolicy->addItem (blankInhibitContainer, Qt::AlignLeft);

    // Note-label
    //% "Note! In the power save mode, screen light settings are affected."
    m_noteLabel = new DuiLabel (qtTrId ("qtn_disp_note"));
//  m_noteLabel->setObjectName("displayLabel");
//    XXX: ^ this should be black
    policy->addItem (m_noteLabel, Qt::AlignLeft);
}

void
DisplayWidget::modify_screenlight_handle (int newValue)
{
    int value = m_screenlight_vals.at(newValue);

#ifdef VALUE_IN_HEADER
    m_screenlightContainer->setTitle
        (qtTrId ("qtn_disp_screenoff").arg (value));
#else
    m_screenlightSlider->setHandleLabel (
        QString::number (m_screenlight_vals.at(newValue)) + " sec");
#endif
}

void
DisplayWidget::retranslateUi ()
{
    modify_screenlight_handle (m_screenlightSlider->value ());

    m_brightnessContainer->setTitle (qtTrId ("qtn_disp_bright"));
    m_noteLabel->setText (qtTrId ("qtn_disp_note"));
    m_blankinhibitLabel->setText (qtTrId ("qtn_disp_screenon"));
}

