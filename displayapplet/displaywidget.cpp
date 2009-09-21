#include "displaywidget.h"
#include "displaytranslation.h"
#include "displaydbusinterface.h"
#include "dcpdisplay.h"

#include <QDebug>
#include <QTimer>

#include <DuiButton>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiLabel>
#include <DuiSlider>
#include <DuiStylableWidget>
#include <DuiSceneManager>

DisplayWidget::DisplayWidget(QGraphicsWidget *parent)
	    :DcpWidget(parent)
{
    setReferer(DcpDisplay::None);
    initWidget();
}

DisplayWidget::~DisplayWidget()
{    
}

void DisplayWidget::initWidget()
{
    //create dbus if
    displayIf = new DisplayDBusInterface();

    // catch display If actions
    connect(displayIf, SIGNAL(brightnessValuesReceived(QStringList)), this, SLOT(initBrightnessSlider(QStringList)));
    connect(displayIf, SIGNAL(brightnessValueReceived(QString)), this, SLOT(updateBrightnessSlider(QString)));
    connect(displayIf, SIGNAL(screenLightsValuesReceived(QStringList)), this, SLOT(initScreenLightsSlider(QStringList)));
    connect(displayIf, SIGNAL(screenLightsValueReceived(QString)), this, SLOT(updateScreenLightsSlider(QString)));
    connect(displayIf, SIGNAL(screenLightsToggleValueReceived(bool)), this, SLOT(updateScreenLightsButton(bool)));    

    /*
     * brightnessLayoutWidget
     */    
    brightnessSlider = new DuiSlider(0, "continuous");
    displayIf->brightnessValuesRequired();
    displayIf->brightnessValueRequired();   

    DuiLayout *brightnessLayout = new DuiLayout();
    DuiLinearLayoutPolicy *brightnessLayoutPolicy = new DuiLinearLayoutPolicy(brightnessLayout, Qt::Vertical);
    brightnessLayoutPolicy->addItemAtPosition(new DuiLabel(DcpDisplay::BrightnessText), 0, Qt::AlignLeft);
    brightnessLayoutPolicy->addItemAtPosition(brightnessSlider, 1, Qt::AlignLeft);

    DuiStylableWidget *brightnessLayoutWidget = new DuiStylableWidget();
    brightnessLayoutWidget->setObjectName("displayLayoutWidget1");
    brightnessLayoutWidget->setLayout(brightnessLayout);

    /*
     * screenLightsLayoutWidget
     */    
    screenLightsSlider = new DuiSlider(0, "continuous");
    displayIf->screenLightsValuesRequired();
    displayIf->screenLightsValueRequired();

    DuiLayout *screenLightsLayout = new DuiLayout();
    DuiLinearLayoutPolicy *screenLightsLayoutPolicy = new DuiLinearLayoutPolicy(screenLightsLayout, Qt::Vertical);
    screenLightsLayoutPolicy->addItemAtPosition(new DuiLabel(DcpDisplay::ScreenLightsText), 0, Qt::AlignLeft);
    screenLightsLayoutPolicy->addItemAtPosition(screenLightsSlider, 1, Qt::AlignLeft);

    DuiStylableWidget *screenLightsLayoutWidget = new DuiStylableWidget();
    screenLightsLayoutWidget->setObjectName("displayLayoutWidget1");
    screenLightsLayoutWidget->setLayout(screenLightsLayout);

    /*
     * screenLightsButtonLayoutWidget
     */    
    screenLightsButton = new DuiButton();
    screenLightsButton->setCheckable(true);
    screenLightsButton->setObjectName("screenLightsButton");
    displayIf->screenLightsToggleValueRequired();

    DuiLayout *screenLightsButtonLayout = new DuiLayout();
    DuiLinearLayoutPolicy *screenLightsButtonLayoutPolicy = new DuiLinearLayoutPolicy(screenLightsButtonLayout, Qt::Horizontal);
    screenLightsButtonLayoutPolicy->addItemAtPosition(new DuiLabel(DcpDisplay::ScreenLightsButtonText), 0, Qt::AlignLeft);
    screenLightsButtonLayoutPolicy->addItemAtPosition(screenLightsButton, 1, Qt::AlignRight);

    DuiStylableWidget *screenLightsButtonLayoutWidget = new DuiStylableWidget();
    screenLightsButtonLayoutWidget->setObjectName("displayLayoutWidget2");
    screenLightsButtonLayoutWidget->setLayout(screenLightsButtonLayout);

    /*
     * NoteLayoutWidget
     */    
    DuiLayout *noteLayout = new DuiLayout();
    DuiLinearLayoutPolicy *noteLayoutPolicy = new DuiLinearLayoutPolicy(noteLayout, Qt::Horizontal);
    noteLayoutPolicy->addItemAtPosition(new DuiLabel(DcpDisplay::NoteText), 0, Qt::AlignLeft);

    DuiStylableWidget *noteLayoutWidget = new DuiStylableWidget();
    noteLayoutWidget->setObjectName("displayLayoutWidget2");
    noteLayoutWidget->setLayout(noteLayout);

    /* 
     * MainLayout     
     */
    QList<DuiStylableWidget*> widgets;
    widgets << brightnessLayoutWidget << screenLightsLayoutWidget << screenLightsButtonLayoutWidget << noteLayoutWidget;

    DuiLayout *mainLayout = new DuiLayout(this);

    DuiLinearLayoutPolicy *landscapeLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);    
    addLayoutWidgets(landscapeLayoutPolicy, widgets);    
    mainLayout->setLandscapePolicy(landscapeLayoutPolicy);

    DuiLinearLayoutPolicy *portraitLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    addLayoutWidgets(portraitLayoutPolicy, widgets);
    mainLayout->setPortraitPolicy(portraitLayoutPolicy);

    // catch user actions
    connect(screenLightsButton, SIGNAL(pressed()), this, SLOT(screenLightsButtonPressed()));
    connect(brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(brightnessSliderValueChanged(int)));
    connect(screenLightsSlider, SIGNAL(valueChanged(int)), this, SLOT(screenLightsSliderValueChanged(int)));           

    this->setLayout(mainLayout);    
}

void DisplayWidget::addLayoutWidgets(DuiLinearLayoutPolicy *policy, const QList<DuiStylableWidget*> &widgets)
{
    for(int i=0; i<widgets.size(); ++i)
        policy->addItemAtPosition(widgets.at(i), i);            
    policy->setSpacing(20);
}


void DisplayWidget::initBrightnessSlider(const QStringList &values)
{    
    initSlider(brightnessSlider, values);
}

void DisplayWidget::initScreenLightsSlider(const QStringList &values)
{
    initSlider(screenLightsSlider, values);
}

void DisplayWidget::initSlider(DuiSlider *slider, const QStringList &values)
{
    if(slider == brightnessSlider)
        brightnessSliderValues = QStringList(values);
    else
        screenLightsSliderValues = QStringList(values);

    slider->setRange(0,values.size()-1);
    slider->setOrientation(Qt::Horizontal);    
}

void DisplayWidget::updateBrightnessSlider(const QString &value)
{
    static bool firstCall = true;
    if(brightnessSlider != NULL) {
        if(firstCall) {
            brightnessSlider->setValue(brightnessSliderValues.indexOf(value)); //in case this is the first call, we need to set the value
            firstCall = false;
        }
        brightnessSlider->setThumbLabel(QString("%1%").arg(value));
    }
}

void DisplayWidget::updateScreenLightsSlider(const QString &value)
{
    static bool firstCall = true;
    if(screenLightsSlider != NULL) {
        if(firstCall) {
            screenLightsSlider->setValue(brightnessSliderValues.indexOf(value)); //in case this is the first call, we need to set the value
            firstCall = false;
        }
        screenLightsSlider->setThumbLabel((DcpDisplay::ScreenLightsValueText).arg(value));
    }
}

void DisplayWidget::brightnessSliderValueChanged(int value)
{      
    displayIf->setBrightnessValue(brightnessSliderValues.at(value));
    updateBrightnessSlider(brightnessSliderValues.at(value));
}

void DisplayWidget::screenLightsSliderValueChanged(int value)
{
    displayIf->setScreenLightsValue(screenLightsSliderValues.at(value));
    updateScreenLightsSlider(screenLightsSliderValues.at(value));
}

void DisplayWidget::updateScreenLightsButton(bool toggle)
{
    screenLightsButton->setChecked(toggle);
}

void DisplayWidget::screenLightsButtonPressed()
{
    displayIf->setScreenLightsToggleValue(screenLightsButton->isChecked());
}
