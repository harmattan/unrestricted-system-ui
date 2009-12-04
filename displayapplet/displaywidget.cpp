#include "displaywidget.h"
#include "displaytranslation.h"
#include "displaybusinesslogic.h"
#include "slidercontainer.h"
#include "dcpdisplay.h"

#include <QDebug>

#include <DuiButton>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>

#include <QGraphicsLinearLayout>

/* TODO:
    1) Display header not yet defined (logical name)
*/

DisplayWidget::DisplayWidget(QGraphicsWidget *parent) :
        DcpWidget(parent),
        blankInhibitButton(NULL),
        logic(NULL),
        brightnessContainer(NULL),
        screenLightsContainer(NULL)
{    
    setReferer(DcpDisplay::None);
    initWidget();
}

DisplayWidget::~DisplayWidget()
{    
}

void DisplayWidget::initWidget()
{
    logic = new DisplayBusinessLogic();    

    // blankInhibitContainer
    blankInhibitButton = new DuiButton();    
    blankInhibitButton->setCheckable(true);
    blankInhibitButton->setChecked(logic->blankInhibitValue());
    blankInhibitButton->setObjectName("blankInhibitButton");
    DuiLayout *blankInhibitLayout = new DuiLayout();
    DuiLinearLayoutPolicy *blankInhibitLayoutPolicy = new DuiLinearLayoutPolicy(blankInhibitLayout, Qt::Horizontal);    
    DuiLabel *blankiInhibitLabel = new DuiLabel(DcpDisplay::BlankInhibitText);
    blankiInhibitLabel->setObjectName("displayLabel");
    blankInhibitLayoutPolicy->addItem(blankiInhibitLabel, Qt::AlignLeft);
    blankInhibitLayoutPolicy->addItem(blankInhibitButton, Qt::AlignRight);
    DuiContainer *blankInhibitContainer = new DuiContainer();
    blankInhibitContainer->centralWidget()->setLayout(blankInhibitLayout);

    // contentContainer
    DuiLayout *contentLayout = new DuiLayout();
    DuiLinearLayoutPolicy *contentLayoutPolicy = new DuiLinearLayoutPolicy(contentLayout, Qt::Vertical);
    brightnessContainer = new SliderContainer(DcpDisplay::BrightnessText);
    brightnessContainer->initSlider(logic->brightnessValues(), logic->selectedBrightnessValue());
    contentLayoutPolicy->addItem(brightnessContainer, Qt::AlignLeft);
    screenLightsContainer = new SliderContainer(DcpDisplay::ScreenLightsText, DcpDisplay::ScreenLightsValueText);
    screenLightsContainer->initSlider(logic->screenLightsValues(), logic->selectedScreenLightsValue());
    contentLayoutPolicy->addItem(screenLightsContainer, Qt::AlignLeft);
    contentLayoutPolicy->addItem(blankInhibitContainer, Qt::AlignLeft);
    DuiLabel *noteLabel = new DuiLabel(DcpDisplay::NoteText);
    noteLabel->setObjectName("displayLabel");
    contentLayoutPolicy->addItem(noteLabel, Qt::AlignLeft);
    contentLayoutPolicy->setSpacing(10);
    DuiContainer *contentContainer = new DuiContainer();
    contentContainer->centralWidget()->setLayout(contentLayout);

    //mainLayout
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout();
    mainLayout->addItem(contentContainer);

    // catch user actions
    connect(brightnessContainer, SIGNAL(sliderValueChanged(int)), logic, SLOT(setBrightnessValue(int)));
    connect(screenLightsContainer, SIGNAL(sliderValueChanged(int)), logic, SLOT(setScreenLightsValue(int)));
    connect(blankInhibitButton, SIGNAL(toggled(bool)), logic, SLOT(setBlankInhibitValue(bool)));

    this->setLayout(mainLayout);    

}

void DisplayWidget::initBlankInhibitButton(bool toggle)
{
    blankInhibitButton->setChecked(toggle);
}
