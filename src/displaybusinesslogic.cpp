#include "displaybusinesslogic.h"

DisplayBusinessLogic::DisplayBusinessLogic(SystemUIGConf *systemUIGConf) :
        systemUIGConf(systemUIGConf)
{
    displayState = new QmDisplayState();    
}

DisplayBusinessLogic::~DisplayBusinessLogic()
{
}

void DisplayBusinessLogic::setBrightnessValue(const QString &value)
{
    //displayState->setBrightness(value.toInt()); //not yet implemented
    systemUIGConf->setValue(SystemUIGConf::DisplayBrightnessKey, QVariant(value));
}

void DisplayBusinessLogic::setScreenLightsValue(const QString &value)
{
    //displayState->setScreenLights(value.toInt()); //not yet implemented
    systemUIGConf->setValue(SystemUIGConf::DisplayScreenLightsKey, QVariant(value));
}

void DisplayBusinessLogic::setScreenLightsToggleValue(bool value)
{
    // TODO: implement the functionality for this
    // - if toggle is true, screenlights should be on while charging
    // - if toggle is false, screenlights should be off while charging
    systemUIGConf->setValue(SystemUIGConf::DisplayScreenLightsToggleKey, QVariant(value));
}
