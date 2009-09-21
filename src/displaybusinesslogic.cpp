#include "displaybusinesslogic.h"

#include <QStringList>

DisplayBusinessLogic::DisplayBusinessLogic(SystemUIGConf *systemUIGConf) :
        systemUIGConf(systemUIGConf)
{
    displayState = new QmDisplayState();    
    initSystemUIGConfKeys();
}

DisplayBusinessLogic::~DisplayBusinessLogic()
{
    delete displayState;
    displayState = NULL;   
}

void DisplayBusinessLogic::initSystemUIGConfKeys()
{    
    if(systemUIGConf->keyCount(SystemUIGConf::Display) < 3) {
        /* GConf keys have not yet been set. */                
        systemUIGConf->setValue(SystemUIGConf::DisplayBrightnessKey, QVariant("50") /*QVariant(displayState->getBrightness() )*/);
        systemUIGConf->setValue(SystemUIGConf::DisplayScreenLightsKey, QVariant("20"));
        systemUIGConf->setValue(SystemUIGConf::DisplayScreenLightsToggleKey, QVariant(false));

        //TODO: replace hardcoded values when QmDisplayState completely implemented and when spec is available
    }
}

void DisplayBusinessLogic::setBrightnessValue(const QString &value)
{
    //displayState->setBrightness(value.toInt()); //not yet implemented
    systemUIGConf->setValue(SystemUIGConf::DisplayBrightnessKey, QVariant(value));
}

void DisplayBusinessLogic::setScreenLightsValue(const QString &value)
{
    // TODO: implement the functionality for this
    // - adjust the time when screen should be dimmed / turned off
    systemUIGConf->setValue(SystemUIGConf::DisplayScreenLightsKey, QVariant(value));
}

void DisplayBusinessLogic::setScreenLightsToggleValue(bool value)
{
    // TODO: implement the functionality for this
    // - if toggle is true, screenlights should be on while charging
    // - if toggle is false, screenlights should be off while charging
    systemUIGConf->setValue(SystemUIGConf::DisplayScreenLightsToggleKey, QVariant(value));
}

QStringList DisplayBusinessLogic::brightnessValues()
{
    QStringList values;
    for(int i=1; i<11; ++i)
        values << QString("%1").arg(i*10);
    return values;
    //TODO: replace hardcoded values when QmDisplayState completely implemented
}

QStringList DisplayBusinessLogic::screenLightsValues()
{
    QStringList values;
    for(int i=1; i<11; ++i)
        values << QString("%1").arg(i*10);
    return values;
    //TODO: replace hardcoded values when UI spec ready
}

QVariant DisplayBusinessLogic::GConfItemValue(SystemUIGConf::GConfKey key)
{
    return systemUIGConf->value(key);
}
