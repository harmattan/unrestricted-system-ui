#include "displaystatestub.h"

namespace Maemo {

QmDisplayState::QmDisplayState(QObject *parent) :
        QObject(parent),
        state(QmDisplayState::On),
        brightness(2),
        blankTimeout(35),
        dimTimeout(30),
        blankingWhenCharging(true)
{    
}

QmDisplayState::~QmDisplayState()
{
}

QmDisplayState::DisplayState QmDisplayState::get() const
{
    return state;
}

bool QmDisplayState::set(DisplayState state)
{
    this->state = state;
    return true;
}

int QmDisplayState::getMaxDisplayBrightnessValue()
{
    return 5;
}

int QmDisplayState::getDisplayBrightnessValue()
{
    return brightness;
}

void QmDisplayState::setDisplayBrightnessValue(int value)
{
    brightness = value;
}

int QmDisplayState::getDisplayBlankTimeout()
{
    return blankTimeout;
}

int QmDisplayState::getDisplayDimTimeout()
{
    return dimTimeout;
}

void QmDisplayState::setDisplayBlankTimeout(int value)
{
    blankTimeout = value;
}

void QmDisplayState::setDisplayDimTimeout(int value)
{
    dimTimeout = value;
}

void QmDisplayState::setBlankingWhenCharging(bool value)
{
    blankingWhenCharging = value;
}

bool QmDisplayState::getBlankingWhenCharging()
{
    return blankingWhenCharging;
}

}
