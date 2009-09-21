#include "systemuigconf.h"

#include <DuiGConfItem>
#include <QDebug>

namespace {
    QString BaseDir = "/systemui/settings";
    QString BatteryDir = "/battery";
    QString DisplayDir = "/display";
}

SystemUIGConf::SystemUIGConf()
{
    // init the gconf keys
    duiGConfItems.insert(SystemUIGConf::BatteryPSMToggleKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::BatteryPSMToggleKey)));
    duiGConfItems.insert(SystemUIGConf::BatteryPSMDisabledKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::BatteryPSMDisabledKey)));
    duiGConfItems.insert(SystemUIGConf::BatteryPSMThresholdKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::BatteryPSMThresholdKey)));
    duiGConfItems.insert(SystemUIGConf::DisplayBrightnessKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::DisplayBrightnessKey)));
    duiGConfItems.insert(SystemUIGConf::DisplayScreenLightsKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::DisplayScreenLightsKey)));
    duiGConfItems.insert(SystemUIGConf::DisplayScreenLightsToggleKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::DisplayScreenLightsToggleKey)));

    QHash<SystemUIGConf::GConfKey, DuiGConfItem *>::iterator i;
    for (i = duiGConfItems.begin(); i != duiGConfItems.end(); ++i)
        connect( i.value(), SIGNAL(valueChanged()), this, SLOT(keyValueChanged()));

}

SystemUIGConf::~SystemUIGConf()
{
    QHash<SystemUIGConf::GConfKey, DuiGConfItem *>::iterator i;
    for (i = duiGConfItems.begin(); i != duiGConfItems.end(); ++i) {
        delete i.value();
        i.value() = NULL;
    }
}

int SystemUIGConf::keyCount(SystemUIGConf::GConfKeyGroup keyGroup)
{
    QString suffixDir;
    switch(keyGroup) {
        case SystemUIGConf::Battery:
            suffixDir = BatteryDir;
            break;
        case SystemUIGConf::Display:
            suffixDir = DisplayDir;
            break;
        default:
            break;
    }
    DuiGConfItem duiGConfItem(QString("%1%2").arg(BaseDir).arg(suffixDir));
    QList<QString> list = duiGConfItem.listEntries();
    return list.size();
}

void SystemUIGConf::setValue(SystemUIGConf::GConfKey key, QVariant value)
{
    duiGConfItems.value(key)->set(value);
}

QVariant SystemUIGConf::value(SystemUIGConf::GConfKey key)
{
    return duiGConfItems.value(key)->value();
}

void SystemUIGConf::keyValueChanged()
{
    DuiGConfItem *item = static_cast<DuiGConfItem *>(this->sender());
    emit valueChanged(duiGConfItems.key(item), item->value());
}

QString SystemUIGConf::mapGConfKey(SystemUIGConf::GConfKey key)
{
    QString keyStr("%1%2%3");
    switch(key) {
        case SystemUIGConf::BatteryPSMToggleKey:
            keyStr = keyStr.arg(BaseDir).arg(BatteryDir).arg("/batteryPSMToggle");
            break;
        case SystemUIGConf::BatteryPSMDisabledKey:
            keyStr = keyStr.arg(BaseDir).arg(BatteryDir).arg("/batteryPSMDisabled");
            break;
        case SystemUIGConf::BatteryPSMThresholdKey:
            keyStr = keyStr.arg(BaseDir).arg(BatteryDir).arg("/batteryPSMThreshold");
            break;
        case SystemUIGConf::DisplayBrightnessKey:
            keyStr = keyStr.arg(BaseDir).arg(DisplayDir).arg("/displayBrightness");
            break;
        case SystemUIGConf::DisplayScreenLightsKey:
            keyStr = keyStr.arg(BaseDir).arg(DisplayDir).arg("/displayScreenLights");
            break;
        case SystemUIGConf::DisplayScreenLightsToggleKey:
            keyStr = keyStr.arg(BaseDir).arg(DisplayDir).arg("/displayScreenLightsToggle");
            break;
        default:
            break;
    }
    return keyStr;
}
