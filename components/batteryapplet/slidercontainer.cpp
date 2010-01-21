/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "slidercontainer.h"

#include <DuiButton>
#include <DuiLinearLayoutPolicy>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiSlider>

#include <QDebug>

//#define DEBUG 
#include "../debug.h"

SliderContainer::SliderContainer (DuiWidget *parent) :
        DuiContainer (parent),
        PSMAutoButton (NULL),
        PSMSlider (new DuiSlider)
{
    SYS_DEBUG ("");

    setHeaderVisible (false);
    setLayout ();
}

SliderContainer::~SliderContainer ()
{
    SYS_DEBUG ("Destroying %p", this);
}


void
SliderContainer::retranslate ()
{
    SYS_DEBUG ("");
    //% "Auto activate power save"
    textLabel->setText (qtTrId ("qtn_ener_autops"));
}

void SliderContainer::setLayout()
{
    SYS_DEBUG ("");
    DuiLayout *layout = new DuiLayout ();
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

    centralWidget()->setLayout (layout);
}

void 
SliderContainer::initSlider (
        const QStringList &values)
{
    SYS_DEBUG ("");
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
    SYS_DEBUG ("");
    updateSlider(sliderValues.at(value));
    emit PSMThresholdValueChanged(sliderValues.at(value));
}

void SliderContainer::toggleSliderExistence(bool toggle)
{
    SYS_DEBUG ("");
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
    SYS_DEBUG ("");
    PSMAutoButton->blockSignals(true);
    initPSMAutoButton(false);
    PSMAutoButton->blockSignals(false);
}

void SliderContainer::PSMAutoButtonToggled(bool toggle)
{
    SYS_DEBUG ("");
    qDebug() << "SliderContainer::PSMAutoButtonToggled(" << toggle << ")";
    toggleSliderExistence (toggle);
    emit PSMAutoToggled(toggle);
}

