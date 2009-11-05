#include "displaybusinesslogic.h"

#include <QDebug>

/*
    TODO:
    1) Make sure the screen lights values and default index are correct
    2) Does the screen light off correcpond the blank or dim timeout?
    3) What is the hard coded gap between dimming and switching off the display?
    4) Does the timeout time really need to double if the display is tapped after it's dimmed?

*/

using namespace Maemo;

static int TIMEGAP = 5; // time gap between blanking and dimming

DisplayBusinessLogic::DisplayBusinessLogic(QObject* parent) :
        QObject(parent),
        display(new QmDisplayState())
{    
    initScreenLightsValues();
}

DisplayBusinessLogic::~DisplayBusinessLogic()
{
    delete display;
    display = NULL;
}

void DisplayBusinessLogic::initScreenLightsValues()
{
    qDebug() << "DisplayBusinessLogic::initScreenLightsValues()";

    screenLightsValues << QString("10") << QString("30") << QString("60") << QString("120") << QString("300");   
    display->setDisplayDimTimeout(screenLightsValues.at(1).toInt());
    display->setDisplayBlankTimeout(screenLightsValues.at(1).toInt() + TIMEGAP);
}

void DisplayBusinessLogic::toggleDisplay(bool toggle)
{
    qDebug() << "DisplayBusinessLogic::toggleDisplay(" << toggle << ")";
    QmDisplayState::DisplayState state = (toggle ? QmDisplayState::On : QmDisplayState::Off);
    display->set(state);
}

void DisplayBusinessLogic::queryBrightnessValues()
{
    qDebug() << "DisplayBusinessLogic::queryBrightnessValues()";
    QStringList values;

    for(int i=0; i<=display->getMaxDisplayBrightnessValue(); ++i)
        values << QString("%1").arg(i);

    int index = values.indexOf(QString("%1").arg(display->getDisplayBrightnessValue()));

    emit brightnessValuesAvailable(index, values);
}

void DisplayBusinessLogic::queryScreenLightsValues()
{
    qDebug() << "DisplayBusinessLogic::queryScreenLightsValues()";    

    QString value = QString("%1").arg(display->getDisplayDimTimeout());
    int index = screenLightsValues.indexOf(value);
    index = ( index == -1 ? 1 : index );
    emit screenLightsValuesAvailable(index, screenLightsValues);
}

bool DisplayBusinessLogic::blankInhibitValue()
{
    qDebug() << "DisplayBusinessLogic::blankInhibitValue()";
    return display->getBlankingWhenCharging();
}

void DisplayBusinessLogic::setBrightnessValue(const QString &value)
{
    qDebug() << "DisplayBusinessLogic::setBrightnessValue(" << value << ")";
    display->setDisplayBrightnessValue(value.toInt());
}

void DisplayBusinessLogic::setScreenLightsValue(const QString &value)
{
    qDebug() << "DisplayBusinessLogic::setScreenLightsValue(" << value << ")";
    display->setDisplayDimTimeout(value.toInt());
    display->setDisplayBlankTimeout(value.toInt() + TIMEGAP);
}

void DisplayBusinessLogic::setBlankInhibitValue(bool value)
{
    qDebug() << "DisplayBusinessLogic::setBlankInhibitValue(" << value << ")";
    display->setBlankingWhenCharging(value);
}
