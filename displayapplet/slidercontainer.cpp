#include "slidercontainer.h"

#include <DuiLabel>
#include <DuiLayout>
#include <DuiGridLayoutPolicy>
#include <DuiSlider>

#include <QDebug>

SliderContainer::SliderContainer(DuiWidget *parent, const QString &infoText, const QString &valuePattern) :
        DuiContainer(parent),
        label(new DuiLabel(infoText, this)),        
        slider(new DuiSlider(this, "continuous")),
        valuePattern(valuePattern)
{    
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
    setLayout();
}

SliderContainer::~SliderContainer()
{
}

void SliderContainer::setLayout()
{    
    DuiLayout *layout = new DuiLayout();
    DuiGridLayoutPolicy *layoutPolicy = new DuiGridLayoutPolicy(layout);
    layoutPolicy->addItemAtPosition(label, 0, 0);
    layoutPolicy->addItemAtPosition(slider, 1, 0);
    layoutPolicy->setRowMaximumHeight(1, 50);

    centralWidget()->setLayout(layout);
}

void SliderContainer::initSlider(int index, const QStringList &values)
{
    sliderValues = QStringList(values);
    slider->setRange(0,sliderValues.size()-1);    
    slider->setOrientation(Qt::Horizontal);
    slider->setValue(index);
    updateSlider(sliderValues.at(index));
}

void SliderContainer::updateSlider(const QString &value)
{
    qDebug() << "SliderContainer::updateSlider(" << value << ")";
    slider->setThumbLabel(valuePattern.arg(value));
}

void SliderContainer::valueChanged(int value)
{
    updateSlider(sliderValues.at(value));
    emit sliderValueChanged(sliderValues.at(value));
}
