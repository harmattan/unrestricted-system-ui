#include "displaywidget.h"
#include "displaytranslation.h"
#include "displaydbusinterface.h"
#include "dcpdisplay.h"

#include <QDebug>
#include <QTimer>

#include <DuiButton>
#include <DuiContainer>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiSceneManager>
#include <DuiSlider>
#include <DuiStylableWidget>
#include <DuiWidgetController>

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
    connect(displayIf, SIGNAL(brightnessValuesReceived(int, QStringList)), this, SLOT(initBrightnessSlider(int, QStringList)));
    connect(displayIf, SIGNAL(screenLightsValuesReceived(int, QStringList)), this, SLOT(initScreenLightsSlider(int, QStringList)));
    connect(displayIf, SIGNAL(blankInhibitValueReceived(bool)), this, SLOT(initBlankInhibitButtonValue(bool)));

    QList<DuiWidgetController *> widgets;
    QMap<DuiWidgetController *, Qt::Alignment> alignments;

    /*
     * brightnessContainer
     */    
    brightnessSlider = new DuiSlider(0, "continuous");
    displayIf->brightnessValuesRequired();
    widgets << new DuiLabel(DcpDisplay::BrightnessText) << brightnessSlider;
    DuiContainer *brightnessContainer = createContainer(widgets, alignments, Qt::Vertical, QString("displayLayoutWidget1"));

    /*
     * screenLightsContainer
     */    
    screenLightsSlider = new DuiSlider(0, "continuous");
    displayIf->screenLightsValuesRequired();
    widgets.clear();
    widgets << new DuiLabel(DcpDisplay::ScreenLightsText) << screenLightsSlider;
    DuiContainer *screenLightsContainer = createContainer(widgets, alignments, Qt::Vertical, QString("displayLayoutWidget1"));

    /*
     * blankInhibitButtonLayoutWidget
     */    
    blankInhibitButton = new DuiButton();
    blankInhibitButton->setCheckable(true);
    blankInhibitButton->setObjectName("blankInhibitButton");
    displayIf->blankInhibitValueRequired();
    widgets.clear();
    widgets << new DuiLabel(DcpDisplay::BlankInhibitText) << blankInhibitButton;
    alignments.insert(blankInhibitButton, Qt::AlignRight);
    DuiContainer *blankInhibitContainer = createContainer(widgets, alignments, Qt::Horizontal, QString("displayLayoutWidget1"));

    /*
     * contentContainer
     */
    widgets.clear();
    alignments.clear();
    widgets << brightnessContainer << screenLightsContainer << blankInhibitContainer << new DuiLabel(DcpDisplay::NoteText);
    DuiContainer *contentContainer = createContainer(widgets, alignments, Qt::Vertical, QString(""), 20);

    /*
     * mainLayout
     */
    DuiLayout *mainLayout = new DuiLayout(this);
    DuiLinearLayoutPolicy *mainLayoutPolicy = new DuiLinearLayoutPolicy(mainLayout, Qt::Horizontal);
    mainLayoutPolicy->addItem(contentContainer);   

    // catch user actions
    connect(brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(screenLightsSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    connect(blankInhibitButton, SIGNAL(pressed()), this, SLOT(blankInhibitButtonPressed()));

    this->setLayout(mainLayout);    

}

DuiContainer* DisplayWidget::createContainer(QList<DuiWidgetController *> widgets, QMap<DuiWidgetController *, Qt::Alignment> alignments,
                                             Qt::Orientation policyOrientation, QString widgetObjectName, int policySpacing)
{
    DuiLayout *layout = new DuiLayout();
    DuiLinearLayoutPolicy *layoutPolicy = new DuiLinearLayoutPolicy(layout, policyOrientation);
    for(int i=0; i<widgets.size(); ++i)
        layoutPolicy->addItem(widgets.at(i), (alignments.value(widgets.at(i)) == 0 ? Qt::AlignLeft : alignments.value(widgets.at(i))));
    layoutPolicy->setSpacing(policySpacing);
    DuiStylableWidget *layoutWidget = new DuiStylableWidget();
    layoutWidget->setLayout(layout);
    if(!widgetObjectName.isEmpty())
        layoutWidget->setObjectName(widgetObjectName);
    DuiContainer *container = new DuiContainer(this);
    container->setCentralWidget(layoutWidget);
    return container;
}

void DisplayWidget::addLayoutWidgets(DuiLinearLayoutPolicy *policy, const QList<DuiContainer*> &containers)
{
    for(int i=0; i<containers.size(); ++i)
        policy->addItem(containers.at(i), Qt::AlignLeft);
    policy->setSpacing(20);
}

void DisplayWidget::initBrightnessSlider(int index, const QStringList &values)
{
    brightnessValues = QStringList(values);
    initSlider(brightnessSlider, index, values);
}

void DisplayWidget::initScreenLightsSlider(int index, const QStringList &values)
{
    screenLightsValues = QStringList(values);
    initSlider(screenLightsSlider, index, values);
}

void DisplayWidget::initSlider(DuiSlider *slider, int index, const QStringList &values)
{
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0,values.size()-1);
    slider->setValue(index);

    QString pattern = (slider == screenLightsSlider ? DcpDisplay::ScreenLightsValueText : QString(""));
    updateSliderThumbLabel(slider, values.at(index), pattern);
}

void DisplayWidget::updateSliderThumbLabel(DuiSlider *slider, const QString &value, const QString &pattern)
{    
    slider->setThumbLabel((pattern.isEmpty() ? value : pattern.arg(value)));
}

void DisplayWidget::sliderValueChanged(int index)
{      
    DuiSlider* slider = static_cast<DuiSlider*>(this->sender());
    if(slider == brightnessSlider) {
        displayIf->setBrightnessValue(brightnessValues.at(index));
        updateSliderThumbLabel(slider, brightnessValues.at(index));
    }
    else {
        displayIf->setScreenLightsValue(screenLightsValues.at(index));
        updateSliderThumbLabel(slider, screenLightsValues.at(index), DcpDisplay::ScreenLightsValueText);
    }
}

void DisplayWidget::initBlankInhibitButton(bool toggle)
{
    blankInhibitButton->setChecked(toggle);
}

void DisplayWidget::blankInhibitButtonPressed()
{
    //NOTE: DuiButton->isChecked() method returns the state before the press at this point
    displayIf->setBlankInhibitValue(!blankInhibitButton->isChecked());
}
