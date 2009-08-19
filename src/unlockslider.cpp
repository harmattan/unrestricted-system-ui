#include "unlockslider.h"

UnlockSlider::UnlockSlider(DuiWidget *parent, const QString &viewType) :
        DuiSlider(parent, viewType)
{
    setRange(0, 100);
    setValue(0);
    connect(this, SIGNAL(sliderPressed()), this, SLOT(pressed()));
    connect(this, SIGNAL(sliderReleased()), this, SLOT(released()));
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(moved(int)));
}

UnlockSlider::~UnlockSlider()
{
}

void UnlockSlider::reset()
{
    setValue(0);
}

void UnlockSlider::released()
{
    reset();
}

void UnlockSlider::moved(int val)
{
    if (val > 99) {
        emit unlocked();
    }
}
