#include "unlockslider.h"

#include <QDebug>

const int RESET_RATE(200); //15);
const int RESET_TIME(300);

UnlockSlider::UnlockSlider(DuiWidget *parent, const QString &viewType) :
        DuiSlider(parent, viewType),
        resetVelocity(0)
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
    resetVelocity = -1;
    timer.start(RESET_RATE, this);
}

void UnlockSlider::moved(int val)
{
    if (val > 99) {
//        qDebug() << "unlocked";
        emit unlocked();
    }
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

