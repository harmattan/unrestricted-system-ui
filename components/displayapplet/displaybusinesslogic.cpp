/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "displaybusinesslogic.h"

#include <QDebug>

//#define DEBUG
#include "../debug.h"

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

void
DisplayBusinessLogic::toggleDisplay (
        bool toggle)
{
    SYS_DEBUG ("*** toggle = %s", toggle ? "true" : "false");
    QmDisplayState::DisplayState state = 
        (toggle ? QmDisplayState::On : QmDisplayState::Off);

    display->set (state);
}

QList<int> DisplayBusinessLogic::brightnessValues()
{
    QList<int> values;

    int max = display->getMaxDisplayBrightnessValue();
    max = (max > 0 ? max : 5);

    for (int i = 0; i <= max; ++i)
        values << i;

    return values;
}

int
DisplayBusinessLogic::selectedBrightnessValue ()
{
    QList<int> values = brightnessValues();
    int index = values.indexOf(display->getDisplayBrightnessValue());
    if (index < 0) {
        index = values.size() / 2;
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

int 
DisplayBusinessLogic::selectedScreenLightsValue ()
{
    QList<int> values = screenLightsValues();
    int index = values.indexOf(display->getDisplayDimTimeout());
    if (index < 0) {
        index = values.size() / 2;
        setScreenLightsValue(values.at(index));
    }

    return index;
}

bool 
DisplayBusinessLogic::blankInhibitValue ()
{
    return display->getBlankingWhenCharging();
}

void 
DisplayBusinessLogic::setBrightnessValue (
        int value)
{
    SYS_DEBUG ("*** value = %d", value);
    display->setDisplayBrightnessValue(value);
}

void 
DisplayBusinessLogic::setScreenLightsValue (
		int value)
{
    int seconds;

    SYS_DEBUG ("*** value = %d", value);

    /*
     * We got the serial number, not the value.
     */
    switch (value) {
        case 0:
            seconds = 10;
            break;

        case 1:
            seconds = 30;
            break;

        case 2:
            seconds = 60;
            break;

        case 3:
            seconds = 120;
            break;

        case 4:
            seconds = 300;
            break;

        default:
            SYS_WARNING ("Unknown value, using 300");
            seconds = 300;
    }

    SYS_DEBUG ("*** seconds = %d", seconds);

    display->setDisplayDimTimeout (seconds);
    display->setDisplayBlankTimeout (value + TIMEGAP);
}

void 
DisplayBusinessLogic::setBlankInhibitValue (
        bool value)
{
    /*
     * Actually the UI string is a negative 'keep the lights on'.
     */
    value = !value;
    SYS_DEBUG ("*** blanking when charging = %s", value ? "true" : "false");
    display->setBlankingWhenCharging(value);
}
