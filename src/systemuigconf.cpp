#include "systemuigconf.h"

#include <DuiGConfItem>
#include <QDebug>

SystemUIGConf::SystemUIGConf()
{
    // init the gconf keys
    duiGConfItems.insert(SystemUIGConf::BatteryPSMToggleKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::BatteryPSMToggleKey)));
    duiGConfItems.insert(SystemUIGConf::BatteryPSMDisabledKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::BatteryPSMDisabledKey)));
    duiGConfItems.insert(SystemUIGConf::BatteryPSMThresholdKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::BatteryPSMThresholdKey)));
    duiGConfItems.insert(SystemUIGConf::DisplayBrightnessMaxLevelKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::DisplayBrightnessMaxLevelKey)));
    duiGConfItems.insert(SystemUIGConf::DisplayBrightnessLevelStepKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::DisplayBrightnessLevelStepKey)));
    duiGConfItems.insert(SystemUIGConf::DisplayBrightnessKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::DisplayBrightnessKey)));
    duiGConfItems.insert(SystemUIGConf::DisplayBlankTimeoutsKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::DisplayBlankTimeoutsKey)));
    duiGConfItems.insert(SystemUIGConf::DisplayBlankTimeoutKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::DisplayBlankTimeoutKey)));
    duiGConfItems.insert(SystemUIGConf::DisplayDimTimeoutsKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::DisplayDimTimeoutsKey)));
    duiGConfItems.insert(SystemUIGConf::DisplayDimTimeoutKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::DisplayDimTimeoutKey)));
    duiGConfItems.insert(SystemUIGConf::DisplayBlankInhibitKey, new DuiGConfItem(mapGConfKey(SystemUIGConf::DisplayBlankInhibitKey)));

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
    DuiGConfItem duiGConfItem(mapGConfKeyGroup(keyGroup));
    QList<QString> list = duiGConfItem.listEntries();
    return list.size();
}

void SystemUIGConf::setValue(SystemUIGConf::GConfKey key, QVariant value)
{
    duiGConfItems.value(key)->set(value);
}

QVariant SystemUIGConf::value(SystemUIGConf::GConfKey key, QVariant def)
{
    if(def.isNull())
        return duiGConfItems.value(key)->value();
    else
        return duiGConfItems.value(key)->value(def);
}

void SystemUIGConf::keyValueChanged()
{
    DuiGConfItem *item = static_cast<DuiGConfItem *>(this->sender());
    emit valueChanged(duiGConfItems.key(item), item->value());
}

QString SystemUIGConf::mapGConfKeyGroup(SystemUIGConf::GConfKeyGroup keyGroup)
{
    QString keyGroupStr;
    switch(keyGroup) {
        case SystemUIGConf::Battery:
            keyGroupStr = "/systemui/settings/battery";
            break;
        case SystemUIGConf::Display:
            keyGroupStr = "/system/osso/dsm/display";
            break;
        default:
            break;
    }
    return keyGroupStr;
}

QString SystemUIGConf::mapGConfKey(SystemUIGConf::GConfKey key)
{
    QString keyStr("%1%2");
    switch(key) {
        case SystemUIGConf::BatteryPSMToggleKey:
            keyStr = keyStr.arg(mapGConfKeyGroup(SystemUIGConf::Battery)).arg("/batteryPSMToggle");
            break;
        case SystemUIGConf::BatteryPSMDisabledKey:
            keyStr = keyStr.arg(mapGConfKeyGroup(SystemUIGConf::Battery)).arg("/batteryPSMDisabled");
            break;
        case SystemUIGConf::BatteryPSMThresholdKey:
            keyStr = keyStr.arg(mapGConfKeyGroup(SystemUIGConf::Battery)).arg("/batteryPSMThreshold");
            break;
        case SystemUIGConf::DisplayBrightnessMaxLevelKey:
            keyStr = keyStr.arg(mapGConfKeyGroup(SystemUIGConf::Display)).arg("/max_display_brightness");
            break;
        case SystemUIGConf::DisplayBrightnessLevelStepKey:
            keyStr = keyStr.arg(mapGConfKeyGroup(SystemUIGConf::Display)).arg("/Display_brightness_level_step");
            break;
        case SystemUIGConf::DisplayBrightnessKey:
            keyStr = keyStr.arg(mapGConfKeyGroup(SystemUIGConf::Display)).arg("/Display_brightness");
            break;
        case SystemUIGConf::DisplayBlankTimeoutsKey:
            keyStr = keyStr.arg(mapGConfKeyGroup(SystemUIGConf::Display)).arg("/Possible_display_blank_timeouts");
            break;
        case SystemUIGConf::DisplayBlankTimeoutKey:
            keyStr = keyStr.arg(mapGConfKeyGroup(SystemUIGConf::Display)).arg("/Display_blank_timeout");
            break;
        case SystemUIGConf::DisplayDimTimeoutsKey:
            keyStr = keyStr.arg(mapGConfKeyGroup(SystemUIGConf::Display)).arg("/Possible_display_dim_timeouts");
            break;
        case SystemUIGConf::DisplayDimTimeoutKey:
            keyStr = keyStr.arg(mapGConfKeyGroup(SystemUIGConf::Display)).arg("/Display_dim_timeout");
            break;
        case SystemUIGConf::DisplayBlankInhibitKey:
            keyStr = keyStr.arg(mapGConfKeyGroup(SystemUIGConf::Display)).arg("/inhibit_blank_mode");
            break;
        default:
            break;
    }
    return keyStr;
}
