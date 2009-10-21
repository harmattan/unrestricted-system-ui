#include "displaywidget.h"
#include "displaytranslation.h"
#include "displaydbusinterface.h"
#include "slidercontainer.h"
#include "dcpdisplay.h"

#include <QDebug>

#include <DuiButton>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>

DisplayWidget::DisplayWidget(QGraphicsWidget *parent) :
        DcpWidget(parent),
        blankInhibitButton(new DuiButton(this)),
        displayIf(new DisplayDBusInterface()),
        brightnessContainer(new SliderContainer(this, DcpDisplay::BrightnessText)),
        screenLightsContainer(new SliderContainer(this, DcpDisplay::ScreenLightsText, DcpDisplay::ScreenLightsValueText))
{    
    setReferer(DcpDisplay::None);
    initWidget();
}

DisplayWidget::~DisplayWidget()
{    
}

void DisplayWidget::initWidget()
{
    // blankInhibitContainer
    blankInhibitButton->setCheckable(true);
    blankInhibitButton->setObjectName("blankInhibitButton");
    DuiLayout *blankInhibitLayout = new DuiLayout();
    DuiLinearLayoutPolicy *blankInhibitLayoutPolicy = new DuiLinearLayoutPolicy(blankInhibitLayout, Qt::Horizontal);    
    blankInhibitLayoutPolicy->addItem(new DuiLabel(DcpDisplay::BlankInhibitText, this), Qt::AlignLeft);
    blankInhibitLayoutPolicy->addItem(blankInhibitButton, Qt::AlignRight);
    DuiContainer *blankInhibitContainer = new DuiContainer(this);
    blankInhibitContainer->centralWidget()->setLayout(blankInhibitLayout);

    // contentContainer
    DuiLayout *contentLayout = new DuiLayout();
    DuiLinearLayoutPolicy *contentLayoutPolicy = new DuiLinearLayoutPolicy(contentLayout, Qt::Vertical);
    contentLayoutPolicy->addItem(brightnessContainer, Qt::AlignLeft);
    contentLayoutPolicy->addItem(screenLightsContainer, Qt::AlignLeft);
    contentLayoutPolicy->addItem(blankInhibitContainer, Qt::AlignLeft);
    contentLayoutPolicy->setSpacing(10);
    DuiContainer *contentContainer = new DuiContainer(this);
    contentContainer->centralWidget()->setLayout(contentLayout);

    //mainLayout
    DuiLayout *mainLayout = new DuiLayout(this);
    DuiLinearLayoutPolicy *mainLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Horizontal);
    mainLayoutPolicy->addItem(contentContainer);

    // catch display If actions
    connect(displayIf, SIGNAL(brightnessValuesReceived(int, QStringList)), brightnessContainer, SLOT(initSlider(int, QStringList)));
    connect(displayIf, SIGNAL(screenLightsValuesReceived(int, QStringList)), screenLightsContainer, SLOT(initSlider(int, QStringList)));
    connect(displayIf, SIGNAL(blankInhibitValueReceived(bool)), this, SLOT(initBlankInhibitButton(bool)));

    // request init values
    displayIf->brightnessValuesRequired();
    displayIf->screenLightsValuesRequired();
    displayIf->blankInhibitValueRequired();

    // catch user actions
    connect(brightnessContainer, SIGNAL(sliderValueChanged(QString)), displayIf, SLOT(setBrightnessValue(QString)));
    connect(screenLightsContainer, SIGNAL(sliderValueChanged(QString)), displayIf, SLOT(setScreenLightsValue(QString)));
    connect(blankInhibitButton, SIGNAL(toggled(bool)), displayIf, SLOT(setBlankInhibitValue(bool)));

    this->setLayout(mainLayout);    

}

void DisplayWidget::initBlankInhibitButton(bool toggle)
{
    blankInhibitButton->setChecked(toggle);
}
