#include "displaybusinesslogicadaptor.h"
#include "displaybusinesslogic.h"
#include <QDebug>

DisplayBusinessLogicAdaptor::DisplayBusinessLogicAdaptor(DisplayBusinessLogic *displayLogic)
        : QDBusAbstractAdaptor(displayLogic), displayLogic(displayLogic)
{
}

DisplayBusinessLogicAdaptor::~DisplayBusinessLogicAdaptor()
{
}

void DisplayBusinessLogicAdaptor::setBrightnessValue(const QString &value)
{
    qDebug() << "DisplayBusinessLogicAdaptor::setBrightnessValue(" << value << ")";
    displayLogic->setBrightnessValue(value);
}

void DisplayBusinessLogicAdaptor::setScreenLightsValue(const QString &value)
{
    qDebug() << "DisplayBusinessLogicAdaptor::setScreenLightsValue(" << value << ")";
    displayLogic->setScreenLightsValue(value);
}

void DisplayBusinessLogicAdaptor::setScreenLightsToggleValue(bool value)
{
    qDebug() << "DisplayBusinessLogicAdaptor::setScreenLightsToggleValue(" << value << ")";
    displayLogic->setScreenLightsToggleValue(value);
}

QString DisplayBusinessLogicAdaptor::brightnessValue()
{
    qDebug() << "DisplayBusinessLogicAdaptor::brightnessValue()";
    return displayLogic->GConfItemValue(SystemUIGConf::DisplayBrightnessKey).toString();
}

QString DisplayBusinessLogicAdaptor::screenLightsValue()
{
    qDebug() << "DisplayBusinessLogicAdaptor::screenLightsValue()";
    return displayLogic->GConfItemValue(SystemUIGConf::DisplayScreenLightsKey).toString();
}

bool DisplayBusinessLogicAdaptor::screenLightsToggleValue()
{
    qDebug() << "DisplayBusinessLogicAdaptor::screenLightsToggleValue()";
    return displayLogic->GConfItemValue(SystemUIGConf::DisplayScreenLightsToggleKey).toBool();
}

QStringList DisplayBusinessLogicAdaptor::brightnessValues()
{
    qDebug() << "DisplayBusinessLogicAdaptor::brightnessValues()";
    return displayLogic->brightnessValues();
}

QStringList DisplayBusinessLogicAdaptor::screenLightsValues()
{
    qDebug() << "DisplayBusinessLogicAdaptor::screenLightsValues()";
    return displayLogic->screenLightsValues();
}
