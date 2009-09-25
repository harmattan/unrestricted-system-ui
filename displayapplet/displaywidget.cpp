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
    qDebug() << "TEST";
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
    connect(displayIf, SIGNAL(brightnessValueReceived(QString)), this, SLOT(initBrightnessSliderValue(QString)));
    connect(displayIf, SIGNAL(blankTimeoutValuesReceived(QStringList)), this, SLOT(initBlankTimeoutSlider(QStringList)));
    connect(displayIf, SIGNAL(blankTimeoutValueReceived(QString)), this, SLOT(initBlankTimeoutSliderValue(QString)));
    connect(displayIf, SIGNAL(dimTimeoutValuesReceived(QStringList)), this, SLOT(initDimTimeoutSlider(QStringList)));
    connect(displayIf, SIGNAL(dimTimeoutValueReceived(QString)), this, SLOT(initDimTimeoutSliderValue(QString)));
    connect(displayIf, SIGNAL(blankInhibitValueReceived(bool)), this, SLOT(initBlankInhibitButtonValue(bool)));

    /*
     * brightnessLayoutWidget
     */    
    brightnessSlider = new DuiSlider(0, "continuous");
    displayIf->brightnessValuesRequired();
    displayIf->brightnessValueRequired();   

    DuiLayout *brightnessLayout = new DuiLayout();
    DuiLinearLayoutPolicy *brightnessLayoutPolicy = new DuiLinearLayoutPolicy(brightnessLayout, Qt::Vertical);
    brightnessLayoutPolicy->addItem(new DuiLabel(DcpDisplay::BrightnessText), Qt::AlignLeft);
    brightnessLayoutPolicy->addItem(brightnessSlider, Qt::AlignLeft);

    DuiStylableWidget *brightnessLayoutWidget = new DuiStylableWidget();
    brightnessLayoutWidget->setObjectName("displayLayoutWidget1");
    brightnessLayoutWidget->setLayout(brightnessLayout);

    /*
     * blankTimeoutLayoutWidget
     */    
    blankTimeoutSlider = new DuiSlider(0, "continuous");
    displayIf->blankTimeoutValuesRequired();
    displayIf->blankTimeoutValueRequired();

    DuiLayout *blankTimeoutLayout = new DuiLayout();
    DuiLinearLayoutPolicy *blankTimeoutLayoutPolicy = new DuiLinearLayoutPolicy(blankTimeoutLayout, Qt::Vertical);
    blankTimeoutLayoutPolicy->addItem(new DuiLabel(DcpDisplay::BlankTimeoutText), Qt::AlignLeft);
    blankTimeoutLayoutPolicy->addItem(blankTimeoutSlider, Qt::AlignLeft);

    DuiStylableWidget *blankTimeoutLayoutWidget = new DuiStylableWidget();
    blankTimeoutLayoutWidget->setObjectName("displayLayoutWidget1");
    blankTimeoutLayoutWidget->setLayout(blankTimeoutLayout);

    /*
     * dimTimeoutLayoutWidget
     */
    dimTimeoutSlider = new DuiSlider(0, "continuous");
    displayIf->dimTimeoutValuesRequired();
    displayIf->dimTimeoutValueRequired();

    DuiLayout *dimTimeoutLayout = new DuiLayout();
    DuiLinearLayoutPolicy *dimTimeoutLayoutPolicy = new DuiLinearLayoutPolicy(dimTimeoutLayout, Qt::Vertical);
    dimTimeoutLayoutPolicy->addItem(new DuiLabel(DcpDisplay::DimTimeoutText), Qt::AlignLeft);
    dimTimeoutLayoutPolicy->addItem(dimTimeoutSlider, Qt::AlignLeft);

    DuiStylableWidget *dimTimeoutLayoutWidget = new DuiStylableWidget();
    dimTimeoutLayoutWidget->setObjectName("displayLayoutWidget1");
    dimTimeoutLayoutWidget->setLayout(dimTimeoutLayout);

    /*
     * blankInhibitButtonLayoutWidget
     */    
    blankInhibitButton = new DuiButton();
    blankInhibitButton->setCheckable(true);
    blankInhibitButton->setObjectName("blankInhibitButton");
    displayIf->blankInhibitValueRequired();

    DuiLayout *blankInhibitButtonLayout = new DuiLayout();
    DuiLinearLayoutPolicy *blankInhibitButtonLayoutPolicy = new DuiLinearLayoutPolicy(blankInhibitButtonLayout, Qt::Horizontal);
    blankInhibitButtonLayoutPolicy->addItem(new DuiLabel(DcpDisplay::BlankInhibitButtonText), Qt::AlignLeft);
    blankInhibitButtonLayoutPolicy->addItem(blankInhibitButton, Qt::AlignRight);

    DuiStylableWidget *blankInhibitButtonLayoutWidget = new DuiStylableWidget();
    blankInhibitButtonLayoutWidget->setObjectName("displayLayoutWidget2");
    blankInhibitButtonLayoutWidget->setLayout(blankInhibitButtonLayout);

    /*
     * NoteLayoutWidget
     */    
    DuiLayout *noteLayout = new DuiLayout();
    DuiLinearLayoutPolicy *noteLayoutPolicy = new DuiLinearLayoutPolicy(noteLayout, Qt::Horizontal);
    noteLayoutPolicy->addItem(new DuiLabel(DcpDisplay::NoteText), Qt::AlignLeft);

    DuiStylableWidget *noteLayoutWidget = new DuiStylableWidget();
    noteLayoutWidget->setObjectName("displayLayoutWidget2");
    noteLayoutWidget->setLayout(noteLayout);

    /* 
     * MainLayout     
     */
    QList<DuiStylableWidget*> widgets;
    widgets << brightnessLayoutWidget << blankTimeoutLayoutWidget << dimTimeoutLayoutWidget
            << blankInhibitButtonLayoutWidget << noteLayoutWidget;

    DuiLayout *mainLayout = new DuiLayout(this);

    DuiLinearLayoutPolicy *landscapeLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);    
    addLayoutWidgets(landscapeLayoutPolicy, widgets);    
    mainLayout->setLandscapePolicy(landscapeLayoutPolicy);

    DuiLinearLayoutPolicy *portraitLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Vertical);
    addLayoutWidgets(portraitLayoutPolicy, widgets);
    mainLayout->setPortraitPolicy(portraitLayoutPolicy);

    // catch user actions

    connect(brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(brightnessSliderValueChanged(int)));    
    connect(blankTimeoutSlider, SIGNAL(valueChanged(int)), this, SLOT(blankTimeoutSliderValueChanged(int)));
    connect(dimTimeoutSlider, SIGNAL(valueChanged(int)), this, SLOT(dimTimeoutSliderValueChanged(int)));
    connect(blankInhibitButton, SIGNAL(pressed()), this, SLOT(blankInhibitButtonPressed()));

    this->setLayout(mainLayout);    
}

void DisplayWidget::addLayoutWidgets(DuiLinearLayoutPolicy *policy, const QList<DuiStylableWidget*> &widgets)
{
    for(int i=0; i<widgets.size(); ++i)
        policy->addItem(widgets.at(i), Qt::AlignLeft);
    policy->setSpacing(20);
}


void DisplayWidget::initBrightnessSlider(const QStringList &values)
{    
    initSlider(brightnessSlider, values);
}

void DisplayWidget::initBlankTimeoutSlider(const QStringList &values)
{
    initSlider(blankTimeoutSlider, values);
}

void DisplayWidget::initDimTimeoutSlider(const QStringList &values)
{
    initSlider(dimTimeoutSlider, values);
}

void DisplayWidget::initSlider(DuiSlider *slider, const QStringList &values)
{    
    if(slider == brightnessSlider)
        brightnessSliderValues = QStringList(values);
    else if(slider == blankTimeoutSlider)
        blankTimeoutSliderValues = QStringList(values);
    else if (slider == dimTimeoutSlider)
        dimTimeoutSliderValues = QStringList(values);

    slider->setRange(0,values.size()-1);
    slider->setOrientation(Qt::Horizontal);    
}

void DisplayWidget::initBrightnessSliderValue(const QString &value)
{
    initSliderValue(brightnessSlider, brightnessSliderValues, value);
    updateBrightnessSliderThumbLabel(value);
}

void DisplayWidget::initBlankTimeoutSliderValue(const QString &value)
{
    initSliderValue(blankTimeoutSlider, blankTimeoutSliderValues, value);
    updateBlankTimeoutSliderThumbLabel(value);
}

void DisplayWidget::initDimTimeoutSliderValue(const QString &value)
{
    initSliderValue(dimTimeoutSlider, dimTimeoutSliderValues, value);
    updateDimTimeoutSliderThumbLabel(value);
}

void DisplayWidget::initSliderValue(DuiSlider *slider, const QStringList &values, const QString &value)
{
    if(slider != NULL)
        slider->setValue(values.indexOf(value));
}

void DisplayWidget::updateBrightnessSliderThumbLabel(const QString &value)
{
    updateSliderThumbLabel(brightnessSlider, value);
}

void DisplayWidget::updateBlankTimeoutSliderThumbLabel(const QString &value)
{
    updateSliderThumbLabel(blankTimeoutSlider, value, DcpDisplay::BlankTimeoutValueText);
}

void DisplayWidget::updateDimTimeoutSliderThumbLabel(const QString &value)
{
    updateSliderThumbLabel(dimTimeoutSlider, value, DcpDisplay::DimTimeoutValueText);
}

void DisplayWidget::updateSliderThumbLabel(DuiSlider *slider, const QString &value, const QString &pattern)
{
    if(slider != NULL) {
        if(!pattern.isEmpty())
            slider->setThumbLabel(pattern.arg(value));
        else
            slider->setThumbLabel(value);
    }
}

void DisplayWidget::brightnessSliderValueChanged(int value)
{      
    displayIf->setBrightnessValue(brightnessSliderValues.at(value));
    updateBrightnessSliderThumbLabel(brightnessSliderValues.at(value));
}

void DisplayWidget::blankTimeoutSliderValueChanged(int value)
{
    displayIf->setBlankTimeoutValue(blankTimeoutSliderValues.at(value));
    updateBlankTimeoutSliderThumbLabel(blankTimeoutSliderValues.at(value));
}

void DisplayWidget::dimTimeoutSliderValueChanged(int value)
{
    displayIf->setDimTimeoutValue(dimTimeoutSliderValues.at(value));
    updateDimTimeoutSliderThumbLabel(dimTimeoutSliderValues.at(value));
}

void DisplayWidget::initBlankInhibitButtonValue(bool toggle)
{
    blankInhibitButton->setChecked(toggle);
}

void DisplayWidget::blankInhibitButtonPressed()
{
    //NOTE: DuiButton->isChecked() method returns the state before the press at this point
    displayIf->setBlankInhibitValue(!blankInhibitButton->isChecked());
}
