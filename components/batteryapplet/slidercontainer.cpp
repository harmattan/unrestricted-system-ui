#include "slidercontainer.h"

#include <DuiButton>
#include <DuiLinearLayoutPolicy>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiSlider>

#include <QDebug>

SliderContainer::SliderContainer(DuiWidget *parent) :
        DuiContainer (parent),
        PSMAutoButton (NULL),
        PSMSlider (NULL)
{
    PSMSlider = new DuiSlider;
    setHeaderVisible (false);
    setLayout ();
}


void
SliderContainer::retranslate ()
{
    //% "Auto activate power save"
    textLabel->setText (qtTrId ("qtn_ener_autops"));
}

void SliderContainer::setLayout()
{
    DuiLayout *layout = new DuiLayout (this);
    layout_policy =
        new DuiLinearLayoutPolicy (layout, Qt::Vertical);

    DuiLayout *hlayout = new DuiLayout;
    DuiLinearLayoutPolicy *hpolicy =
        new DuiLinearLayoutPolicy (hlayout, Qt::Horizontal);

    // battery label
    textLabel = new DuiLabel;
    textLabel->setObjectName("batteryLabel");
    retranslate ();

    hpolicy->addItem (textLabel, Qt::AlignLeft);

    // PSMAutoButton
    PSMAutoButton = new DuiButton ();
    connect (PSMAutoButton, SIGNAL (toggled (bool)),
             this, SLOT (PSMAutoButtonToggled (bool)));
    PSMAutoButton->setCheckable(true);
    PSMAutoButton->setObjectName("PSMAutoButton");

    hpolicy->addItem (PSMAutoButton, Qt::AlignRight);

    layout_policy->addItem (hlayout);
}

void SliderContainer::initSlider(const QStringList &values)
{
    sliderValues = QStringList (values);
    PSMSlider->setRange (0, sliderValues.size() - 1);
    PSMSlider->setOrientation (Qt::Horizontal);
    PSMSlider->setHandleLabelVisible (true);
    connect (PSMSlider, SIGNAL (valueChanged (int)),
             this, SLOT (sliderValueChanged (int)));
}

void SliderContainer::updateSlider(const QString &value)
{
    qDebug() << "SliderContainer::updateSlider(" << value << ")";
    PSMSlider->setValue (sliderValues.indexOf(value));
    //^ in case this is the first call, we need to set the value
    PSMSlider->setHandleLabel (QString ("%1%").arg (value));
}

void SliderContainer::sliderValueChanged(int value)
{
    updateSlider(sliderValues.at(value));
    emit PSMThresholdValueChanged(sliderValues.at(value));
}

void SliderContainer::toggleSliderExistence(bool toggle)
{
    if (toggle) {
        if (layout_policy->count() < 2)
            layout_policy->addItem (PSMSlider);
    } else {
        if (layout_policy->count() > 1)
            layout_policy->removeItem (PSMSlider);
    }
}

void SliderContainer::initPSMAutoButton(bool toggle)
{
    PSMAutoButton->setChecked (toggle);
    toggleSliderExistence (toggle);
}

void SliderContainer::PSMAutoDisabled()
{
    PSMAutoButton->blockSignals(true);
    initPSMAutoButton(false);
    PSMAutoButton->blockSignals(false);
}

void SliderContainer::PSMAutoButtonToggled(bool toggle)
{
    qDebug() << "SliderContainer::PSMAutoButtonToggled(" << toggle << ")";
    toggleSliderExistence (toggle);
    emit PSMAutoToggled(toggle);
}

