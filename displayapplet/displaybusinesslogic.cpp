#include "displaybusinesslogic.h"

#include <QDebug>

/*
    TODO:
    1) Make sure the screen lights values and default index are correct

*/

using namespace Maemo;

static int TIMEGAP = 5; // time gap between blanking and dimming

DisplayBusinessLogic::DisplayBusinessLogic(QObject* parent) :
        QObject(parent),
        display(new QmDisplayState())
{        
}

DisplayBusinessLogic::~DisplayBusinessLogic()
{
    delete display;
    display = NULL;
}

void DisplayBusinessLogic::toggleDisplay(bool toggle)
{ 
    QmDisplayState::DisplayState state = (toggle ? QmDisplayState::On : QmDisplayState::Off);
    display->set(state);
}

QList<int> DisplayBusinessLogic::brightnessValues()
{    
    QList<int> values;

    int max = display->getMaxDisplayBrightnessValue();
    max = (max > 0 ? max : 5);

    for(int i=0; i<=max; ++i)
        values << i;

    return values;
}

int DisplayBusinessLogic::selectedBrightnessValue()
{
    QList<int> values = brightnessValues();
    int index = values.indexOf(display->getDisplayBrightnessValue());
    if(index < 0) {
        index = values.size()/2;
        setBrightnessValue(values.at(index));
    }
    return index;
}

QList<int> DisplayBusinessLogic::screenLightsValues()
{
    QList<int> values;
    values << 10 << 30 << 60 << 120 << 300;
    return values;
}

int DisplayBusinessLogic::selectedScreenLightsValue()
{
    QList<int> values = screenLightsValues();
    int index = values.indexOf(display->getDisplayDimTimeout());
    if(index < 0) {
        index = values.size()/2;
        setScreenLightsValue(values.at(index));
    }
    return index;
}

bool DisplayBusinessLogic::blankInhibitValue()
{    
    return display->getBlankingWhenCharging();
}

void DisplayBusinessLogic::setBrightnessValue(int value)
{    
    display->setDisplayBrightnessValue(value);
}

void DisplayBusinessLogic::setScreenLightsValue(int value)
{    
    display->setDisplayDimTimeout(value);
    display->setDisplayBlankTimeout(value + TIMEGAP);
}

void DisplayBusinessLogic::setBlankInhibitValue(bool value)
{    
    display->setBlankingWhenCharging(value);
}
