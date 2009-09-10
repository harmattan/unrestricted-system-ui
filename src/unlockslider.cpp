#include "unlockslider.h"

#include <QDebug>

const int RESET_RATE(200); //15);
const int RESET_TIME(300);

UnlockSlider::UnlockSlider(DuiWidget *parent, const QString &viewType, int range) :
        DuiSlider(parent, viewType),
        resetVelocity(0),
        range(range)
{
    setRange(0, range);
    setValue(0);
    aHWKeyDown = false;
    setThumbLabelVisible(false);
    connect(this, SIGNAL(sliderPressed()), this, SLOT(pressed()));
    connect(this, SIGNAL(sliderReleased()), this, SLOT(released()));
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(moved(int)));    
}

UnlockSlider::~UnlockSlider()
{
}

void UnlockSlider::reset()
{
    resetVelocity = 0;
    timer.stop();
    setValue(0);
}

void UnlockSlider::pressed()
{
    timer.stop();
}

void UnlockSlider::released()
{
    qDebug() << "released()";
    if(!aHWKeyDown && model()->state() == DuiSliderModel::Released) {        
        resetVelocity = -1;
        timer.start(RESET_RATE, this);
    }
}

void UnlockSlider::moved(int val)
{
    qDebug() << "moved";
    emit valueChanged();
    if (val > range-1) {
//        qDebug() << "unlocked";
        emit unlocked();
    }
}

void UnlockSlider::updateValue(int newValue)
{    
    setValue(newValue);
}

void UnlockSlider::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timer.timerId()) {
        resetVelocity *= 1.2;
        int pos = value() + resetVelocity;

        if (pos < 1)  {
            reset();
        }

        setValue(pos);
        update();
    }
}

void UnlockSlider::hwKeyDown(bool down)
{
    qDebug() << "hwKeyDown " << down;
    aHWKeyDown = down;
    if(!aHWKeyDown) //hw button released
        released();
}
