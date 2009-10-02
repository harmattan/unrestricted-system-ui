#include "displaybusinesslogic.h"
#include <qmsystem/qmdisplaystate.h>

#include <QStringList>
#include <QDebug>

using namespace Maemo;

DisplayBusinessLogic::DisplayBusinessLogic()        
{
}

DisplayBusinessLogic::~DisplayBusinessLogic()
{
}

void DisplayBusinessLogic::queryBrightnessValues()
{
    QmDisplayState display;
    QStringList values;

    //TODO remove the hardcoded value when QmDisplayState is updated
    for(int i=0; i<=5 /*display.getMaxDisplayBrightnessValue()*/; ++i)
        values << QString("%1").arg(i);

    //TODO remove the hardcoded value when QmDisplayState is updated
    int index = values.indexOf(QString("%1").arg(2/*display.getDisplayBrightness()*/));

    emit brightnessValuesAvailable(index, values);
}

void DisplayBusinessLogic::queryScreenLightsValues()
{
    QmDisplayState display;
    QStringList values;

    //TODO: Make sure the values are the ones wanted
    values << QString("10") << QString("30") << QString("60") << QString("120") << QString("300");

    //TODO remove the hardcoded value when QmDisplayState is updated
    QString value = QString("%1").arg(60/*display.getDisplayBlankTimeout()*/);
    int index = (values.contains(value) ? values.indexOf(value) : 1);

    emit screenLightsValuesAvailable(index, values);
}

bool DisplayBusinessLogic::blankInhibitValue()
{
    QmDisplayState display;

    //TODO remove the hardcoded value when QmDisplayState is updated
    return true/*display.getBlankingWhenCharging()*/;
}

void DisplayBusinessLogic::setBrightnessValue(const QString &value)
{    
    QmDisplayState display;

    //TODO uncomment when QmDisplayState is updated
    //display.setDisplayBrightness(value.toInt());
}

void DisplayBusinessLogic::setScreenLightsValue(const QString &value)
{
    QmDisplayState display;

    //TODO uncomment when QmDisplayState is updated
    //display.setDisplayBlankTimeout(value.toInt());
    //display.setDisplayDimTimeout(value.toInt() - 3);
}

void DisplayBusinessLogic::setBlankInhibitValue(bool value)
{
    QmDisplayState display;

    //TODO uncomment when QmDisplayState is updated
    //display.setBlankingWhenCharging(value);
}
