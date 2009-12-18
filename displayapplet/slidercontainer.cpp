#include "slidercontainer.h"

#include <DuiLabel>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiSlider>

#include <QDebug>

SliderContainer::SliderContainer(const QString &infoText, const QString &valuePattern, DuiWidget *parent) :
        DuiContainer(parent),
        label(NULL),
        slider(NULL),
        valuePattern(valuePattern)
{
    label = new DuiLabel(infoText);
    label->setObjectName("displayLabel");
    slider = new DuiSlider(this, "continuous");
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
    setLayout();
}

SliderContainer::~SliderContainer()
{
}

void SliderContainer::setLayout()
{
    DuiLayout *layout = new DuiLayout();
    DuiLinearLayoutPolicy *layoutPolicy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    layoutPolicy->addItem(label, Qt::AlignLeft);
    layoutPolicy->addItem(slider, Qt::AlignLeft);
    layoutPolicy->setItemSpacing(0, 25);
    centralWidget()->setLayout(layout);
}

void SliderContainer::initSlider(const QList<int> &values, int index)
{
    sliderValues = values;
    slider->setRange(0, sliderValues.size() - 1);
    slider->setOrientation(Qt::Horizontal);
    slider->setValue(index);
    updateSlider(sliderValues.at(index));
}

void SliderContainer::updateSlider(int value)
{
    slider->setThumbLabel(valuePattern.arg(value));
}

void SliderContainer::valueChanged(int index)
{
    int value = sliderValues.at(index);
    updateSlider(value);
    emit sliderValueChanged(value);
}
