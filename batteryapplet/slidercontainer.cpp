#include "slidercontainer.h"
#include "batterytranslation.h"

#include <DuiButton>
#include <DuiGridLayoutPolicy>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiSlider>

#include <QDebug>

SliderContainer::SliderContainer(DuiWidget *parent) :
        DuiContainer(parent),
        PSMAutoButton(new DuiButton(this)),
        PSMSlider(new DuiSlider(this, "continuous"))
{
    connect(PSMAutoButton, SIGNAL(toggled(bool)), this, SLOT(PSMAutoButtonToggled(bool)));
    connect(PSMSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
    setLayout();
}

SliderContainer::~SliderContainer()
{
}

void SliderContainer::setLayout()
{   
    PSMAutoButton->setCheckable(true);
    PSMAutoButton->setObjectName("PSMAutoButton");

    DuiLayout *layout = new DuiLayout();
    layoutPolicy = new DuiGridLayoutPolicy(layout);
    layoutPolicy->addItemAtPosition(new DuiLabel(DcpBattery::PSMAutoActivateText), 0, 0);
    layoutPolicy->addItemAtPosition(PSMAutoButton, 0, 1);
    layoutPolicy->setRowMaximumHeight(1, 75);

    centralWidget()->setLayout(layout);
}

void SliderContainer::initSlider(const QStringList &values)
{
    sliderValues = QStringList(values);
    PSMSlider->setRange(0,sliderValues.size()-1);
    PSMSlider->setOrientation(Qt::Horizontal);
}

void SliderContainer::updateSlider(const QString &value)
{    
    qDebug() << "SliderContainer::updateSlider(" << value << ")";
    PSMSlider->setValue(sliderValues.indexOf(value)); //in case this is the first call, we need to set the value
    PSMSlider->setThumbLabel(QString("%1 bars").arg(value)); //replace with %
}

void SliderContainer::sliderValueChanged(int value)
{        
    updateSlider(sliderValues.at(value));
    emit PSMThresholdValueChanged(sliderValues.at(value));
}

void SliderContainer::toggleSliderExistence(bool toggle)
{
    qDebug() << "SliderContainer::toggleSliderExistence(" << toggle << ")";
    if(toggle) {
        if(layoutPolicy->itemAt(1, 0) != PSMSlider)
            layoutPolicy->addItemAtPosition(PSMSlider, 1, 0, 1, 2);
    }
    else {
        if(layoutPolicy->itemAt(1, 0) == PSMSlider)
            layoutPolicy->removeItem(PSMSlider);
    }
}

void SliderContainer::initPSMAutoButton(bool toggle)
{
    PSMAutoButton->setChecked(toggle);
    toggleSliderExistence(toggle);
}

void SliderContainer::PSMAutoButtonToggled(bool toggle)
{
    qDebug() << "SliderContainer::PSMAutoButtonToggled(" << toggle << ")";
    toggleSliderExistence(toggle);
    emit PSMAutoToggled(toggle);
}
