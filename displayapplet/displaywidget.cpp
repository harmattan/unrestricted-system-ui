#include "displaywidget.h"
#include "displaytranslation.h"
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

    m_brightnessSlider->setMinLabelVisible (true);
    m_brightnessSlider->setMaxLabelVisible (true);
    m_brightnessSlider->setMinLabelIconID ("icon-l-brightness-min");
    m_brightnessSlider->setMaxLabelIconID ("icon-l-brightness-max");

    connect (m_brightnessSlider, SIGNAL (valueChanged (int)),
             m_logic, SLOT (setBrightnessValue (int)));
    connect (m_brightnessSlider, SIGNAL (valueChanged (int)),
             this, SLOT (modify_brightness_handle (int)));

    m_brightnessContainer = new DuiContainer;
    m_brightnessContainer->setTitle (DcpDisplay::BrightnessText);
    m_brightnessContainer->centralWidget ()->setLayout (brightnessLayout);

    modify_brightness_handle (m_logic->selectedBrightnessValue ());
#ifndef VALUE_IN_HEADER
    m_brightnessSlider->setHandleLabelVisible (true);
#endif

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
             m_logic, SLOT (setScreenLightsValue (int)));
    connect (m_screenlightSlider, SIGNAL (valueChanged (int)),
             this, SLOT (modify_screenlight_handle (int)));

    m_screenlightContainer = new DuiContainer;
    m_screenlightContainer->setTitle (DcpDisplay::ScreenLightsText);
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

    DuiLabel *blankInhibitLabel = new DuiLabel (DcpDisplay::BlankInhibitText);
//    blankInhibitLabel->setObjectName ("displayLabel");
//    XXX: ^ this should be black
    blankInhibitLayoutPolicy->addItem (blankInhibitLabel, Qt::AlignLeft);
    blankInhibitLayoutPolicy->addItem (blankInhibitButton, Qt::AlignRight);

    DuiContainer *blankInhibitContainer = new DuiContainer;
    blankInhibitContainer->setHeaderVisible (false);
    blankInhibitContainer->centralWidget ()->setLayout (blankInhibitLayout);

    connect (blankInhibitButton, SIGNAL (toggled (bool)),
             m_logic, SLOT (setBlankInhibitValue (bool)));

    // policy->addItem(blankInhibitContainer, Qt::AlignLeft);
    // This inhibit button better in screenlights container:
    screenlightLayoutPolicy->addItem (blankInhibitContainer, Qt::AlignLeft);

    // Note-label
    DuiLabel *noteLabel = new DuiLabel (DcpDisplay::NoteText);
//    noteLabel->setObjectName("displayLabel");
//    XXX: ^ this should be black
    policy->addItem (noteLabel, Qt::AlignLeft);
}

void
DisplayWidget::modify_brightness_handle (int newValue)
{
    // FIXME: val * 20 % is okey?
    int value = m_brightness_vals.at(newValue) * 20;

#ifdef VALUE_IN_HEADER
    m_brightnessContainer->setTitle
        (QString ("Brightness: %1%").arg (value));
#else
    m_brightnessSlider->setHandleLabel (QString::number (value) + "%");
#endif
}

void
DisplayWidget::modify_screenlight_handle (int newValue)
{
    int value = m_screenlight_vals.at(newValue);

    // Todo localize this with qtTrId
#ifdef VALUE_IN_HEADER
    m_screenlightContainer->setTitle
        (QString ("Screen lights off after: %1 sec").arg (value));
#else
    m_screenlightSlider->setHandleLabel (
        QString::number (m_screenlight_vals.at(newValue)) + " sec");
#endif
}

