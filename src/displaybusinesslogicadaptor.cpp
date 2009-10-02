#include "displaybusinesslogicadaptor.h"
#include "displaybusinesslogic.h"
#include <QDebug>

DisplayBusinessLogicAdaptor::DisplayBusinessLogicAdaptor(QObject *obj, DisplayBusinessLogic *displayLogic)
        : QDBusAbstractAdaptor(obj), displayLogic(displayLogic)
{
    connect(displayLogic, SIGNAL(brightnessValuesAvailable(int, QStringList)), this, SIGNAL(brightnessValuesReceived(int,QStringList)));
    connect(displayLogic, SIGNAL(screenLightsValuesAvailable(int, QStringList)), this, SIGNAL(screenLightsValuesReceived(int,QStringList)));
}

DisplayBusinessLogicAdaptor::~DisplayBusinessLogicAdaptor()
{
}

void DisplayBusinessLogicAdaptor::brightnessValues()
{
    qDebug() << "DisplayBusinessLogicAdaptor::brightnessValues()";
    displayLogic->queryBrightnessValues();
}

void DisplayBusinessLogicAdaptor::screenLightsValues()
{
    qDebug() << "DisplayBusinessLogicAdaptor::screenLightsValues()";
    displayLogic->queryScreenLightsValues();
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

void DisplayBusinessLogicAdaptor::setBlankInhibitValue(bool value)
{
    qDebug() << "DisplayBusinessLogicAdaptor::setBlankInhibitValue(" << value << ")";
    displayLogic->setBlankInhibitValue(value);
}

bool DisplayBusinessLogicAdaptor::blankInhibitValue()
{
    qDebug() << "DisplayBusinessLogicAdaptor::blankInhibitValue()";
    return displayLogic->blankInhibitValue();
}
