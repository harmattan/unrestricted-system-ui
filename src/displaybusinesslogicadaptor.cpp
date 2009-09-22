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

void DisplayBusinessLogicAdaptor::setBlankTimeoutValue(const QString &value)
{
    qDebug() << "DisplayBusinessLogicAdaptor::setBlankTimeoutValue(" << value << ")";
    displayLogic->setBlankTimeoutValue(value);
}

void DisplayBusinessLogicAdaptor::setDimTimeoutValue(const QString &value)
{
    qDebug() << "DisplayBusinessLogicAdaptor::setDimTimeoutValue(" << value << ")";
    displayLogic->setDimTimeoutValue(value);
}

void DisplayBusinessLogicAdaptor::setBlankInhibitValue(bool value)
{
    qDebug() << "DisplayBusinessLogicAdaptor::setBlankInhibitValue(" << value << ")";
    displayLogic->setBlankInhibitValue(value);
}

QString DisplayBusinessLogicAdaptor::brightnessValue()
{
    qDebug() << "DisplayBusinessLogicAdaptor::brightnessValue()";
    return displayLogic->GConfItemValue(SystemUIGConf::DisplayBrightnessKey).toString();
}

QString DisplayBusinessLogicAdaptor::blankTimeoutValue()
{
    qDebug() << "DisplayBusinessLogicAdaptor::blankTimeoutValue()";
    return displayLogic->GConfItemValue(SystemUIGConf::DisplayBlankTimeoutKey).toString();
}

QString DisplayBusinessLogicAdaptor::dimTimeoutValue()
{
    qDebug() << "DisplayBusinessLogicAdaptor::dimTimeoutValue()";
    return displayLogic->GConfItemValue(SystemUIGConf::DisplayDimTimeoutKey).toString();
}

bool DisplayBusinessLogicAdaptor::blankInhibitValue()
{
    qDebug() << "DisplayBusinessLogicAdaptor::blankInhibitValue()";
    return displayLogic->GConfItemValue(SystemUIGConf::DisplayBlankInhibitKey).toBool();
}

QStringList DisplayBusinessLogicAdaptor::brightnessValues()
{
    qDebug() << "DisplayBusinessLogicAdaptor::brightnessValues()";
    return displayLogic->brightnessValues();
}

QStringList DisplayBusinessLogicAdaptor::blankTimeoutValues()
{
    qDebug() << "DisplayBusinessLogicAdaptor::blankTimeoutValues()";
    return displayLogic->blankTimeoutValues();
}

QStringList DisplayBusinessLogicAdaptor::dimTimeoutValues()
{
    qDebug() << "DisplayBusinessLogicAdaptor::dimTimeoutValues()";
    return displayLogic->dimTimeoutValues();
}
