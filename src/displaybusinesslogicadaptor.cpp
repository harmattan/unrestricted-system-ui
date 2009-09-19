#include "displaybusinesslogicadaptor.h"
#include "displaybusinesslogic.h"

DisplayBusinessLogicAdaptor::DisplayBusinessLogicAdaptor(DisplayBusinessLogic *displayLogic)
        : QDBusAbstractAdaptor(displayLogic), displayLogic(displayLogic)
{
}

DisplayBusinessLogicAdaptor::~DisplayBusinessLogicAdaptor()
{
}

void DisplayBusinessLogicAdaptor::setBrightnessValue(const QString &value)
{
    displayLogic->setBrightnessValue(value);
}

void DisplayBusinessLogicAdaptor::setScreenLightsValue(const QString &value)
{
    displayLogic->setScreenLightsValue(value);
}

void DisplayBusinessLogicAdaptor::setScreenLightsToggleValue(bool value)
{
    displayLogic->setScreenLightsToggleValue(value);
}

QString DisplayBusinessLogicAdaptor::brightnessValue()
{
    return displayLogic->GConfItemValue(SystemUIGConf::DisplayBrightnessKey).toString();
}

QString DisplayBusinessLogicAdaptor::screenLightsValue()
{
    return displayLogic->GConfItemValue(SystemUIGConf::DisplayScreenLightsKey).toString();
}

bool DisplayBusinessLogicAdaptor::screenLightsToggleValue()
{
    return displayLogic->GConfItemValue(SystemUIGConf::DisplayScreenLightsToggleKey).toBool();
}

QStringList DisplayBusinessLogicAdaptor::brightnessValues()
{
    return displayLogic->brightnessValues();
}

QStringList DisplayBusinessLogicAdaptor::screenLightsValues()
{
    return displayLogic->screenLightsValues();
}
