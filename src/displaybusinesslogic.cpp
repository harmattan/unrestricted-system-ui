#include "displaybusinesslogic.h"

#include <QStringList>
#include <QDebug>

DisplayBusinessLogic::DisplayBusinessLogic(SystemUIGConf *systemUIGConf) :
        systemUIGConf(systemUIGConf)
{
    displayState = new QmDisplayState();
}

DisplayBusinessLogic::~DisplayBusinessLogic()
{
    delete displayState;
    displayState = NULL;   
}

void DisplayBusinessLogic::setBrightnessValue(const QString &value)
{    
    systemUIGConf->setValue(SystemUIGConf::DisplayBrightnessKey, QVariant(value.toInt()));
}

void DisplayBusinessLogic::setBlankTimeoutValue(const QString &value)
{
    systemUIGConf->setValue(SystemUIGConf::DisplayBlankTimeoutKey, QVariant(value.toInt()));
}

void DisplayBusinessLogic::setDimTimeoutValue(const QString &value)
{
    systemUIGConf->setValue(SystemUIGConf::DisplayDimTimeoutKey, QVariant(value.toInt()));
}

void DisplayBusinessLogic::setBlankInhibitValue(bool value)
{    
    systemUIGConf->setValue(SystemUIGConf::DisplayBlankInhibitKey, QVariant(value ? 1 : 0));
}

QStringList DisplayBusinessLogic::brightnessValues()
{    
    QStringList values;
    int maxValue = systemUIGConf->value(SystemUIGConf::DisplayBrightnessMaxLevelKey,
                                        defaultValue(SystemUIGConf::DisplayBrightnessMaxLevelKey)).toInt(); 
    int valueStep = systemUIGConf->value(SystemUIGConf::DisplayBrightnessLevelStepKey,
                                         defaultValue(SystemUIGConf::DisplayBrightnessLevelStepKey)).toInt();    
    QString value = systemUIGConf->value(SystemUIGConf::DisplayBrightnessKey,
                                         defaultValue(SystemUIGConf::DisplayBrightnessKey)).toString();    

    for(int i=maxValue; i>=valueStep; ) {
        values << QString("%1").arg(i);
        i -= valueStep;
    }
    if(!values.contains(value))
        systemUIGConf->setValue(SystemUIGConf::DisplayBrightnessKey, QVariant((values.at((values.size()-1)/2).toInt())));

    return values;
}

QStringList DisplayBusinessLogic::blankTimeoutValues()
{
    QStringList values = systemUIGConf->value(SystemUIGConf::DisplayBlankTimeoutsKey,
                                              defaultValue(SystemUIGConf::DisplayBlankTimeoutsKey)).toStringList();
    QString value = systemUIGConf->value(SystemUIGConf::DisplayBlankTimeoutKey,
                                         defaultValue(SystemUIGConf::DisplayBlankTimeoutKey)).toString();
    if(!values.contains(value))
        systemUIGConf->setValue(SystemUIGConf::DisplayBlankTimeoutKey, QVariant((values.at((values.size()-1)/2).toInt())));
    return values;
}

QStringList DisplayBusinessLogic::dimTimeoutValues()
{    
    QStringList values = systemUIGConf->value(SystemUIGConf::DisplayDimTimeoutsKey,
                                              defaultValue(SystemUIGConf::DisplayDimTimeoutsKey)).toStringList();
    QString value = systemUIGConf->value(SystemUIGConf::DisplayDimTimeoutKey,
                                         defaultValue(SystemUIGConf::DisplayDimTimeoutKey)).toString();
    if(!values.contains(value))
        systemUIGConf->setValue(SystemUIGConf::DisplayDimTimeoutKey, QVariant((values.at((values.size()-1)/2).toInt())));
    return values;
}

QVariant DisplayBusinessLogic::GConfItemValue(SystemUIGConf::GConfKey key)
{
    return systemUIGConf->value(key, defaultValue(key));
}

QVariant DisplayBusinessLogic::defaultValue(SystemUIGConf::GConfKey key)
{
    QVariant def = NULL;
    QStringList defList;

    switch(key) {
        case SystemUIGConf::DisplayBrightnessKey:
            def = QVariant(3);
            break;
        case SystemUIGConf::DisplayBlankTimeoutKey:
            def = QVariant(30);
            break;
        case SystemUIGConf::DisplayDimTimeoutKey:
            def = QVariant(10);
            break;
        case SystemUIGConf::DisplayBlankInhibitKey:
            def = QVariant(0);
            break;
        case SystemUIGConf::DisplayBrightnessMaxLevelKey:
            def = QVariant(5);
            break;
        case SystemUIGConf::DisplayBrightnessLevelStepKey:
            def = QVariant(1);
            break;
        case SystemUIGConf::DisplayBlankTimeoutsKey:
            defList << QString("30") << QString("60") << QString("120") << QString("300");
            def = QVariant(defList);
            break;
        case SystemUIGConf::DisplayDimTimeoutsKey:
            defList << QString("10") << QString("30") << QString("60") << QString("120");
            def = QVariant(defList);
            break;
        default:
            break;
    }
    return def;
}
