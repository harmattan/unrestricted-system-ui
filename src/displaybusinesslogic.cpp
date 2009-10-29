#include "displaybusinesslogic.h"

#include <QDebug>

/*
    TODO:
    1) Make sure the screen lights values are correct
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
}

DisplayBusinessLogic::~DisplayBusinessLogic()
{
    delete display;
    display = NULL;
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
    QStringList values;

    // are these OK?
    values << QString("10") << QString("30") << QString("60") << QString("120") << QString("300");

    QString value = QString("%1").arg(display->getDisplayBlankTimeout());
    int index;
    if(values.contains(value))
        index = values.indexOf(value);
    else {
        index = 1;
        int newBlankTimeout = values.at(index).toInt();
        display->setDisplayBlankTimeout(newBlankTimeout);
        display->setDisplayDimTimeout(newBlankTimeout - TIMEGAP);
    }

    emit screenLightsValuesAvailable(index, values);
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
    display->setDisplayBlankTimeout(value.toInt());
    display->setDisplayDimTimeout(value.toInt() - TIMEGAP);
}

void DisplayBusinessLogic::setBlankInhibitValue(bool value)
{
    qDebug() << "DisplayBusinessLogic::setBlankInhibitValue(" << value << ")";
    display->setBlankingWhenCharging(value);
}
